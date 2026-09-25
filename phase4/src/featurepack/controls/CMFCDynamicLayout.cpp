// CMFCDynamicLayout — OpenMFC implementation.
// Sources: global_static_utils.cpp

#include "detail/CMFCDynamicLayoutSupport.h"
#include <cstddef>
#include <cstring>
#include <cwchar>

// ---------------------------------------------------------------------------
// CMFCDynamicLayout object layout.
//
// The class (retail afxlayout.h:33, `class CMFCDynamicLayout : public CObject`)
// is not declared anywhere in the OpenMFC headers (include/openmfc), so it is
// pinned here from the retail constructor ??0CMFCDynamicLayout@@QEAA@XZ,
// RVA 0x768d0 (mfc140u), which stores exactly:
//   +0x00 vfptr                     (CObject)
//   +0x08 0                         m_pHostWnd
//   +0x10 0 (one qword)             m_MinSize  (CSize, 2 x int)
//   +0x18 CList vfptr               m_listWnd  (CList<AFX_DYNAMIC_LAYOUT_ITEM*, ...>)
//   +0x20 0  +0x28 0  +0x30 0  +0x38 0  +0x40 0    head / tail / count / free / blocks
//   +0x48 10                        m_listWnd.m_nBlockSize
// sizeof == 0x50, which agrees with the m_nObjectSize 80 of the CMFCDynamicLayout
// descriptor in featurepack/controls/RuntimeClasses.cpp.  The member order
// matches afxlayout.h (m_pHostWnd, m_MinSize, m_listWnd).
//
// DEPENDENCY -- NOT YET SATISFIED: OpenMFC's own ??0CMFCDynamicLayout@@QEAA@XZ
// (featurepack/controls/Thunks.cpp) does NOT perform the stores listed above;
// it only records pThis in a side map.  An object built through that export
// therefore has uninitialised m_pNodeHead/m_nCount/m_pNodeFree, and AddItem,
// Adjust and FindItem below walk whatever the allocator left there.  These
// bodies assume the retail-faithful constructor (and a destructor that frees
// the 0x38-byte items and the list's CPlex blocks, retail RVA 0x76960
// (mfc140u)); that change is requested from the owner of Thunks.cpp.
//
// CList<T*,T*> has the CPtrList layout (vfptr, m_pNodeHead, m_pNodeTail,
// m_nCount, m_pNodeFree, m_pBlocks, m_nBlockSize; CNode = {pNext, pPrev, data},
// 0x18 bytes).  Retail AddItem appends through an unexported CList<>::AddTail
// instantiation (0x180007988 in mfc140) whose NewNode carves CPlex blocks of
// m_nBlockSize nodes of 0x18 bytes -- the same algorithm and node layout as
// CPtrList::AddTail, so this file appends through the CPtrList::AddTail thunk.
//
// AFX_DYNAMIC_LAYOUT_ITEM is forward-declared only in afxlayout.h.  Its layout
// comes from AddItem(HWND), RVA 0x76bb0 (mfc140u): the operator new request is
// 0x38 bytes, it stores the HWND at +0x00, zeroes the four qwords +0x08..+0x20,
// and stores the MoveSettings qword at +0x28 and the SizeSettings qword at
// +0x30.  PrepareItem (0x76ea0, mfc140u) writes doubles to +0x08/+0x10/+0x18/
// +0x20 and AdjustItemRect (0x77010, mfc140u) reads them back.  The member
// names below are OpenMFC's; retail's are not in any header.
// ---------------------------------------------------------------------------
class CWnd;
struct CRuntimeClass;

