@echo off
REM MSVC Compilation Test
REM Must be run on Windows with MSVC installed

echo Compiling MSVC ABI test...

REM Compile with MSVC
cl.exe /nologo /EHsc /std:c++17 /I ..\include test_msvc_abi_real.cpp openmfc.lib

if %ERRORLEVEL% neq 0 (
    echo FAIL: MSVC compilation failed
    exit /b 1
)

echo SUCCESS: MSVC compiled and linked successfully
echo The mangled names in openmfc.lib match MSVC's expectations

REM Run the test
test_msvc_abi_real.exe

if %ERRORLEVEL% neq 0 (
    echo FAIL: Test execution failed
    exit /b 1
)

echo SUCCESS: MSVC ABI test passed