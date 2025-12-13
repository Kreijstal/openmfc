# OpenMFC Phase 4 Implementation Checklist

This tracks actual implementations in phase4 (not stubs).

## Completed ✅

### Exception Throwing
- [x] `AfxThrowMemoryException` - Full MSVC-compatible C++ exception
  - Catchable as `CMemoryException*`, `CException*`, or `CObject*`
  - Works with real MFC headers (`<afx.h>`)
  - Tested on Windows with MSVC-compiled binaries

### Version Functions
- [x] `AfxGetDllVersion` - Returns 0x0E00 (MFC 14.0)

---

## In Progress 🔄

### Exception RTTI Infrastructure
- [x] `CMemoryException*` TypeDescriptor (`.PEAVCMemoryException@@`)
- [x] `CException*` TypeDescriptor (`.PEAVCException@@`)
- [x] `CObject*` TypeDescriptor (`.PEAVCObject@@`)
- [x] ThrowInfo, CatchableType, CatchableTypeArray structures
- [x] Dummy type_info vftable fallback

---

## Not Started (Stubs Only) ❌

### Other Exception Functions
- [ ] `AfxThrowNotSupportedException`
- [ ] `AfxThrowArchiveException`
- [ ] `AfxThrowFileException`
- [ ] `AfxThrowInvalidArgException`
- [ ] `AfxThrowOleDispatchException`
- [ ] `AfxThrowOleException`
- [ ] `AfxThrowResourceException`
- [ ] `AfxThrowUserException`
- [ ] `AfxThrowDaoException`
- [ ] `AfxThrowDBException`
- [ ] `AfxThrowInternetException`

### Core Classes
- [ ] `CObject` - Base class with RTTI
- [ ] `CString` - String handling
- [ ] `CException` - Exception base class
- [ ] `CCmdTarget` - Command target
- [ ] `CWnd` - Window base class
- [ ] `CWinThread` / `CWinApp` - Application classes

### Application Functions
- [ ] `AfxGetApp`
- [ ] `AfxGetMainWnd`
- [ ] `AfxGetInstanceHandle`
- [ ] `AfxGetResourceHandle`
- [ ] `AfxGetThread`
- [ ] `AfxWinInit`
- [ ] `AfxRegisterClass`
- [ ] `AfxMessageBox`

### Window/Dialog Classes
- [ ] `CFrameWnd`
- [ ] `CDialog`
- [ ] `CMDIFrameWnd`
- [ ] `CMDIChildWnd`
- [ ] All control classes (CButton, CEdit, CListBox, etc.)

### Document-View
- [ ] `CDocument`
- [ ] `CView`
- [ ] `CDocTemplate`

### GDI
- [ ] `CDC`
- [ ] `CGdiObject` and subclasses
- [ ] `CPoint`, `CSize`, `CRect`

### Collections
- [ ] `CArray`, `CList`, `CMap`
- [ ] `CStringArray`, `CStringList`

### Threading
- [ ] `AfxBeginThread`
- [ ] Synchronization objects

---

## Test Results

| Test | Status | Description |
|------|--------|-------------|
| `test_exception_simple` | ✅ Pass | `catch(...)` catches exception |
| `test_exception_typed` | ✅ Pass | `catch(CMemoryException*)` works |
| `test_exception_mfc` | ✅ Pass | Real MFC headers (`<afx.h>`) |
| `test_version` | ✅ Pass | `AfxGetDllVersion()` returns 0x0E00 |

---

## Progress Summary

| Category | Implemented | Total | Status |
|----------|-------------|-------|--------|
| Exception Throwing | 1 | 12 | 8% |
| Version/Info | 1 | 1 | 100% |
| Core Classes | 0 | 6 | 0% |
| App Functions | 0 | 8 | 0% |
| Window/Dialog | 0 | 20+ | 0% |
| GDI | 0 | 15+ | 0% |
| Collections | 0 | 11 | 0% |

**Overall: ~2% complete (2 functions with real implementations)**

---

## Exit Criteria

- [ ] All `AfxThrowXxxException` functions work
- [ ] Basic CString operations work
- [ ] CObject RTTI works (`IsKindOf`, `GetRuntimeClass`)
- [ ] Simple MFC app links and runs basic code
