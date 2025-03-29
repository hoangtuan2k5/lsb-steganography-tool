#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

// Cấu trúc header của file WAV
struct WAVHeader {
    char riff[4];                // Chuỗi RIFF
    unsigned int overall_size;   // Kích thước tổng thể của file (bytes)
    char wave[4];               // Chuỗi WAVE
    char fmt_chunk_marker[4];   // Chuỗi fmt với ký tự null ở cuối
    unsigned int length_of_fmt;  // Độ dài của phần format
    unsigned short format_type;  // Loại format. 1-PCM, 3-IEEE float, 6-8bit A law, 7-8bit mu law
    unsigned short channels;     // Số kênh
    unsigned int sample_rate;    // Tốc độ lấy mẫu (block/giây)
    unsigned int byterate;       // SampleRate * NumChannels * BitsPerSample/8
    unsigned short block_align;  // NumChannels * BitsPerSample/8
    unsigned short bits_per_sample; // Số bit mỗi mẫu, 8-8bits, 16-16 bits
};

class WAVSteganography {
private:
    WAVHeader header;
    vector<char> audioData;
    
    bool readWAVFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file.is_open()) {
            cout << "Lỗi: Không thể mở file WAV." << endl;
            return false;
        }

        // Đọc header
        file.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

        // Kiểm tra định dạng WAV
        if (strncmp(header.riff, "RIFF", 4) != 0 || strncmp(header.wave, "WAVE", 4) != 0) {
            cout << "Lỗi: File không phải định dạng WAV hợp lệ." << endl;
            file.close();
            return false;
        }

        // Đọc dữ liệu âm thanh
        audioData.clear();
        char byte;
        while (file.read(&byte, sizeof(char))) {
            audioData.push_back(byte);
        }

        file.close();
        return true;
    }

    // Hàm lưu chuỗi vào các bit LSB
    void hideString(const string& str, int& startIndex) {
        int strLen = str.length();
        // Lưu độ dài chuỗi (32 bit)
        for (int i = 0; i < 32; i++) {
            audioData[startIndex + i] = (audioData[startIndex + i] & 0xFE) | ((strLen >> i) & 1);
        }
        startIndex += 32;

        // Lưu từng ký tự của chuỗi
        for (char c : str) {
            for (int bit = 0; bit < 8; bit++) {
                audioData[startIndex] = (audioData[startIndex] & 0xFE) | ((c >> bit) & 1);
                startIndex++;
            }
        }
    }

    // Hàm đọc chuỗi từ các bit LSB
    string extractString(int& startIndex) {
        // Đọc độ dài chuỗi
        int strLen = 0;
        for (int i = 0; i < 32; i++) {
            strLen |= (audioData[startIndex + i] & 1) << i;
        }
        startIndex += 32;

        // Đọc chuỗi
        string result;
        for (int i = 0; i < strLen; i++) {
            char byte = 0;
            for (int bit = 0; bit < 8; bit++) {
                byte |= (audioData[startIndex] & 1) << bit;
                startIndex++;
            }
            result += byte;
        }
        return result;
    }

