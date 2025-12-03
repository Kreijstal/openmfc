# Compare committed DEF against freshly harvested MSVC symbols for CObject.
param(
    [string]$OutDir = "phase0b/artifacts/harvest_check"
)

$ErrorActionPreference = "Stop"

$expectedDef = "phase0b/def/mfc140u.def"
if (-not (Test-Path $expectedDef)) {
    throw "Expected DEF not found: $expectedDef"
}

if (Test-Path $OutDir) {
    Remove-Item -Recurse -Force $OutDir
}
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

Write-Host "Harvesting CObject symbols into $OutDir ..."
./phase0b/scripts/harvest_cobject_symbols.ps1 -OutDir $OutDir | Write-Host

$generatedDef = Join-Path $OutDir "mfc140u.generated.def"
if (-not (Test-Path $generatedDef)) {
    throw "Generated DEF missing: $generatedDef"
}

$expected = Get-Content $expectedDef
$generated = Get-Content $generatedDef

$diff = Compare-Object $expected $generated -SyncWindow 1
if ($diff) {
    Write-Host "DEF mismatch between committed and generated versions:"
    $diff | Format-Table | Out-String | Write-Host
    throw "DEF files differ"
}

Write-Host "DEF sync check passed."
