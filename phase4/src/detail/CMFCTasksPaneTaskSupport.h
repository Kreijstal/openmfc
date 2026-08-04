#pragma once
// Shared internals of the former global_cmfctaskspanetask.cpp translation unit.
// Definitions live in detail/CMFCTasksPaneTaskSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctaskspanetask {} } }
using namespace openmfc::detail::cmfctaskspanetask;
// OpenMFC: real implementation of the exported CMFCTasksPaneTask virtual override.
//
// CMFCTasksPaneTask (afxtaskspane.h) is a CObject-derived value object that describes a
// single clickable task (or a label / separator) shown inside a CMFCTasksPane group.
// It carries the task's display name, icon index, command id, window handle and a set of
// presentation flags (visible / enabled / bold / separator) plus its cached hit rectangle.
//
// Exactly one symbol is exported for this class: the virtual SetACCData override, which
// hands the task's accessibility information to the tasks-pane accessibility layer. No
// constructor / destructor is exported, so the vptr is never installed here and the class
// layout below exists purely to anchor the static_asserts and to read the flag fields.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view of CMFCTasksPaneTask, harvested size(104).
//   0  {vfptr}                     (CObject vtable pointer)
//   8  m_pGroup                    (CMFCTasksPaneTaskGroup*)
//  16  m_strName                   (CString -> single LPWSTR data pointer)
//  24  m_rect                      (CRect / RECT, 16 bytes)
//  40  m_nIcon                     (int)
//  44  m_nWindowHeight             (int)
//  48  m_uiCommandID               (UINT)
//  52  <pad for 8-byte alignment>
//  56  m_dwUserData                (DWORD_PTR)
//  64  m_hwndTask                  (HWND)
//  72  m_bAutoDestroyWindow        (BOOL)
//  76  m_bVisible                  (BOOL)
//  80  m_bEnabled                  (BOOL)
//  84  m_bIsSeparator              (BOOL)
//  88  m_bIsBold                   (BOOL)
//  92  m_clrText                   (COLORREF)
//  96  m_clrTextHot                (COLORREF)
// 100  <tail pad to 104>
namespace openmfc { namespace detail { namespace cmfctaskspanetask {
struct S_Cmfctaskspanetask {
    void*      vfptr;                // 0
    void*      m_pGroup;             // 8
    void*      m_strName;            // 16  (CString: single pointer to shared buffer)
    RECT       m_rect;              // 24
    int        m_nIcon;             // 40
    int        m_nWindowHeight;     // 44
    unsigned   m_uiCommandID;       // 48
    unsigned   _pad0;               // 52
    DWORD_PTR  m_dwUserData;        // 56
    HWND       m_hwndTask;          // 64
    BOOL       m_bAutoDestroyWindow;// 72
    BOOL       m_bVisible;          // 76
    BOOL       m_bEnabled;          // 80
    BOOL       m_bIsSeparator;      // 84
    BOOL       m_bIsBold;           // 88
    COLORREF   m_clrText;           // 92
    COLORREF   m_clrTextHot;        // 96
    unsigned   _pad1;               // 100
};
} } }



