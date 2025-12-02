# MSVC x64 ABI Datasheet (Phase 0A)

**Artifact Source:** `phase0a_msvc_artifacts.zip`  
**Date:** (Insert Date)

## 1. Class Layout & Padding
*Source: `layout.log`*

| Class | Size (bytes) | vptr Offset | Notes |
| :--- | :--- | :--- | :--- |
| `CStage1_Simple` | 16 | 0 | 4-byte member padded to 8 bytes |
| `CStage4_Multi` | ? | ? | **TODO:** Check layout.log |

## 2. Return Value Optimization (RVO)
*Source: `disassembly.txt` (Search for `CStage5_RVO::Ret*`)*

| Type | Size | Return Storage | Hidden Ptr? |
| :--- | :--- | :--- | :--- |
| `Pod8` | 8 | `RAX` (Example) | No |
| `Pod16` | 16 | **TODO** | ? |
| `Pod24` | 24 | **TODO** | ? |
| `NonPod8` | 8 | **TODO** | ? |

**Critical Question:** Does `NonPod8` (8 bytes + dtor) return in RAX or via hidden pointer?  
**Answer:** _______________

## 3. Special Member Mangling
*Source: `exports.csv` (Output of parse_exports.py)*

| Member | Signature | Mangled Name Pattern |
| :--- | :--- | :--- |
| Constructor | `CStage6_Modern()` | `??0CStage6_Modern@@QEAA@XZ` (Example) |
| Dtor | `~CStage6_Modern()` | **TODO** |
| Move Ctor | `CStage6_Modern(&&)` | **TODO** |
| Move Assign | `operator=(&&)` | **TODO** |

## 4. Scalar Deleting Destructor
*Source: `symbols.txt` or `exports.csv`*

* Does `CStage3_Base` have a symbol containing `??_G`?
* **Answer:** _______________

## 5. Multiple Inheritance Thunks
*Source: `disassembly.txt` (Search for `CStage4_Multi::FuncB`)*

* When `FuncB` is called, is there a `sub rcx, X` instruction before the jump?
* **Answer:** _______________
