$ScriptDir = $PSScriptRoot
$SourceDir = Join-Path $ScriptDir ".."
$OutDir    = Join-Path $SourceDir "bin"

if (!(Test-Path $OutDir)) { New-Item -ItemType Directory -Path $OutDir | Out-Null }

$Src = "$SourceDir\msvc_host.cpp"
$Exe = "$OutDir\msvc_host.exe"

Write-Host "Building MSVC Host..."
cl.exe /nologo /EHsc /MD $Src /Fe:$Exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "Success: $Exe"
} else {
    Write-Error "Build failed"
    exit 1
}
