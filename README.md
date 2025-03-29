# LSB Steganography Tool cho File WAV 🎵
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/hoang/LSBSteganographyTool)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)](https://isocpp.org/)

Công cụ giấu tin sử dụng kỹ thuật LSB (Least Significant Bit) cho phép giấu file bất kỳ vào file âm thanh WAV và khôi phục lại file gốc một cách an toàn.

## 📑 Mục Lục
- [Tổng Quan](#tong-quan)
- [Tính Năng](#tinh-nang)
- [Yêu Cầu Hệ Thống](#yeu-cau-he-thong)
- [Cài Đặt](#cai-dat)
- [Cách Sử Dụng](#cach-su-dung)
- [Cơ Chế Hoạt Động](#co-che-hoat-dong)
- [Thông Số Kỹ Thuật](#thong-so-ky-thuat)
- [Hạn Chế](#han-che)
- [Đóng Góp](#dong-gop)
- [Tác Giả](#tac-gia)
- [Giấy Phép](#giay-phep)

## 📋 Tổng Quan
Chương trình này sử dụng kỹ thuật LSB để giấu file bất kỳ vào file âm thanh WAV một cách an toàn và hiệu quả. Việc thay đổi bit ít quan trọng nhất (LSB) trong dữ liệu âm thanh đảm bảo chất lượng âm thanh gần như không bị ảnh hưởng.

## ✨ Tính Năng
- 🔒 Giấu file bất kỳ vào file WAV
- 🔄 Khôi phục file gốc với độ chính xác 100%
- 📝 Tự động lưu và khôi phục tên file gốc
- 🎯 Hỗ trợ file WAV ở mọi độ sâu bit
- 🔧 Giao diện dòng lệnh đơn giản

## 💻 Yêu Cầu Hệ Thống
- Hệ điều hành: Windows/Linux/macOS
- Trình biên dịch C++ (GCC/G++)
- File WAV không nén

## ⚙️ Cài Đặt

### Windows
```batch
compile.bat
```

### Linux/macOS
```bash
g++ -o wav_stego wav_stego.cpp
```

## 📖 Cách Sử Dụng

### Giấu File
```bash
./wav_stego -e <file_wav_goc> <file_can_giau> <file_wav_dau_ra>
```

Ví dụ:
```bash
./wav_stego -e original.wav secret.txt hidden.wav
```

### Giải Mã File

#### Tự động dùng tên file gốc
```bash
./wav_stego -d <file_wav_chua_du_lieu>
```

#### Chỉ định tên file đầu ra
```bash
./wav_stego -d <file_wav_chua_du_lieu> <ten_file_dau_ra>
```

Ví dụ:
```bash
# Tự động dùng tên file gốc
./wav_stego -d hidden.wav

# Chỉ định tên file mới
./wav_stego -d hidden.wav extracted_file.txt
```

## 🔍 Cơ Chế Hoạt Động
### Kỹ Thuật LSB
- Sử dụng bit cuối cùng (LSB) của mỗi byte trong dữ liệu âm thanh
- Thay đổi tối thiểu, không ảnh hưởng đáng kể đến chất lượng âm thanh

### Cấu Trúc Dữ Liệu Giấu
1. Tên file gốc (bao gồm phần mở rộng)
2. Kích thước file (32 bit)
3. Nội dung file

## 🛠 Thông Số Kỹ Thuật

### Định Dạng WAV
- ✅ Hỗ trợ file WAV chuẩn
- ✅ Giữ nguyên header WAV
- ✅ Tương thích mọi độ sâu bit

### Dung Lượng
- 📦 Kích thước tối đa phụ thuộc vào file WAV
- 🏷️ Tự động quản lý tên file
- 🎯 Tùy chọn đặt tên khi giải mã

### Bảo Mật
- 🔐 Steganography cơ bản
- ⚠️ Không có mã hóa bổ sung
- 📊 Có thể phát hiện qua phân tích thống kê

## ⚠️ Hạn Chế
1. Dung lượng file cần giấu phải nhỏ hơn khả năng chứa của file WAV
2. Chỉ hoạt động với file WAV không nén
3. File WAV đầu ra có thay đổi nhỏ ở các bit cuối

## 🤝 Đóng Góp
Mọi đóng góp đều được chào đón! Hãy tạo issue hoặc pull request nếu bạn muốn:
- Báo lỗi
- Thêm tính năng mới
- Cải thiện tài liệu
- Tối ưu mã nguồn

## 👨‍💻 Tác Giả
**Hoàng Chiều Nguyễn Tuấn**

## 📄 Giấy Phép
Dự án này được phân phối dưới Giấy phép MIT. Xem file `LICENSE` để biết thêm chi tiết.