namespace {

struct DynLayoutPoint { double x; double y; };

struct DynLayoutItem {
    HWND           m_hWnd;     // 0x00
    DynLayoutPoint m_ptInit;   // 0x08  initial position, minus the ratio share
    DynLayoutPoint m_szInit;   // 0x18  initial size, minus the ratio share
    DynRatio       m_move;     // 0x28  CMFCDynamicLayout::MoveSettings
    DynRatio       m_size;     // 0x30  CMFCDynamicLayout::SizeSettings
};
static_assert(offsetof(DynLayoutItem, m_hWnd)   == 0x00, "AFX_DYNAMIC_LAYOUT_ITEM::hwnd");
static_assert(offsetof(DynLayoutItem, m_ptInit) == 0x08, "AFX_DYNAMIC_LAYOUT_ITEM point");
static_assert(offsetof(DynLayoutItem, m_szInit) == 0x18, "AFX_DYNAMIC_LAYOUT_ITEM size");
static_assert(offsetof(DynLayoutItem, m_move)   == 0x28, "AFX_DYNAMIC_LAYOUT_ITEM move settings");
static_assert(offsetof(DynLayoutItem, m_size)   == 0x30, "AFX_DYNAMIC_LAYOUT_ITEM size settings");
static_assert(sizeof(DynLayoutItem) == 0x38, "AFX_DYNAMIC_LAYOUT_ITEM is 0x38 bytes (operator new in AddItem)");

struct DynLayoutNode {
    DynLayoutNode* pNext;
    DynLayoutNode* pPrev;
    DynLayoutItem* data;
};
static_assert(sizeof(DynLayoutNode) == 0x18, "CList<T*> CNode is 0x18 bytes");

struct S_CMFCDynamicLayout {
    void*          vfptr;          // 0x00
    CWnd*          m_pHostWnd;     // 0x08
    SIZE           m_MinSize;      // 0x10
    void*          list_vfptr;     // 0x18  m_listWnd (CList, CObject-derived)
    DynLayoutNode* m_pNodeHead;    // 0x20
    DynLayoutNode* m_pNodeTail;    // 0x28
    INT_PTR        m_nCount;       // 0x30
    DynLayoutNode* m_pNodeFree;    // 0x38
    void*          m_pBlocks;      // 0x40
    INT_PTR        m_nBlockSize;   // 0x48
};
static_assert(offsetof(S_CMFCDynamicLayout, m_pHostWnd)  == 0x08, "CMFCDynamicLayout::m_pHostWnd");
static_assert(offsetof(S_CMFCDynamicLayout, m_MinSize)   == 0x10, "CMFCDynamicLayout::m_MinSize");
static_assert(offsetof(S_CMFCDynamicLayout, list_vfptr)  == 0x18, "CMFCDynamicLayout::m_listWnd");
static_assert(offsetof(S_CMFCDynamicLayout, m_pNodeHead) == 0x20, "m_listWnd.m_pNodeHead");
static_assert(offsetof(S_CMFCDynamicLayout, m_nCount)    == 0x30, "m_listWnd.m_nCount");
static_assert(offsetof(S_CMFCDynamicLayout, m_nBlockSize)== 0x48, "m_listWnd.m_nBlockSize");
static_assert(sizeof(S_CMFCDynamicLayout) == 0x50, "sizeof(CMFCDynamicLayout) == 80");

// CWnd and CObject are only handled through pointers here: the OpenMFC class
// headers are not included (they drag in C++ symbols this TU must not link).
// Retail reads CWnd::m_hWnd at +0x40 throughout (e.g. Create, RVA 0x76a10
// (mfc140u): `cmp %rax,0x40(%rdx)`), which is also where OpenMFC's CWnd keeps it
// (include/openmfc/afxwin.h, "m_hWnd at offset 64").
inline HWND WndHwnd(const CWnd* pWnd) {
    return *reinterpret_cast<const HWND*>(reinterpret_cast<const char*>(pWnd) + 0x40);
}

inline S_CMFCDynamicLayout* DL(void* p) { return static_cast<S_CMFCDynamicLayout*>(p); }
inline const S_CMFCDynamicLayout* DL(const void* p) { return static_cast<const S_CMFCDynamicLayout*>(p); }

// MoveSettings / SizeSettings are 8-byte aggregates passed by value in a
// register under the MS x64 ABI (AddItem stores r8 / r9 whole); they arrive
// here as unsigned long long and are copied bit-for-bit.
inline DynRatio UnpackRatio(unsigned long long v) {
    DynRatio r;
    std::memcpy(&r, &v, sizeof(r));
    return r;
}

// CMFCPropertySheet members read by GetHostWndRect (0x293980, mfc140u).  The
// offsets are the ones the retail body uses; the names are the afxpropertysheet.h
// declarations they line up with (m_look is the first CMFCPropertySheet member,
// directly after the 0x190-byte CPropertySheet base; m_nBarWidth, m_nActivePage,
// m_nHeaderHeight are three consecutive ints).
const std::size_t kSheet_m_look          = 0x190;
const std::size_t kSheet_m_nBarWidth     = 0x71d0;
const std::size_t kSheet_m_nHeaderHeight = 0x71d8;
inline int SheetInt(const void* pSheet, std::size_t off) {
    return *reinterpret_cast<const int*>(static_cast<const char*>(pSheet) + off);
}

} // namespace

// --- thunks this file calls (definitions verified in the files named) ------
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);                   // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                           // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__GetScrollPos_CWnd__QEBAHH_Z(const CWnd* pThis, int nBar);                    // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);  // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const void* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPropertyPage__SAPEAUCRuntimeClass__XZ();          // featurepack/propertygrid/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ();         // featurepack/propertygrid/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ();                 // core/view/CFormView.cpp
extern "C" void* MS_ABI impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(void* pThis, void* newElement); // core/collections/CPtrList.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                            // detail/MemcoreSupport.cpp (::operator new)

