@echo off
if "%COMPUTERNAME%"=="K0SH-PC" goto :location2
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
PATH=%PATH%;C:\Development\Editor\Emacs\bin
goto end
:location2
call "D:\Software\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
PATH=%PATH%;D:\Software\emacs64\bin
:end
call cmd.exe