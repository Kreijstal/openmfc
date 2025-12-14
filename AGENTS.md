# Agent Guide: Harvesting CI Artifacts with `gh`

This repo uses GitHub Actions to build and harvest ABI data. You can pull artifacts directly from the CLI or let the helper script poll for you.

- Agents should push their own changes; do not ask the user to push for you.

## Prereqs
- Install GitHub CLI: `sudo apt install gh`
- Authenticate once: `gh auth login`

## Quick commands
- List recent runs (filter by workflow name if needed):
  - `gh run list --limit 5`
  - `gh run list --workflow phase0a_harvest.yml --limit 3`
- Watch a run until it finishes (exit status is forwarded):
  - `gh run watch <run-id> --exit-status` (avoid `--interval`; no `--timeout` flag exists; runs can take 20+ minutes—be patient)
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
- OpenMFC uses **Windows/MSVC ABI** (not Itanium/GCC ABI)
- **Do NOT use MinGW g++ for C++ code** - MinGW uses Itanium ABI which is incompatible
- For local testing, use **msvc-wine** to get real MSVC on Linux
- CI runs on Windows with real MSVC (e.g., `phase0a_harvest.yml`, `phase0a_verify_mingw.yml`)

## Local MSVC Testing with msvc-wine
To test MSVC linking locally on Linux:

```bash
# Install prerequisites
sudo apt-get install -y wine64 python3 msitools ca-certificates

# Clone msvc-wine
git clone --depth=1 https://github.com/mstorsjo/msvc-wine.git /tmp/msvc-wine

# Download and install MSVC (accepts license automatically)
cd /tmp/msvc-wine
./vsdownload.py --dest /tmp/msvc --accept-license
./install.sh /tmp/msvc

# Add MSVC to PATH
export PATH=/tmp/msvc/bin/x64:$PATH

# Create import library from .def file
lib.exe /DEF:build-phase4/openmfc.def /OUT:openmfc.lib /MACHINE:X64

# Compile and link test with MSVC
cl.exe /nologo /EHsc tests/test_mfc_smoke.cpp openmfc.lib /Fe:test_mfc_smoke.exe

# Run with Wine (copy DLL to same directory)
cp build-phase4/openmfc.dll .
wine test_mfc_smoke.exe
```

**Why MSVC is required for tests:**
- Tests use `__declspec(dllimport)` which generates MSVC-mangled names
- OpenMFC DLL exports MSVC-mangled symbols (e.g., `?AfxThrowMemoryException@@YAXXZ`)
- MinGW would generate Itanium-mangled names (e.g., `_Z23AfxThrowMemoryExceptionv`) - incompatible!

## No-binaries rule
- Never commit generated binaries or CI artifacts to the repo.
- Keep downloads under `/tmp` or `artifacts/` (ignored) and out of git.
- A pre-commit hook in `.githooks/pre-commit` blocks binary blobs; enable it locally with:
  - `git config core.hooksPath .githooks`
