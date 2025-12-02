param(
    [string]$OutDir = "$PWD/artifacts/phase0b/harvest"
)

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$Root = Resolve-Path (Join-Path $ScriptDir "..")

if (-not (Test-Path $OutDir)) { New-Item -ItemType Directory -Path $OutDir -Force | Out-Null }

$TmpObj = Join-Path $OutDir "cobject.obj"
$TmpCpp = Join-Path $OutDir "cobject_temp.cpp"
$TmpLog = Join-Path $OutDir "dumpbin_symbols.txt"
$DefOut = Join-Path $OutDir "mfc140u.generated.def"
$Readme = Join-Path $OutDir "README.txt"

Write-Host "Generating temporary source..."
@'
#include "afx.h"
int main() { CObject o; return o.IsKindOf(o.GetRuntimeClass()) ? 0 : 1; }
'@ | Out-File -FilePath $TmpCpp -Encoding ASCII

Write-Host "Compiling with cl..."
cl.exe /nologo /c /std:c++17 /EHsc /MD /I"$($Root)\include" /I"$($Root)\phase0b\include" "$TmpCpp" /Fo:"$TmpObj"
if ($LASTEXITCODE -ne 0) { Write-Error "Compile failed"; exit $LASTEXITCODE }

Write-Host "Dumping symbols..."
dumpbin /SYMBOLS "$TmpObj" > "$TmpLog"

Write-Host "Extracting CObject decorated names..."
$symbols = Get-Content "$TmpLog" | Select-String "\?CObject" -CaseSensitive
$exports = @()
foreach ($s in $symbols) {
    $parts = $s.ToString().Split(" ", [System.StringSplitOptions]::RemoveEmptyEntries)
    $decorated = $parts[-1]
    if ($decorated -match "\?CObject") {
        $exports += $decorated
    }
}
$exports = $exports | Sort-Object -Unique

"LIBRARY openmfc" | Out-File -FilePath $DefOut -Encoding ASCII
"EXPORTS" | Out-File -FilePath $DefOut -Encoding ASCII -Append
foreach ($e in $exports) {
    $e | Out-File -FilePath $DefOut -Encoding ASCII -Append
}

Write-Host "Wrote generated def to $DefOut"
Write-Host "Symbol dump saved to $TmpLog"

"CObject symbol harvest" | Out-File -FilePath $Readme -Encoding ASCII
"Generated: $(Get-Date -Format o)" | Out-File -FilePath $Readme -Encoding ASCII -Append
"Source file: $TmpCpp" | Out-File -FilePath $Readme -Encoding ASCII -Append
"Object file: $TmpObj" | Out-File -FilePath $Readme -Encoding ASCII -Append
"Symbols: $TmpLog" | Out-File -FilePath $Readme -Encoding ASCII -Append
"DEF: $DefOut" | Out-File -FilePath $Readme -Encoding ASCII -Append
