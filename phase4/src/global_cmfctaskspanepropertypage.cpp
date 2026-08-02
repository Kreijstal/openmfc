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

namespace {

// Layout-faithful view of CMFCTasksPanePropertyPage (size 24).
struct S {
    void* vfptr;        // 0   CObject vtable pointer
    void* m_strName;    // 8   CString: single pointer to the shared string buffer
    void* m_pTaskPane;  // 16  CMFCTasksPane*
};

static_assert(sizeof(S) == 24, "CMFCTasksPanePropertyPage size must match harvested layout");
static_assert(offsetof(S, m_strName) == 8,   "m_strName @8");
static_assert(offsetof(S, m_pTaskPane) == 16, "m_pTaskPane @16");

} // namespace

// CMFCTasksPanePropertyPage::SetACCData -- reports this page's accessibility information to the
// tasks-pane accessibility layer. In real mfc140u the method fills the supplied
// CAccessibilityData with the page's accessible name (m_strName), a grouping/pane role, the
// default state, and the page's screen rectangle, then returns TRUE to signal that data was
// provided. The page rectangle is derived from the parent window's client area and converted to
// screen coordinates exactly as the framework does before the record is stored. Because the
// exact binary layout of CAccessibilityData is an internal, unexported feature-pack type that
// is not modeled in this drop-in, the field population cannot be performed byte-faithfully; we
// reproduce the observable coordinate transform (which is a no-op on a NULL / invalid handle)
// and return the framework's success value.
// Symbol: ?SetACCData@CMFCTasksPanePropertyPage@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI
impl__SetACCData_CMFCTasksPanePropertyPage__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData)
{
    if (!pThis || !pData) return FALSE;
    S* self = static_cast<S*>(pThis);
    CAccessibilityData* data = static_cast<CAccessibilityData*>(pData);
    data->m_strAccName = self->m_strName
        ? CString(static_cast<const wchar_t*>(self->m_strName))
        : CString();
    data->m_strAccValue.Empty();
    data->m_strDescription.Empty();
    data->m_strAccKeys.Empty();
    data->m_strAccHelp.Empty();
    data->m_strAccDefAction.Empty();
    data->m_nAccRole = 0x14; // ROLE_SYSTEM_GROUPING
    data->m_bAccState = 0;
    data->m_nAccHit = 0;

    // Faithful geometry step: derive the page's client rectangle from the parent window and
    // convert it to screen space, matching the framework's mapping before it records the
    // accessible location. CWnd::m_hWnd lives at offset 64 of the CWnd object.
    if (pParent != nullptr)
    {
        HWND hParent = *reinterpret_cast<HWND*>(static_cast<char*>(pParent) + 64);
        if (::IsWindow(hParent))
        {
            RECT rc;
            if (::GetClientRect(hParent, &rc))
            {
                POINT ptTL = { rc.left,  rc.top };
                POINT ptBR = { rc.right, rc.bottom };
                ::ClientToScreen(hParent, &ptTL);
                ::ClientToScreen(hParent, &ptBR);
                data->m_rectAccLocation = CRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y);
                data->m_ptAccHit.x = (ptTL.x + ptBR.x) / 2;
                data->m_ptAccHit.y = (ptTL.y + ptBR.y) / 2;
            }
        }
    }

    // The framework always reports that accessibility data was supplied for a page.
    return TRUE;
}
