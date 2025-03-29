# Chương Trình Giấu Tin LSB Trong File WAV
### Tác giả: Hoàng Chiều Nguyễn Tuấn

Chương trình này sử dụng kỹ thuật LSB (Least Significant Bit) để giấu file bất kỳ vào file âm thanh WAV và có thể khôi phục lại file gốc sau đó.

## Cách Hoạt Động

1. **Kỹ thuật LSB**: Chương trình sử dụng bit cuối cùng (LSB) của mỗi byte trong dữ liệu âm thanh WAV để lưu trữ dữ liệu cần giấu. Điều này gây ảnh hưởng tối thiểu đến chất lượng âm thanh.

2. **Cấu Trúc Dữ Liệu**:
   - Lưu tên file gốc (bao gồm cả phần mở rộng)
   - Lưu kích thước của file cần giấu (32 bit)
   - Lưu nội dung của file cần giấu

## Biên Dịch

```bash
g++ -o wav_stego wav_stego.cpp
```

Hoặc chạy file `compile.bat` trên Windows.

## Cách Sử Dụng

### Để Giấu File
```bash
./wav_stego -e <file_wav_goc> <file_can_giau> <file_wav_dau_ra>
```

Ví dụ:
```bash
./wav_stego -e original.wav secret.txt hidden.wav
```

### Để Giải Mã File

1. Tự động dùng tên file gốc:
```bash
./wav_stego -d <file_wav_chua_du_lieu>
```

2. Chỉ định tên file đầu ra tùy chọn:
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

## Thông Tin Kỹ Thuật

1. **Hỗ Trợ Định Dạng WAV**:
   - Hỗ trợ file WAV chuẩn
   - Giữ nguyên header WAV
   - Hoạt động với file WAV ở mọi độ sâu bit

2. **Dung Lượng**:
   - Có thể giấu file với kích thước tối đa phụ thuộc vào kích thước file WAV
   - Tự động lưu và khôi phục tên file gốc
   - Cho phép đặt tên tùy chọn khi giải mã

3. **Lưu Ý Về Bảo Mật**:
   - Đây là cài đặt steganography cơ bản
   - Không có mã hóa dữ liệu
   - Có thể phát hiện sự hiện diện của dữ liệu ẩn qua phân tích thống kê

## Hạn Chế

1. Kích thước file cần giấu phải nhỏ hơn dung lượng cho phép của file WAV
2. Chỉ hoạt động với file WAV không nén
3. File WAV đầu ra sẽ có thay đổi nhỏ ở các bit cuối

## Tính Năng Mới

- Hiển thị tên file gốc khi giải mã
- Cho phép chọn tên file tùy ý khi giải mã
- Tất cả thông báo và giao diện bằng tiếng Việt