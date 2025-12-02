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
