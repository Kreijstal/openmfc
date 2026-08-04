#include "ManualSmallStubImplementationsSupport.h"

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
HFONT GetChevronOwnerDrawMenuFont() {
    NONCLIENTMETRICSW ncm {};
    ncm.cbSize = sizeof(ncm);
    HFONT font = nullptr;
    if (::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0)) {
        font = ::CreateFontIndirectW(&ncm.lfMenuFont);
    }
    if (font == nullptr) {
        font = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    }
    return font;
}
} } }

// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__CreateEx_CHotKeyCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long dwExStyle, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID);
extern "C" int MS_ABI impl__SetActivePage_CPropertySheet__QEAAHH_Z(CPropertySheet* pThis, int p0);

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
unsigned long long HashRawBytes(const void* data, std::size_t bytes) {
    if (data == nullptr) {
        return 0;
    }
    const unsigned char* bytesPtr = static_cast<const unsigned char*>(data);
    unsigned long long hash = 2166136261u;
    for (std::size_t i = 0; i < bytes; ++i) {
        hash ^= static_cast<unsigned long long>(bytesPtr[i]);
        hash *= 16777619u;
    }
    return hash;
}
std::mutex g_cd2dBitmapStateMutex;
std::unordered_map<void*, CD2DBitmapState> g_cd2dbitmapStates;
std::mutex g_cd2dBitmapBrushStateMutex;
std::unordered_map<void*, CD2DBitmapBrushState> g_cd2dbitmapBrushStates;
std::mutex g_cd2dBrushStateMutex;
std::unordered_map<void*, CD2DBrushState> g_cd2dBrushStates;
std::mutex g_cd2dLinearGradientBrushStateMutex;
std::unordered_map<void*, CD2DLinearGradientBrushState> g_cd2dLinearGradientBrushStates;
std::mutex g_cd2dGradientBrushStateMutex;
std::unordered_map<void*, CD2DGradientBrushState> g_cd2dGradientBrushStates;
std::mutex g_cd2dRadialGradientBrushStateMutex;
std::unordered_map<void*, CD2DRadialGradientBrushState> g_cd2dRadialGradientBrushStates;
std::mutex g_baseTransitionStateMutex;
std::unordered_map<void*, CBaseTransitionState> g_baseTransitionStates;
std::mutex g_enumConnPointsStateMutex;
std::unordered_map<void*, CEnumConnPointsState> g_enumConnPointsStates;
std::mutex g_enumConnectionsStateMutex;
std::unordered_map<void*, CEnumConnectionsState> g_enumConnectionsStates;
std::mutex g_docObjectServerItemStateMutex;
std::unordered_map<void*, CDocObjectServerItemState> g_docObjectServerItemStates;
std::mutex g_printInfoStateMutex;
std::unordered_map<void*, bool> g_printInfoStates;
std::mutex g_keyFrameStateMutex;
std::unordered_map<void*, CKeyFrameState> g_keyFrameStates;
std::mutex g_hotKeyCtrlStateMutex;
std::unordered_map<void*, CHotKeyCtrlState> g_hotKeyCtrlStates;
std::mutex g_outlookCustomizeButtonStateMutex;
std::unordered_map<void*, COutlookCustomizeButtonState> g_outlookCustomizeButtonStates;
std::mutex g_cmfccaptionButtonStateMutex;
std::unordered_map<void*, CMFCCaptionButtonState> g_cmfccaptionButtonStates;
std::mutex g_printPreviewToolBarStateMutex;
std::unordered_map<void*, CMFCPrintPreviewToolBarState> g_printPreviewToolBarStates;
std::mutex g_colorPopupMenuStateMutex;
std::unordered_map<void*, CMFCColorPopupMenuState> g_colorPopupMenuStates;
std::mutex g_animationValueStateMutex;
std::unordered_map<void*, CAnimationValueState> g_animationValueStates;
void ApplyBitmapBrushDefaults(CD2DBitmapBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_bitmap = nullptr;
    state.m_bitmapObject = nullptr;
    state.m_extendModeX = 0;
    state.m_extendModeY = 0;
    state.m_interpolationMode = 0;
}
CD2DSizeU CalcSizeFromRect(const CD2DRectU* pRect) {
    if (pRect == nullptr) {
        return CD2DSizeU{0, 0};
    }

    CD2DSizeU size{0, 0};
    if (pRect->right >= pRect->left) {
        size.width = pRect->right - pRect->left;
    }
    if (pRect->bottom >= pRect->top) {
        size.height = pRect->bottom - pRect->top;
    }
    return size;
}
void ApplyBitmapDefaults(CD2DBitmapState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_dpi = CD2DSizeF_ManualSmallStubImplementations{96.0f, 96.0f};
    state.m_pixelSize = CD2DSizeU{0, 0};
    state.m_pixelFormat = D2D1_PIXEL_FORMAT{0, 0};
}
CD2DBitmapState& EnsureCD2DBitmapState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    auto [it, inserted] = g_cd2dbitmapStates.try_emplace(pThis);
    if (inserted) {
        ApplyBitmapDefaults(it->second);
    }
    return it->second;
}
bool TryLoadCD2DBitmapState(void* pThis, CD2DBitmapState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    auto it = g_cd2dbitmapStates.find(pThis);
    if (it == g_cd2dbitmapStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}
CD2DBitmapBrushState& EnsureCD2DBitmapBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    auto [it, inserted] = g_cd2dbitmapBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyBitmapBrushDefaults(it->second);
    }
    return it->second;
}
bool TryLoadCD2DBitmapBrushState(void* pThis, CD2DBitmapBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    auto it = g_cd2dbitmapBrushStates.find(pThis);
    if (it == g_cd2dbitmapBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}
CKeyFrameState& EnsureCKeyFrameState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_keyFrameStateMutex);
    auto [it, inserted] = g_keyFrameStates.try_emplace(pThis);
    if (inserted) {
        it->second = CKeyFrameState{};
    }
    return it->second;
}
bool TryLoadCKeyFrameState(void* pThis, CKeyFrameState& outState) {
    std::lock_guard<std::mutex> lock(g_keyFrameStateMutex);
    auto it = g_keyFrameStates.find(pThis);
    if (it == g_keyFrameStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}
CHotKeyCtrlState& EnsureCHotKeyCtrlState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_hotKeyCtrlStateMutex);
    auto [it, inserted] = g_hotKeyCtrlStates.try_emplace(pThis);
    if (inserted) {
        it->second = CHotKeyCtrlState{};
    }
    return it->second;
}
bool TryLoadCHotKeyCtrlState(void* pThis, CHotKeyCtrlState& outState) {
    std::lock_guard<std::mutex> lock(g_hotKeyCtrlStateMutex);
    auto it = g_hotKeyCtrlStates.find(pThis);
    if (it == g_hotKeyCtrlStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}
void ClearCHotKeyCtrlState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_hotKeyCtrlStateMutex);
    g_hotKeyCtrlStates.erase(pThis);
}
COutlookCustomizeButtonState& EnsureCOutlookCustomizeButtonState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_outlookCustomizeButtonStateMutex);
    auto [it, inserted] = g_outlookCustomizeButtonStates.try_emplace(pThis);
    if (inserted) {
        it->second = COutlookCustomizeButtonState{};
    }
    return it->second;
}
void ClearCOutlookCustomizeButtonState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_outlookCustomizeButtonStateMutex);
    auto it = g_outlookCustomizeButtonStates.find(pThis);
    if (it == g_outlookCustomizeButtonStates.end()) {
        return;
    }
    if (it->second.m_popupMenu != nullptr) {
        delete reinterpret_cast<CObject*>(it->second.m_popupMenu);
    }
    g_outlookCustomizeButtonStates.erase(it);
}
CMFCCaptionButtonState& EnsureCMFCCaptionButtonState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cmfccaptionButtonStateMutex);
    auto [it, inserted] = g_cmfccaptionButtonStates.try_emplace(pThis);
    if (inserted) {
        it->second = CMFCCaptionButtonState{};
    }
    return it->second;
}
CMFCPrintPreviewToolBarState& EnsureCMFCPrintPreviewToolBarState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printPreviewToolBarStateMutex);
    auto [it, inserted] = g_printPreviewToolBarStates.try_emplace(pThis);
    if (inserted) {
        it->second = CMFCPrintPreviewToolBarState{};
    }
    return it->second;
}
void ClearCMFCPrintPreviewToolBarState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printPreviewToolBarStateMutex);
    g_printPreviewToolBarStates.erase(pThis);
}
CMFCColorPopupMenuState& EnsureCMFCColorPopupMenuState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_colorPopupMenuStateMutex);
    auto [it, inserted] = g_colorPopupMenuStates.try_emplace(pThis);
    if (inserted) {
        it->second = CMFCColorPopupMenuState{};
    }
    return it->second;
}
void ClearCMFCColorPopupMenuState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_colorPopupMenuStateMutex);
    auto it = g_colorPopupMenuStates.find(pThis);
    if (it == g_colorPopupMenuStates.end()) {
        return;
    }
    if (it->second.m_tearOffBar != nullptr) {
        delete static_cast<CPane*>(it->second.m_tearOffBar);
    }
    g_colorPopupMenuStates.erase(it);
}
void ApplyCD2DBrushDefaults(CD2DBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_opacity = 1.0f;
    state.m_transform = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
}
CD2DBrushState& EnsureCD2DBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    auto [it, inserted] = g_cd2dBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyCD2DBrushDefaults(it->second);
    }
    return it->second;
}
bool TryLoadCD2DBrushState(void* pThis, CD2DBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    auto it = g_cd2dBrushStates.find(pThis);
    if (it == g_cd2dBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}
void ApplyTransformFromBrushProperties(CD2DBrushState& state, const float* pBrushProps) {
    if (pBrushProps == nullptr) {
        return;
    }
    state.m_opacity = pBrushProps[0];
    state.m_transform._11 = pBrushProps[1];
    state.m_transform._12 = pBrushProps[2];
    state.m_transform._21 = pBrushProps[3];
    state.m_transform._22 = pBrushProps[4];
    state.m_transform._31 = pBrushProps[5];
    state.m_transform._32 = pBrushProps[6];
}
void ApplyLinearGradientBrushDefaults(CD2DLinearGradientBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_startPoint = {0.0f, 0.0f};
    state.m_endPoint = {0.0f, 0.0f};
}
CD2DLinearGradientBrushState& EnsureCD2DLinearGradientBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    auto [it, inserted] = g_cd2dLinearGradientBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyLinearGradientBrushDefaults(it->second);
    }
    return it->second;
}
bool TryLoadCD2DLinearGradientBrushState(void* pThis, CD2DLinearGradientBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    auto it = g_cd2dLinearGradientBrushStates.find(pThis);
    if (it == g_cd2dLinearGradientBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}
void ApplyLinearGradientBrushProperties(
    CD2DLinearGradientBrushState& state, const void* pProperties) {
    const float* properties = static_cast<const float*>(pProperties);
    if (properties == nullptr) {
        return;
    }
    state.m_startPoint = {properties[0], properties[1]};
    state.m_endPoint = {properties[2], properties[3]};
}
void ApplyCD2DRadialGradientBrushDefaults(CD2DRadialGradientBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
}
CD2DRadialGradientBrushState& EnsureCD2DRadialGradientBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dRadialGradientBrushStateMutex);
    auto [it, inserted] = g_cd2dRadialGradientBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyCD2DRadialGradientBrushDefaults(it->second);
    }
    return it->second;
}
bool TryLoadCD2DRadialGradientBrushState(void* pThis, CD2DRadialGradientBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dRadialGradientBrushStateMutex);
    auto it = g_cd2dRadialGradientBrushStates.find(pThis);
    if (it == g_cd2dRadialGradientBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}
