@echo off
REM run all sample binaries with the local vm
for /r %%i in (testFiles\*.bin) do vm %%i