// --- sibling thunks in this file (signatures derived from the mangled names) -
extern "C" int  MS_ABI impl__AddItem_CMFCDynamicLayout__QEAAHPEAUHWND____UMoveSettings_1_USizeSettings_1__Z(void* pThis, HWND hWnd, unsigned long long moveSettings, unsigned long long sizeSettings);
extern "C" unsigned int MS_ABI impl__AdjustItemRect_CMFCDynamicLayout__IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM__AEAVCRect___Z(const void* pThis, DynLayoutItem* pItem, RECT* pRectItem);
extern "C" void MS_ABI impl__CorrectItem_CMFCDynamicLayout__IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(const void* pThis, DynLayoutItem* pItem);
extern "C" void MS_ABI impl__GetHostWndRect_CMFCDynamicLayout__QEBAXAEAVCRect___Z(const void* pThis, RECT* pRect);
extern "C" RECT* MS_ABI impl__GetItemRect_CMFCDynamicLayout__IEBA_AVCRect__AEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(const void* pThis, RECT* pRet, DynLayoutItem* pItem);
extern "C" int  MS_ABI impl__PrepareItem_CMFCDynamicLayout__IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(const void* pThis, DynLayoutItem* pItem);


// Symbol: ?MoveHorizontal@CMFCDynamicLayout@@SA?AUMoveSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__MoveHorizontal_CMFCDynamicLayout__SA_AUMoveSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = nPercent; ret->y = 0; return ret;
}
// Symbol: ?MoveVertical@CMFCDynamicLayout@@SA?AUMoveSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__MoveVertical_CMFCDynamicLayout__SA_AUMoveSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = 0; ret->y = nPercent; return ret;
}
// Symbol: ?MoveHorizontalAndVertical@CMFCDynamicLayout@@SA?AUMoveSettings@1@HH@Z
extern "C" MS_ABI DynRatio* impl__MoveHorizontalAndVertical_CMFCDynamicLayout__SA_AUMoveSettings_1_HH_Z(DynRatio* ret, int nXPercent, int nYPercent)
{
    ret->x = nXPercent; ret->y = nYPercent; return ret;
}
// Symbol: ?MoveNone@CMFCDynamicLayout@@SA?AUMoveSettings@1@XZ
extern "C" MS_ABI DynRatio* impl__MoveNone_CMFCDynamicLayout__SA_AUMoveSettings_1_XZ(DynRatio* ret)
{
    ret->x = 0; ret->y = 0; return ret;
}
// Symbol: ?SizeHorizontal@CMFCDynamicLayout@@SA?AUSizeSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__SizeHorizontal_CMFCDynamicLayout__SA_AUSizeSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = nPercent; ret->y = 0; return ret;
}
// Symbol: ?SizeVertical@CMFCDynamicLayout@@SA?AUSizeSettings@1@H@Z
extern "C" MS_ABI DynRatio* impl__SizeVertical_CMFCDynamicLayout__SA_AUSizeSettings_1_H_Z(DynRatio* ret, int nPercent)
{
    ret->x = 0; ret->y = nPercent; return ret;
}
// Symbol: ?SizeHorizontalAndVertical@CMFCDynamicLayout@@SA?AUSizeSettings@1@HH@Z
extern "C" MS_ABI DynRatio* impl__SizeHorizontalAndVertical_CMFCDynamicLayout__SA_AUSizeSettings_1_HH_Z(DynRatio* ret, int nXPercent, int nYPercent)
{
    ret->x = nXPercent; ret->y = nYPercent; return ret;
}
// Symbol: ?SizeNone@CMFCDynamicLayout@@SA?AUSizeSettings@1@XZ
extern "C" MS_ABI DynRatio* impl__SizeNone_CMFCDynamicLayout__SA_AUSizeSettings_1_XZ(DynRatio* ret)
{
    ret->x = 0; ret->y = 0; return ret;
}
// Symbol: ?AddItem@CMFCDynamicLayout@@QEAAHIUMoveSettings@1@USizeSettings@1@@Z
// Transcribed from RVA 0x76b50 (mfc140u):
//   if (m_pHostWnd == NULL || m_pHostWnd->m_hWnd == NULL) return FALSE;
//   CWnd* pWnd = m_pHostWnd->GetDlgItem(nID);
//   return AddItem(pWnd->GetSafeHwnd(), moveSettings, sizeSettings);
extern "C" int MS_ABI impl__AddItem_CMFCDynamicLayout__QEAAHIUMoveSettings_1_USizeSettings_1__Z(
        void* pThis, unsigned int nID, unsigned long long moveSettings, unsigned long long sizeSettings) {
    CWnd* pHost = DL(pThis)->m_pHostWnd;
    if (pHost == nullptr || WndHwnd(pHost) == nullptr)
        return FALSE;
    CWnd* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pHost, static_cast<int>(nID));
    return impl__AddItem_CMFCDynamicLayout__QEAAHPEAUHWND____UMoveSettings_1_USizeSettings_1__Z(
        pThis, pWnd != nullptr ? WndHwnd(pWnd) : nullptr, moveSettings, sizeSettings);
}

