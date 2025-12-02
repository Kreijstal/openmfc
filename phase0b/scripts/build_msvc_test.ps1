$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$Root = Resolve-Path (Join-Path $ScriptDir "..")
$Bin = Join-Path $Root "bin"
$Build = Join-Path $Root "build"

if (-not (Test-Path $Bin)) { New-Item -ItemType Directory -Path $Bin | Out-Null }

$Src = Join-Path $Root "tests/msvc_test_cobject.cpp"
$Dll = Join-Path $Build "openmfc.dll"
$Lib = Join-Path $Build "libopenmfc.a"
$Exe = Join-Path $Bin "msvc_test_cobject.exe"

Write-Host "Building MSVC host..."
cl.exe /nologo /EHsc /MD /I"$($Root)\include" "$Src" /link /OUT:"$Exe" /LIBPATH:"$Build" openmfc.lib libopenmfc.a 2>&1

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit $LASTEXITCODE
}

Write-Host "Built host: $Exe"
Write-Host "Expect to place openmfc.dll next to the exe before running."

