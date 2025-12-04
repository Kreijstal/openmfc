param(
    [string]$BuildDir = "phase0/abi_ref/build",
    [string]$BinDir   = "phase0/abi_ref/bin"
)

New-Item -ItemType Directory -Force -Path $BuildDir, $BinDir | Out-Null

$clArgs = @(
    "/std:c++17", "/EHsc", "/MD",
    "/Iphase0/abi_ref/include",
    "phase0/abi_ref/tests/msvc_consumer.cpp",
    "/link",
    "/LIBPATH:$BuildDir",
    "shim_test.lib",
    "/out:$BinDir/msvc_consumer.exe"
)

cl @clArgs
