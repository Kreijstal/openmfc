#pragma once
// Shared internals of the former global_cmfccontrolcontainer.cpp translation unit.
// Definitions live in detail/CMFCControlContainerSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccontrolcontainer {} } }
using namespace openmfc::detail::cmfccontrolcontainer;
// OpenMFC: CMFCControlContainer exports.
//
// All 10 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the layout is mirrored with the file-local struct below: the parent CWnd*
// at +0x08, the CObArray at +0x10 (m_pData +0x18 / m_nSize +0x20 / m_nMaxSize
// +0x28 / m_nGrowBy +0x30) and the unmodeled CMapWordToOb at +0x38.  The
// subclassed controls are CWnd-derived, so each control's m_hWnd sits at +0x40.
// The stub constructors leave the object state uninitialized, so every pointer
// access is guarded and a zeroed object behaves like a fresh retail object.
//
// Faithfully transcribed: IsSubclassedFeaturePackControl, ReSubclassControl,
// SubclassDlgControls and UTF8ToString.  The CMapWordToOb at +0x38 and the
// 11-control class factory in CreateDlgControl are unmodeled and marked
// "TODO(clean-room)".
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//---------------------------------------------------------------------------
// Cross-file impl helpers, plus forward declarations of exports defined
// further down in this file.
//---------------------------------------------------------------------------
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(
    CWnd* pThis, HWND__* p0);
extern "C" int MS_ABI impl__GetControlData_CMFCControlContainer__QEAAHGAEAKAEAPEAE_Z(
    void* pThis, unsigned short nID, unsigned long& cbData, unsigned char*& lpData);
extern "C" CWnd* MS_ABI impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(
    void* pThis, const wchar_t* lpszClassName);


namespace openmfc { namespace detail { namespace cmfccontrolcontainer {
struct CContainer {
    char      _pad0[0x8];        // 0x00: CObject vtable
    void*     m_pWnd;            // 0x08: parent CWnd* (ctor arg)
    char      _pad10[0x18 - 0x10];   // 0x10: CObArray vtable (opaque)
    void**    m_pControls;       // 0x18: CObArray::m_pData
    long long m_nControls;       // 0x20: CObArray::m_nSize
    long long m_nMaxControls;    // 0x28: CObArray::m_nMaxSize
    long long m_nGrowBy;         // 0x30: CObArray::m_nGrowBy
    // 0x38: CMapWordToOb (unmodeled; accessed by byte offset only)
};
} } }


// CObArray::SetAtGrow equivalent used by SubclassDlgControls.  Grows the array
// by doubling (or to 4 when empty) and appends the control.  Safe on a
// zeroed object (m_pData == nullptr, m_nSize == m_nMaxSize == 0).
namespace openmfc { namespace detail { namespace cmfccontrolcontainer {
void ContainerArrayAppend(CContainer* s, void* pObj);
} } }