// Symbol: ?AddItem@CMFCDynamicLayout@@QEAAHPEAUHWND__@@UMoveSettings@1@USizeSettings@1@@Z
// Transcribed from RVA 0x76bb0 (mfc140u):
//   if (hWnd == NULL || !::IsWindow(hWnd)) return FALSE;
//   if (!::IsChild(m_pHostWnd->GetSafeHwnd(), hWnd)) return FALSE;
//   if (FindItem(hWnd) != NULL) return FALSE;        // inlined list walk
//   pItem = new AFX_DYNAMIC_LAYOUT_ITEM(hWnd, move, size);   // operator new(0x38)
//   CorrectItem(*pItem);
//   if (PrepareItem(*pItem)) m_listWnd.AddTail(pItem);
//   return TRUE;
// Retail returns TRUE even when PrepareItem fails, and in that case the item is
// neither linked nor freed (it leaks); that is reproduced as-is.
// Deviations: (1) retail also calls m_pHostWnd->IsKindOf(RUNTIME_CLASS(CFormView))
// when m_pHostWnd is non-NULL and never tests the result (eax is overwritten by
// the next instruction); only the virtual GetRuntimeClass call inside IsKindOf
// is lost by omitting it.  (2) retail's ??2@YAPEAX_K@Z (RVA 0x2840, mfc140)
// retries malloc through the new handler it reads from AfxGetModuleThreadState()
// (+0x50) and returns NULL when there is no handler or the handler returns 0.
// Retail's new-expression then passes the NULL item on to CorrectItem, which
// dereferences it.  OpenMFC's ::operator new thunk is a plain malloc, and a
// failed allocation returns FALSE here instead of crashing.
extern "C" int MS_ABI impl__AddItem_CMFCDynamicLayout__QEAAHPEAUHWND____UMoveSettings_1_USizeSettings_1__Z(
        void* pThis, HWND hWnd, unsigned long long moveSettings, unsigned long long sizeSettings) {
    S_CMFCDynamicLayout* d = DL(pThis);
    if (hWnd == nullptr || !::IsWindow(hWnd))
        return FALSE;
    if (!::IsChild(d->m_pHostWnd != nullptr ? WndHwnd(d->m_pHostWnd) : nullptr, hWnd))
        return FALSE;
    for (DynLayoutNode* pNode = d->m_pNodeHead; pNode != nullptr; pNode = pNode->pNext) {
        if (pNode->data->m_hWnd == hWnd)
            return FALSE;
    }

    DynLayoutItem* pItem = static_cast<DynLayoutItem*>(impl___2_YAPEAX_K_Z(sizeof(DynLayoutItem)));
    if (pItem == nullptr)
        return FALSE;   // deviation (2) above
    pItem->m_hWnd = hWnd;
    pItem->m_ptInit.x = 0.0;
    pItem->m_ptInit.y = 0.0;
    pItem->m_szInit.x = 0.0;
    pItem->m_szInit.y = 0.0;
    pItem->m_move = UnpackRatio(moveSettings);
    pItem->m_size = UnpackRatio(sizeSettings);

    impl__CorrectItem_CMFCDynamicLayout__IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(pThis, pItem);
    if (impl__PrepareItem_CMFCDynamicLayout__IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(pThis, pItem))
        impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&d->list_vfptr, pItem);   // m_listWnd at +0x18
    return TRUE;
}

// Symbol: ?Adjust@CMFCDynamicLayout@@QEAAXXZ
// Transcribed from RVA 0x76a30 (mfc140u):
//   int nCount = (int)m_listWnd.GetCount();  if (nCount == 0) return;
//   HDWP hdwp = ::BeginDeferWindowPos(nCount);
//   for each item in m_listWnd (head to tail):
//       if (!::IsWindow(item->hwnd)) continue;
//       CRect rect(0,0,0,0);
//       UINT uFlags = AdjustItemRect(*item, rect);
//       if ((uFlags & (SWP_NOSIZE | SWP_NOMOVE)) == (SWP_NOSIZE | SWP_NOMOVE)) continue;
//       ::DeferWindowPos(hdwp, item->hwnd, NULL, rect.left, rect.top,
//                        rect.Width(), rect.Height(), uFlags | 0x314);
//   ::EndDeferWindowPos(hdwp);
// 0x314 = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER.
// Retail discards DeferWindowPos's return value and keeps passing the HDWP from
// BeginDeferWindowPos; reproduced as-is.
extern "C" void MS_ABI impl__Adjust_CMFCDynamicLayout__QEAAXXZ(void* pThis) {
    S_CMFCDynamicLayout* d = DL(pThis);
    const int nCount = static_cast<int>(d->m_nCount);
    if (nCount == 0)
        return;
    HDWP hdwp = ::BeginDeferWindowPos(nCount);
    for (DynLayoutNode* pNode = d->m_pNodeHead; pNode != nullptr; ) {
        DynLayoutItem* pItem = pNode->data;
        pNode = pNode->pNext;
        HWND hWnd = pItem->m_hWnd;
        if (!::IsWindow(hWnd))
            continue;
        RECT rect = {0, 0, 0, 0};
        const UINT uFlags = impl__AdjustItemRect_CMFCDynamicLayout__IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM__AEAVCRect___Z(pThis, pItem, &rect);
        if ((uFlags & (SWP_NOSIZE | SWP_NOMOVE)) == (SWP_NOSIZE | SWP_NOMOVE))
            continue;
        ::DeferWindowPos(hdwp, hWnd, nullptr, rect.left, rect.top,
                         rect.right - rect.left, rect.bottom - rect.top,
                         uFlags | (SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER));
    }
    ::EndDeferWindowPos(hdwp);
}

