#pragma once
// Shared internals of the former global_cmfccustomizebutton.cpp translation unit.
// Definitions live in detail/CMFCCustomizeButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccustomizebutton {} } }
using namespace openmfc::detail::cmfccustomizebutton;
// OpenMFC: CMFCCustomizeButton exports.
//
// All 9 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the members are reached through raw offsets.  It derives from
// CMFCToolBarMenuButton (296 bytes, declared in afxmfc.h) with its own members
// starting at +0x128; the whole object is 0x198 bytes.  The bodies that need
// the CMFCPopupMenu / visual-manager machinery (CreatePopupMenu / OnDraw /
// DrawCheckBox / OnCalculateSize) or the CMFCToolBar internals behind
// ButtonToIndex / OnCancelMode are kept as conservative type-correct versions
// with a `// TODO(clean-room)` marker.
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
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(
    void* pThis, const CMFCToolBarButton* src);
extern "C" CMFCToolBarButton* MS_ABI
impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(
    CMFCToolBar* pThis, int iButton);


namespace openmfc { namespace detail { namespace cmfccustomizebutton {
struct CZButton : public CMFCToolBarMenuButton {
    UINT    m_nCommandID;      // 0x128
    int     m_b12c;            // 0x12c
    int     m_b130;            // 0x130
    int     m_b134;            // 0x134
    CString m_str138;          // 0x138
    void*   m_pToolbar;        // 0x140
    int     m_n148;            // 0x148
    int     m_n14c;            // 0x14c
    int     m_n150;            // 0x150
    int     m_b154;            // 0x154
    int     m_n158;            // 0x158
    void*   m_p160;            // 0x160: embedded vtable (opaque)
    void*   m_p168;            // 0x168
    void*   m_p170;            // 0x170
    void*   m_p178;            // 0x178
    void*   m_p180;            // 0x180
    void*   m_p188;            // 0x188
    long long m_n190;          // 0x190 (10)
};
} } }











