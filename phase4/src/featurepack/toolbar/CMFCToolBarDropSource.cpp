// Toolbar-specific OLE drop source and cursor feedback.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <new>

extern "C" HINSTANCE impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t*, const wchar_t*);

namespace {
HCURSOR LoadToolbarCursor(UINT id) {
    const auto name = MAKEINTRESOURCEW(id);
    const auto module = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, RT_GROUP_CURSOR);
    return ::LoadCursorW(module, name);
}
}

CMFCToolBarDropSource::CMFCToolBarDropSource()
    : m_bDeleteOnDrop(TRUE), m_bEscapePressed(FALSE), m_bDragStarted(FALSE), m_bReleased(FALSE),
      m_hcurDelete(nullptr), m_hcurMove(nullptr), m_hcurCopy(nullptr) {}

// LoadCursorW returns shared handles owned by the resource module.
CMFCToolBarDropSource::~CMFCToolBarDropSource() = default;

// Symbol: ?m_DropSource@CMFCToolBar@@1VCMFCToolBarDropSource@@A
extern "C" CMFCToolBarDropSource impl__m_DropSource_CMFCToolBar__1VCMFCToolBarDropSource__A = {};

SCODE CMFCToolBarDropSource::GiveFeedback(DROPEFFECT effect) {
    const HCURSOR cursor = effect == DROPEFFECT_COPY ? m_hcurCopy :
                          effect == DROPEFFECT_MOVE ? m_hcurMove : m_hcurDelete;
    if (!cursor) return COleDropSource::GiveFeedback(effect);
    ::SetCursor(cursor);
    return S_OK;
}

SCODE CMFCToolBarDropSource::QueryContinueDrag(BOOL escape, DWORD keys) {
    if (m_bDeleteOnDrop && m_hcurDelete) ::SetCursor(m_hcurDelete);
    m_bEscapePressed = escape;
    const SCODE result = COleDropSource::QueryContinueDrag(escape, keys);
    m_bReleased = result == DRAGDROP_S_DROP;
    return result;
}

BOOL CMFCToolBarDropSource::OnBeginDrag(CWnd* window) {
    if (!m_hcurDelete) {
        m_hcurDelete = LoadToolbarCursor(16133);
        m_hcurMove = LoadToolbarCursor(16146);
        m_hcurCopy = LoadToolbarCursor(16004);
    }
    m_bDragStarted = TRUE;
    m_bReleased = FALSE;
    return COleDropSource::OnBeginDrag(window);
}

// Symbol: ??0CMFCToolBarDropSource@@QEAA@XZ
extern "C" void* impl___0CMFCToolBarDropSource__QEAA_XZ(void* self) {
    return new (self) CMFCToolBarDropSource();
}

// Symbol: ??1CMFCToolBarDropSource@@UEAA@XZ
extern "C" void impl___1CMFCToolBarDropSource__UEAA_XZ(CMFCToolBarDropSource* self) {
    self->CMFCToolBarDropSource::~CMFCToolBarDropSource();
}

// Symbol: ?GiveFeedback@CMFCToolBarDropSource@@UEAAJK@Z
extern "C" SCODE impl__GiveFeedback_CMFCToolBarDropSource__UEAAJK_Z(
    CMFCToolBarDropSource* self, DROPEFFECT effect) {
    return self->CMFCToolBarDropSource::GiveFeedback(effect);
}

// Symbol: ?QueryContinueDrag@CMFCToolBarDropSource@@UEAAJHK@Z
extern "C" SCODE impl__QueryContinueDrag_CMFCToolBarDropSource__UEAAJHK_Z(
    CMFCToolBarDropSource* self, BOOL escape, DWORD keys) {
    return self->CMFCToolBarDropSource::QueryContinueDrag(escape, keys);
}

// Symbol: ?OnBeginDrag@CMFCToolBarDropSource@@UEAAHPEAVCWnd@@@Z
extern "C" BOOL impl__OnBeginDrag_CMFCToolBarDropSource__UEAAHPEAVCWnd___Z(
    CMFCToolBarDropSource* self, CWnd* window) {
    return self->CMFCToolBarDropSource::OnBeginDrag(window);
}
