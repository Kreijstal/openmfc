---
description: Build phase4 DLL and verify MSVC ABI compat locally (clang-cl + Wine, no cl.exe needed)
argument-hint: "[test-file]"
---
Verify MSVC ABI compatibility locally using the clang-cl + Wine approach (AGENTS.md Option A). Never use MinGW g++ for C++ code — it uses the Itanium ABI and is incompatible.

1. **Build the phase4 DLL**:
   ```bash
   ./phase4/scripts/build_phase4.sh
   ```
2. **Ensure MSVC SDK headers/libs are available** (download once, cached under /tmp):
   ```bash
   MSVC_WINE_REV=514f8ea34842cd6d831804d0e9658d3a32870ae1
   if [ ! -d /tmp/msvc-wine/.git ]; then git clone --filter=blob:none --no-checkout https://github.com/mstorsjo/msvc-wine.git /tmp/msvc-wine; fi
   git -C /tmp/msvc-wine fetch --depth=1 origin "$MSVC_WINE_REV"
   git -C /tmp/msvc-wine checkout --detach "$MSVC_WINE_REV"
   test "$(git -C /tmp/msvc-wine rev-parse HEAD)" = "$MSVC_WINE_REV"
   if [ ! -d /tmp/msvc-dl ]; then
     python3 /tmp/msvc-wine/vsdownload.py --accept-license --dest /tmp/msvc-dl
     /tmp/msvc-wine/install.sh /tmp/msvc-dl
   fi
   ```
3. **Build & run the MSVC ABI compat test** (the script handles case-sensitivity automatically):
   ```bash
   MSVC_DIR=/tmp/msvc-dl ./scripts/build_msvc_clang_test.sh "${1:-phase4/tests/test_mfc_abi_compat.cpp}"
   ```
4. **Report results**:
   - Test pass/fail and any mismatched symbols (expected MSVC-mangled name vs actual export).
   - If verification scripts exist, optionally cross-check exports: `./scripts/verify_msvc_abi_local.sh` or similar.
   - Summarize which exports/layouts were verified and any failures.

If the DLL wasn't built or the test binary is missing, diagnose the build first. Keep generated binaries out of git.
