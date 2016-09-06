@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
if defined HOMEDEV goto location2
echo Default location settings loaded.
PATH=%PATH%;C:\Development\Editor\Emacs\bin
goto end
:location2
call "D:\Software\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
echo Home location settings loaded.
PATH=%PATH%;D:\Software\emacs64\bin;D:\Software\Git\bin
:end
call cmd.exe
