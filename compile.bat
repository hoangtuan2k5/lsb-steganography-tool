@echo off
echo Compiling Steganography LSB WAV program...
g++ -o wav_stego wav_stego.cpp
if %errorlevel% equ 0 (
    echo Compilation successful! Now you can use wav_stego.exe and thank you Tuan
) else (
    echo Compilation failed!
)
pause