CD2DGradientBrushState& EnsureCD2DGradientBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dGradientBrushStateMutex);
    auto [it, inserted] = g_cd2dGradientBrushStates.try_emplace(pThis);
    if (inserted) {
        it->second.m_resource = nullptr;
        it->second.m_parentRenderTarget = nullptr;
        it->second.m_opacity = 1.0f;
    }
    return it->second;
}
CBaseTransitionState& EnsureCBaseTransitionState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_baseTransitionStateMutex);
    auto [it, inserted] = g_baseTransitionStates.try_emplace(pThis);
    return it->second;
}
CEnumConnPointsState& EnsureCEnumConnPointsState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_enumConnPointsStateMutex);
    auto [it, inserted] = g_enumConnPointsStates.try_emplace(pThis);
    return it->second;
}
CEnumConnectionsState& EnsureCEnumConnectionsState(void* pThis, bool synthetic) {
    std::lock_guard<std::mutex> lock(g_enumConnectionsStateMutex);
    auto [it, inserted] = g_enumConnectionsStates.try_emplace(pThis);
    if (inserted) {
        it->second.m_synthetic = synthetic;
    }
    return it->second;
}
void ClearCEnumConnPointsState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_enumConnPointsStateMutex);
    const auto it = g_enumConnPointsStates.find(pThis);
    if (it == g_enumConnPointsStates.end()) {
        return;
    }
    for (void* connectionPoint : it->second.m_connectionPoints) {
        if (connectionPoint != nullptr) {
            static_cast<IUnknown*>(connectionPoint)->Release();
        }
    }
    g_enumConnPointsStates.erase(it);
}
void ClearCEnumConnectionsState(void* pThis) {
    bool synthetic = false;
    {
        std::lock_guard<std::mutex> lock(g_enumConnectionsStateMutex);
        const auto it = g_enumConnectionsStates.find(pThis);
        if (it == g_enumConnectionsStates.end()) {
            return;
        }
        for (const CONNECTDATA& entry : it->second.m_connections) {
            if (entry.pUnk != nullptr) {
                entry.pUnk->Release();
            }
        }
        synthetic = it->second.m_synthetic;
        g_enumConnectionsStates.erase(it);
    }
    if (synthetic) {
        ::operator delete(pThis);
    }
}
CDocObjectServerItemState& EnsureCDocObjectServerItemState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_docObjectServerItemStateMutex);
    auto [it, inserted] = g_docObjectServerItemStates.try_emplace(pThis);
    return it->second;
}
void ClearCDocObjectServerItemState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_docObjectServerItemStateMutex);
    g_docObjectServerItemStates.erase(pThis);
}
void TrackCPrintInfo(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printInfoStateMutex);
    g_printInfoStates[pThis] = true;
}
void UntrackCPrintInfo(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printInfoStateMutex);
    g_printInfoStates.erase(pThis);
}
std::mutex g_bitmapButtonStateMutex;
std::unordered_map<void*, CBitmapButtonState> g_bitmapButtonStates;
std::mutex g_bitmapRenderTargetMutex;
std::unordered_map<void*, void*> g_bitmapRenderTargetBitmaps;
std::mutex g_handleMapMutex;
std::unordered_map<void*, std::unordered_map<void*, CTempHandleObject_ManualSmallStubImplementations*>> g_tempHandleMap;
std::mutex g_innerUnknownMutex;
std::unordered_map<void*, unsigned long> g_innerUnknownRefCounts;
std::mutex g_coleUIlinkInfoMutex;
std::unordered_map<void*, std::pair<COleDocument*, COleClientItem*>> g_coleUILinkInfoTargets;
std::mutex g_reflectorControlMutex;
std::unordered_map<void*, COleControl*> g_reflectorControls_ManualSmallStubImplementations;
HBITMAP LoadBitmapFromResource(const wchar_t* resource) {
    if (resource == nullptr) {
        return nullptr;
    }
    return static_cast<HBITMAP>(::LoadImageW(AfxGetInstanceHandle(), resource, IMAGE_BITMAP, 0, 0,
                                            LR_CREATEDIBSECTION | LR_DEFAULTCOLOR));
}
HBITMAP PickBitmapForBitmapButton(const CBitmapButtonState& state, unsigned int itemState) {
    if ((itemState & ODS_DISABLED) != 0 && state.disabled != nullptr) {
        return state.disabled;
    }
    if ((itemState & ODS_SELECTED) != 0 && state.selected != nullptr) {
        return state.selected;
    }
    if ((itemState & ODS_FOCUS) != 0 && state.focus != nullptr) {
        return state.focus;
    }
    if (state.normal != nullptr) {
        return state.normal;
    }
    if (state.selected != nullptr) {
        return state.selected;
    }
    if (state.disabled != nullptr) {
        return state.disabled;
    }
    return state.focus;
}
bool LoadBitmapButtonStateImages(
    void* pThis, const wchar_t* normal, const wchar_t* selected,
    const wchar_t* disabled, const wchar_t* focus) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr) {
        return false;
    }

    CBitmapButtonState nextState{
        LoadBitmapFromResource(normal),
        LoadBitmapFromResource(selected),
        LoadBitmapFromResource(disabled),
        LoadBitmapFromResource(focus),
    };
    if (nextState.normal == nullptr && nextState.selected == nullptr &&
        nextState.disabled == nullptr && nextState.focus == nullptr) {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(g_bitmapButtonStateMutex);
        auto previous = g_bitmapButtonStates.find(pThis);
        if (previous != g_bitmapButtonStates.end()) {
            if (previous->second.normal != nullptr &&
                previous->second.normal != nextState.normal) {
                ::DeleteObject(previous->second.normal);
            }
            if (previous->second.selected != nullptr &&
                previous->second.selected != nextState.selected) {
                ::DeleteObject(previous->second.selected);
            }
            if (previous->second.disabled != nullptr &&
                previous->second.disabled != nextState.disabled) {
                ::DeleteObject(previous->second.disabled);
            }
            if (previous->second.focus != nullptr &&
                previous->second.focus != nextState.focus) {
                ::DeleteObject(previous->second.focus);
            }
        }

        g_bitmapButtonStates[pThis] = nextState;
    }

    HBITMAP image = PickBitmapForBitmapButton(nextState, 0);
    if (image != nullptr) {
        pWnd->SendMessageW(BM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(image));
        return true;
    }
    return false;
}
void SetCOleUILinkInfoTarget(void* pThis, COleDocument* document, COleClientItem* item) {
    std::lock_guard<std::mutex> lock(g_coleUIlinkInfoMutex);
    g_coleUILinkInfoTargets[pThis] = std::make_pair(document, item);
}
bool GetCOleUILinkInfoTarget(void* pThis, COleDocument*& pDocument, COleClientItem*& pItem) {
    std::lock_guard<std::mutex> lock(g_coleUIlinkInfoMutex);
    auto it = g_coleUILinkInfoTargets.find(pThis);
    if (it == g_coleUILinkInfoTargets.end()) {
        pDocument = nullptr;
        pItem = nullptr;
        return false;
    }
    pDocument = it->second.first;
    pItem = it->second.second;
    return true;
}
COleClientItem* FindItemByLinkIndex(const COleDocument* pDocument, unsigned long link) {
    if (!pDocument || link == 0) {
        return nullptr;
    }
    POSITION pos = pDocument->GetStartPosition();
    for (unsigned long i = 1; pos != nullptr; ++i) {
        COleClientItem* item = pDocument->GetNextClientItem(pos);
        if (i == link) {
            return item;
        }
    }
    return nullptr;
}
HRESULT UpdateSingleOleLinkItem(COleClientItem* pItem) {
    if (!pItem || !pItem->m_lpLink) {
        return OLE_E_NOTRUNNING;
    }
    return pItem->m_lpLink->Update(nullptr);
}
void DrawBitmapCentered(HDC hdc, HBITMAP hBitmap, const RECT& rect) {
    if (hdc == nullptr || hBitmap == nullptr) {
        return;
    }

    HDC mem = ::CreateCompatibleDC(hdc);
    if (mem == nullptr) {
        return;
    }

    HGDIOBJ prevBitmap = ::SelectObject(mem, hBitmap);
    BITMAP bmp {};
    if (!::GetObjectW(hBitmap, sizeof(bmp), &bmp)) {
        ::SelectObject(mem, prevBitmap);
        ::DeleteDC(mem);
        return;
    }

    const int targetW = std::max(static_cast<LONG>(0), rect.right - rect.left);
    const int targetH = std::max(static_cast<LONG>(0), rect.bottom - rect.top);
    const int bitmapW = static_cast<int>(bmp.bmWidth);
    const int bitmapH = static_cast<int>(bmp.bmHeight);
    const int drawW = std::min(targetW, bitmapW);
    const int drawH = std::min(targetH, bitmapH);
    const int x = rect.left + std::max(0, (targetW - drawW) / 2);
    const int y = rect.top + std::max(0, (targetH - drawH) / 2);

    ::StretchBlt(hdc, x, y, drawW, drawH, mem, 0, 0, bitmapW, bitmapH, SRCCOPY);
    ::SelectObject(mem, prevBitmap);
    ::DeleteDC(mem);
}
void ResetCDBVariantMemory(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    struct CDBVariantStorage {
        void* vfptr;
        std::uint32_t m_dwType;
        std::uint32_t pad;
        union {
            int m_boolVal;
            unsigned char m_chVal;
            short m_iVal;
            long m_lVal;
            float m_fltVal;
            double m_dblVal;
            void* m_ptr;
        };
    };

    enum {
        DBVT_NULL = 0,
        DBVT_DATE = 7,
        DBVT_STRING = 8,
        DBVT_BINARY = 9,
        DBVT_ASTRING = 10,
        DBVT_WSTRING = 11
    };

    auto* pVar = static_cast<CDBVariantStorage*>(pThis);
    switch (pVar->m_dwType) {
    case DBVT_DATE:
    case DBVT_STRING:
    case DBVT_BINARY:
    case DBVT_ASTRING:
    case DBVT_WSTRING:
        if (pVar->m_ptr != nullptr) {
            ::operator delete(pVar->m_ptr);
        }
        break;
    default:
        break;
    }

    pVar->m_dwType = DBVT_NULL;
    pVar->m_ptr = nullptr;
}
std::mutex g_maintainStateMutex;
std::unordered_map<void*, MaintainStateRecord> g_maintainStateMap;
void* MS_ABI enter_maintain_state(void* pThis, void* pState, bool useThreadLock) {
    if (pThis == nullptr) {
        return pThis;
    }

    MaintainStateRecord info{};
    if (pState == nullptr) {
        pState = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    }
    info.prevModuleState = static_cast<LocalAFX_MODULE_STATE*>(impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(pState));

    if (useThreadLock) {
        if (auto* threadState = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()) {
            info.trackedThreadLock = true;
            info.prevThreadLock = threadState->m_nTempMapLock;
            ++threadState->m_nTempMapLock;
        }
    }

    std::lock_guard<std::mutex> lock(g_maintainStateMutex);
    g_maintainStateMap[pThis] = info;
    return pThis;
}
void MS_ABI leave_maintain_state(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    MaintainStateRecord info{};
    {
        std::lock_guard<std::mutex> lock(g_maintainStateMutex);
        auto it = g_maintainStateMap.find(pThis);
        if (it == g_maintainStateMap.end()) {
            return;
        }
        info = it->second;
        g_maintainStateMap.erase(it);
    }

    impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(info.prevModuleState);
    if (info.trackedThreadLock) {
        if (auto* threadState = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()) {
            threadState->m_nTempMapLock = info.prevThreadLock;
        }
    }
}
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarColorButton__SAPEAVCObject__XZ() {
    return new (std::nothrow) CMFCToolBarColorButtonObject();
}
extern "C" int MS_ABI impl__Create_CHotKeyCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID) {
    return impl__CreateEx_CHotKeyCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
        pThis, 0, nStyles, pRect, pParentWnd, nID);
}
int AddBitmapViaHandle(CWnd* pWnd, int nNumBits, HBITMAP hBitmap) {
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || hBitmap == nullptr) {
        return -1;
    }

    TBADDBITMAP info{};
    info.hInst = nullptr;
    info.nID = reinterpret_cast<UINT_PTR>(hBitmap);
    return static_cast<int>(::SendMessageW(pWnd->GetSafeHwnd(), TB_ADDBITMAP, static_cast<WPARAM>(nNumBits), reinterpret_cast<LPARAM>(&info)));
}
extern "C" void MS_ABI impl__DrawItem_CChevronOwnerDrawMenu__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, void* pDis) {
    (void)pThis;
    auto* dis = static_cast<DRAWITEMSTRUCT*>(pDis);
    if (dis == nullptr || dis->CtlType != ODT_MENU || dis->hDC == nullptr) {
        return;
    }

    HDC hdc = dis->hDC;
    const RECT& rc = dis->rcItem;
    bool selected = (dis->itemState & ODS_SELECTED) != 0;
    bool grayed = (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) != 0;

    COLORREF bk = ::GetSysColor(selected ? COLOR_HIGHLIGHT : COLOR_MENU);
    HBRUSH hBrush = ::CreateSolidBrush(bk);
    if (hBrush) {
        ::FillRect(hdc, &rc, hBrush);
        ::DeleteObject(hBrush);
    }

    LPCWSTR text = reinterpret_cast<LPCWSTR>(dis->itemData);
    if (text != nullptr && text[0] != 0) {
        HFONT font = GetChevronOwnerDrawMenuFont();
        HGDIOBJ oldFont = ::SelectObject(hdc, font);
        int oldBk = ::SetBkMode(hdc, TRANSPARENT);
        COLORREF oldText = ::SetTextColor(
            hdc,
            ::GetSysColor(grayed ? COLOR_GRAYTEXT : (selected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT)));
        RECT textRect = rc;
        textRect.left += ::GetSystemMetrics(SM_CXMENUCHECK) + 2;
        ::DrawTextW(hdc, text, -1, &textRect,
            DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
        ::SetTextColor(hdc, oldText);
        ::SetBkMode(hdc, oldBk);
        if (oldFont) {
            ::SelectObject(hdc, oldFont);
        }
    }
}
HWND WndHandleForObject(void* pWnd) {
    return pWnd ? *reinterpret_cast<HWND*>(static_cast<char*>(pWnd) + 64) : NULL;
}
HDC DCHandleForCDC(void* pDC) {
    return pDC ? *reinterpret_cast<HDC*>(static_cast<char*>(pDC) + 8) : NULL;
}
RECT GetCaptionMenuRect(void* pThis) {
    auto* self = static_cast<CMFCCaptionMenuButtonLayout*>(pThis);
    RECT rc = {};
    if (self != nullptr) {
        rc.left = self->m_ptOrg.x;
        rc.top = self->m_ptOrg.y;
        rc.right = self->m_ptOrg.x;
        rc.bottom = self->m_ptOrg.y;
    }
    return rc;
}
extern "C" int MS_ABI impl__SetActiveTab_CMFCPropertySheetTabCtrl__EEAAHH_Z(
    void* pThis, int nTab) {
    struct CMFCPropertySheetTabCtrlLayout {
        void* vfptr;
        unsigned int m_dwRef;
        unsigned int _align;
        char _opaque[15720 - 16];
        void* m_pParent;
    };

    auto* self = static_cast<CMFCPropertySheetTabCtrlLayout*>(pThis);
    if (self == nullptr || self->m_pParent == nullptr || nTab < 0) {
        return 0;
    }

    return impl__SetActivePage_CPropertySheet__QEAAHH_Z(self->m_pParent, nTab);
}
} } }  // namespace openmfc::detail::manualsmallstubimplementations
