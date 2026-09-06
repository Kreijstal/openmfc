// COleLinkingDoc — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

COleLinkingDoc::COleLinkingDoc()
    : COleDocument(), m_bRegistered(FALSE), m_lpMoniker(nullptr) {
    memset(_colelinkingdoc_padding, 0, sizeof(_colelinkingdoc_padding));
}
COleLinkingDoc::~COleLinkingDoc() {
    Revoke();
}
LPMONIKER COleLinkingDoc::GetMoniker(OLEGETMONIKER nAssign) {
    if (!m_lpMoniker && nAssign != OLEGETMONIKER_ONLYIFTHERE) {
        EnsureLinkingDocMoniker(this, nullptr, FALSE);
    }
    return m_lpMoniker;
}
LPMONIKER COleLinkingDoc::GetFileMoniker() {
    return m_lpMoniker;
}
BOOL COleLinkingDoc::RegisterIfServerAttached(const wchar_t* lpszFileName, BOOL bSetModified) {
    return EnsureLinkingDocMoniker(this, lpszFileName, bSetModified);
}
void COleLinkingDoc::Revoke() {
    if (m_lpMoniker) {
        m_lpMoniker->Release();
        m_lpMoniker = nullptr;
    }
    m_bRegistered = FALSE;
}
BOOL COleLinkingDoc::IsRegistered() const {
    return m_bRegistered;
}
void COleLinkingDoc::OnShowDocument(BOOL bShow) {
    if (!bShow) Revoke();
}
// Symbol: ?EndDeferErrors@COleLinkingDoc@@QEAAJJ@Z
extern "C" long MS_ABI impl__EndDeferErrors_COleLinkingDoc__QEAAJJ_Z(long p0) {
    return 0;
}

// Symbol: ?GetContainer@COleLinkingDoc@@UEAAPEAUIOleItemContainer@@XZ
extern "C" void* MS_ABI impl__GetContainer_COleLinkingDoc__UEAAPEAUIOleItemContainer__XZ() {
    return nullptr;
}

// Symbol: ?LockExternal@COleLinkingDoc@@IEAAXHH@Z
extern "C" void MS_ABI impl__LockExternal_COleLinkingDoc__IEAAXHH_Z(int p0, int p1) {}

// Symbol: ?OnCloseDocument@COleLinkingDoc@@UEAAXXZ
extern "C" void MS_ABI impl__OnCloseDocument_COleLinkingDoc__UEAAXXZ() {}

// Symbol: ?OnFindEmbeddedItem@COleLinkingDoc@@MEAAPEAVCOleClientItem@@PEB_W@Z
extern "C" void* MS_ABI impl__OnFindEmbeddedItem_COleLinkingDoc__MEAAPEAVCOleClientItem__PEB_W_Z(const wchar_t* p0) {
    return nullptr;
}

// Symbol: ?OnGetLinkedItem@COleLinkingDoc@@MEAAPEAVCOleServerItem@@PEB_W@Z
extern "C" void* MS_ABI impl__OnGetLinkedItem_COleLinkingDoc__MEAAPEAVCOleServerItem__PEB_W_Z(const wchar_t* p0) {
    return nullptr;
}

// Symbol: ?OnNewDocument@COleLinkingDoc@@UEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_COleLinkingDoc__UEAAHXZ() {
    return 0;
}

// Symbol: ?OnOpenDocument@COleLinkingDoc@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnOpenDocument_COleLinkingDoc__UEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnSaveDocument@COleLinkingDoc@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_COleLinkingDoc__UEAAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}

// Symbol: ?OnShowViews@COleLinkingDoc@@MEAAXH@Z
extern "C" void MS_ABI impl__OnShowViews_COleLinkingDoc__MEAAXH_Z(int p0) {}

