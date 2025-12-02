# MSVC x64 ABI Datasheet (Phase 0A)

**Artifact Source:** `phase0a_msvc_artifacts.zip`  
**Date:** (Insert Date)

## 1. Class Layout & Padding
*Source: `layout.log`*

| Class | Size (bytes) | vptr Offset | Notes |
| :--- | :--- | :--- | :--- |
| `CStage1_Simple` | 16 | 0 | `m_value` at 8 (padding to 16) |
| `CStage4_Multi` | 40 | 0 (A), 16 (B) | Secondary base `CStage4_B` at offset 16 |
| `CStage7_NoVtable` | 8 | 0 | vftable present despite `__declspec(novtable)` |

## Vtable Ordering (examples)
* `CStage1_Simple`: slot0 dtor, slot1 `GetValue`, slot2 `SetValue` (layout vftable listing).
* `CStage2_Signatures`: six overloads in declaration order (layout vftable listing).

## 2. Return Value Optimization (RVO)
*Source: `disassembly.txt` (Search for `CStage5_RVO::Ret*`)*

| Type | Size | Return Storage | Hidden Ptr? |
| :--- | :--- | :--- | :--- |
| `Pod8` | 8 | `RAX` (stack scratch, returned via `rax`) | No |
| `Pod16` | 16 | Stored via `rcx` out-param | Yes |
| `Pod24` | 24 | Stored via `rcx` out-param | Yes |
| `NonPod8` | 8 | Stored via `rcx` out-param | Yes |

**Critical Question:** Does `NonPod8` (8 bytes + dtor) return in RAX or via hidden pointer?  
**Answer:** Hidden pointer (writes to `[rcx]`, returns `rax=rcx`).

## 3. Special Member Mangling
*Source: `exports.csv` (Output of parse_exports.py)*

| Member | Signature | Mangled Name Pattern |
| :--- | :--- | :--- |
| Constructor | `CStage6_Modern()` | `??0CStage6_Modern@@QEAA@XZ` |
| Dtor | `~CStage6_Modern()` | Not present (not defined) |
| Move Ctor | `CStage6_Modern(&&)` | `??0CStage6_Modern@@QEAA@$$QEAV0@@Z` |
| Move Assign | `operator=(&&)` | `??4CStage6_Modern@@QEAAAEAV0@$$QEAV0@@Z` |

## 4. Scalar Deleting Destructor
*Source: `symbols.txt` or `exports.csv`*

* Does `CStage3_Base` have a symbol containing `??_G`?
* **Answer:** Yes (layout.log shows `__delDtor` entry for `CStage3_Base` vftable).

## 4b. Qualifiers / noexcept
* `ConstFunc` (`const`): `?ConstFunc@CStage6_Modern@@UEBAXXZ` (const encoded in signature).
* `NoExceptFunc` (`noexcept`): `?NoExceptFunc@CStage6_Modern@@UEAAXXZ` (noexcept encoded).

## 5. Multiple Inheritance Thunks
*Source: `disassembly.txt` (Search for `CStage4_Multi::FuncB`)*

* When `FuncB` is called, is there a `sub rcx, X` instruction before the jump?
* **Answer:** Layout shows `this adjustor: 16` for `FuncB` (secondary base at +16); adjustor thunk implied via vftable entry (function body trivial so no explicit prologue).

## 6. Calling Convention Notes
* Leaf virtuals here are optimized to `ret 0`; no shadow space visible in trivial bodies (no `sub rsp,20h`).
* Aggregate returns >8 bytes or non-POD use hidden pointer in `rcx`; `rax` mirrors `rcx`.
* Small POD (8 bytes) can return in `rax` using stack scratch space at `[rsp+8]`.

## 7. RTTI / Pure Virtuals
* Layout shows meta pointers for vftables (e.g., `&CStageX_meta`); RTTI symbols are present in object-level dump (`symbols_objects.txt`), e.g., `??_R4CStage1_Simple@@6B@` (COL) and `??_R0?AVCStage1_Simple@@@8` (type descriptor).
* `IStage7_Abstract` not instantiated; `_purecall` appears as undefined import in `symbols_objects.txt`, and the IPure vftable slots pure virtuals accordingly.
* `CStage7_NoVtable` present with vftable and size 8 despite `__declspec(novtable)`.

## 8. Covariant Returns
* `CCovariantBase::Clone` returns `CCovariantBase*`; `CCovariantDerived::Clone` returns `CCovariantDerived*` (same slot, no thunk emitted in current build).
* Disassembly shows direct vtable entries; no separate thunk symbol for base adjustment (single inheritance).

## 9. SEH / EH Notes
* All functions here are trivial; disassembly shows no SEH prologues/epilogues.
* MinGW exception sanity test (`Shim_SafeException`) catches internally; host sees return 42 (CI passes).

## Knowledge Checks (from Section 8)
* Non-POD 8-byte return: hidden pointer (`rcx`), `rax=rcx`, data stored via pointer (`RetNonPod8`).
* `CStage6_Modern::operator=(CStage6_Modern&&)`: `??4CStage6_Modern@@QEAAAEAV0@$$QEAV0@@Z` (exports).
* `CStage4_Multi::FuncB` `this` adjustment: +16 (secondary base offset) per layout; vftable entry implies adjustor thunk, though function body is trivial.
