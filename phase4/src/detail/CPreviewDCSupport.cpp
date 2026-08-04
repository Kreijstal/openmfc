#include "CPreviewDCSupport.h"

namespace openmfc { namespace detail { namespace cpreviewdc {
std::mutex g_previewDcMutex;
std::unordered_map<void*, PreviewDCState> g_previewDcState;
CDC* PreviewAsCDC(void* pThis) {
    return static_cast<CDC*>(pThis);
}
PreviewDCState GetPreviewState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_previewDcMutex);
    auto it = g_previewDcState.find(pThis);
    return it != g_previewDcState.end() ? it->second : PreviewDCState();
}
void SetPreviewScale(void* pThis, int num, int den) {
    std::lock_guard<std::mutex> lock(g_previewDcMutex);
    auto& state = g_previewDcState[pThis];
    state.scaleNum = num ? num : 1;
    state.scaleDen = den ? den : 1;
}
void SetPreviewOffset(void* pThis, CSize offset) {
    std::lock_guard<std::mutex> lock(g_previewDcMutex);
    g_previewDcState[pThis].topLeftOffset = offset;
}
int ScaleCoord(int value, const PreviewDCState& state) {
    return ::MulDiv(value, state.scaleNum, state.scaleDen);
}
RECT ScaleRect(const RECT* rect, const PreviewDCState& state) {
    RECT out = {};
    if (!rect) return out;
    out.left = ScaleCoord(rect->left, state);
    out.top = ScaleCoord(rect->top, state);
    out.right = ScaleCoord(rect->right, state);
    out.bottom = ScaleCoord(rect->bottom, state);
    return out;
}
HDC OutputDC(void* pThis) {
    CDC* dc = PreviewAsCDC(pThis);
    return dc ? dc->m_hDC : nullptr;
}
HDC AttribDC(void* pThis) {
    CDC* dc = PreviewAsCDC(pThis);
    if (!dc) return nullptr;
    return dc->m_hAttribDC ? dc->m_hAttribDC : dc->m_hDC;
}
} } }  // namespace openmfc::detail::cpreviewdc
