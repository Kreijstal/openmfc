// Native toolbar-specific handling for the embedded OLE drop target.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <new>

extern "C" {
extern BOOL impl__m_bCustomizeMode_CMFCToolBar__1HA;
BOOL impl__OnDrop_CMFCToolBar__MEAAHPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar*, COleDataObject*, DROPEFFECT, CPoint);
DROPEFFECT impl__OnDragEnter_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar*, COleDataObject*, DWORD, CPoint);
void impl__OnDragLeave_CMFCToolBar__MEAAXXZ(CMFCToolBar*);
DROPEFFECT impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBar*, COleDataObject*, DWORD, CPoint);
}

CMFCToolBarDropTarget::CMFCToolBarDropTarget() : m_pOwner(nullptr) {}
CMFCToolBarDropTarget::~CMFCToolBarDropTarget() = default;

bool CMFCToolBarDropTarget::Accepts(COleDataObject* data) const {
    if (!m_pOwner) AfxThrowInvalidArgException();
    return impl__m_bCustomizeMode_CMFCToolBar__1HA && data &&
        data->IsDataAvailable(CMFCToolBarButton::GetClipboardFormat());
}

BOOL CMFCToolBarDropTarget::Register(CMFCToolBar* owner) {
    if (!owner) return FALSE;
    if (m_hWnd) return m_pOwner == owner && m_hWnd == owner->GetSafeHwnd();

    m_pOwner = owner;
    if (COleDropTarget::Register(owner)) return TRUE;
    m_pOwner = nullptr;
    return FALSE;
}

DROPEFFECT CMFCToolBarDropTarget::OnDragEnter(CWnd*, COleDataObject* data, DWORD keys, CPoint point) {
    return Accepts(data) ?
        impl__OnDragEnter_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(m_pOwner, data, keys, point) : DROPEFFECT_NONE;
}

void CMFCToolBarDropTarget::OnDragLeave(CWnd*) {
    if (!m_pOwner) AfxThrowInvalidArgException();
    impl__OnDragLeave_CMFCToolBar__MEAAXXZ(m_pOwner);
}

DROPEFFECT CMFCToolBarDropTarget::OnDragOver(CWnd*, COleDataObject* data, DWORD keys, CPoint point) {
    return Accepts(data) ?
        impl__OnDragOver_CMFCToolBar__MEAAKPEAVCOleDataObject__KVCPoint___Z(m_pOwner, data, keys, point) : DROPEFFECT_NONE;
}

DROPEFFECT CMFCToolBarDropTarget::OnDropEx(CWnd*, COleDataObject* data, DROPEFFECT effect, DROPEFFECT, CPoint point) {
    if (!Accepts(data) || effect == DROPEFFECT_NONE) return DROPEFFECT_NONE;
    return impl__OnDrop_CMFCToolBar__MEAAHPEAVCOleDataObject__KVCPoint___Z(m_pOwner, data, effect, point) ?
        effect : DROPEFFECT_NONE;
}

// Symbol: ??0CMFCToolBarDropTarget@@QEAA@XZ
extern "C" void* impl___0CMFCToolBarDropTarget__QEAA_XZ(void* self) {
    return new (self) CMFCToolBarDropTarget();
}

// Symbol: ??1CMFCToolBarDropTarget@@UEAA@XZ
extern "C" void impl___1CMFCToolBarDropTarget__UEAA_XZ(CMFCToolBarDropTarget* self) {
    self->CMFCToolBarDropTarget::~CMFCToolBarDropTarget();
}

// Symbol: ?Register@CMFCToolBarDropTarget@@QEAAHPEAVCMFCToolBar@@@Z
extern "C" BOOL impl__Register_CMFCToolBarDropTarget__QEAAHPEAVCMFCToolBar___Z(
    CMFCToolBarDropTarget* self, CMFCToolBar* owner) {
    return self->Register(owner);
}

// Symbol: ?OnDragEnter@CMFCToolBarDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
extern "C" DROPEFFECT impl__OnDragEnter_CMFCToolBarDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBarDropTarget* self, CWnd* window, COleDataObject* data, DWORD keys, CPoint point) {
    return self->CMFCToolBarDropTarget::OnDragEnter(window, data, keys, point);
}

// Symbol: ?OnDragLeave@CMFCToolBarDropTarget@@UEAAXPEAVCWnd@@@Z
extern "C" void impl__OnDragLeave_CMFCToolBarDropTarget__UEAAXPEAVCWnd___Z(
    CMFCToolBarDropTarget* self, CWnd* window) {
    self->CMFCToolBarDropTarget::OnDragLeave(window);
}

// Symbol: ?OnDragOver@CMFCToolBarDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
extern "C" DROPEFFECT impl__OnDragOver_CMFCToolBarDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(
    CMFCToolBarDropTarget* self, CWnd* window, COleDataObject* data, DWORD keys, CPoint point) {
    return self->CMFCToolBarDropTarget::OnDragOver(window, data, keys, point);
}

// Symbol: ?OnDropEx@CMFCToolBarDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KKVCPoint@@@Z
extern "C" DROPEFFECT impl__OnDropEx_CMFCToolBarDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KKVCPoint___Z(
    CMFCToolBarDropTarget* self, CWnd* window, COleDataObject* data, DROPEFFECT effect, DROPEFFECT allowed, CPoint point) {
    return self->CMFCToolBarDropTarget::OnDropEx(window, data, effect, allowed, point);
}
