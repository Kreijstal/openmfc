// COleServerItem — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0COleServerItem@@IEAA@PEAVCOleServerDoc@@H@Z
// COleServerItem::COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete)
extern "C" void* MS_ABI impl___0COleServerItem__IEAA_PEAVCOleServerDoc__H_Z(COleServerDoc* pServerDoc, unsigned int bAutoDelete) {
    return new COleServerItem(pServerDoc, bAutoDelete);
}
COleServerItem::COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete)
    : m_pServerDoc(pServerDoc), m_bAutoDelete(bAutoDelete) {
    memset(_oleserveritem_padding, 0, sizeof(_oleserveritem_padding));
    m_pDocument = pServerDoc;
    GetServerItemState(this, true);
    if (pServerDoc) AddServerDocItem(pServerDoc, this);
}
COleServerItem::~COleServerItem() {
    if (m_pServerDoc) RemoveServerDocItem(m_pServerDoc, this);
    RemoveServerItemState(this);
}
BOOL COleServerItem::OnDraw(CDC* pDC, CSize& rSize) {
    return OnDrawEx(pDC, DVASPECT_CONTENT, rSize);
}
BOOL COleServerItem::OnDrawEx(CDC* pDC, DVASPECT nDrawAspect, CSize& rSize) {
    (void)pDC; // Lifecycle-only fallback: use stored extent when no rendering backend is available.
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    return OnGetExtent(nDrawAspect, rSize);
}
BOOL COleServerItem::OnGetExtent(DVASPECT nDrawAspect, CSize& rSize) {
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    ServerItemState* state = GetServerItemState(this, false);
    if (!state || !state->hasExtent) {
        rSize.cx = 0;
        rSize.cy = 0;
        return TRUE;
    }
    rSize.cx = state->contentExtent.cx;
    rSize.cy = state->contentExtent.cy;
    return TRUE;
}
BOOL COleServerItem::OnSetExtent(DVASPECT nDrawAspect, const CSize& size) {
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    ServerItemState* state = GetServerItemState(this, true);
    if (!state) return FALSE;
    state->contentExtent.cx = size.cx;
    state->contentExtent.cy = size.cy;
    state->hasExtent = TRUE;
    return TRUE;
}
void COleServerItem::Serialize(CArchive& ar) {
    CDocItem::Serialize(ar);
}
COleDataSource* COleServerItem::GetDataSource() {
    ServerItemState* state = GetServerItemState(this, true);
    if (!state) return nullptr;
    if (!state->dataSource) state->dataSource = new COleDataSource();
    return state->dataSource;
}
void COleServerItem::CopyToClipboard(BOOL bIncludeLink) {
    (void)bIncludeLink;
    COleDataSource* dataSource = GetDataSource();
    if (dataSource) dataSource->SetClipboard();
}
void COleServerItem::NotifyChanged() {
    if (m_pServerDoc) m_pServerDoc->NotifyChanged();
}
BOOL COleServerItem::IsConnected() const {
    return m_pServerDoc && m_pServerDoc->IsRegistered();
}
COleServerDoc* COleServerItem::GetDocument() const {
    return m_pServerDoc;
}