// Symbol: ?Register@COleLinkingDoc@@QEAAHPEAVCOleObjectFactory@@PEB_W@Z
extern "C" int MS_ABI impl__Register_COleLinkingDoc__QEAAHPEAVCOleObjectFactory__PEB_W_Z(void* /*class*/* p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?ReportSaveLoadException@COleLinkingDoc@@UEAAXPEB_WPEAVCException@@HI@Z
extern "C" void MS_ABI impl__ReportSaveLoadException_COleLinkingDoc__UEAAXPEB_WPEAVCException__HI_Z(const wchar_t* p0, void* /*class*/* p1, int p2, unsigned int p3) {}

// Symbol: ?SaveToStorage@COleLinkingDoc@@MEAAXPEAVCObject@@@Z
extern "C" void MS_ABI impl__SaveToStorage_COleLinkingDoc__MEAAXPEAVCObject___Z(void* /*class*/* p0) {}

// Symbol: ?UpdateVisibleLock@COleLinkingDoc@@IEAAXHH@Z
extern "C" void MS_ABI impl__UpdateVisibleLock_COleLinkingDoc__IEAAXHH_Z(int p0, int p1) {}

// Symbol: ?EnumObjects@XOleItemContainer@COleLinkingDoc@@UEAAJKPEAPEAUIEnumUnknown@@@Z
extern "C" long MS_ABI impl__EnumObjects_XOleItemContainer_COleLinkingDoc__UEAAJKPEAPEAUIEnumUnknown___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?GetClassID@XPersistFile@COleLinkingDoc@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetClassID_XPersistFile_COleLinkingDoc__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetCurFile@XPersistFile@COleLinkingDoc@@UEAAJPEAPEA_W@Z
extern "C" long MS_ABI impl__GetCurFile_XPersistFile_COleLinkingDoc__UEAAJPEAPEA_W_Z(wchar_t** p0) {
    return 0;
}

// Symbol: ?GetObjectStorage@XOleItemContainer@COleLinkingDoc@@UEAAJPEA_WPEAUIBindCtx@@AEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetObjectStorage_XOleItemContainer_COleLinkingDoc__UEAAJPEA_WPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(wchar_t* p0, void* /*struct*/* p1, const void* /*struct*/* p2, void** p3) {
    return 0;
}

// Symbol: ?GetObjectW@XOleItemContainer@COleLinkingDoc@@UEAAJPEA_WKPEAUIBindCtx@@AEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetObjectW_XOleItemContainer_COleLinkingDoc__UEAAJPEA_WKPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(wchar_t* p0, unsigned long p1, void* /*struct*/* p2, const void* /*struct*/* p3, void** p4) {
    return 0;
}

// Symbol: ?IsDirty@XPersistFile@COleLinkingDoc@@UEAAJXZ
extern "C" long MS_ABI impl__IsDirty_XPersistFile_COleLinkingDoc__UEAAJXZ() {
    return 0;
}

// Symbol: ?IsRunning@XOleItemContainer@COleLinkingDoc@@UEAAJPEA_W@Z
extern "C" long MS_ABI impl__IsRunning_XOleItemContainer_COleLinkingDoc__UEAAJPEA_W_Z(wchar_t* p0) {
    return 0;
}

// Symbol: ?Load@XPersistFile@COleLinkingDoc@@UEAAJPEB_WK@Z
extern "C" long MS_ABI impl__Load_XPersistFile_COleLinkingDoc__UEAAJPEB_WK_Z(const wchar_t* p0, unsigned long p1) {
    return 0;
}

// Symbol: ?LockContainer@XOleItemContainer@COleLinkingDoc@@UEAAJH@Z
extern "C" long MS_ABI impl__LockContainer_XOleItemContainer_COleLinkingDoc__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?ParseDisplayName@XOleItemContainer@COleLinkingDoc@@UEAAJPEAUIBindCtx@@PEA_WPEAKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__ParseDisplayName_XOleItemContainer_COleLinkingDoc__UEAAJPEAUIBindCtx__PEA_WPEAKPEAPEAUIMoniker___Z(void* /*struct*/* p0, wchar_t* p1, unsigned long* p2, void* /*struct*/** p3) {
    return 0;
}

// Symbol: ?Save@XPersistFile@COleLinkingDoc@@UEAAJPEB_WH@Z
extern "C" long MS_ABI impl__Save_XPersistFile_COleLinkingDoc__UEAAJPEB_WH_Z(const wchar_t* p0, int p1) {
    return 0;
}

// Symbol: ?SaveCompleted@XPersistFile@COleLinkingDoc@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__SaveCompleted_XPersistFile_COleLinkingDoc__UEAAJPEB_W_Z(const wchar_t* p0) {
    return 0;
}
