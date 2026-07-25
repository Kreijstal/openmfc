---
description: Harvest ABI data from CI - trigger phase0a_harvest.yml, download artifacts, show ABI snippets
argument-hint: "[trigger-new-run]"
---
Run the Phase 0A ABI harvest workflow end-to-end (see AGENTS.md "Harvesting CI Artifacts"):

1. **Prereq check**: Confirm `gh` is installed and authenticated (`gh auth status`). If not, install with `sudo apt install gh` and authenticate with `gh auth login` (ask the user to complete any interactive auth).
2. **Trigger CI** (only if the user asked for a fresh run, i.e. arg contains "trigger"): run `gh workflow run phase0a_harvest.yml` from the repo root.
3. **Poll for completion**: Use the helper script, which finds the latest run, waits via `gh run watch`, downloads the `phase0a_msvc_artifacts` bundle, and prints the key ABI snippets:
   ```bash
   ./phase0/abi_stress/scripts/harvest_check.sh
   ```
   Output goes to `/tmp/phase0a_msvc_artifacts` by default (set `LOCAL_DIR=` to override).
4. **If the helper script fails** (e.g., run still queued or no runs), fall back to manual steps:
   - `gh run list --workflow phase0a_harvest.yml --limit 1`
   - `gh run watch <run-id> --exit-status` (runs can take 20+ minutes — be patient)
   - `gh run download <run-id> -n phase0a_msvc_artifacts -D /tmp/phase0a_msvc_artifacts`
5. **Inspect and report**:
   - `head -n 20 /tmp/phase0a_msvc_artifacts/exports.txt` — check MSVC-mangled names (e.g. `?AfxThrowMemoryException@@YAXXZ`)
   - Layout snippet from `/tmp/phase0a_msvc_artifacts/layout.log` (class `CStage1_Simple` vptr layout)
   - Report the mangled names and layout findings to the user.

Keep artifacts in `/tmp` — never commit binaries or CI artifacts to the repo (no-binaries rule).
