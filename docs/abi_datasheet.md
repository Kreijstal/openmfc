# MSVC x64 ABI Datasheet (Phase 0A)

**Artifact Source:** `phase0a_msvc_artifacts.zip`  
**Date:** 2025-12-02

Evidence files live in `/tmp/phase0a_msvc_artifacts`: `layout.log`, `disassembly.txt`, `symbols.txt`, `symbols_objects.txt`, `exports.txt`, `README.txt`.

---

## 6.6.1 RVO Strategies (Return Value Optimisation)
*Primary source: `disassembly.txt` (see `Ret*` in `CStage5_RVO`)*  
`RetNonPod8`, `RetPod16`, `RetPod24` store through `rcx` then return `rax=rcx` (hidden pointer). `RetPod8` uses scratch at `[rsp+8]` and returns `rax` directly.

| Type | Size | Traits | Return Storage | Hidden Ptr? | Evidence |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `Pod8` | 8 | POD | `rax` | No | `disassembly.txt` lines 1D0C–1D21 |
| `Pod16` | 16 | POD | via `[rcx]` | Yes | lines 1CD0–1CE2 |
| `Pod24` | 24 | POD | via `[rcx]` | Yes | lines 1CE8–1D02 |
| `NonPod8` | 8 | non-trivial dtor | via `[rcx]` | Yes | lines 1CB8–1CC8 |

Critical answer: non-trivial 8-byte return uses hidden pointer (`rcx`, `rax=rcx`).

---

## 6.6.2 Class Layout & Padding
*Primary source: `layout.log`*

| Class | Size | Alignment | vptr Offset | Key Offsets | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `CStage1_Simple` | 16 | 8 | 0 | `m_value` @8 | vptr @0, padded to 16 |
| `CStage4_Multi` | 40 | 8 | A@0 / B@16 | base B @16 | Primary A at 0, secondary base at +16 |
| `CStage7_NoVtable` | 8 | 8 | 0 | n/a | vptr present despite `novtable` |

---

## 6.6.3 Vtable Ordering & Destructors
*Primary source: `layout.log`, `symbols.txt`, `abi_vtable_slots.md`*

- `CStage1_Simple`: slot0 dtor, slot1 `GetValue`, slot2 `SetValue`.
- `CStage3_Base`: scalar deleting dtor present (`??_GCStage3_Base@@UEAAPEAXI@Z` in `symbols.txt`).
- `CStage2_Signatures`: six overloads in declaration order.
- `CCovariantBase/Derived`: slot0 `Clone`, slot1 dtor.

---

## 6.6.4 Name Mangling – Overloads & Pointers
*Primary source: `exports.txt`*

- `DoWork(int)`: `?Func@CStage2_Signatures@@UEAAXH@Z`
- `DoWork(long)`: `?Func@CStage2_Signatures@@UEAAXJ@Z`
- `DoWork(double)`: `?Func@CStage2_Signatures@@UEAAXN@Z`
- `DoPtr(void*)`: `?Func@CStage2_Signatures@@UEAAXPEAX@Z`
- `DoPtr(const void*)`: `?Func@CStage2_Signatures@@UEAAXPEBX@Z`
- `StaticFunc()`: `?StaticFunc@CStage2_Signatures@@SAXXZ`

Pattern: pointer cv-qualifiers encode as `PEA` vs `PEB`; overloads follow argument encoding.

---

## 6.6.5 Special Member Functions (CStage6_Modern)
*Primary source: `exports.txt`*

- Default ctor: `??0CStage6_Modern@@QEAA@XZ`
- Move ctor: `??0CStage6_Modern@@QEAA@$$QEAV0@@Z`
- Move assign: `??4CStage6_Modern@@QEAAAEAV0@$$QEAV0@@Z`
- Copy ctor/assign: present in `symbols.txt` (`??0...AEBV0@@Z`, `??4...AEBV0@@Z`)
- Destructor: not defined (no `??1` export)
- Scalar deleting dtor: n/a (not emitted)

---

## 6.6.6 Qualifier & `noexcept` Mangling
*Primary source: `exports.txt`*

- `ConstFunc() const`: `?ConstFunc@CStage6_Modern@@UEBAXXZ`
- `NoExceptFunc() noexcept`: `?NoExceptFunc@CStage6_Modern@@UEAAXXZ`
- Volatile/const-volatile not present in this stage.
`noexcept` changes the mangled name (different from non-noexcept placeholder).

---

## 6.6.7 Covariant Return Types
*Primary source: `exports.txt`, `symbols.txt`*

- Impl symbol: `?Clone@CCovariantDerived@@UEAAPEAV1@XZ` (returns `CCovariantDerived*`).
- Base symbol: `?Clone@CCovariantBase@@UEAAPEAV1@XZ` (for base).
- Vtable shares single slot; no separate thunk emitted (single inheritance, no adjustment).

---

## 6.6.8 MI Thunk Logic (`this` Adjustment)
*Primary source: `layout.log`, `disassembly.txt`*

- `CStage4_Multi::FuncB` recorded with `this adjustor: 16` (secondary base offset).
- Disassembly shows trivial body (`ret 0`); adjustor handled via vftable entry/slot selection rather than explicit `add/sub rcx` in body.

---

## 6.6.9 Pure Virtuals & `__declspec(novtable)`
*Primary source: `symbols.txt`, `symbols_objects.txt`, `layout.log`*

- `IPure`/`IStage7_Abstract`: `_purecall` imported; vftable slot points to purecall (no concrete body).
- `CStage7_NoVtable`: still emits vftable at size 8 despite `__declspec(novtable)`.

---

## 6.6.10 x64 Calling Convention Details
*Primary source: `disassembly.txt` (calling-conv probe at lines 21A4–21DB)*

- Shadow space not materialized in trivial leaf bodies, but 5th/6th args are read from `[rsp+28h]` and `[rsp+30h]` (`SumFive`, `CCStdCall`, `CCThiscall`).
- Register usage: RCX/RDX/R8/R9 for first four ints; additional on stack.
- Non-volatiles not touched in trivial probes; `_M@...` helper shows standard prologue with pushes and `sub rsp,38h` (aligned to 16).
- Stack alignment preserved in `_M@...` frame; SEH-style prologue visible there.

---

## 6.6.11 RTTI Metadata & COL
*Primary source: `symbols_objects.txt`*

- COL examples: `??_R4CStage1_Simple@@6B@`, `??_R4CStage4_Multi@@6BCStage4_B@@@`, `??_R4CCovariantDerived@@6B@`.
- Type descriptors: `??_R0?AVCStage1_Simple@@@8`, `??_R0?AVCStage4_Multi@@@8`, etc.
- Vtable regions include COL pointer at slot [-1] (per layout log meta entries).

---

## 6.6.12 Phase 0A Knowledge Tests (answered)
- Q1 (NonPod8 return): Hidden pointer in `rcx`; `RetNonPod8` writes to `[rcx]`, returns `rax=rcx` (`disassembly.txt` 1CB8–1CC8).
- Q2 (Move-assign mangling): `??4CStage6_Modern@@QEAAAEAV0@$$QEAV0@@Z` from `exports.txt`.
- Q3 (MI `FuncB` adjustment): Secondary base at +16; adjustor noted in `layout.log` entry for `CStage4_Multi::FuncB` (B vftable).

---

## SEH / EH Snapshot
- Trivial stage functions show no SEH prologues; `_M@YAXPEAX_K1P6AX0@Z@Z` helper exhibits standard frame setup (`push` non-volatiles, `sub rsp,38h`).
- `_purecall` imported for pure virtuals; RTTI present via `??_R*` symbols.
---
