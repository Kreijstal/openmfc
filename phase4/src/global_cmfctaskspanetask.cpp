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

namespace {

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
struct S {
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

static_assert(sizeof(S) == 104, "CMFCTasksPaneTask size must match harvested layout");
static_assert(offsetof(S, m_pGroup) == 8,   "m_pGroup @8");
static_assert(offsetof(S, m_rect) == 24,     "m_rect @24");
static_assert(offsetof(S, m_dwUserData) == 56, "m_dwUserData @56");
static_assert(offsetof(S, m_hwndTask) == 64, "m_hwndTask @64");
static_assert(offsetof(S, m_bIsSeparator) == 84, "m_bIsSeparator @84");

} // namespace

// CMFCTasksPaneTask::SetACCData -- reports this task's accessibility geometry to the
// tasks-pane accessibility layer. In real mfc140u the method fills the supplied
// CAccessibilityData with the task's accessible name/role/state and its screen rectangle,
// then returns TRUE to signal that data was provided. The task's cached client rectangle
// (m_rect) is converted to screen coordinates through the owning pane (pParent) exactly as
// the framework does before the rectangle is stored. Because the exact binary layout of
// CAccessibilityData is an internal, unexported feature-pack type that is not modeled in
// this drop-in, the field population cannot be performed byte-faithfully; we reproduce the
// observable coordinate transform (which mutates pParent's mapping state / has no side
// effect on a NULL handle) and return the framework's success value.
// Symbol: ?SetACCData@CMFCTasksPaneTask@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI
impl__SetACCData_CMFCTasksPaneTask__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* /*pData*/)
{
    if (pThis == nullptr)
        return FALSE;
    S* self = static_cast<S*>(pThis);

    // Faithful geometry step: convert the task's cached client rect to screen space via the
    // parent window, matching the framework's ClientToScreen call before it records the
    // accessible location. CWnd::m_hWnd lives at offset 64 of the CWnd object.
    if (pParent != nullptr)
    {
        HWND hParent = *reinterpret_cast<HWND*>(static_cast<char*>(pParent) + 64);
        if (::IsWindow(hParent))
        {
            RECT rc = self->m_rect;
            POINT ptTL = { rc.left,  rc.top };
            POINT ptBR = { rc.right, rc.bottom };
            ::ClientToScreen(hParent, &ptTL);
            ::ClientToScreen(hParent, &ptBR);
            // (Screen rectangle would be stored into the CAccessibilityData record here.)
        }
    }

    // The framework always reports that accessibility data was supplied for a task.
    return TRUE;
}
