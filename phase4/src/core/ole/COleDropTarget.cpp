// COleDropTarget — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Symbol: ?DragEnter@XDropTarget@COleDropTarget@@UEAAJPEAUIDataObject@@KU_POINTL@@PEAK@Z
extern "C" long MS_ABI impl__DragEnter_XDropTarget_COleDropTarget__UEAAJPEAUIDataObject__KU_POINTL__PEAK_Z(
    COleDropTarget::XDropTarget* pThis, IDataObject* pDataObj, unsigned long grfKeyState, POINTL pt, unsigned long* pdwEffect) {
    return pThis->DragEnter(pDataObj, grfKeyState, pt, pdwEffect);
}
// Symbol: ?DragLeave@XDropTarget@COleDropTarget@@UEAAJXZ
extern "C" long MS_ABI impl__DragLeave_XDropTarget_COleDropTarget__UEAAJXZ(
    COleDropTarget::XDropTarget* pThis) {
    return pThis->DragLeave();
}
// Symbol: ?DragOver@XDropTarget@COleDropTarget@@UEAAJKU_POINTL@@PEAK@Z
extern "C" long MS_ABI impl__DragOver_XDropTarget_COleDropTarget__UEAAJKU_POINTL__PEAK_Z(
    COleDropTarget::XDropTarget* pThis, unsigned long grfKeyState, POINTL pt, unsigned long* pdwEffect) {
    return pThis->DragOver(grfKeyState, pt, pdwEffect);
}
// Symbol: ?Drop@XDropTarget@COleDropTarget@@UEAAJPEAUIDataObject@@KU_POINTL@@PEAK@Z
extern "C" long MS_ABI impl__Drop_XDropTarget_COleDropTarget__UEAAJPEAUIDataObject__KU_POINTL__PEAK_Z(
    COleDropTarget::XDropTarget* pThis, IDataObject* pDataObj, unsigned long grfKeyState, POINTL pt, unsigned long* pdwEffect) {
    return pThis->Drop(pDataObj, grfKeyState, pt, pdwEffect);
}
// Symbol: ?nScrollDelay@COleDropTarget@@1IA
extern "C" unsigned int impl__nScrollDelay_COleDropTarget__1IA = 200;
// Symbol: ?nScrollInset@COleDropTarget@@1HA
extern "C" int impl__nScrollInset_COleDropTarget__1HA = 1;
// Symbol: ?nScrollInterval@COleDropTarget@@1IA
extern "C" unsigned int impl__nScrollInterval_COleDropTarget__1IA = 50;
COleDropTarget::COleDropTarget()
    : m_lRefCount(0), m_pWnd(nullptr), m_bRegistered(FALSE) {
    memset(_oledroptarget_padding, 0, sizeof(_oledroptarget_padding));
    m_xDropTarget.m_pDropTarget = this;
}
COleDropTarget::~COleDropTarget() {
    Revoke();
}
BOOL COleDropTarget::Register(CWnd* pWnd) {
    if (m_bRegistered) return TRUE;
    if (!pWnd || !pWnd->GetSafeHwnd()) return FALSE;
    m_pWnd = pWnd;
    DropTargetState* state = GetDropTargetState(this, true);
    if (!state) return FALSE;
    state->adapter = new DropTargetAdapter(this);
    HRESULT hr = RegisterDragDrop(pWnd->GetSafeHwnd(), state->adapter);
    if (SUCCEEDED(hr)) {
        m_bRegistered = TRUE;
        return TRUE;
    }
    state->adapter->Release();
    state->adapter = nullptr;
    RemoveDropTargetState(this);
    m_pWnd = nullptr;
    return FALSE;
}
void COleDropTarget::Revoke() {
    DropTargetState* state = GetDropTargetState(this, false);
    if (m_bRegistered && m_pWnd) {
        RevokeDragDrop(m_pWnd->GetSafeHwnd());
        m_bRegistered = FALSE;
    }
    if (state && state->adapter) {
        state->adapter->Release();
        state->adapter = nullptr;
    }
    RemoveDropTargetState(this);
    m_pWnd = nullptr;
}
DROPEFFECT COleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
                                        DWORD dwKeyState, CPoint point) {
    (void)pWnd; (void)point;
    if (!pDataObject || !pDataObject->GetIDataObject(FALSE)) return DROPEFFECT_NONE;
    return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
}
DROPEFFECT COleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
                                       DWORD dwKeyState, CPoint point) {
    return OnDragEnter(pWnd, pDataObject, dwKeyState, point);
}
void COleDropTarget::OnDragLeave(CWnd* pWnd) {
    (void)pWnd;
}
BOOL COleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
                             DROPEFFECT dropEffect, CPoint point) {
    (void)pWnd; (void)point;
    return pDataObject && pDataObject->GetIDataObject(FALSE) && dropEffect != DROPEFFECT_NONE;
}
DROPEFFECT COleDropTarget::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
                                     DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point) {
    return OnDrop(pWnd, pDataObject, dropDefault, point);
}
DROPEFFECT COleDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point) {
    return DROPEFFECT_SCROLL;
}
STDMETHODIMP COleDropTarget::XDropTarget::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}
STDMETHODIMP COleDropTarget::XDropTarget::DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    if (!pdwEffect) return E_POINTER;
    *pdwEffect = DROPEFFECT_NONE;
    if (!m_pDropTarget) return S_OK;
    COleDataObject data;
    if (pDataObj) data.Attach(pDataObj, FALSE);
    *pdwEffect = m_pDropTarget->OnDragEnter(m_pDropTarget->m_pWnd, &data, grfKeyState,
                                            CPoint(static_cast<int>(pt.x), static_cast<int>(pt.y)));
    return S_OK;
}
STDMETHODIMP COleDropTarget::XDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    if (!pdwEffect) return E_POINTER;
    *pdwEffect = m_pDropTarget ? m_pDropTarget->OnDragOver(m_pDropTarget->m_pWnd, nullptr,
                  grfKeyState, CPoint(static_cast<int>(pt.x), static_cast<int>(pt.y))) : DROPEFFECT_NONE;
    return S_OK;
}
STDMETHODIMP COleDropTarget::XDropTarget::DragLeave() {
    if (m_pDropTarget) m_pDropTarget->OnDragLeave(m_pDropTarget->m_pWnd);
    return S_OK;
}
STDMETHODIMP COleDropTarget::XDropTarget::Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    if (!pdwEffect) return E_POINTER;
    DROPEFFECT requested = *pdwEffect;
    *pdwEffect = DROPEFFECT_NONE;
    if (!m_pDropTarget) return S_OK;
    COleDataObject data;
    if (pDataObj) data.Attach(pDataObj, FALSE);
    if (m_pDropTarget->OnDrop(m_pDropTarget->m_pWnd, &data, requested,
                              CPoint(static_cast<int>(pt.x), static_cast<int>(pt.y)))) {
        *pdwEffect = requested;
    }
    (void)grfKeyState;
    return S_OK;
}
