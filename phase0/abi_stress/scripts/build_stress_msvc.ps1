# Setup directories
$ScriptDir = $PSScriptRoot
$SourceDir = Join-Path $ScriptDir ".."
$OutDir    = Join-Path $SourceDir "artifacts"
if (Test-Path $OutDir) { Remove-Item -Recurse -Force $OutDir }
New-Item -ItemType Directory -Path $OutDir | Out-Null

# Define source files
$SrcFiles = @(
    "$SourceDir\src\dllmain.cpp",
    "$SourceDir\src\01_basics.cpp",
    "$SourceDir\src\02_signatures.cpp",
    "$SourceDir\src\03_lifecycle.cpp",
    "$SourceDir\src\04_inheritance.cpp",
    "$SourceDir\src\05_rvo.cpp",
    "$SourceDir\src\06_modern.cpp",
    "$SourceDir\src\07_abstract.cpp",
    "$SourceDir\src\08_covariant.cpp",
    "$SourceDir\src\09_abstract_pure.cpp"
)

# Compiler Flags
# /LD  = Create DLL
# /MD  = Link with dynamic CRT (match standard MFC)
# /O1  = Minimize size (makes disassembly easier to read)
# /Zi  = Generate Debug Info (PDB) for symbol dumping
# /d1reportSingleClassLayout... = UNDOCUMENTED FLAG to dump class memory layout
$Flags = "/nologo", "/std:c++17", "/EHsc", "/LD", "/MD", "/O1", "/Zi",
         "/I", "$SourceDir\include",
         "/DABI_STRESS_EXPORTS",
         "/d1reportAllClassLayout",
         "/d1reportSingleClassLayoutCStage1_Simple",
         "/d1reportSingleClassLayoutCStage3_Base",
         "/d1reportSingleClassLayoutCStage4_Multi",
         "/d1reportSingleClassLayoutCStage7_NoVtable",
         "/d1reportSingleClassLayoutCCovariantDerived",
         "/d1reportSingleClassLayoutIPure"

Write-Host "Building abi_stress.dll..."
# We pipe the build output to a file because the /d1report flag prints to stdout
& cl.exe $SrcFiles $Flags /Fe:"$OutDir\abi_stress.dll" /Fo:"$OutDir\" /Fd:"$OutDir\abi_stress.pdb" > "$OutDir\layout.log" 2>&1

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed. Check layout.log"
    Get-Content "$OutDir\layout.log"
    exit 1
}

Write-Host "Harvesting ABI data..."

Set-Location $OutDir

# 1. Dump Exports (Mangling)
& dumpbin /EXPORTS abi_stress.dll > exports.txt 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Warning "dumpbin /EXPORTS failed, exports.txt may be incomplete"
}

# 2. Dump Symbols (Vtables, RTTI)
& dumpbin /SYMBOLS abi_stress.dll > symbols.txt 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Warning "dumpbin /SYMBOLS failed, symbols.txt may be incomplete"
}

# 2a. Dump object-level symbols (captures RTTI, per-class details)
$objSymbols = Join-Path $OutDir "symbols_objects.txt"
& dumpbin /SYMBOLS *.obj > $objSymbols 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Warning "dumpbin /SYMBOLS on objects failed, symbols_objects.txt may be incomplete"
}

# 3. Dump Disassembly (Calling convention, thunks)
& dumpbin /DISASM abi_stress.dll > disassembly.txt 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Warning "dumpbin /DISASM failed, disassembly.txt may be incomplete"
}

# 4. Create Metadata
$metadata = @{
    timestamp = (Get-Date).ToString("o")
    stage     = "01_basics"
}
$metadata | ConvertTo-Json | Out-File "metadata.json"

Write-Host "Success. Artifacts located in $OutDir"
