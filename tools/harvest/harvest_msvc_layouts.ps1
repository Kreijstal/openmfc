param(
    [string]$Targets = "tools/harvest/rtti_layout_targets.txt",
    [string]$ProbeSource = "tools/harvest/msvc_layout_probe.cpp",
    [string]$OutputDir = "artifacts/msvc_layouts",
    [int]$ShardIndex = 0,
    [int]$ShardCount = 1
)

$ErrorActionPreference = "Stop"

if ($ShardCount -lt 1 -or $ShardIndex -lt 0 -or $ShardIndex -ge $ShardCount) {
    throw "invalid shard $ShardIndex/$ShardCount"
}
if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    throw "cl.exe is not available; run from an MSVC developer environment"
}
if (-not (Test-Path $Targets)) {
    throw "target list not found: $Targets"
}
if (-not (Test-Path $ProbeSource)) {
    throw "probe source not found: $ProbeSource"
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null
$rawDir = Join-Path $OutputDir "raw"
$scratchDir = Join-Path $OutputDir "scratch"
New-Item -ItemType Directory -Force -Path $rawDir, $scratchDir | Out-Null

$allTargets = @(
    Get-Content $Targets |
        ForEach-Object { $_.Trim() } |
        Where-Object { $_ -and -not $_.StartsWith("#") }
)
$selected = @()
for ($i = 0; $i -lt $allTargets.Count; $i++) {
    if (($i % $ShardCount) -eq $ShardIndex) {
        $selected += $allTargets[$i]
    }
}

$succeeded = @()
$failed = @()
$started = Get-Date

foreach ($className in $selected) {
    $safeName = $className -replace '[^A-Za-z0-9_.-]', '_'
    $rawPath = Join-Path $rawDir "$safeName.txt"
    $objectPath = Join-Path $scratchDir "$safeName.obj"
    $arguments = @(
        "/nologo",
        "/c",
        "/EHsc",
        "/MD",
        "/D_AFXDLL",
        "/DHARVEST_CLASS=$className",
        "/d1reportSingleClassLayout$className",
        $ProbeSource,
        "/Fo$objectPath"
    )

    Write-Host "[$($succeeded.Count + $failed.Count + 1)/$($selected.Count)] $className"
    $compilerOutput = & cl.exe @arguments 2>&1
    $exitCode = $LASTEXITCODE
    $compilerOutput | Out-File -FilePath $rawPath -Encoding utf8

    $hasLayout = Select-String -Path $rawPath -SimpleMatch "$className size(" -Quiet
    if ($exitCode -eq 0 -and $hasLayout) {
        $succeeded += $className
    }
    else {
        $failed += [ordered]@{
            class = $className
            compiler_exit_code = $exitCode
            layout_found = [bool]$hasLayout
            log = "raw/$safeName.txt"
        }
    }
    Remove-Item -Force -ErrorAction SilentlyContinue $objectPath
}

$summary = [ordered]@{
    shard_index = $ShardIndex
    shard_count = $ShardCount
    total_inventory = $allTargets.Count
    selected = $selected.Count
    succeeded_count = $succeeded.Count
    failed_count = $failed.Count
    succeeded = $succeeded
    failed = $failed
    started_utc = $started.ToUniversalTime().ToString("o")
    finished_utc = (Get-Date).ToUniversalTime().ToString("o")
    visual_studio_version = $env:VisualStudioVersion
    vc_tools_version = $env:VCToolsVersion
}
$summary | ConvertTo-Json -Depth 6 |
    Out-File -FilePath (Join-Path $OutputDir "summary.json") -Encoding utf8

Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $scratchDir
Write-Host "MSVC layouts shard $ShardIndex/$ShardCount: $($succeeded.Count)/$($selected.Count) harvested"

if ($succeeded.Count -eq 0) {
    throw "no class layouts were harvested"
}
