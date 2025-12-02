param(
    [string[]] $TargetDlls = @(),
    [string] $OutDir = "undname_compare"
)

function Resolve-RedistDll([string] $name) {
    $vcRedist = "$Env:VCToolsRedistDir\x64"
    if (-not $vcRedist) { return $null }
    Get-ChildItem -Path $vcRedist -Filter $name -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
}

$hadFailure = $false

if (-not $TargetDlls -or $TargetDlls.Count -eq 0) {
    $TargetDlls = @("msvcp140.dll", "vcruntime140.dll", "concrt140.dll") | ForEach-Object {
        $resolved = Resolve-RedistDll $_
        if ($null -ne $resolved) { $resolved } else {
            Write-Error "Missing DLL: $_ under VCToolsRedistDir" -ErrorAction Continue
            $hadFailure = $true
            $null
        }
    }
    $TargetDlls = $TargetDlls | Where-Object { $_ }
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
        $und = ((& undname $sym 2>&1) -join "`n").Trim()
        $py  = ((& python tools/abi/open_undname.py $sym 2>&1) -join "`n").Trim()
        $match = ($und -eq $py)
        if ($match) { $dllResult.Matches++ } else { $dllResult.Mismatches++ }
        "$sym`n  undname: $und`n  open:   $py`n  match: $($match)`n" | Add-Content $compareLog
    }

    $results += $dllResult
}

$summary = Join-Path $OutDir "summary.txt"
$summaryLines = @("Dll,Total,Matches,Mismatches")
foreach ($r in $results) {
    $summaryLines += "$($r.Dll),$($r.Total),$($r.Matches),$($r.Mismatches)"
}
$summaryLines | Set-Content $summary

Write-Host "Comparison complete. Summary:"
Get-Content $summary

if ($hadFailure) {
    Write-Error "compare_undname encountered errors (see above)" -ErrorAction Stop
}

# Fail the job if any mismatches were found
if ($results | Where-Object { $_.Mismatches -gt 0 }) {
    Write-Error "compare_undname: mismatches detected" -ErrorAction Stop
}