// Symbol: ?AdjustItemRect@CMFCDynamicLayout@@IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM@@AEAVCRect@@@Z
// Transcribed from RVA 0x77010 (mfc140u):
//   ::SetRectEmpty(&rectItem);
//   CRect rectHost(0,0,0,0); GetHostWndRect(rectHost);
//   if (rectHost is all zero) return SWP_NOSIZE | SWP_NOMOVE;       // 3
//   dx = Width(host) * 0.01;  dy = Height(host) * 0.01;
//   x  = ptInit.x (+ move.x * dx if move.x > 0);  y  = ptInit.y (+ move.y * dy if move.y > 0)
//   cx = szInit.x (+ size.x * dx if size.x > 0);  cy = szInit.y (+ size.y * dy if size.y > 0)
//   rectItem = { (int)x + host.left, (int)y + host.top, left + (int)cx, top + (int)cy }  (truncating)
//   flags = 0;
//   if ((double)rectItem.left == host.left + ptInit.x && (double)rectItem.top == host.top + ptInit.y)
//       flags = SWP_NOMOVE;
//   if ((double)(int)cx == szInit.x && (double)(int)cy == szInit.y) flags |= SWP_NOSIZE;
//   return flags;
extern "C" unsigned int MS_ABI impl__AdjustItemRect_CMFCDynamicLayout__IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM__AEAVCRect___Z(
        const void* pThis, DynLayoutItem* pItem, RECT* pRectItem) {
    ::SetRectEmpty(pRectItem);
    RECT rectHost = {0, 0, 0, 0};
    impl__GetHostWndRect_CMFCDynamicLayout__QEBAXAEAVCRect___Z(pThis, &rectHost);
    if (rectHost.left == 0 && rectHost.right == 0 && rectHost.top == 0 && rectHost.bottom == 0)
        return SWP_NOSIZE | SWP_NOMOVE;

    const double dx = static_cast<double>(rectHost.right - rectHost.left) * 0.01;
    const double dy = static_cast<double>(rectHost.bottom - rectHost.top) * 0.01;

    double x  = pItem->m_ptInit.x;
    double y  = pItem->m_ptInit.y;
    double cx = pItem->m_szInit.x;
    double cy = pItem->m_szInit.y;
    if (pItem->m_move.x > 0) x  = static_cast<double>(pItem->m_move.x) * dx + pItem->m_ptInit.x;
    if (pItem->m_move.y > 0) y  = pItem->m_ptInit.y + static_cast<double>(pItem->m_move.y) * dy;
    if (pItem->m_size.x > 0) cx = pItem->m_szInit.x + static_cast<double>(pItem->m_size.x) * dx;
    if (pItem->m_size.y > 0) cy = pItem->m_szInit.y + static_cast<double>(pItem->m_size.y) * dy;

    const int nCx = static_cast<int>(cx);
    const int nCy = static_cast<int>(cy);
    pRectItem->left   = static_cast<int>(x) + rectHost.left;
    pRectItem->top    = static_cast<int>(y) + rectHost.top;
    pRectItem->right  = nCx + pRectItem->left;
    pRectItem->bottom = nCy + pRectItem->top;

    unsigned int uFlags = 0;
    if (static_cast<double>(pRectItem->left) == static_cast<double>(rectHost.left) + pItem->m_ptInit.x &&
        static_cast<double>(pRectItem->top)  == static_cast<double>(rectHost.top)  + pItem->m_ptInit.y)
        uFlags = SWP_NOMOVE;
    if (static_cast<double>(nCx) == pItem->m_szInit.x && static_cast<double>(nCy) == pItem->m_szInit.y)
        uFlags |= SWP_NOSIZE;
    return uFlags;
}

