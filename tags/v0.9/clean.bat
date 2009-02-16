@echo off
rem I just configure the environment variables what I need.
rem set PATH=%PATH%;d:\devkitPro\msys\bin;d:\devkitPro\devkitARM\bin;
set PATH=d:\devkitPro\msys\bin;g:\nds\devkitPro\devkitARM\bin;
set DEVKITPRO=/g/nds
set DEVKITARM=/g/nds/devkitARM
rem using PALib
set PAPATH=/g/nds/PALib/lib
@echo on

make clean