public:
    bool encode(const string& wavFile, const string& secretFile, const string& outputFile) {
        // Đọc file WAV
        if (!readWAVFile(wavFile)) {
            return false;
        }

        // Đọc file cần giấu
        ifstream secret(secretFile, ios::binary);
        if (!secret.is_open()) {
            cout << "Lỗi: Không thể mở file cần giấu." << endl;
            return false;
        }

        // Lấy kích thước file cần giấu
        secret.seekg(0, ios::end);
        unsigned int secretSize = static_cast<unsigned int>(secret.tellg());
        secret.seekg(0, ios::beg);

        // Lấy tên file gốc
        string originalFilename = secretFile.substr(secretFile.find_last_of("/\\") + 1);

        // Tính toán không gian cần thiết
        int requiredSpace = 32 + // Độ dài file
                          32 + originalFilename.length() * 8 + // Tên file (32 bit độ dài + nội dung)
                          secretSize * 8; // Nội dung file

        // Kiểm tra dung lượng
        if (requiredSpace > audioData.size()) {
            cout << "Lỗi: File WAV quá nhỏ để giấu file." << endl;
            return false;
        }

        int dataIndex = 0;

        // Lưu tên file gốc
        hideString(originalFilename, dataIndex);

        // Lưu kích thước file cần giấu
        for (int i = 0; i < 32; i++) {
            audioData[dataIndex] = (audioData[dataIndex] & 0xFE) | ((secretSize >> i) & 1);
            dataIndex++;
        }

        // Đọc và giấu nội dung file
        char byte;
        while (secret.read(&byte, sizeof(char))) {
            for (int bit = 0; bit < 8; bit++) {
                audioData[dataIndex] = (audioData[dataIndex] & 0xFE) | ((byte >> bit) & 1);
                dataIndex++;
            }
        }

        // Ghi file WAV đã chỉnh sửa
        ofstream outFile(outputFile, ios::binary);
        if (!outFile.is_open()) {
            cout << "Lỗi: Không thể tạo file đầu ra." << endl;
            return false;
        }

        outFile.write(reinterpret_cast<char*>(&header), sizeof(WAVHeader));
        outFile.write(audioData.data(), audioData.size());

        outFile.close();
        secret.close();
        return true;
    }

    bool decode(const string& wavFile, const string& customOutputFile = "") {
        // Đọc file WAV
        if (!readWAVFile(wavFile)) {
            return false;
        }

        int dataIndex = 0;

        // Đọc tên file gốc
        string originalFilename = extractString(dataIndex);

        // Đọc kích thước file
        unsigned int secretSize = 0;
        for (int i = 0; i < 32; i++) {
            secretSize |= (audioData[dataIndex] & 1) << i;
            dataIndex++;
        }

        // Kiểm tra tính hợp lệ
        if (dataIndex + secretSize * 8 > audioData.size()) {
            cout << "Lỗi: Dữ liệu ẩn không hợp lệ." << endl;
            return false;
        }

        // Sử dụng tên file tùy chọn nếu được cung cấp
        string outputFilename = customOutputFile.empty() ? originalFilename : customOutputFile;
        cout << "Tên file gốc: " << originalFilename << endl;
        cout << "Đang giải nén vào file: " << outputFilename << endl;

        // Tạo file đầu ra
        ofstream outFile(outputFilename, ios::binary);
        if (!outFile.is_open()) {
            cout << "Lỗi: Không thể tạo file đầu ra." << endl;
            return false;
        }

        // Trích xuất file ẩn
        for (unsigned int i = 0; i < secretSize; i++) {
            char byte = 0;
            for (int bit = 0; bit < 8; bit++) {
                byte |= (audioData[dataIndex] & 1) << bit;
                dataIndex++;
            }
            outFile.write(&byte, sizeof(char));
        }

        outFile.close();
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Cách sử dụng:" << endl;
        cout << "Để giấu file: " << argv[0] << " -e <file_wav_goc> <file_can_giau> <file_wav_dau_ra>" << endl;
        cout << "Để giải mã (tự động): " << argv[0] << " -d <file_wav_chua_du_lieu>" << endl;
        cout << "Để giải mã (tùy chọn tên): " << argv[0] << " -d <file_wav_chua_du_lieu> <ten_file_dau_ra>" << endl;
        return 1;
    }

    WAVSteganography stego;
    string operation = argv[1];

    if (operation == "-e" && argc == 5) {
        // Chế độ giấu file
        if (stego.encode(argv[2], argv[3], argv[4])) {
            cout << "Đã giấu file thành công." << endl;
        } else {
            cout << "Giấu file thất bại." << endl;
            return 1;
        }
    }
    else if (operation == "-d" && (argc == 3 || argc == 4)) {
        // Chế độ giải mã
        string outputFile = (argc == 4) ? argv[3] : "";
        if (stego.decode(argv[2], outputFile)) {
            cout << "Đã giải mã file thành công." << endl;
        } else {
            cout << "Giải mã file thất bại." << endl;
            return 1;
        }
    }
    else {
        cout << "Tham số không hợp lệ." << endl;
        return 1;
    }

    return 0;
}