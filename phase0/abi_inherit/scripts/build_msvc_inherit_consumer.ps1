param(
    [string]$BuildDir = "phase0/abi_inherit/build",
    [string]$BinDir   = "phase0/abi_inherit/bin"
)

New-Item -ItemType Directory -Force -Path $BuildDir, $BinDir | Out-Null

$clArgs = @(
    "/std:c++17", "/EHsc", "/MD",
    "/Iphase0/abi_inherit/include",
    "phase0/abi_inherit/tests/msvc_inherit_consumer.cpp",
    "/link",
    "/LIBPATH:$BuildDir",
    "abi_inherit.lib",
    "/out:$BinDir/msvc_inherit_consumer.exe"
)

cl @clArgs
