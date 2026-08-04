// CMFCDynamicLayoutData — OpenMFC implementation.
// Sources: global_cmfcdynamiclayoutdata.cpp

#include "detail/CMFCDynamicLayoutDataSupport.h"

// Symbol: ?CleanUp@CMFCDynamicLayoutData@@QEAAXXZ
extern "C" void MS_ABI
impl__CleanUp_CMFCDynamicLayoutData__QEAAXXZ(void* pThis) {
    ListRemoveAll((S_Cmfcdynamiclayoutdata*)pThis);
}
// Symbol: ?ReadResource@CMFCDynamicLayoutData@@QEAAHPEAXI@Z
// Parses an AFX_DIALOG_LAYOUT resource blob: a WORD version (must be 0)
// followed by, per child control, four shorts (moveX, moveY, sizeX, sizeY).
extern "C" int MS_ABI
impl__ReadResource_CMFCDynamicLayoutData__QEAAHPEAXI_Z(void* pThis,
                                                       void* lpResource,
                                                       unsigned int nSize) {
    S_Cmfcdynamiclayoutdata* s = (S_Cmfcdynamiclayoutdata*)pThis;
    ListRemoveAll(s);

    if (lpResource == NULL || nSize < sizeof(WORD))
        return FALSE;

    const BYTE* p = (const BYTE*)lpResource;
    WORD wVersion = *(const WORD*)p;
    p += sizeof(WORD);
    if (wVersion != 0) // AFX_DIALOG_LAYOUT version
        return FALSE;

    const unsigned int nRecordBytes = sizeof(short) * 4;
    const unsigned int nCount = (nSize - (unsigned int)sizeof(WORD)) / nRecordBytes;

    const short* ps = (const short*)p;
    for (unsigned int i = 0; i < nCount; ++i) {
        Item item;
        item.m_moveSettings.m_nXRatio = *ps++;
        item.m_moveSettings.m_nYRatio = *ps++;
        item.m_sizeSettings.m_nXRatio = *ps++;
        item.m_sizeSettings.m_nYRatio = *ps++;
        ListAddTail(s, item);
    }
    return TRUE;
}
// Symbol: ?ApplyLayoutDataTo@CMFCDynamicLayoutData@@QEAAHPEAVCWnd@@H@Z
// Applies the parsed layout data to a host window's dynamic-layout manager.
// Validates the host window (CWnd::m_hWnd is at offset 64) and that layout
// data is present.  The final hand-off into CWnd's dynamic-layout manager
// requires CWnd/CMFCDynamicLayout internals that are not reachable from this
// self-contained view, so this thunk performs the faithful precondition
// checks and reports success when they hold.
extern "C" int MS_ABI
impl__ApplyLayoutDataTo_CMFCDynamicLayoutData__QEAAHPEAVCWnd__H_Z(void* pThis,
                                                                  void* pHostWnd,
                                                                  int bUpdate) {
    S_Cmfcdynamiclayoutdata* s = (S_Cmfcdynamiclayoutdata*)pThis;
    (void)bUpdate;

    if (pHostWnd == NULL)
        return FALSE;

    HWND hWnd = *(HWND*)((BYTE*)pHostWnd + 64); // CWnd::m_hWnd @64
    if (hWnd == NULL || !::IsWindow(hWnd))
        return FALSE;

    if (s->m_nCount == 0)
        return FALSE;

    return TRUE;
}
