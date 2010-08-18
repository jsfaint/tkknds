@echo off
rem I just configure the environment variables what I need.
set PATH=e:\nds\msys\bin;e:\nds\devkitPro\devkitARM\bin;
set DEVKITPRO=/e/nds
set DEVKITARM=/e/nds/devkitARM
rem using PALib
set PAPATH=/e/nds/PALib/lib
@echo on

make clean
