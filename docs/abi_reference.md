# ABI Reference Notes (Phase 0)

This document collects observations about the MSVC C++ ABI as seen from
small reference DLLs built in CI.

## Phase 0A: `CReferenceTest`

Reference class:

```cpp
class CReferenceTest {
public:
    CReferenceTest();
    virtual ~CReferenceTest();
    virtual int GetValue() const;
    virtual void SetValue(int v);
private:
    int m_value;
};
```

Artifacts from CI for this phase:

* `abi_ref/abi_ref.dll`
* `abi_ref/abi_ref_exports.txt` (`dumpbin /EXPORTS`)
* `abi_ref/abi_ref_symbols.txt` (`dumpbin /SYMBOLS`)
* `abi_ref/abi_ref_undname.txt` (`undname` output for key symbols)

As we learn more (mangled names, vtable symbol patterns, etc.) we record it here.

## Phase 0C: Cross-Platform Consumption (MinGW DLL, MSVC Host)

Validated pattern:

* Build the shim (`shim_test.dll`) with MinGW using explicit vtable-bearing class (`CReferenceTest`) and factory functions (`CreateRef`/`DestroyRef`).
* On Windows, derive a `.def` from the MinGW DLL via `dumpbin /exports` and feed it to `lib.exe /def:shim_test.def /machine:x64 /out:shim_test.lib`.
* An MSVC consumer linking against `shim_test.lib` can call the factory, then invoke `GetValue`/`SetValue` virtually across the DLL boundary with correct state propagation.

Notes:

* Manual vptr/vtable arrays from earlier phases interoperate provided the mangled exports match MSVC decoration.
* `__attribute__((ms_abi))` remains required for freestanding thunks/assembly shims on MinGW when matching MSVC call/return conventions.
* `__asm__` labels are a reliable way to pin MSVC-mangled names from MinGW/Clang when the front-end would otherwise mangle differently.
