#pragma once
// Shared internals of the former global_cmfctaskspanepropertypage.cpp translation unit.
// Definitions live in detail/CMFCTasksPanePropertyPageSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctaskspanepropertypage {} } }
using namespace openmfc::detail::cmfctaskspanepropertypage;
// OpenMFC: real implementation of the exported CMFCTasksPanePropertyPage virtual override.
//
// CMFCTasksPanePropertyPage (afxtaskspane.h) is a small CObject-derived value object that
// represents a single named "page" (a scrollable collection of task groups) shown inside a
// CMFCTasksPane. It carries only the page's display name and a back-pointer to the owning
// CMFCTasksPane:
//
//   CMFCTasksPanePropertyPage(LPCTSTR lpszName, CMFCTasksPane* pTaskPane)
//       : m_strName(lpszName), m_pTaskPane(pTaskPane) {}
//   virtual ~CMFCTasksPanePropertyPage() { m_pTaskPane = NULL; }
//   virtual BOOL SetACCData(CWnd* pParent, CAccessibilityData& data);
//
// Exactly one symbol is exported for this class: the virtual SetACCData override, which hands
// the page's accessibility information to the tasks-pane accessibility layer. No constructor /
// destructor is exported, so the vptr is never installed here and the layout-faithful struct
// below exists purely to anchor the static_asserts and to read the page name.
//
// Layout harvested via cl.exe /d1reportSingleClassLayout, size(24):
//   0  {vfptr}       (CObject vtable pointer)
//   8  m_strName     (CString -> single LPWSTR data pointer)
//  16  m_pTaskPane   (CMFCTasksPane*)

#include <windows.h>
#include <cstddef>
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful view of CMFCTasksPanePropertyPage (size 24).
namespace openmfc { namespace detail { namespace cmfctaskspanepropertypage {
struct S_Cmfctaskspanepropertypage {
    void* vfptr;        // 0   CObject vtable pointer
    void* m_strName;    // 8   CString: single pointer to the shared string buffer
    void* m_pTaskPane;  // 16  CMFCTasksPane*
};
} } }



