# Agent Guide: Harvesting CI Artifacts with `gh`

This repo uses GitHub Actions to build and harvest ABI data. You can pull artifacts directly from the CLI or let the helper script poll for you.

## Prereqs
- Install GitHub CLI: `sudo apt install gh`
- Authenticate once: `gh auth login`

## Quick commands
- List recent runs (filter by workflow name if needed):
  - `gh run list --limit 5`
  - `gh run list --workflow phase0a_harvest.yml --limit 3`
- Watch a run until it finishes (exit status is forwarded):
  - `gh run watch <run-id> --exit-status --interval 10 --timeout 900` (use a longer timeout/interval to avoid premature exit)
- Download artifacts from a run:
  - `gh run download <run-id> -D /tmp/phase0a_msvc_artifacts`
    - Use `-n <artifact-name>` to pick a specific artifact if multiple exist.

## Automated polling helper
- Script: `phase0/abi_stress/scripts/harvest_check.sh`
  - Looks up the latest `phase0a_harvest.yml` run, waits for completion via `gh run watch`, downloads the `phase0a_msvc_artifacts` bundle, and prints key ABI snippets (layout and mangled names).
  - Usage (from repo root):
    - `./phase0/abi_stress/scripts/harvest_check.sh`
  - Output goes to `downloaded_artifacts/` by default (configurable inside the script).

## Typical flow
1) Trigger CI (e.g., push or `gh workflow run phase0a_harvest.yml`).
2) Wait for completion:
   - `gh run watch <run-id> --exit-status`
3) Download artifacts:
   - `gh run download <run-id> -D /tmp/phase0a_msvc_artifacts`
4) Inspect:
   - `ls /tmp/phase0a_msvc_artifacts`
   - `head -n 20 /tmp/phase0a_msvc_artifacts/exports.txt`

Keep artifacts outside the repo (e.g., under `/tmp` or a local `artifacts/` directory) to avoid committing binaries.***

## MSVC usage policy
- MSVC is used exclusively via GitHub Actions workflows; no local MSVC installs are assumed. Harvests and host builds run on Windows CI (e.g., `phase0a_harvest.yml`, `phase0a_verify_mingw.yml`, `phase0b_harvest_cobject.yml`, `phase0b_verify_cobject.yml`).
