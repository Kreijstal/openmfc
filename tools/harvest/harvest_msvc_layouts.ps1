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
$resolvedHeaders = [ordered]@{}
$started = Get-Date
$mfcIncludeDir = Join-Path $env:VCToolsInstallDir "ATLMFC/include"
$mfcHeaders = @()
if (Test-Path $mfcIncludeDir) {
    $mfcHeaders = @(Get-ChildItem -Path $mfcIncludeDir -Filter *.h -File)
}

foreach ($className in $selected) {
    $safeName = $className -replace '[^A-Za-z0-9_.-]', '_'
    $rawPath = Join-Path $rawDir "$safeName.txt"
    $objectPath = Join-Path $scratchDir "$safeName.obj"
    $baseArguments = @(
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
    $compilerOutput = & cl.exe @baseArguments 2>&1
    $exitCode = $LASTEXITCODE
    $layoutPattern = "\b$([regex]::Escape($className))\s+size\("
    $hasLayout = [bool]($compilerOutput | Select-String -Pattern $layoutPattern -Quiet)
    $selectedHeader = $null

    # Umbrella MFC headers intentionally leave some public classes forward
    # declared. Find the SDK header containing the concrete declaration and
    # retry it after the broad include set in the probe source.
    if (($exitCode -ne 0 -or -not $hasLayout) -and $mfcHeaders.Count -gt 0) {
        $declarationPattern = "\b(class|struct)\b[^;{}]*\b$([regex]::Escape($className))\b"
        $candidateHeaders = @(
            $mfcHeaders |
                Select-String -Pattern $declarationPattern |
                ForEach-Object { $_.Path } |
                Sort-Object -Unique
        )
        foreach ($headerPath in $candidateHeaders) {
            $headerName = Split-Path -Leaf $headerPath
            $retryArguments = @($baseArguments)
            $retryArguments = @(
                $retryArguments[0..5]
                "/DHARVEST_EXTRA_HEADER=<$headerName>"
                $retryArguments[6..($retryArguments.Count - 1)]
            )
            $retryOutput = & cl.exe @retryArguments 2>&1
            $retryExitCode = $LASTEXITCODE
            $retryHasLayout = [bool]($retryOutput | Select-String -Pattern $layoutPattern -Quiet)
            if ($retryExitCode -eq 0 -and $retryHasLayout) {
                $compilerOutput = $retryOutput
                $exitCode = $retryExitCode
                $hasLayout = $retryHasLayout
                $selectedHeader = $headerName
                break
            }
        }
    }

    $compilerOutput | Out-File -FilePath $rawPath -Encoding utf8
    if ($exitCode -eq 0 -and $hasLayout) {
        $succeeded += $className
        if ($selectedHeader) {
            $resolvedHeaders[$className] = $selectedHeader
        }
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
    resolved_headers = $resolvedHeaders
    failed = $failed
    started_utc = $started.ToUniversalTime().ToString("o")
    finished_utc = (Get-Date).ToUniversalTime().ToString("o")
    visual_studio_version = $env:VisualStudioVersion
    vc_tools_version = $env:VCToolsVersion
}
$summary | ConvertTo-Json -Depth 6 |
    Out-File -FilePath (Join-Path $OutputDir "summary.json") -Encoding utf8

Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $scratchDir
Write-Host "MSVC layouts shard $ShardIndex/${ShardCount}: $($succeeded.Count)/$($selected.Count) harvested"

if ($succeeded.Count -eq 0) {
    throw "no class layouts were harvested"
}

# A failed cl.exe attempt leaves LASTEXITCODE non-zero even when the harvest
# script intentionally records it and continues. Do not leak that status to
# the GitHub Actions step; the strict merge job owns completeness validation.
exit 0
