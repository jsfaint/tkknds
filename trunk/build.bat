@echo off
rem I just configure the environment variables what I need.
rem set PATH=%PATH%;d:\devkitPro\msys\bin;d:\devkitPro\devkitARM\bin;
set PATH=d:\devkitPro\msys\bin;d:\devkitPro\devkitARM\bin;
set DEVKITPRO=/d/devkitPro
set DEVKITARM=/d/devkitPro/devkitARM
rem using PALib
set PAPATH=/d/devkitPro/PALib/lib
@echo on

make clean
make
pause