// Symbol: ?CorrectItem@CMFCDynamicLayout@@IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM@@@Z
// Transcribed from RVA 0x76ce0 (mfc140u):
//   CString strClass;
//   ::GetClassNameW(item.hwnd, strClass.GetBuffer(1024), 1024); strClass.ReleaseBuffer();
//   if (_wcsicmp(strClass, L"ComboBox") == 0 || _wcsicmp(strClass, L"ComboBoxEx32") == 0) {
//       DWORD dwStyle = ::GetWindowLongW(item.hwnd, GWL_STYLE);
//       if (item.m_sizeSettings.m_nYRatio > 0 && (dwStyle & 0x1 /*CBS_SIMPLE*/) == 0)
//           item.m_sizeSettings.m_nYRatio = 0;     // only simple combos grow vertically
//   }
// (IAT slots resolved with iatu.py: GetClassNameW, wcsnlen, _wcsicmp,
// GetWindowLongW; strings read from mfc140u .rdata.)  Retail tests bit 0 only,
// so CBS_DROPDOWNLIST (3) also keeps its vertical ratio; reproduced as-is.
// Deviation: a 1024-char stack buffer (plus terminator) replaces the CString;
// the comparisons see the same NUL-terminated class name.
extern "C" void MS_ABI impl__CorrectItem_CMFCDynamicLayout__IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(
        const void* pThis, DynLayoutItem* pItem) {
    (void)pThis;
    wchar_t szClass[1024 + 1];
    szClass[0] = L'\0';
    szClass[1024] = L'\0';
    ::GetClassNameW(pItem->m_hWnd, szClass, 1024);
    if (_wcsicmp(szClass, L"ComboBox") == 0 || _wcsicmp(szClass, L"ComboBoxEx32") == 0) {
        const DWORD dwStyle = static_cast<DWORD>(::GetWindowLongW(pItem->m_hWnd, GWL_STYLE));
        if (pItem->m_size.y > 0 && (dwStyle & 0x1) == 0)
            pItem->m_size.y = 0;
    }
}

// Symbol: ?Create@CMFCDynamicLayout@@QEAAHPEAVCWnd@@@Z
// Transcribed from RVA 0x76a10 (mfc140u):
//   if (pHostWnd == NULL || pHostWnd->m_hWnd == NULL) return FALSE;
//   m_pHostWnd = pHostWnd; return TRUE;
// (retail does not call ::IsWindow here.)
extern "C" int MS_ABI impl__Create_CMFCDynamicLayout__QEAAHPEAVCWnd___Z(void* pThis, CWnd* pHostWnd) {
    if (pHostWnd == nullptr || WndHwnd(pHostWnd) == nullptr)
        return FALSE;
    DL(pThis)->m_pHostWnd = pHostWnd;
    return TRUE;
}

// Symbol: ?FindItem@CMFCDynamicLayout@@IEAAPEAUAFX_DYNAMIC_LAYOUT_ITEM@@PEAUHWND__@@@Z
// Transcribed from RVA 0x76cc0 (mfc140u): walk m_listWnd from m_pNodeHead (+0x20)
// and return the first item whose hwnd (+0x00) equals hWnd, else NULL.
extern "C" DynLayoutItem* MS_ABI impl__FindItem_CMFCDynamicLayout__IEAAPEAUAFX_DYNAMIC_LAYOUT_ITEM__PEAUHWND_____Z(
        void* pThis, HWND hWnd) {
    for (DynLayoutNode* pNode = DL(pThis)->m_pNodeHead; pNode != nullptr; pNode = pNode->pNext) {
        if (pNode->data->m_hWnd == hWnd)
            return pNode->data;
    }
    return nullptr;
}

