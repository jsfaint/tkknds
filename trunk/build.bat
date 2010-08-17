@echo off
rem I just configure the environment variables what I need.
rem set PATH=%PATH%;d:\devkitPro\msys\bin;d:\devkitPro\devkitARM\bin;
set PATH=d:\msys\bin;h:\nds\devkitPro\devkitARM\bin;
set DEVKITPRO=/h/nds
set DEVKITARM=/h/nds/devkitARM
rem using PALib
set PAPATH=/h/nds/PALib/lib
@echo on

make
pause
