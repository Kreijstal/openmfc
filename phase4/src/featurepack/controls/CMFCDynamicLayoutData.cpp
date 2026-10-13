// CMFCDynamicLayoutData — OpenMFC implementation.
// Sources: global_cmfcdynamiclayoutdata.cpp

#include "detail/CMFCDynamicLayoutDataSupport.h"
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxlayout.h"
#include <cstring>

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
    if (lpResource == NULL || nSize < sizeof(WORD))
        return FALSE;

    ListRemoveAll(s);

    const BYTE* p = (const BYTE*)lpResource;
    WORD wVersion;
    std::memcpy(&wVersion,p,sizeof(wVersion));
    p += sizeof(WORD);
    if (wVersion != 0) // AFX_DIALOG_LAYOUT version
        return FALSE;

    const unsigned int nRecordBytes = sizeof(short) * 4;
    const unsigned int nCount = (nSize - (unsigned int)sizeof(WORD)) / nRecordBytes;

    for (unsigned int i = 0; i < nCount; ++i) {
        short ratios[4];std::memcpy(ratios,p,sizeof(ratios));p+=sizeof(ratios);
        auto clamp=[](int value) { return value<0 ? 0 : value>100 ? 100 : value; };
        openmfc::detail::cmfcdynamiclayoutdata::Item item;
        item.m_moveSettings.m_nXRatio = clamp(ratios[0]);
        item.m_moveSettings.m_nYRatio = clamp(ratios[1]);
        item.m_sizeSettings.m_nXRatio = clamp(ratios[2]);
        item.m_sizeSettings.m_nYRatio = clamp(ratios[3]);
        ListAddTail(s, item);
    }
    return s->m_nCount > 0;
}
// Symbol: ?ApplyLayoutDataTo@CMFCDynamicLayoutData@@QEAAHPEAVCWnd@@H@Z
// Replace the host layout, associate it with the host and walk resource records
// in native child-window order. The update flag requests an immediate Adjust.
extern "C" int MS_ABI
impl__ApplyLayoutDataTo_CMFCDynamicLayoutData__QEAAHPEAVCWnd__H_Z(void* pThis,
                                                                  void* pHostWnd,
                                                                  int bUpdate) {
    S_Cmfcdynamiclayoutdata* s = (S_Cmfcdynamiclayoutdata*)pThis;
    if (pHostWnd == NULL)
        return FALSE;

    HWND hWnd = *(HWND*)((BYTE*)pHostWnd + 64); // CWnd::m_hWnd @64
    if (hWnd == NULL)
        return FALSE;

    if (s->m_nCount == 0)
        return FALSE;

    auto* host=static_cast<CWnd*>(pHostWnd);
    host->EnableDynamicLayout(FALSE);
    host->EnableDynamicLayout(TRUE);
    auto* layout=host->GetDynamicLayout();
    if(!layout || !layout->Create(host))return FALSE;
    HWND child=::GetWindow(hWnd,GW_CHILD);
    for(auto* node=s->m_pNodeHead;child && node;node=node->pNext,child=::GetWindow(child,GW_HWNDNEXT)) {
        const auto& item=node->data;
        if(item.m_moveSettings.m_nXRatio>0 || item.m_moveSettings.m_nYRatio>0 ||
           item.m_sizeSettings.m_nXRatio>0 || item.m_sizeSettings.m_nYRatio>0) {
            CMFCDynamicLayout::MoveSettings move;
            CMFCDynamicLayout::SizeSettings size;
            move.m_nXRatio=item.m_moveSettings.m_nXRatio;move.m_nYRatio=item.m_moveSettings.m_nYRatio;
            size.m_nXRatio=item.m_sizeSettings.m_nXRatio;size.m_nYRatio=item.m_sizeSettings.m_nYRatio;
            layout->AddItem(child,move,size);
        }
    }
    if(bUpdate)layout->Adjust();
    return TRUE;
}