// Symbol: ?GetHostWndRect@CMFCDynamicLayout@@QEBAXAEAVCRect@@@Z
// Transcribed from RVA 0x293980 (mfc140u):
//   ::SetRectEmpty(&rect);
//   if (m_pHostWnd == NULL || m_pHostWnd->m_hWnd == NULL) return;
//   ::GetClientRect(m_pHostWnd->m_hWnd, &rect);
//   if (m_pHostWnd->IsKindOf(RUNTIME_CLASS(CMFCPropertyPage))) {
//       CMFCPropertySheet* pSheet = CWnd::FromHandle(::GetParent(m_pHostWnd->m_hWnd));
//       if (pSheet != NULL && pSheet->IsKindOf(RUNTIME_CLASS(CMFCPropertySheet))) {
//           int nNav = (m_look is 1, 2 or 4 -- OutlookBar/Tree/List) ? m_nBarWidth : 0;
//           if (m_nHeaderHeight > 0) {
//               rect.top += m_nHeaderHeight;
//               if (m_look != 0) rect.bottom -= m_nHeaderHeight;
//           }
//           rect.left += nNav;
//       }
//       ::OffsetRect(&rect, -rect.left, -rect.top);
//   } else if (m_pHostWnd->IsKindOf(RUNTIME_CLASS(CFormView))) {
//       int nV = m_pHostWnd->GetScrollPos(SB_VERT), nH = m_pHostWnd->GetScrollPos(SB_HORZ);
//       rect.bottom += nV; rect.right += nH;
//       ::OffsetRect(&rect, -nH, -nV);
//   }
//   rect.right  = rect.left + max(m_MinSize.cx, rect.Width());
//   rect.bottom = rect.top  + max(m_MinSize.cy, rect.Height());
// (IAT slots: SetRectEmpty, GetClientRect, GetParent, OffsetRect.  The descriptor
// arguments of the three IsKindOf calls are CMFCPropertyPage, CMFCPropertySheet
// and CFormView, identified by name from the descriptors in mfc140.)
// The sheet fields are read at retail's offsets (0x190 / 0x71d0 / 0x71d8).
// NOTE: OpenMFC's CMFCPropertySheet currently keeps its look and nav-bar width in
// a side table (detail/CMFCPropertySheetSupport.h) and zero-fills its own object
// body, so against an OpenMFC-constructed sheet these read 0 and the property-
// page branch reduces to the OffsetRect.  The retail offsets are used anyway,
// because they are what a client-visible CMFCPropertySheet holds.
extern "C" void MS_ABI impl__GetHostWndRect_CMFCDynamicLayout__QEBAXAEAVCRect___Z(const void* pThis, RECT* pRect) {
    const S_CMFCDynamicLayout* d = DL(pThis);
    ::SetRectEmpty(pRect);
    if (d->m_pHostWnd == nullptr || WndHwnd(d->m_pHostWnd) == nullptr)
        return;
    ::GetClientRect(WndHwnd(d->m_pHostWnd), pRect);

    if (d->m_pHostWnd != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            d->m_pHostWnd, impl__GetThisClass_CMFCPropertyPage__SAPEAUCRuntimeClass__XZ())) {
        CWnd* pSheet = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(WndHwnd(d->m_pHostWnd)));
        if (pSheet != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                pSheet, impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ())) {
            const int nLook = SheetInt(pSheet, kSheet_m_look);
            const int nNavBarWidth = (nLook == 1 || nLook == 2 || nLook == 4)
                                         ? SheetInt(pSheet, kSheet_m_nBarWidth) : 0;
            const int nHeaderHeight = SheetInt(pSheet, kSheet_m_nHeaderHeight);
            if (nHeaderHeight > 0) {
                pRect->top += nHeaderHeight;
                if (nLook != 0)
                    pRect->bottom -= nHeaderHeight;
            }
            pRect->left += nNavBarWidth;
        }
        ::OffsetRect(pRect, -pRect->left, -pRect->top);
    } else if (d->m_pHostWnd != nullptr &&
               impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                   d->m_pHostWnd, impl__GetThisClass_CFormView__SAPEAUCRuntimeClass__XZ())) {
        const int nVert = impl__GetScrollPos_CWnd__QEBAHH_Z(d->m_pHostWnd, SB_VERT);
        const int nHorz = impl__GetScrollPos_CWnd__QEBAHH_Z(d->m_pHostWnd, SB_HORZ);
        pRect->bottom += nVert;
        pRect->right  += nHorz;
        ::OffsetRect(pRect, -nHorz, -nVert);
    }

    const int cx = pRect->right - pRect->left;
    const int cy = pRect->bottom - pRect->top;
    pRect->right  = pRect->left + (d->m_MinSize.cx > cx ? d->m_MinSize.cx : cx);
    pRect->bottom = pRect->top  + (d->m_MinSize.cy > cy ? d->m_MinSize.cy : cy);
}

// Symbol: ?GetItemRect@CMFCDynamicLayout@@IEBA?AVCRect@@AEAUAFX_DYNAMIC_LAYOUT_ITEM@@@Z
// Returns CRect by value: hidden return pointer in RDX after `this`, item in R8.
// Transcribed from RVA 0x76e20 (mfc140u):
//   if (m_pHostWnd == NULL) return CRect((LPCRECT)NULL);   // ::CopyRect(&ret, NULL)
//   CRect rect(0,0,0,0);
//   ::GetWindowRect(item.hwnd, &rect);
//   m_pHostWnd->ScreenToClient(&rect);
//   return rect;
// The NULL-host path really is ::CopyRect(pRet, NULL) in retail: CopyRect fails
// and leaves *pRet unwritten.  Reproduced as-is.
extern "C" RECT* MS_ABI impl__GetItemRect_CMFCDynamicLayout__IEBA_AVCRect__AEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(
        const void* pThis, RECT* pRet, DynLayoutItem* pItem) {
    const S_CMFCDynamicLayout* d = DL(pThis);
    RECT rect = {0, 0, 0, 0};
    if (d->m_pHostWnd == nullptr) {
        ::CopyRect(pRet, nullptr);
        return pRet;
    }
    ::GetWindowRect(pItem->m_hWnd, &rect);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(d->m_pHostWnd, &rect);
    *pRet = rect;
    return pRet;
}

