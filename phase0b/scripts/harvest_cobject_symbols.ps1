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
$ExpectedExports = @(
    "??_7CObject@@6B@",
    "??0CObject@@QEAA@XZ",
    "??1CObject@@UEAA@XZ",
    "??_GCObject@@UEAAPEAXI@Z",
    "??_ECObject@@UEAAPEAXI@Z",
    "??2CObject@@SAPEAX_K@Z",
    "??3CObject@@SAXPEAX@Z",
    "?GetRuntimeClass@CObject@@UEBAPEAUCRuntimeClass@@XZ",
    "?IsKindOf@CObject@@QEBAHPEBUCRuntimeClass@@@Z",
    "?Serialize@CObject@@UEAAXAEAVCArchive@@@Z",
    "?CreateObject@CObject@@SAPEAV1@XZ"
)

Write-Host "Generating temporary source..."
@'
#include "afx.h"
class CArchive {};

int main() {
    // Static CreateObject (should be nullptr for CObject)
    CObject* p = CObject::CreateObject();
    if (!p) {
        p = new CObject();
    }

    p->Serialize(*reinterpret_cast<CArchive*>(nullptr));
    auto cls = p->GetRuntimeClass();
    p->IsKindOf(cls);

    // Trigger vector deleting destructor symbols
    CObject* arr = new CObject[1];
    delete[] arr;

    delete p;
    return 0;
}
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
    $line = $s.ToString()
    if ($line -match "(__imp_)?(\?+[^\\s]*CObject[^\s]*)") {
        $exports += $Matches[2]
    }
}
$exports = $exports + $ExpectedExports
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
