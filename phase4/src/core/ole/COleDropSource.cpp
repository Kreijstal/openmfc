// COleDropSource — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Symbol: ?GiveFeedback@XDropSource@COleDropSource@@UEAAJK@Z
extern "C" long MS_ABI impl__GiveFeedback_XDropSource_COleDropSource__UEAAJK_Z(
    COleDropSource::XDropSource* pThis, unsigned long p0) {
    return pThis->GiveFeedback(p0);
}
// Symbol: ?QueryContinueDrag@XDropSource@COleDropSource@@UEAAJHK@Z
extern "C" long MS_ABI impl__QueryContinueDrag_XDropSource_COleDropSource__UEAAJHK_Z(
    COleDropSource::XDropSource* pThis, int p0, unsigned long p1) {
    return pThis->QueryContinueDrag(p0, p1);
}
// Symbol: ?OnBeginDrag@COleDropSource@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnBeginDrag_COleDropSource__UEAAHPEAVCWnd___Z(
    COleDropSource* pThis, CWnd* p0) {
    return pThis->OnBeginDrag(p0);
}
// Symbol: ?nDragDelay@COleDropSource@@1IA
extern "C" unsigned int impl__nDragDelay_COleDropSource__1IA = 0;
// Symbol: ?nDragMinDist@COleDropSource@@1IA
extern "C" unsigned int impl__nDragMinDist_COleDropSource__1IA = 0;
COleDropSource::COleDropSource()
    : m_lRefCount(0) {
    memset(_oledropsource_padding, 0, sizeof(_oledropsource_padding));
    m_xDropSource.m_pDropSource = this;
}
COleDropSource::~COleDropSource() {
    m_lRefCount = 0;
}
SCODE COleDropSource::QueryContinueDrag(BOOL bEscapePressed, DWORD dwKeyState) {
    if (bEscapePressed) return DRAGDROP_S_CANCEL;
    if (!(dwKeyState & (MK_LBUTTON | MK_RBUTTON))) return DRAGDROP_S_DROP;
    return S_OK;
}
SCODE COleDropSource::GiveFeedback(DROPEFFECT dropEffect) {
    (void)dropEffect;
    return DRAGDROP_S_USEDEFAULTCURSORS;
}
BOOL COleDropSource::OnBeginDrag(CWnd* pWnd) {
    // Real MFC default: capture mouse if not already captured
    if (pWnd != nullptr && ::GetCapture() != pWnd->m_hWnd)
        ::SetCapture(pWnd->m_hWnd);
    return TRUE;
}
STDMETHODIMP COleDropSource::XDropSource::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropSource)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}
STDMETHODIMP COleDropSource::XDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState) {
    return m_pDropSource ? m_pDropSource->QueryContinueDrag(fEscapePressed, grfKeyState) : DRAGDROP_S_CANCEL;
}
STDMETHODIMP COleDropSource::XDropSource::GiveFeedback(DWORD dwEffect) {
    return m_pDropSource ? m_pDropSource->GiveFeedback((DROPEFFECT)dwEffect) : DRAGDROP_S_USEDEFAULTCURSORS;
}
