---
description: Harvest ABI data from CI - trigger phase0a_harvest.yml, download artifacts, show ABI snippets
argument-hint: "[trigger-new-run]"
---
Run the Phase 0A ABI harvest workflow end-to-end (see AGENTS.md "Harvesting CI Artifacts"):

1. **Prereq check**: Confirm `gh` is installed and authenticated (`gh auth status`). If not, install with `sudo apt install gh` and authenticate with `gh auth login` (ask the user to complete any interactive auth).
2. **Trigger CI** (only if the user asked for a fresh run, i.e. arg contains "trigger"): dispatch the workflow at the current checkout ref, then capture the matching run ID:
   ```bash
   HARVEST_REF="$(git symbolic-ref --quiet --short HEAD || git rev-parse HEAD)"
   HARVEST_SHA="$(git rev-parse HEAD)"
   gh workflow run phase0a_harvest.yml --ref "$HARVEST_REF"
   until RUN_ID="$(gh run list --workflow phase0a_harvest.yml --event workflow_dispatch --commit "$HARVEST_SHA" --limit 1 --json databaseId --jq '.[0].databaseId // empty')" && [ -n "$RUN_ID" ]; do sleep 2; done
   ```
3. **Poll for completion**: Use the helper script with the captured run ID so it watches and downloads that run rather than the latest unrelated run:
   ```bash
   LOCAL_DIR="${LOCAL_DIR:-/tmp/phase0a_msvc_artifacts}"
   RUN_ID="$RUN_ID" LOCAL_DIR="$LOCAL_DIR" ./phase0/abi_stress/scripts/harvest_check.sh
   ```
   Output goes to `/tmp/phase0a_msvc_artifacts` by default. If isolation is needed, use a dedicated phase0a-prefixed temporary destination, for example `LOCAL_DIR="$(mktemp -d /tmp/phase0a_msvc_artifacts.XXXXXX)"`.
4. **If the helper script fails** (e.g., run still queued or no runs), fall back to manual steps:
   - `gh run view "$RUN_ID"`
   - `gh run watch "$RUN_ID" --exit-status` (runs can take 20+ minutes — be patient)
   - `gh run download "$RUN_ID" -n phase0a_msvc_artifacts -D "$LOCAL_DIR"`
5. **Inspect and report**:
   - `head -n 20 "$LOCAL_DIR/exports.txt"` — check MSVC-mangled names (e.g. `?AfxThrowMemoryException@@YAXXZ`)
   - Layout snippet from `"$LOCAL_DIR/layout.log"` (class `CStage1_Simple` vptr layout)
   - Report the mangled names and layout findings to the user.

Keep artifacts in `/tmp` — never commit binaries or CI artifacts to the repo (no-binaries rule).
