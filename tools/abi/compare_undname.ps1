param(
    [string[]] $TargetDlls = @(),
    [string] $OutDir = "undname_compare"
)

if (-not $TargetDlls -or $TargetDlls.Count -eq 0) {
    $vcRedist = "$Env:VCToolsRedistDir\x64"
    $TargetDlls = @(
        Join-Path $vcRedist "msvcp140.dll",
        Join-Path $vcRedist "vcruntime140.dll",
        Join-Path $vcRedist "concrt140.dll"
    )
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$results = @()

foreach ($dll in $TargetDlls) {
    if (-not (Test-Path $dll)) {
        Write-Warning "Missing DLL: $dll"
        continue
    }

    $dumpPath = Join-Path $OutDir ("exports_" + (Split-Path $dll -Leaf) + ".txt")
    & dumpbin /EXPORTS $dll > $dumpPath 2>&1

    $symbols = @()
    Get-Content $dumpPath | ForEach-Object {
        if ($_ -match "^\s*\d+\s+\d+\s+[0-9A-Fa-f]+\s+(\S+)\s*$") {
            $name = $Matches[1]
            if ($name.StartsWith("?")) { $symbols += $name }
        }
    }

    $dllResult = [PSCustomObject]@{
        Dll        = $dll
        Total      = $symbols.Count
        Matches    = 0
        Mismatches = 0
    }

    $compareLog = Join-Path $OutDir ("compare_" + (Split-Path $dll -Leaf) + ".txt")
    foreach ($sym in $symbols) {
        $und = (& undname $sym 2>&1).Trim()
        $py  = (& python tools/abi/open_undname.py $sym 2>&1).Trim()
        $match = ($und -eq $py)
        if ($match) { $dllResult.Matches++ } else { $dllResult.Mismatches++ }
        "$sym`n  undname: $und`n  open:   $py`n  match: $match`n" | Add-Content $compareLog
    }

    $results += $dllResult
}

$summary = Join-Path $OutDir "summary.txt"
$results | Format-Table | Out-String | Set-Content $summary

Write-Host "Comparison complete. Summary:"
Get-Content $summary