// Symbol: ?LoadResource@CMFCDynamicLayout@@SAHPEAVCWnd@@PEAXK@Z
// STUB.  Retail, RVA 0x773f0 (mfc140u), was disassembled:
//   if (!pHostWnd || !pHostWnd->m_hWnd || !::IsWindow(pHostWnd->m_hWnd) || !lpResource) return FALSE;
//   CMFCDynamicLayoutData data;  BOOL bRet = data.ReadResource(lpResource, dwSize);
//   if (pHostWnd->m_hWnd && data has items) {
//       pHostWnd->EnableDynamicLayout(FALSE); pHostWnd->EnableDynamicLayout(TRUE);
//       CMFCDynamicLayout* pLayout = pHostWnd->m_pDynamicLayout;      // CWnd +0x98
//       if (pLayout && pHostWnd->m_hWnd) {
//           pLayout->m_pHostWnd = pHostWnd;                            // inlined Create
//           walk the child windows (GW_CHILD, then GW_HWNDNEXT) in step with the
//           data items; AddItem(child, move, size) for each item whose settings
//           are not both IsNone();
//       }
//   }
//   free data's CPlex chain; return bRet;
// Not implemented because it depends on CWnd infrastructure OpenMFC lacks:
// OpenMFC's CWnd::EnableDynamicLayout (core/window/CWnd.cpp) only toggles a
// side-table flag and never allocates a CMFCDynamicLayout, and OpenMFC's CWnd
// has no m_pDynamicLayout at +0x98 (that range is _cwnd_padding2).  Reading
// +0x98 and writing through it, as retail does, would dereference padding.
extern "C" int MS_ABI impl__LoadResource_CMFCDynamicLayout__SAHPEAVCWnd__PEAXK_Z(
        CWnd* pHostWnd, void* lpResource, unsigned long dwSize) {
    (void)pHostWnd; (void)lpResource; (void)dwSize;
    return FALSE;
}

// Symbol: ?PrepareItem@CMFCDynamicLayout@@IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM@@@Z
// Transcribed from RVA 0x76ea0 (mfc140u):
//   CRect rectHost(0,0,0,0); GetHostWndRect(rectHost);
//   if (rectHost is all zero) return FALSE;
//   CRect rectItem = GetItemRect(item);
//   dx = Width(host) * 0.01;  dy = Height(host) * 0.01;
//   ptInit = (rectItem.left, rectItem.top);    szInit = (rectItem.Width(), rectItem.Height());
//   if (move.x > 0) ptInit.x = rectItem.left   - move.x * dx;
//   if (move.y > 0) ptInit.y = rectItem.top    - move.y * dy;
//   if (size.x > 0) szInit.x = rectItem.Width() - size.x * dx;
//   if (size.y > 0) szInit.y = rectItem.Height()- size.y * dy;
//   return TRUE;
extern "C" int MS_ABI impl__PrepareItem_CMFCDynamicLayout__IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(
        const void* pThis, DynLayoutItem* pItem) {
    RECT rectHost = {0, 0, 0, 0};
    impl__GetHostWndRect_CMFCDynamicLayout__QEBAXAEAVCRect___Z(pThis, &rectHost);
    if (rectHost.left == 0 && rectHost.right == 0 && rectHost.top == 0 && rectHost.bottom == 0)
        return FALSE;

    RECT rectItem;
    impl__GetItemRect_CMFCDynamicLayout__IEBA_AVCRect__AEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(pThis, &rectItem, pItem);

    const double dx = static_cast<double>(rectHost.right - rectHost.left) * 0.01;
    const double dy = static_cast<double>(rectHost.bottom - rectHost.top) * 0.01;

    const double left = static_cast<double>(rectItem.left);
    const double top  = static_cast<double>(rectItem.top);
    pItem->m_ptInit.x = left;
    pItem->m_ptInit.y = top;
    if (pItem->m_move.x > 0) pItem->m_ptInit.x = left - static_cast<double>(pItem->m_move.x) * dx;
    if (pItem->m_move.y > 0) pItem->m_ptInit.y = top  - static_cast<double>(pItem->m_move.y) * dy;

    const double width  = static_cast<double>(rectItem.right - rectItem.left);
    const double height = static_cast<double>(rectItem.bottom - rectItem.top);
    pItem->m_szInit.x = width;
    pItem->m_szInit.y = height;
    if (pItem->m_size.x > 0) pItem->m_szInit.x = width  - static_cast<double>(pItem->m_size.x) * dx;
    if (pItem->m_size.y > 0) pItem->m_szInit.y = height - static_cast<double>(pItem->m_size.y) * dy;
    return TRUE;
}

// Generated 5893 function stubs + 46 data stubs
// Errors (could not parse): 0
// Excluded (real implementations): 8282

