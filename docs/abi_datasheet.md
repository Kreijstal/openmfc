# MSVC x64 ABI Datasheet (Phase 0A)

**Artifact Source:** `phase0a_msvc_artifacts.zip`  
**Date:** (Insert Date)

## 1. Class Layout & Padding
*Source: `layout.log`*

| Class | Size (bytes) | vptr Offset | Notes |
| :--- | :--- | :--- | :--- |
| `CStage1_Simple` | 16 | 0 | `m_value` at 8 (padding to 16) |
| `CStage4_Multi` | 40 | 0 (A), 16 (B) | Secondary base `CStage4_B` at offset 16 |

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

## 5. Multiple Inheritance Thunks
*Source: `disassembly.txt` (Search for `CStage4_Multi::FuncB`)*

* When `FuncB` is called, is there a `sub rcx, X` instruction before the jump?
* **Answer:** Layout shows `this adjustor: 16` for `FuncB` (secondary base at +16); adjustor thunk implied via vftable entry (function body trivial so no explicit prologue).

## 6. Calling Convention Notes
* Leaf virtuals in these probes are optimized to `ret 0`; no shadow-space allocation observed.
* Aggregate returns >8 bytes or non-POD use hidden pointer in `rcx`; `rax` mirrors `rcx`.
* Small POD (8 bytes) can return in `rax` using stack scratch space.

## 7. RTTI / Pure Virtuals
* Vftable entries show meta pointers in layout, but no RTTI symbols were emitted in `symbols.txt` dump (likely due to current dump step limits).
* `IStage7_Abstract` was not instantiated; no `_purecall` entries observed.
* `CStage7_NoVtable` present with vftable and size 8 despite `__declspec(novtable)`.
