#pragma once
// Shared internals of the former global_cdataexchange.cpp translation unit.
// Definitions live in detail/CDataExchangeSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cdataexchange {} } }
using namespace openmfc::detail::cdataexchange;
// OpenMFC real implementation of CDataExchange.
//
// CDataExchange is the non-polymorphic context object threaded through every
// DDX_/DDV_ routine during CWnd::UpdateData. It owns no vtable; it simply
// tracks the dialog window, the save/validate direction, and bookkeeping for
// the last control touched (used by Fail() to restore focus/selection before
// aborting the exchange).
//
// Layout (cl.exe /d1reportSingleClassLayout) — class CDataExchange size(24):
//    0 | BOOL  m_bSaveAndValidate
//    4 | <alignment padding>
//    8 | CWnd* m_pDlgWnd
//   16 | UINT  m_idLastControl
//   20 | BOOL  m_bEditLastControl
//
// Exports (all aliased in the .def to the impl_ symbols below):
//   ??0CDataExchange@@QEAA@PEAVCWnd@@H@Z            (ctor)
//   ?Fail@CDataExchange@@QEAAXXZ
//   ?PrepareCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
//   ?PrepareEditCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
//   ?PrepareOleCtrl@CDataExchange@@QEAAPEAVCOleControlSite@@H@Z

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view of CDataExchange (size 24).
namespace openmfc { namespace detail { namespace cdataexchange {
struct S_Cdataexchange {
    BOOL  m_bSaveAndValidate;  // 0
    char  _pad0[4];            // 4  (alignment before the pointer)
    void* m_pDlgWnd;           // 8  (CWnd*)
    UINT  m_idLastControl;     // 16
    BOOL  m_bEditLastControl;  // 20
};
} } }


// CWnd::m_hWnd lives at offset 64 in the MFC x64 layout. Recover the raw
// window handle of the dialog container so we can drive the real ::GetDlgItem /
// focus APIs exactly as MFC does internally.
namespace openmfc { namespace detail { namespace cdataexchange {
inline HWND DlgHwnd(const S_Cdataexchange* self)
{
    if (self->m_pDlgWnd == nullptr)
        return nullptr;
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(self->m_pDlgWnd) + 64);
}
} } }


// The real DLL's CDataExchange::Fail() calls AfxThrowUserException() (throw new
// CUserException) to unwind out of the DDX pass. openmfc exports that function as
// an MS_ABI thunk that throws through the MSVC C++ exception-ABI bridge
// (mfc_exceptions.cpp), so an MSVC client's catch(CUserException*)/catch(CException*)
// catches it. Call it directly here rather than throwing a mingw-local type that
// the client could never catch.
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ();





