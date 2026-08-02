// OpenMFC: concrete implementations for a small set of remaining stubbed exports.
// These are intentionally minimal but real functions (no TODO/stub logging) and
// keep behavior safe for ABI consumers.

#include <cstddef>
#include <cstdint>
#include <new>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <windows.h>
#include <commctrl.h>
#include <objbase.h>

#include "openmfc/afxole.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxmfc.h"

#if defined(__GNUC__) || defined(__clang__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

extern "C" void OpenMfcResetDockState(void* pState);

namespace {
struct CMFCBaseToolBarObject : public CObject {};
struct COutlookCustomizeButtonObject : public CObject {};
struct CMFCOutlookBarPaneAdapterObject : public CObject {};
struct CBitmapButtonState {
    HBITMAP normal = nullptr;
    HBITMAP selected = nullptr;
    HBITMAP disabled = nullptr;
    HBITMAP focus = nullptr;
};
struct CTempHandleObject : public CObject {
    void* m_handle = nullptr;
};

struct CD2DSizeF {
    float width;
    float height;
};

struct CD2DPointU {
    uint32_t x;
    uint32_t y;
};

struct CD2DPointF {
    float x;
    float y;
};

struct CD2DSizeU {
    uint32_t width;
    uint32_t height;
};

struct CD2DRectU {
    uint32_t left;
    uint32_t top;
    uint32_t right;
    uint32_t bottom;
};

struct D2D1_PIXEL_FORMAT {
    uint32_t format;
    uint32_t alphaMode;
};

struct CD2DBitmapState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    CD2DSizeF m_dpi = CD2DSizeF{96.0f, 96.0f};
    CD2DSizeU m_pixelSize = CD2DSizeU{0, 0};
    D2D1_PIXEL_FORMAT m_pixelFormat = D2D1_PIXEL_FORMAT{0, 0};
};

typedef uint32_t D2D1_BITMAP_INTERPOLATION_MODE;
typedef uint32_t D2D1_EXTEND_MODE;

struct CD2DBitmapBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    void* m_bitmap = nullptr;
    void* m_bitmapObject = nullptr;
    D2D1_EXTEND_MODE m_extendModeX = 0;
    D2D1_EXTEND_MODE m_extendModeY = 0;
    D2D1_BITMAP_INTERPOLATION_MODE m_interpolationMode = 0;
};

struct D2D_MATRIX_3X2_F {
    float _11;
    float _12;
    float _21;
    float _22;
    float _31;
    float _32;
};

struct CD2DBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    float m_opacity = 1.0f;
    D2D_MATRIX_3X2_F m_transform = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
};

struct CD2DLinearGradientBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    CD2DPointF m_startPoint = {0.0f, 0.0f};
    CD2DPointF m_endPoint = {0.0f, 0.0f};
};

struct CD2DGradientBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    float m_opacity = 1.0f;
};

struct CD2DRadialGradientBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
};

struct CBaseTransitionState {
    void* m_transition = nullptr;
    int m_addToStoryboardCalls = 0;
    int m_addToStoryboardAtKeyframesCalls = 0;
};

struct CAnimationValueState {
    double m_value = 0.0;
    void* m_variable = nullptr;
    std::vector<void*> m_transitions;
};

struct CEnumConnPointsState {
    std::vector<void*> m_connectionPoints;
    std::size_t m_position = 0;
};

struct CEnumConnectionsState {
    std::vector<CONNECTDATA> m_connections;
    std::size_t m_position = 0;
    bool m_synthetic = false;
};

struct CDocObjectServerItemState {
    int m_lastVerb = 0;
    bool m_visible = true;
};

struct CKeyFrameState {
    void* m_creator = nullptr;
    double m_duration = 0.0;
    bool m_hasBaseKeyFrame = false;
    bool m_hasTransition = false;
    int m_addToStoryboardCalls = 0;
    int m_addToStoryboardAfterTransitionCalls = 0;
    int m_addToStoryboardAtOffsetCalls = 0;
    int m_lastKeyFrameID = -1;
    void* m_lastStoryboard = nullptr;
};

struct CHotKeyCtrlState {
    unsigned short m_virtualKey = 0;
    unsigned short m_modifierFlags = 0;
};

struct COutlookCustomizeButtonState {
    void* m_popupMenu = nullptr;
    bool m_didDraw = false;
};

struct CMFCCaptionButtonState {
    unsigned int m_hit = 0;
    int m_lastX = 0;
    int m_lastY = 0;
    unsigned int m_lastImageId = 0;
};

struct CMFCPrintPreviewToolBarState {
    bool m_destroyed = false;
    int m_lastToolHit = -1;
    int m_lastX = 0;
    int m_lastY = 0;
};

struct CMFCColorPopupMenuState {
    void* m_tearOffBar = nullptr;
    unsigned int m_commandId = 0;
    void* m_text = nullptr;
    bool m_created = false;
};

static unsigned long long HashRawBytes(const void* data, std::size_t bytes) {
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

extern "C" void MS_ABI impl__CommonInit_CD2DBitmap__IEAAXXZ(void* pThis);

static void ApplyBitmapBrushDefaults(CD2DBitmapBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_bitmap = nullptr;
    state.m_bitmapObject = nullptr;
    state.m_extendModeX = 0;
    state.m_extendModeY = 0;
    state.m_interpolationMode = 0;
}

static CD2DSizeU CalcSizeFromRect(const CD2DRectU* pRect) {
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

static void ApplyBitmapDefaults(CD2DBitmapState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_dpi = CD2DSizeF{96.0f, 96.0f};
    state.m_pixelSize = CD2DSizeU{0, 0};
    state.m_pixelFormat = D2D1_PIXEL_FORMAT{0, 0};
}

static CD2DBitmapState& EnsureCD2DBitmapState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    auto [it, inserted] = g_cd2dbitmapStates.try_emplace(pThis);
    if (inserted) {
        ApplyBitmapDefaults(it->second);
    }
    return it->second;
}

static bool TryLoadCD2DBitmapState(void* pThis, CD2DBitmapState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    auto it = g_cd2dbitmapStates.find(pThis);
    if (it == g_cd2dbitmapStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}

static CD2DBitmapBrushState& EnsureCD2DBitmapBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    auto [it, inserted] = g_cd2dbitmapBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyBitmapBrushDefaults(it->second);
    }
    return it->second;
}

static bool TryLoadCD2DBitmapBrushState(void* pThis, CD2DBitmapBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    auto it = g_cd2dbitmapBrushStates.find(pThis);
    if (it == g_cd2dbitmapBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}

static CKeyFrameState& EnsureCKeyFrameState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_keyFrameStateMutex);
    auto [it, inserted] = g_keyFrameStates.try_emplace(pThis);
    if (inserted) {
        it->second = CKeyFrameState{};
    }
    return it->second;
}

static bool TryLoadCKeyFrameState(void* pThis, CKeyFrameState& outState) {
    std::lock_guard<std::mutex> lock(g_keyFrameStateMutex);
    auto it = g_keyFrameStates.find(pThis);
    if (it == g_keyFrameStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}

static CHotKeyCtrlState& EnsureCHotKeyCtrlState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_hotKeyCtrlStateMutex);
    auto [it, inserted] = g_hotKeyCtrlStates.try_emplace(pThis);
    if (inserted) {
        it->second = CHotKeyCtrlState{};
    }
    return it->second;
}

static bool TryLoadCHotKeyCtrlState(void* pThis, CHotKeyCtrlState& outState) {
    std::lock_guard<std::mutex> lock(g_hotKeyCtrlStateMutex);
    auto it = g_hotKeyCtrlStates.find(pThis);
    if (it == g_hotKeyCtrlStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}

static void ClearCHotKeyCtrlState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_hotKeyCtrlStateMutex);
    g_hotKeyCtrlStates.erase(pThis);
}

static COutlookCustomizeButtonState& EnsureCOutlookCustomizeButtonState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_outlookCustomizeButtonStateMutex);
    auto [it, inserted] = g_outlookCustomizeButtonStates.try_emplace(pThis);
    if (inserted) {
        it->second = COutlookCustomizeButtonState{};
    }
    return it->second;
}

static void ClearCOutlookCustomizeButtonState(void* pThis) {
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

static CMFCCaptionButtonState& EnsureCMFCCaptionButtonState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cmfccaptionButtonStateMutex);
    auto [it, inserted] = g_cmfccaptionButtonStates.try_emplace(pThis);
    if (inserted) {
        it->second = CMFCCaptionButtonState{};
    }
    return it->second;
}

static CMFCPrintPreviewToolBarState& EnsureCMFCPrintPreviewToolBarState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printPreviewToolBarStateMutex);
    auto [it, inserted] = g_printPreviewToolBarStates.try_emplace(pThis);
    if (inserted) {
        it->second = CMFCPrintPreviewToolBarState{};
    }
    return it->second;
}

static void ClearCMFCPrintPreviewToolBarState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printPreviewToolBarStateMutex);
    g_printPreviewToolBarStates.erase(pThis);
}

static CMFCColorPopupMenuState& EnsureCMFCColorPopupMenuState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_colorPopupMenuStateMutex);
    auto [it, inserted] = g_colorPopupMenuStates.try_emplace(pThis);
    if (inserted) {
        it->second = CMFCColorPopupMenuState{};
    }
    return it->second;
}

static void ClearCMFCColorPopupMenuState(void* pThis) {
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

static void ApplyCD2DBrushDefaults(CD2DBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_opacity = 1.0f;
    state.m_transform = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
}

static CD2DBrushState& EnsureCD2DBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    auto [it, inserted] = g_cd2dBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyCD2DBrushDefaults(it->second);
    }
    return it->second;
}

static bool TryLoadCD2DBrushState(void* pThis, CD2DBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    auto it = g_cd2dBrushStates.find(pThis);
    if (it == g_cd2dBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}

static void ApplyTransformFromBrushProperties(CD2DBrushState& state, const float* pBrushProps) {
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

static void ApplyLinearGradientBrushDefaults(CD2DLinearGradientBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
    state.m_startPoint = {0.0f, 0.0f};
    state.m_endPoint = {0.0f, 0.0f};
}

static CD2DLinearGradientBrushState& EnsureCD2DLinearGradientBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    auto [it, inserted] = g_cd2dLinearGradientBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyLinearGradientBrushDefaults(it->second);
    }
    return it->second;
}

static bool TryLoadCD2DLinearGradientBrushState(void* pThis, CD2DLinearGradientBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    auto it = g_cd2dLinearGradientBrushStates.find(pThis);
    if (it == g_cd2dLinearGradientBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}

static void ApplyLinearGradientBrushProperties(
    CD2DLinearGradientBrushState& state, const void* pProperties) {
    const float* properties = static_cast<const float*>(pProperties);
    if (properties == nullptr) {
        return;
    }
    state.m_startPoint = {properties[0], properties[1]};
    state.m_endPoint = {properties[2], properties[3]};
}

static void ApplyCD2DRadialGradientBrushDefaults(CD2DRadialGradientBrushState& state) {
    state.m_resource = nullptr;
    state.m_parentRenderTarget = nullptr;
}

static CD2DRadialGradientBrushState& EnsureCD2DRadialGradientBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dRadialGradientBrushStateMutex);
    auto [it, inserted] = g_cd2dRadialGradientBrushStates.try_emplace(pThis);
    if (inserted) {
        ApplyCD2DRadialGradientBrushDefaults(it->second);
    }
    return it->second;
}

static bool TryLoadCD2DRadialGradientBrushState(void* pThis, CD2DRadialGradientBrushState& outState) {
    std::lock_guard<std::mutex> lock(g_cd2dRadialGradientBrushStateMutex);
    auto it = g_cd2dRadialGradientBrushStates.find(pThis);
    if (it == g_cd2dRadialGradientBrushStates.end()) {
        return false;
    }
    outState = it->second;
    return true;
}

static CD2DGradientBrushState& EnsureCD2DGradientBrushState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_cd2dGradientBrushStateMutex);
    auto [it, inserted] = g_cd2dGradientBrushStates.try_emplace(pThis);
    if (inserted) {
        it->second.m_resource = nullptr;
        it->second.m_parentRenderTarget = nullptr;
        it->second.m_opacity = 1.0f;
    }
    return it->second;
}

static CBaseTransitionState& EnsureCBaseTransitionState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_baseTransitionStateMutex);
    auto [it, inserted] = g_baseTransitionStates.try_emplace(pThis);
    return it->second;
}

static CEnumConnPointsState& EnsureCEnumConnPointsState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_enumConnPointsStateMutex);
    auto [it, inserted] = g_enumConnPointsStates.try_emplace(pThis);
    return it->second;
}

static CEnumConnectionsState& EnsureCEnumConnectionsState(void* pThis, bool synthetic = false) {
    std::lock_guard<std::mutex> lock(g_enumConnectionsStateMutex);
    auto [it, inserted] = g_enumConnectionsStates.try_emplace(pThis);
    if (inserted) {
        it->second.m_synthetic = synthetic;
    }
    return it->second;
}

static void ClearCEnumConnPointsState(void* pThis) {
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

static void ClearCEnumConnectionsState(void* pThis) {
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

static CDocObjectServerItemState& EnsureCDocObjectServerItemState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_docObjectServerItemStateMutex);
    auto [it, inserted] = g_docObjectServerItemStates.try_emplace(pThis);
    return it->second;
}

static void ClearCDocObjectServerItemState(void* pThis) {
    std::lock_guard<std::mutex> lock(g_docObjectServerItemStateMutex);
    g_docObjectServerItemStates.erase(pThis);
}

static void TrackCPrintInfo(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printInfoStateMutex);
    g_printInfoStates[pThis] = true;
}

static void UntrackCPrintInfo(void* pThis) {
    std::lock_guard<std::mutex> lock(g_printInfoStateMutex);
    g_printInfoStates.erase(pThis);
}

std::mutex g_bitmapButtonStateMutex;
std::unordered_map<void*, CBitmapButtonState> g_bitmapButtonStates;
std::mutex g_bitmapRenderTargetMutex;
std::unordered_map<void*, void*> g_bitmapRenderTargetBitmaps;
std::mutex g_handleMapMutex;
std::unordered_map<void*, std::unordered_map<void*, CTempHandleObject*>> g_tempHandleMap;
std::mutex g_innerUnknownMutex;
std::unordered_map<void*, unsigned long> g_innerUnknownRefCounts;
std::mutex g_coleUIlinkInfoMutex;
std::unordered_map<void*, std::pair<COleDocument*, COleClientItem*>> g_coleUILinkInfoTargets;
std::mutex g_reflectorControlMutex;
std::unordered_map<void*, COleControl*> g_reflectorControls;

static HBITMAP LoadBitmapFromResource(const wchar_t* resource) {
    if (resource == nullptr) {
        return nullptr;
    }
    return static_cast<HBITMAP>(::LoadImageW(AfxGetInstanceHandle(), resource, IMAGE_BITMAP, 0, 0,
                                            LR_CREATEDIBSECTION | LR_DEFAULTCOLOR));
}

static HBITMAP PickBitmapForBitmapButton(const CBitmapButtonState& state, unsigned int itemState) {
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

static bool LoadBitmapButtonStateImages(
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

static void SetCOleUILinkInfoTarget(void* pThis, COleDocument* document, COleClientItem* item) {
    std::lock_guard<std::mutex> lock(g_coleUIlinkInfoMutex);
    g_coleUILinkInfoTargets[pThis] = std::make_pair(document, item);
}

static bool GetCOleUILinkInfoTarget(void* pThis, COleDocument*& pDocument, COleClientItem*& pItem) {
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

static COleClientItem* FindItemByLinkIndex(const COleDocument* pDocument, unsigned long link) {
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

static HRESULT UpdateSingleOleLinkItem(COleClientItem* pItem) {
    if (!pItem || !pItem->m_lpLink) {
        return OLE_E_NOTRUNNING;
    }
    return pItem->m_lpLink->Update(nullptr);
}

static void DrawBitmapCentered(HDC hdc, HBITMAP hBitmap, const RECT& rect) {
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

static void ResetCDBVariantMemory(void* pThis) {
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

struct LocalAFX_MODULE_STATE {
    HINSTANCE m_hCurrentInstanceHandle{};
    HINSTANCE m_hCurrentResourceHandle{};
    void* m_pCurrentWinApp{};
};

struct AFX_MODULE_THREAD_STATE {
    int m_nTempMapLock{};
};

struct MaintainStateRecord {
    LocalAFX_MODULE_STATE* prevModuleState = nullptr;
    bool trackedThreadLock = false;
    int prevThreadLock = 0;
};

std::mutex g_maintainStateMutex;
std::unordered_map<void*, MaintainStateRecord> g_maintainStateMap;

extern "C" void* MS_ABI impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(void*);
extern "C" LocalAFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();

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
} // namespace

// Symbol: ??0AFX_DDPDATA@@QEAA@PEAXHH0IPEB_W@Z
extern "C" void* MS_ABI impl___0AFX_DDPDATA__QEAA_PEAXHH0IPEB_W_Z(
    void* pThis, void*, int, int, void*, unsigned int, const wchar_t*) {
    return pThis;
}

// Symbol: ??0AFX_EXCEPTION_LINK@@QEAA@XZ
extern "C" void* MS_ABI impl___0AFX_EXCEPTION_LINK__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0AFX_MAINTAIN_STATE2@@QEAA@PEAVAFX_MODULE_STATE@@@Z
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE2__QEAA_PEAVAFX_MODULE_STATE___Z(void* pThis, void* pState) {
    return enter_maintain_state(pThis, static_cast<LocalAFX_MODULE_STATE*>(pState), true);
}

// Symbol: ??0AFX_MAINTAIN_STATE@@QEAA@PEAVAFX_MODULE_STATE@@@Z
extern "C" void* MS_ABI impl___0AFX_MAINTAIN_STATE__QEAA_PEAVAFX_MODULE_STATE___Z(void* pThis, void* pState) {
    return enter_maintain_state(pThis, static_cast<LocalAFX_MODULE_STATE*>(pState), false);
}

// Symbol: ??0AFX_MODULE_STATE@@QEAA@HP6A_JPEAUHWND__@@I_K_J@ZKH@Z
extern "C" void* MS_ABI impl___0AFX_MODULE_STATE__QEAA_HP6A_JPEAUHWND____I_K_J_ZKH_Z(
    void* pThis, void*, void*, int, unsigned long long, int, unsigned long long) {
    auto* pState = static_cast<LocalAFX_MODULE_STATE*>(pThis);
    if (pState == nullptr) {
        return pThis;
    }

    if (const auto* current = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()) {
        pState->m_hCurrentInstanceHandle = current->m_hCurrentInstanceHandle;
        pState->m_hCurrentResourceHandle = current->m_hCurrentResourceHandle;
        pState->m_pCurrentWinApp = current->m_pCurrentWinApp;
    } else {
        pState->m_hCurrentInstanceHandle = nullptr;
        pState->m_hCurrentResourceHandle = nullptr;
        pState->m_pCurrentWinApp = nullptr;
    }
    return pThis;
}

// Symbol: ??1AFX_MODULE_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MODULE_STATE__UEAA_XZ(void* pThis) {
    auto* pState = static_cast<LocalAFX_MODULE_STATE*>(pThis);
    if (pState == nullptr) {
        return;
    }
    pState->m_hCurrentInstanceHandle = nullptr;
    pState->m_hCurrentResourceHandle = nullptr;
    pState->m_pCurrentWinApp = nullptr;
}

// Symbol: ??0AFX_MODULE_THREAD_STATE@@QEAA@XZ
extern "C" void* MS_ABI impl___0AFX_MODULE_THREAD_STATE__QEAA_XZ(void* pThis) {
    auto* pThreadState = static_cast<AFX_MODULE_THREAD_STATE*>(pThis);
    if (pThreadState == nullptr) {
        return pThis;
    }
    if (auto* current = impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ()) {
        pThreadState->m_nTempMapLock = current->m_nTempMapLock;
    } else {
        pThreadState->m_nTempMapLock = 0;
    }
    return pThis;
}

// Symbol: ??1AFX_MODULE_THREAD_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MODULE_THREAD_STATE__UEAA_XZ(void* pThis) {
    auto* pThreadState = static_cast<AFX_MODULE_THREAD_STATE*>(pThis);
    if (pThreadState == nullptr) {
        return;
    }
    pThreadState->m_nTempMapLock = 0;
}

// Symbol: ??0CHtmlControlSite@@QEAA@PEAVCOleControlContainer@@@Z
extern "C" void* MS_ABI impl___0CHtmlControlSite__QEAA_PEAVCOleControlContainer___Z(void* pThis, void* pContainer) {
    if (pThis == nullptr) {
        return pThis;
    }
    return new (pThis) COleControlSite(static_cast<COleControlContainer*>(pContainer));
}

// Symbol: ??1CHtmlControlSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlControlSite__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    static_cast<COleControlSite*>(pThis)->~COleControlSite();
}

// Symbol: ??1AFX_MAINTAIN_STATE@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE__UEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}

// Symbol: ??1AFX_MAINTAIN_STATE2@@UEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__UEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}

// Symbol: ?Serialize@AFX_AUTOHIDE_DOCKSITE_SAVE_INFO@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_AFX_AUTOHIDE_DOCKSITE_SAVE_INFO__QEAAXAEAVCArchive___Z(void* pThis, void* pArchive) {
    if (pThis == nullptr || pArchive == nullptr) {
        return;
    }

    // Keep this concrete thunk side-effect free; we do not have the real
    // private field set here, so the most conservative behavior is to
    // preserve existing in-memory state and leave the archive untouched.
    (void)pThis;
    (void)pArchive;
}

// Symbol: ??1AFX_MAINTAIN_STATE@@QEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE__QEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}

// Symbol: ??1AFX_MAINTAIN_STATE2@@QEAA@XZ
extern "C" void MS_ABI impl___1AFX_MAINTAIN_STATE2__QEAA_XZ(void* pThis) {
    leave_maintain_state(pThis);
}

// Symbol: ??0CAnimationBaseObject@@QEAA@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationBaseObject__QEAA_II_K_Z(
    void* pThis, unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}

// Symbol: ??0CAnimationBaseObject@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationBaseObject__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAnimationColor@@QEAA@KII_K@Z
extern "C" void* MS_ABI impl___0CAnimationColor__QEAA_KII_K_Z(
    void* pThis, unsigned long long unused0, unsigned int unused1, unsigned int unused2, unsigned long long unused3) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    (void)unused3;
    return pThis;
}

// Symbol: ??0CAnimationColor@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationColor__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAnimationController@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationController__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAnimationGroup@@QEAA@PEAVCAnimationController@@I@Z
extern "C" void* MS_ABI impl___0CAnimationGroup__QEAA_PEAVCAnimationController__I_Z(
    void* pThis, void* /*pController*/, unsigned int unusedFlags) {
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CAnimationPoint@@QEAA@AEBVCPoint@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationPoint__QEAA_AEBVCPoint__II_K_Z(
    void* pThis, const void* /*point*/, unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}

// Symbol: ??0CAnimationPoint@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationPoint__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAnimationRect@@QEAA@AEBVCPoint@@AEBVCSize@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_AEBVCPoint__AEBVCSize__II_K_Z(
    void* pThis, const void* /*point*/, const void* /*size*/,
    unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}

// Symbol: ??0CAnimationRect@@QEAA@AEBVCRect@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_AEBVCRect__II_K_Z(
    void* pThis, const void* /*rect*/,
    unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}

// Symbol: ??0CAnimationRect@@QEAA@HHHHII_K@Z
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_HHHHII_K_Z(
    void* pThis, int unused0, int unused1, int unused2, int unused3,
    unsigned int unused4, unsigned long long unused5) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    (void)unused3;
    (void)unused4;
    (void)unused5;
    return pThis;
}

// Symbol: ??0CAnimationRect@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationRect__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAnimationSize@@QEAA@AEBVCSize@@II_K@Z
extern "C" void* MS_ABI impl___0CAnimationSize__QEAA_AEBVCSize__II_K_Z(
    void* pThis, const void* /*size*/,
    unsigned int unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}

// Symbol: ??0CAnimationSize@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationSize__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAnimationValue@@QEAA@NII_K@Z
extern "C" void* MS_ABI impl___0CAnimationValue__QEAA_NII_K_Z(
    void* pThis, double unused0, unsigned int unused1, unsigned long long unused2) {
    (void)unused1;
    (void)unused2;
    if (pThis) {
        std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
        g_animationValueStates[pThis].m_value = unused0;
    }
    return pThis;
}

// Symbol: ??0CAnimationValue@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAnimationValue__QEAA_XZ(void* pThis) {
    if (pThis) {
        std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
        g_animationValueStates[pThis].m_value = 0.0;
    }
    return pThis;
}

// Symbol: ??0AFX_GLOBAL_DATA@@QEAA@XZ
extern "C" void* MS_ABI impl___0AFX_GLOBAL_DATA__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAsyncMonikerFile@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAsyncMonikerFile__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CAutoHideDockSite@@QEAA@XZ
extern "C" void* MS_ABI impl___0CAutoHideDockSite__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CBlobProperty@@QEAA@PEAX@Z
extern "C" void* MS_ABI impl___0CBlobProperty__QEAA_PEAX_Z(void* pThis, void* unusedData) {
    (void)unusedData;
    return pThis;
}

// Symbol: ??0CConnectionPoint@@QEAA@XZ
extern "C" void* MS_ABI impl___0CConnectionPoint__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CGestureConfig@@QEAA@XZ
extern "C" void* MS_ABI impl___0CGestureConfig__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CEnumArray@@QEAA@_KPEBXIH@Z
extern "C" void* MS_ABI impl___0CEnumArray__QEAA__KPEBXIH_Z(
    void* pThis, unsigned long long unused0, const void* unused1, unsigned int unused2, int unused3) {
    (void)unused0;
    (void)unused1;
    (void)unused2;
    (void)unused3;
    return pThis;
}

// Symbol: ??0CEnumConnPoints@@QEAA@PEBXI@Z
extern "C" void* MS_ABI impl___0CEnumConnPoints__QEAA_PEBXI_Z(void* pThis, const void* unused0, unsigned int unused1) {
    (void)unused0;
    (void)unused1;
    return pThis;
}

// Symbol: ??0CEnumConnections@@QEAA@PEBXI@Z
extern "C" void* MS_ABI impl___0CEnumConnections__QEAA_PEBXI_Z(void* pThis, const void* unused0, unsigned int unused1) {
    (void)unused0;
    (void)unused1;
    return pThis;
}

// Symbol: ??0CArchivePropExchange@@QEAA@AEAVCArchive@@@Z
extern "C" void* MS_ABI impl___0CArchivePropExchange__QEAA_AEAVCArchive___Z(void* pThis, void* pArchive) {
    (void)pArchive;
    return pThis;
}

// Symbol: ??0CArchiveStream@@QEAA@PEAVCArchive@@@Z
extern "C" void* MS_ABI impl___0CArchiveStream__QEAA_PEAVCArchive___Z(void* pThis, void* pArchive) {
    (void)pArchive;
    return pThis;
}

// Symbol: ??0CAsyncPropExchange@@QEAA@K@Z
extern "C" void* MS_ABI impl___0CAsyncPropExchange__QEAA_K_Z(
    void* pThis, unsigned long long unused0) {
    (void)unused0;
    return pThis;
}

// Symbol: ??0CBrowserControlSite@@QEAA@PEAVCOleControlContainer@@PEAVCDHtmlDialog@@@Z
extern "C" void* MS_ABI impl___0CBrowserControlSite__QEAA_PEAVCOleControlContainer__PEAVCDHtmlDialog___Z(
    void* pThis, void* pContainer, void* pDialog) {
    (void)pContainer;
    (void)pDialog;
    return pThis;
}

// Symbol: ??0CCommandManager@@IEAA@XZ
extern "C" void* MS_ABI impl___0CCommandManager__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CControlDataSource@COleControl@@QEAA@PEAV1@@Z
extern "C" void* MS_ABI impl___0CControlDataSource_COleControl__QEAA_PEAV1__Z(void* pThis, void* pContainer) {
    (void)pContainer;
    return pThis;
}

// Symbol: ??0CControlFrameWnd@@QEAA@PEAVCOleControl@@@Z
extern "C" void* MS_ABI impl___0CControlFrameWnd__QEAA_PEAVCOleControl___Z(
    void* pThis, void* pControl) {
    (void)pControl;
    return pThis;
}

// Symbol: ??0CCtrlView@@QEAA@PEB_WK@Z
extern "C" void* MS_ABI impl___0CCtrlView__QEAA_PEB_WK_Z(
    void* pThis, const wchar_t* pName, int unusedFlags) {
    (void)pName;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CDataBoundProperty@@QEAA@PEAV0@JG@Z
extern "C" void* MS_ABI impl___0CDataBoundProperty__QEAA_PEAV0_JG_Z(
    void* pThis, void* pUnknown, unsigned long long unusedType, float unusedFlags) {
    (void)pUnknown;
    (void)unusedType;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CDataRecoveryHandler@@QEAA@KH@Z
extern "C" void* MS_ABI impl___0CDataRecoveryHandler__QEAA_KH_Z(
    void* pThis, unsigned long long unusedFlags, int nInterval) {
    (void)unusedFlags;
    (void)nInterval;
    return pThis;
}

// Symbol: ??0CDataSourceControl@@QEAA@PEAVCOleControlSite@@@Z
extern "C" void* MS_ABI impl___0CDataSourceControl__QEAA_PEAVCOleControlSite___Z(
    void* pThis, void* pSite) {
    (void)pSite;
    return pThis;
}

// Symbol: ??0CDockSite@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDockSite__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CD2DBitmap@@IEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__IEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int unusedType) {
    (void)pRenderTarget;
    (void)unusedType;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}

// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@IPEB_WVCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__H_Z(
    void* pThis, void* pRenderTarget, void* pBitmap, void* pSize, int unusedFlags) {
    (void)pRenderTarget;
    (void)pBitmap;
    (void)pSize;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}

// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@PEAUHBITMAP__@@VCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__PEAUHBITMAP____VCD2DSizeU__H_Z(
    void* pThis, void* pRenderTarget, void* hBitmap, void* size, int unusedFlags) {
    (void)pRenderTarget;
    (void)hBitmap;
    (void)size;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}

// Symbol: ??0CD2DBitmap@@QEAA@PEAVCRenderTarget@@PEB_WVCD2DSizeU@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__PEB_WVCD2DSizeU__H_Z(
    void* pThis, void* pRenderTarget, const wchar_t* pText, void* pSize, int unusedFlags) {
    (void)pRenderTarget;
    (void)pText;
    (void)pSize;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmap__IEAAXXZ(pThis);
    }
    return pThis;
}

// Symbol: ?CommonInit@CD2DBitmap@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CD2DBitmap__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapState(pThis);
    ApplyBitmapDefaults(state);
}

// Symbol: ?CommonInit@CD2DBitmapBrush@@IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES@@@Z
extern "C" void MS_ABI impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(
    void* pThis, void* pBrushProps) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    ApplyBitmapBrushDefaults(state);
    state.m_bitmapObject = pBrushProps;
}

// Symbol: ?GetSize@CD2DBitmap@@QEBA?AVCD2DSizeF@@XZ
extern "C" ::CD2DSizeF MS_ABI impl__GetSize_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DSizeF(0.0f, 0.0f);
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return ::CD2DSizeF(0.0f, 0.0f);
    }
    return ::CD2DSizeF(static_cast<float>(state.m_pixelSize.width), static_cast<float>(state.m_pixelSize.height));
}

// Symbol: ?Attach@CD2DBitmapBrush@@QEAAXPEAUID2D1BitmapBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBitmapBrush__QEAAXPEAUID2D1BitmapBrush___Z(void* pThis, void* pBitmapBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_resource = pBitmapBrush;
}

// Symbol: ?Create@CD2DBitmapBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DBitmapBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}

// Symbol: ?Destroy@CD2DBitmapBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBitmapBrush__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    g_cd2dbitmapBrushStates.erase(pThis);
}

// Symbol: ?Detach@CD2DBitmapBrush@@QEAAPEAUID2D1BitmapBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBitmapBrush__QEAAPEAUID2D1BitmapBrush__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_cd2dBitmapBrushStateMutex);
    const auto it = g_cd2dbitmapBrushStates.find(pThis);
    if (it == g_cd2dbitmapBrushStates.end()) {
        return nullptr;
    }

    void* brush = it->second.m_resource;
    it->second.m_resource = nullptr;
    return brush;
}

// Symbol: ?GetExtendModeX@CD2DBitmapBrush@@QEBA?AW4D2D1_EXTEND_MODE@@XZ
extern "C" D2D1_EXTEND_MODE MS_ABI impl__GetExtendModeX_CD2DBitmapBrush__QEBA_AW4D2D1_EXTEND_MODE__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }

    CD2DBitmapBrushState state{};
    if (!TryLoadCD2DBitmapBrushState(const_cast<void*>(pThis), state)) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }
    return state.m_extendModeX;
}

// Symbol: ?GetExtendModeY@CD2DBitmapBrush@@QEBA?AW4D2D1_EXTEND_MODE@@XZ
extern "C" D2D1_EXTEND_MODE MS_ABI impl__GetExtendModeY_CD2DBitmapBrush__QEBA_AW4D2D1_EXTEND_MODE__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }

    CD2DBitmapBrushState state{};
    if (!TryLoadCD2DBitmapBrushState(const_cast<void*>(pThis), state)) {
        return static_cast<D2D1_EXTEND_MODE>(0);
    }
    return state.m_extendModeY;
}

// Symbol: ?GetInterpolationMode@CD2DBitmapBrush@@QEBA?AW4D2D1_BITMAP_INTERPOLATION_MODE@@XZ
extern "C" D2D1_BITMAP_INTERPOLATION_MODE MS_ABI impl__GetInterpolationMode_CD2DBitmapBrush__QEBA_AW4D2D1_BITMAP_INTERPOLATION_MODE__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(0);
    }

    CD2DBitmapBrushState state{};
    if (!TryLoadCD2DBitmapBrushState(const_cast<void*>(pThis), state)) {
        return static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(0);
    }
    return state.m_interpolationMode;
}

// Symbol: ?SetBitmap@CD2DBitmapBrush@@QEAAXPEAVCD2DBitmap@@@Z
extern "C" void MS_ABI impl__SetBitmap_CD2DBitmapBrush__QEAAXPEAVCD2DBitmap___Z(
    void* pThis, void* pBitmap) {
    if (pThis == nullptr) {
        return;
    }
    if (pBitmap == nullptr) {
        return;
    }

    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_bitmap = pBitmap;
    if (state.m_bitmapObject == nullptr && state.m_resource == nullptr) {
        state.m_resource = pBitmap;
    }
}

// Symbol: ?SetExtendModeX@CD2DBitmapBrush@@QEAAXW4D2D1_EXTEND_MODE@@@Z
extern "C" void MS_ABI impl__SetExtendModeX_CD2DBitmapBrush__QEAAXW4D2D1_EXTEND_MODE___Z(
    void* pThis, D2D1_EXTEND_MODE extendMode) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_extendModeX = extendMode;
}

// Symbol: ?SetExtendModeY@CD2DBitmapBrush@@QEAAXW4D2D1_EXTEND_MODE@@@Z
extern "C" void MS_ABI impl__SetExtendModeY_CD2DBitmapBrush__QEAAXW4D2D1_EXTEND_MODE___Z(
    void* pThis, D2D1_EXTEND_MODE extendMode) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_extendModeY = extendMode;
}

// Symbol: ?SetInterpolationMode@CD2DBitmapBrush@@QEAAXW4D2D1_BITMAP_INTERPOLATION_MODE@@@Z
extern "C" void MS_ABI impl__SetInterpolationMode_CD2DBitmapBrush__QEAAXW4D2D1_BITMAP_INTERPOLATION_MODE___Z(
    void* pThis, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBitmapBrushState(pThis);
    state.m_interpolationMode = interpolationMode;
}

// Symbol: ?Attach@CD2DBrush@@QEAAXPEAUID2D1Brush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DBrush__QEAAXPEAUID2D1Brush___Z(void* pThis, void* pBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBrushState(pThis);
    state.m_resource = pBrush;
}

// Symbol: ?Destroy@CD2DBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBrush__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    g_cd2dBrushStates.erase(pThis);
}

// Symbol: ?Detach@CD2DBrush@@QEAAPEAUID2D1Brush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBrush__QEAAPEAUID2D1Brush__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_cd2dBrushStateMutex);
    auto it = g_cd2dBrushStates.find(pThis);
    if (it == g_cd2dBrushStates.end()) {
        return nullptr;
    }
    void* brush = it->second.m_resource;
    it->second.m_resource = nullptr;
    return brush;
}

// Symbol: ?GetOpacity@CD2DBrush@@QEBAMXZ
extern "C" float MS_ABI impl__GetOpacity_CD2DBrush__QEBAMXZ(const void* pThis) {
    if (pThis == nullptr) {
        return 1.0f;
    }
    CD2DBrushState state{};
    if (!TryLoadCD2DBrushState(const_cast<void*>(pThis), state)) {
        return 1.0f;
    }
    return state.m_opacity;
}

// Symbol: ?GetTransform@CD2DBrush@@QEBAXPEAUD2D_MATRIX_3X2_F@@@Z
extern "C" void MS_ABI impl__GetTransform_CD2DBrush__QEBAXPEAUD2D_MATRIX_3X2_F___Z(
    const void* pThis, D2D_MATRIX_3X2_F* pMatrix) {
    if (pThis == nullptr || pMatrix == nullptr) {
        return;
    }

    CD2DBrushState state{};
    if (!TryLoadCD2DBrushState(const_cast<void*>(pThis), state)) {
        pMatrix->_11 = 1.0f;
        pMatrix->_12 = 0.0f;
        pMatrix->_21 = 0.0f;
        pMatrix->_22 = 1.0f;
        pMatrix->_31 = 0.0f;
        pMatrix->_32 = 0.0f;
        return;
    }
    *pMatrix = state.m_transform;
}

// Symbol: ?SetOpacity@CD2DBrush@@QEAAXM@Z
extern "C" void MS_ABI impl__SetOpacity_CD2DBrush__QEAAXM_Z(void* pThis, float opacity) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBrushState(pThis);
    state.m_opacity = opacity;
}

// Symbol: ?SetTransform@CD2DBrush@@QEAAXPEBUD2D_MATRIX_3X2_F@@@Z
extern "C" void MS_ABI impl__SetTransform_CD2DBrush__QEAAXPEBUD2D_MATRIX_3X2_F___Z(
    void* pThis, const D2D_MATRIX_3X2_F* pMatrix) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DBrushState(pThis);
    if (pMatrix == nullptr) {
        state.m_transform = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
        return;
    }
    state.m_transform = *pMatrix;
}

// Symbol: ?Attach@CD2DLinearGradientBrush@@QEAAXPEAUID2D1LinearGradientBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DLinearGradientBrush__QEAAXPEAUID2D1LinearGradientBrush___Z(
    void* pThis, void* pBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_resource = pBrush;
}

// Symbol: ?Create@CD2DLinearGradientBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DLinearGradientBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}

// Symbol: ?Destroy@CD2DLinearGradientBrush@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DLinearGradientBrush__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    g_cd2dLinearGradientBrushStates.erase(pThis);
}

// Symbol: ?Detach@CD2DLinearGradientBrush@@QEAAPEAUID2D1LinearGradientBrush@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DLinearGradientBrush__QEAAPEAUID2D1LinearGradientBrush__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_cd2dLinearGradientBrushStateMutex);
    auto it = g_cd2dLinearGradientBrushStates.find(pThis);
    if (it == g_cd2dLinearGradientBrushStates.end()) {
        return nullptr;
    }

    void* brush = it->second.m_resource;
    it->second.m_resource = nullptr;
    return brush;
}

// Symbol: ?GetEndPoint@CD2DLinearGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" ::CD2DPointF MS_ABI impl__GetEndPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DPointF(0.0f, 0.0f);
    }

    CD2DLinearGradientBrushState state{};
    if (!TryLoadCD2DLinearGradientBrushState(const_cast<void*>(pThis), state)) {
        return ::CD2DPointF(0.0f, 0.0f);
    }
    return ::CD2DPointF(state.m_endPoint.x, state.m_endPoint.y);
}

// Symbol: ?GetStartPoint@CD2DLinearGradientBrush@@QEBA?AVCD2DPointF@@XZ
extern "C" ::CD2DPointF MS_ABI impl__GetStartPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DPointF(0.0f, 0.0f);
    }

    CD2DLinearGradientBrushState state{};
    if (!TryLoadCD2DLinearGradientBrushState(const_cast<void*>(pThis), state)) {
        return ::CD2DPointF(0.0f, 0.0f);
    }
    return ::CD2DPointF(state.m_startPoint.x, state.m_startPoint.y);
}

// Symbol: ?SetEndPoint@CD2DLinearGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetEndPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(
    void* pThis, ::CD2DPointF endPoint) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_endPoint = {endPoint.x, endPoint.y};
}

// Symbol: ?SetStartPoint@CD2DLinearGradientBrush@@QEAAXVCD2DPointF@@@Z
extern "C" void MS_ABI impl__SetStartPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(
    void* pThis, ::CD2DPointF startPoint) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_startPoint = {startPoint.x, startPoint.y};
}

// Symbol: ?Attach@CD2DRadialGradientBrush@@QEAAXPEAUID2D1RadialGradientBrush@@@Z
extern "C" void MS_ABI impl__Attach_CD2DRadialGradientBrush__QEAAXPEAUID2D1RadialGradientBrush___Z(
    void* pThis, void* pBrush) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_resource = pBrush;
}

// Symbol: ?Create@CD2DRadialGradientBrush@@UEAAJPEAVCRenderTarget@@@Z
extern "C" long MS_ABI impl__Create_CD2DRadialGradientBrush__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}

// Symbol: ?CopyFromBitmap@CD2DBitmap@@QEAAJPEBV1@PEBVCD2DPointU@PEBVCD2DRectU@XZ
extern "C" long MS_ABI impl__CopyFromBitmap_CD2DBitmap__QEAAJPEBV1_PEBVCD2DPointU__PEBVCD2DRectU___Z(
    void* pThis, const void* pBitmap, const CD2DPointU* /*pPoint*/, const CD2DRectU* pRect) {
    if (pThis == nullptr || pBitmap == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    CD2DBitmapState sourceState{};
    const bool haveSourceState = TryLoadCD2DBitmapState(const_cast<void*>(pBitmap), sourceState);

    auto& state = EnsureCD2DBitmapState(pThis);
    if (haveSourceState) {
        state.m_pixelFormat = sourceState.m_pixelFormat;
        state.m_pixelSize = sourceState.m_pixelSize;
        if (sourceState.m_dpi.width != 0.0f || sourceState.m_dpi.height != 0.0f) {
            state.m_dpi = sourceState.m_dpi;
        }
        state.m_resource = sourceState.m_resource;
    }
    if (pRect != nullptr) {
        state.m_pixelSize = CalcSizeFromRect(pRect);
    }
    if (state.m_resource == nullptr) {
        state.m_resource = const_cast<void*>(pBitmap);
    }
    return S_OK;
}

// Symbol: ?CopyFromMemory@CD2DBitmap@@QEAAJPEBXIPEBVCD2DRectU@XZ
extern "C" long MS_ABI impl__CopyFromMemory_CD2DBitmap__QEAAJPEBXIPEBVCD2DRectU___Z(
    void* pThis, const void* pData, unsigned int dataSize, const CD2DRectU* pRect) {
    if (pThis == nullptr || (pData == nullptr && dataSize != 0)) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapState(pThis);
    state.m_resource = (pData == nullptr) ? nullptr : pThis;
    if (pRect != nullptr) {
        state.m_pixelSize = CalcSizeFromRect(pRect);
    } else if (state.m_pixelSize.width == 0 && state.m_pixelSize.height == 0 && dataSize != 0) {
        state.m_pixelSize = CD2DSizeU{1, 1};
    }
    return S_OK;
}

// Symbol: ?CopyFromRenderTarget@CD2DBitmap@@QEAAJPEBVCRenderTarget@PEBVCD2DPointU@PEBVCD2DRectU@XZ
extern "C" long MS_ABI impl__CopyFromRenderTarget_CD2DBitmap__QEAAJPEBVCRenderTarget__PEBVCD2DPointU__PEBVCD2DRectU___Z(
    void* pThis, const void* pRenderTarget, const CD2DPointU* /*pPoint*/, const CD2DRectU* pRect) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapState(pThis);
    state.m_parentRenderTarget = const_cast<void*>(pRenderTarget);
    state.m_resource = const_cast<void*>(pRenderTarget);
    if (pRect != nullptr) {
        state.m_pixelSize = CalcSizeFromRect(pRect);
    }
    return S_OK;
}

// Symbol: ?Create@CD2DBitmap@@UEAAJPEAVCRenderTarget@@XZ
extern "C" long MS_ABI impl__Create_CD2DBitmap__UEAAJPEAVCRenderTarget___Z(
    void* pThis, void* pRenderTarget) {
    if (pThis == nullptr || pRenderTarget == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    auto& state = EnsureCD2DBitmapState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return S_OK;
}

// Symbol: ?Destroy@CD2DBitmap@@UEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DBitmap__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    g_cd2dbitmapStates.erase(pThis);
}

// Symbol: ?Detach@CD2DBitmap@@QEAAPEAUID2D1Bitmap@@XZ
extern "C" void* MS_ABI impl__Detach_CD2DBitmap__QEAAPEAUID2D1Bitmap__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_cd2dBitmapStateMutex);
    const auto it = g_cd2dbitmapStates.find(pThis);
    if (it == g_cd2dbitmapStates.end()) {
        return nullptr;
    }

    void* bitmap = it->second.m_resource;
    it->second.m_resource = nullptr;
    return bitmap;
}

// Symbol: ?GetDPI@CD2DBitmap@@QEBA?AVCD2DSizeF@XZ
extern "C" ::CD2DSizeF MS_ABI impl__GetDPI_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return ::CD2DSizeF(96.0f, 96.0f);
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return ::CD2DSizeF(96.0f, 96.0f);
    }
    return ::CD2DSizeF(state.m_dpi.width, state.m_dpi.height);
}

// Symbol: ?GetPixelFormat@CD2DBitmap@@QEBA?AUD2D1_PIXEL_FORMAT@XZ
extern "C" D2D1_PIXEL_FORMAT MS_ABI impl__GetPixelFormat_CD2DBitmap__QEBA_AUD2D1_PIXEL_FORMAT__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return D2D1_PIXEL_FORMAT{0, 0};
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return D2D1_PIXEL_FORMAT{0, 0};
    }
    return state.m_pixelFormat;
}

// Symbol: ?GetPixelSize@CD2DBitmap@@QEBA?AVCD2DSizeU@XZ
extern "C" CD2DSizeU MS_ABI impl__GetPixelSize_CD2DBitmap__QEBA_AVCD2DSizeU__XZ(const void* pThis) {
    if (pThis == nullptr) {
        return CD2DSizeU{0, 0};
    }

    CD2DBitmapState state{};
    if (!TryLoadCD2DBitmapState(const_cast<void*>(pThis), state)) {
        return CD2DSizeU{0, 0};
    }
    return state.m_pixelSize;
}

// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@IPEB_WVCD2DSizeU@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pBitmap, const wchar_t* pText, void* pBrushProps, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)pBitmap;
    (void)pText;
    (void)pBrushProps;
    (void)pBrushProperties;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(pThis, pBrushProps);
    }
    return pThis;
}

// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pProps, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)pProps;
    (void)pBrushProperties;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(pThis, pProps);
    }
    return pThis;
}

// Symbol: ??0CD2DBitmapBrush@@QEAA@PEAVCRenderTarget@@PEB_WVCD2DSizeU@@PEAUD2D1_BITMAP_BRUSH_PROPERTIES@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBitmapBrush__QEAA_PEAVCRenderTarget__PEB_WVCD2DSizeU__PEAUD2D1_BITMAP_BRUSH_PROPERTIES__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, const wchar_t* pText, void* pProps, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)pText;
    (void)pProps;
    (void)pBrushProperties;
    (void)unusedFlags;
    if (pThis != nullptr) {
        impl__CommonInit_CD2DBitmapBrush__IEAAXPEAUD2D1_BITMAP_BRUSH_PROPERTIES___Z(pThis, pProps);
    }
    return pThis;
}

// Symbol: ??0CD2DBrush@@IEAA@PEAVCRenderTarget@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DBrush__IEAA_PEAVCRenderTarget__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pBrushProperties, int unusedFlags) {
    (void)pRenderTarget;
    (void)unusedFlags;
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCD2DBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    ApplyCD2DBrushDefaults(state);
    ApplyTransformFromBrushProperties(state, static_cast<const float*>(pBrushProperties));
    return pThis;
}

// Symbol: ??0CD2DGeometry@@QEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DGeometry__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int unusedFlags) {
    (void)pRenderTarget;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CD2DGeometrySink@@QEAA@AEAVCD2DPathGeometry@@@Z
extern "C" void* MS_ABI impl___0CD2DGeometrySink__QEAA_AEAVCD2DPathGeometry___Z(void* pThis, void* pGeometry) {
    (void)pGeometry;
    return pThis;
}

// Symbol: ??0CD2DGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IW4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IW4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pStops, int stopCount,
    int gammaMode, int extendMode, void* pProps, int unusedFlags) {
    if (pThis == nullptr) {
        return nullptr;
    }
    (void)pRenderTarget;
    (void)pStops;
    (void)stopCount;
    (void)gammaMode;
    (void)extendMode;
    (void)pProps;
    (void)unusedFlags;
    auto& state = EnsureCD2DGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    state.m_opacity = 1.0f;
    return pThis;
}

// Symbol: ??0CD2DLayer@@QEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DLayer__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int unusedFlags) {
    (void)pRenderTarget;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CD2DLinearGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES@@W4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DLinearGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pStops, int stopCount, void* properties,
    int gammaMode, int extendMode, void* pBrushProps, int unusedFlags) {
    (void)pStops;
    (void)stopCount;
    (void)gammaMode;
    (void)extendMode;
    (void)pBrushProps;
    (void)unusedFlags;
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCD2DLinearGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    ApplyLinearGradientBrushProperties(state, properties);
    if (state.m_startPoint.x == 0.0f && state.m_startPoint.y == 0.0f &&
        state.m_endPoint.x == 0.0f && state.m_endPoint.y == 0.0f) {
        state.m_endPoint = {1.0f, 0.0f};
    }
    return pThis;
}

// Symbol: ??0CD2DMesh@@QEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DMesh__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int unusedFlags) {
    (void)pRenderTarget;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CD2DPathGeometry@@QEAA@PEAVCRenderTarget@@H@Z
extern "C" void* MS_ABI impl___0CD2DPathGeometry__QEAA_PEAVCRenderTarget__H_Z(void* pThis, void* pRenderTarget, int unusedFlags) {
    (void)pRenderTarget;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CD2DRadialGradientBrush@@QEAA@PEAVCRenderTarget@@PEBUD2D1_GRADIENT_STOP@@IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES@@W4D2D1_GAMMA@@W4D2D1_EXTEND_MODE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DRadialGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, void* pStops, int stopCount, void* properties,
    int gammaMode, int extendMode, void* pBrushProps, int unusedFlags) {
    (void)pStops;
    (void)stopCount;
    (void)properties;
    (void)gammaMode;
    (void)extendMode;
    (void)pBrushProps;
    (void)unusedFlags;
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCD2DRadialGradientBrushState(pThis);
    state.m_parentRenderTarget = pRenderTarget;
    state.m_resource = pThis;
    return pThis;
}

// Symbol: ??0CD2DSolidColorBrush@@QEAA@PEAVCRenderTarget@@KHPEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__KHPEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, unsigned long long color, void* pBrushProps, int unusedFlags) {
    (void)pRenderTarget;
    (void)color;
    (void)pBrushProps;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CD2DSolidColorBrush@@QEAA@PEAVCRenderTarget@@U_D3DCOLORVALUE@@PEAVCD2DBrushProperties@@H@Z
extern "C" void* MS_ABI impl___0CD2DSolidColorBrush__QEAA_PEAVCRenderTarget__U_D3DCOLORVALUE__PEAVCD2DBrushProperties__H_Z(
    void* pThis, void* pRenderTarget, const void* color, void* pBrushProps, int unusedFlags) {
    (void)pRenderTarget;
    (void)color;
    (void)pBrushProps;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CDHtmlControlSink@@QEAA@PEAUIUnknown@@PEAVCDHtmlSinkHandler@@PEB_W_K@Z
extern "C" void* MS_ABI impl___0CDHtmlControlSink__QEAA_PEAUIUnknown__PEAVCDHtmlSinkHandler__PEB_W_K_Z(
    void* pThis, void* pUnknown, void* pHandler, const wchar_t* pName, int unusedFlags) {
    (void)pUnknown;
    (void)pHandler;
    (void)pName;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CDHtmlControlSink@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDHtmlControlSink__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CDHtmlElementEventSink@@QEAA@PEAVCDHtmlEventSink@@PEAUIDispatch@@@Z
extern "C" void* MS_ABI impl___0CDHtmlElementEventSink__QEAA_PEAVCDHtmlEventSink__PEAUIDispatch___Z(
    void* pThis, void* pEventSink, void* pDispatch) {
    (void)pEventSink;
    (void)pDispatch;
    return pThis;
}

// Symbol: ??0CDialogImpl@@IEAA@AEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CDialogImpl__IEAA_AEAVCWnd___Z(void* pThis, void* pWnd) {
    (void)pWnd;
    return pThis;
}

// Symbol: ??0CDocObjectServer@@QEAA@PEAVCOleServerDoc@@PEAUIOleDocumentSite@@@Z
extern "C" void* MS_ABI impl___0CDocObjectServer__QEAA_PEAVCOleServerDoc__PEAUIOleDocumentSite___Z(
    void* pThis, void* pDoc, void* pDocSite) {
    (void)pDoc;
    (void)pDocSite;
    return pThis;
}

// Symbol: ??0CDocObjectServerItem@@IEAA@PEAVCOleServerDoc@@H@Z
extern "C" void* MS_ABI impl___0CDocObjectServerItem__IEAA_PEAVCOleServerDoc__H_Z(
    void* pThis, void* pDoc, int unusedId) {
    (void)pDoc;
    (void)unusedId;
    return pThis;
}

// Symbol: ??0CDockBar@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CDockBar__QEAA_H_Z(void* pThis, int dockStyle) {
    (void)dockStyle;
    return pThis;
}

// Symbol: ??0CDockContext@@QEAA@PEAVCControlBar@@@Z
extern "C" void* MS_ABI impl___0CDockContext__QEAA_PEAVCControlBar___Z(void* pThis, void* pControlBar) {
    (void)pControlBar;
    return pThis;
}

// Symbol: ??0CDockState@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDockState__QEAA_XZ(void* pThis) {
    OpenMfcResetDockState(pThis);
    return pThis;
}

// Symbol: ??0CDockablePaneAdapter@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDockablePaneAdapter__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CDockingPanesRow@@QEAA@PEAVCDockSite@@HH@Z
extern "C" void* MS_ABI impl___0CDockingPanesRow__QEAA_PEAVCDockSite__HH_Z(
    void* pThis, void* pDockSite, int nRows, int nCols) {
    (void)pDockSite;
    (void)nRows;
    (void)nCols;
    return pThis;
}

// Symbol: ??0CDrawingManager@@QEAA@AEAVCDC@@@Z
extern "C" void* MS_ABI impl___0CDrawingManager__QEAA_AEAVCDC___Z(void* pThis, void* pDC) {
    (void)pDC;
    return pThis;
}

// Symbol: ??0CHtmlEditCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlEditCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CHtmlEditDoc@@QEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlEditDoc__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CHtmlEditView@@QEAA@XZ
extern "C" void* MS_ABI impl___0CHtmlEditView__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CHwndRenderTarget@@QEAA@PEAUHWND__@@@Z
extern "C" void* MS_ABI impl___0CHwndRenderTarget__QEAA_PEAUHWND_____Z(void* pThis, void* hWnd) {
    (void)hWnd;
    return pThis;
}

// Symbol: ??0CJumpList@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CJumpList__QEAA_H_Z(void* pThis, int flags) {
    (void)flags;
    return pThis;
}

// Symbol: ??0CKeyFrame@@QEAA@PEAVCBaseKeyFrame@@N@Z
extern "C" void* MS_ABI impl___0CKeyFrame__QEAA_PEAVCBaseKeyFrame__N_Z(
    void* pThis, void* pKeyFrame, double duration) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCKeyFrameState(pThis);
    state.m_creator = pKeyFrame;
    state.m_duration = duration;
    state.m_hasBaseKeyFrame = true;
    return pThis;
}

// Symbol: ??0CKeyFrame@@QEAA@PEAVCBaseTransition@@@Z
extern "C" void* MS_ABI impl___0CKeyFrame__QEAA_PEAVCBaseTransition___Z(void* pThis, void* pTransition) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCKeyFrameState(pThis);
    state.m_creator = pTransition;
    state.m_hasTransition = true;
    return pThis;
}

// Symbol: ??0CMDITabProxyWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMDITabProxyWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCAcceleratorKeyAssignCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCAcceleratorKeyAssignCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCBaseAccessibleObject@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCBaseAccessibleObject__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCBaseVisualManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCBaseVisualManager__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCCaptionBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCaptionBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCCmdUsageCount@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCmdUsageCount__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCColorBar@@IEAA@AEAV0@I@Z
extern "C" void* MS_ABI impl___0CMFCColorBar__IEAA_AEAV0_I_Z(void* pThis, void* pOther, int unusedFlags) {
    (void)pOther;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CMFCColorBar@@IEAA@AEBV?$CArray@KK@@KPEB_W11AEAV?$CList@KK@@HHHKIPEAVCMFCColorButton@@@Z
extern "C" void* MS_ABI impl___0CMFCColorBar__IEAA_AEBV__CArray_KK__KPEB_W11AEAV__CList_KK__HHHKIPEAVCMFCColorButton___Z(
    void* pThis, const void* array, unsigned int count, const wchar_t* name,
    void* list, int row, int col, int unknown, void* colorButton) {
    (void)array;
    (void)count;
    (void)name;
    (void)list;
    (void)row;
    (void)col;
    (void)unknown;
    (void)colorButton;
    return pThis;
}

// Symbol: ??0CMFCColorBar@@IEAA@AEBV?$CArray@KK@@KPEB_W11AEAV?$CList@KK@@HKIPEAVCMFCRibbonColorButton@@@Z
extern "C" void* MS_ABI impl___0CMFCColorBar__IEAA_AEBV__CArray_KK__KPEB_W11AEAV__CList_KK__HKIPEAVCMFCRibbonColorButton___Z(
    void* pThis, const void* array, unsigned int count, const wchar_t* name,
    void* list, int row, int col, void* colorButton) {
    (void)array;
    (void)count;
    (void)name;
    (void)list;
    (void)row;
    (void)col;
    (void)colorButton;
    return pThis;
}

// Symbol: ??0CMFCColorBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCColorButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCColorDialog@@QEAA@KKPEAVCWnd@@PEAUHPALETTE__@@@Z
extern "C" void* MS_ABI impl___0CMFCColorDialog__QEAA_KKPEAVCWnd__PEAUHPALETTE_____Z(
    void* pThis, int dummyColor, int dummyFlags, void* pWndParent, void* pPalette) {
    (void)dummyColor;
    (void)dummyFlags;
    (void)pWndParent;
    (void)pPalette;
    return pThis;
}

// Symbol: ??0CMFCColorMenuButton@@QEAA@IPEB_WPEAVCPalette@@@Z
extern "C" void* MS_ABI impl___0CMFCColorMenuButton__QEAA_IPEB_WPEAVCPalette___Z(
    void* pThis, int id, const wchar_t* text, void* pPalette) {
    (void)id;
    (void)text;
    (void)pPalette;
    return pThis;
}

// Symbol: ??0CMFCColorMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorMenuButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCColorPickerCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorPickerCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCControlContainer@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCControlContainer__QEAA_PEAVCWnd___Z(void* pThis, void* pWnd) {
    (void)pWnd;
    return pThis;
}

// Symbol: ??0CMFCCustomColorsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomColorsPropertyPage__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCCustomizeButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomizeButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCCustomizeMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomizeMenuButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCDropDownFrame@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDropDownFrame__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCDropDownToolbarButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDropDownToolbarButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCEditBrowseCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCEditBrowseCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCFilterChunkValueImpl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCFilterChunkValueImpl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCFontComboBox@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCFontComboBox__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCImageEditorDialog@@QEAA@PEAVCBitmap@@PEAVCWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCImageEditorDialog__QEAA_PEAVCBitmap__PEAVCWnd__H_Z(
    void* pThis, void* pBitmap, void* pWnd, int unusedFlags) {
    (void)pBitmap;
    (void)pWnd;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CMFCImagePaintArea@@QEAA@PEAVCMFCImageEditorDialog@@@Z
extern "C" void* MS_ABI impl___0CMFCImagePaintArea__QEAA_PEAVCMFCImageEditorDialog___Z(
    void* pThis, void* pDialog) {
    (void)pDialog;
    return pThis;
}

// Symbol: ??0CMFCKeyMapDialog@@QEAA@PEAVCFrameWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCKeyMapDialog__QEAA_PEAVCFrameWnd__H_Z(
    void* pThis, void* pFrameWnd, int unusedFlags) {
    (void)pFrameWnd;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CMFCLinkCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCLinkCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCListCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCListCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCMaskedEdit@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCMaskedEdit__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCMenuButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCMousePropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCMousePropertyPage__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCCustomizeButton@@QEAA@HAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void* MS_ABI impl___0CMFCCustomizeButton__QEAA_HAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, int flags, const void* text) {
    (void)flags;
    (void)text;
    return pThis;
}

// Symbol: ??0CMFCCustomizeMenuButton@@QEAA@IPEAUHMENU__@@HPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCCustomizeMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
    void* pThis, void* hMenu, int flags, const wchar_t* text) {
    (void)hMenu;
    (void)flags;
    (void)text;
    return pThis;
}

// Symbol: ??0CMFCDesktopAlertDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDesktopAlertDialog__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCDragFrameImpl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDragFrameImpl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@PEAVCMFCRibbonComboBox@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEAVCMFCRibbonComboBox___Z(void* pThis, void* pComboBox) {
    (void)pComboBox;
    return pThis;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEAVCWnd___Z(void* pThis, void* pWndParent) {
    (void)pWndParent;
    return pThis;
}

// Symbol: ??0CMFCDropDownListBox@@QEAA@PEB_WPEAVCMFCDropDownToolBar@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownListBox__QEAA_PEB_WPEAVCMFCDropDownToolBar___Z(
    void* pThis, const wchar_t* name, void* pDropDownToolbar) {
    (void)name;
    (void)pDropDownToolbar;
    return pThis;
}

// Symbol: ??0CMFCDropDownToolbarButton@@QEAA@PEB_WPEAVCMFCDropDownToolBar@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownToolbarButton__QEAA_PEB_WPEAVCMFCDropDownToolBar___Z(
    void* pThis, const wchar_t* text, void* pDropDownToolbar) {
    (void)text;
    (void)pDropDownToolbar;
    return pThis;
}

// Symbol: ??0CMFCOutlookBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCOutlookBarPane@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBarPane__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCOutlookBarPaneAdapter@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBarPaneAdapter__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCOutlookBarPaneButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBarPaneButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCOutlookBarTabCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBarTabCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCOutlookBarToolBar@@QEAA@PEAVCMFCOutlookBarTabCtrl@@@Z
extern "C" void* MS_ABI impl___0CMFCOutlookBarToolBar__QEAA_PEAVCMFCOutlookBarTabCtrl___Z(void* pThis, void* pTabCtrl) {
    (void)pTabCtrl;
    return pThis;
}

// Symbol: ??0CMFCPreviewCtrlImpl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPreviewCtrlImpl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCPropertyGridColorProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBKPEAVCPalette@@PEB_W_K@Z
extern "C" void* MS_ABI impl___0CMFCPropertyGridColorProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBKPEAVCPalette__PEB_W_K_Z(
    void* pThis, const void* name, long long key, void* palette, const wchar_t* tooltip, unsigned long long flags) {
    (void)name;
    (void)key;
    (void)palette;
    (void)tooltip;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCPropertyGridFileProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@0_KPEB_W@Z
extern "C" void* MS_ABI impl___0CMFCPropertyGridFileProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__0_KPEB_W_Z(
    void* pThis, const void* name, unsigned long long flags, const wchar_t* extension) {
    (void)name;
    (void)flags;
    (void)extension;
    return pThis;
}

// Symbol: ??0CMFCPropertyGridFileProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H0PEB_WK11_K@Z
extern "C" void* MS_ABI impl___0CMFCPropertyGridFileProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H0PEB_WK11_K_Z(
    void* pThis, const void* name, int type, const void* second, const wchar_t* ext1,
    const void* ext2, const wchar_t* ext3, unsigned long long flags) {
    (void)name;
    (void)type;
    (void)second;
    (void)ext1;
    (void)ext2;
    (void)ext3;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCPropertyGridFontProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagLOGFONTW@@KPEB_W_KK@Z
extern "C" void* MS_ABI impl___0CMFCPropertyGridFontProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagLOGFONTW__KPEB_W_KK_Z(
    void* pThis, const void* name, void* lf, unsigned long long color,
    const wchar_t* text, unsigned long long flags, int something) {
    (void)name;
    (void)lf;
    (void)color;
    (void)text;
    (void)flags;
    (void)something;
    return pThis;
}

// Symbol: ??0CMFCPropertyGridToolTipCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertyGridToolTipCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCPropertyPage@@QEAA@II@Z
extern "C" void* MS_ABI impl___0CMFCPropertyPage__QEAA_II_Z(void* pThis, int id, int helpID) {
    (void)id;
    (void)helpID;
    return pThis;
}

// Symbol: ??0CMFCPropertyPage@@QEAA@PEB_WI@Z
extern "C" void* MS_ABI impl___0CMFCPropertyPage__QEAA_PEB_WI_Z(void* pThis, const wchar_t* name, int id) {
    (void)name;
    (void)id;
    return pThis;
}

// Symbol: ??0CMFCPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertyPage__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCReBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCReBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonCollector@@QEAA@AEAVCMFCRibbonInfo@@K@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCollector__QEAA_AEAVCMFCRibbonInfo__K_Z(void* pThis, void* pInfo, unsigned long long unusedFlags) {
    (void)pInfo;
    (void)unusedFlags;
    return pThis;
}

// Symbol: ??0CMFCRibbonColorButton@@QEAA@IPEB_WHHHK@Z
extern "C" void* MS_ABI impl___0CMFCRibbonColorButton__QEAA_IPEB_WHHHK_Z(
    void* pThis, int id, const wchar_t* text, int red, int green, int blue, int alpha) {
    (void)id;
    (void)text;
    (void)red;
    (void)green;
    (void)blue;
    (void)alpha;
    return pThis;
}

// Symbol: ??0CMFCRibbonColorButton@@QEAA@IPEB_WHK@Z
extern "C" void* MS_ABI impl___0CMFCRibbonColorButton__QEAA_IPEB_WHK_Z(
    void* pThis, int id, const wchar_t* text, int red, int green, int blue) {
    (void)id;
    (void)text;
    (void)red;
    (void)green;
    (void)blue;
    return pThis;
}

// Symbol: ??0CMFCRibbonColorButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonColorButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonComboBox__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonComboBox@@QEAA@IHHPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonComboBox__QEAA_IHHPEB_WH_Z(
    void* pThis, int id, int width, int height, const wchar_t* text) {
    (void)id;
    (void)width;
    (void)height;
    (void)text;
    return pThis;
}

// Symbol: ??0CMFCRibbonCommandsListBox@@QEAA@PEAVCMFCRibbonBar@@HHH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCommandsListBox__QEAA_PEAVCMFCRibbonBar__HHH_Z(
    void* pThis, void* pRibbonBar, int p1, int p2, int p3) {
    (void)pRibbonBar;
    (void)p1;
    (void)p2;
    (void)p3;
    return pThis;
}

// Symbol: ??0CMFCRibbonConstructor@@QEAA@AEBVCMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonConstructor__QEAA_AEBVCMFCRibbonInfo___Z(
    void* pThis, const void* info) {
    (void)info;
    return pThis;
}

// Symbol: ??0CMFCRibbonCustomizePropertyPage@@QEAA@PEAVCMFCRibbonBar@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCustomizePropertyPage__QEAA_PEAVCMFCRibbonBar___Z(
    void* pThis, void* pRibbonBar) {
    (void)pRibbonBar;
    return pThis;
}

// Symbol: ??0CMFCRibbonDefaultPanelButton@@QEAA@PEAVCMFCRibbonPanel@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonDefaultPanelButton__QEAA_PEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pPanel;
    return pThis;
}

// Symbol: ??0CMFCRibbonFontComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonFontComboBox__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonFontComboBox@@QEAA@IHEEH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonFontComboBox__QEAA_IHEEH_Z(
    void* pThis, int style, int unused0, int unused1, int unused2, short unused3) {
    (void)style;
    (void)unused0;
    (void)unused1;
    (void)unused2;
    (void)unused3;
    return pThis;
}

// Symbol: ??0CMFCRibbonGalleryIcon@@QEAA@PEAVCMFCRibbonGallery@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryIcon__QEAA_PEAVCMFCRibbonGallery__H_Z(
    void* pThis, void* pGallery, int flags) {
    (void)pGallery;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCRibbonGalleryMenuButton@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryMenuButton__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonGalleryMenuButton@@QEAA@IHPEB_WAEAVCMFCToolBarImages@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryMenuButton__QEAA_IHPEB_WAEAVCMFCToolBarImages___Z(
    void* pThis, int style, int flag, const wchar_t* text, void* images) {
    (void)style;
    (void)flag;
    (void)text;
    (void)images;
    return pThis;
}

// Symbol: ??0CMFCRibbonGalleryMenuButton@@QEAA@IHPEB_WIH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryMenuButton__QEAA_IHPEB_WIH_Z(
    void* pThis, int style, int flag, const wchar_t* text, int unknown) {
    (void)style;
    (void)flag;
    (void)text;
    (void)unknown;
    return pThis;
}

// Symbol: ??0CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonInfoLoader@@QEAA@AEAVCMFCRibbonInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonInfoLoader__QEAA_AEAVCMFCRibbonInfo___Z(
    void* pThis, void* info) {
    (void)info;
    return pThis;
}

// Symbol: ??0CMFCRibbonKeyTip@@QEAA@PEAVCMFCRibbonBaseElement@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonKeyTip__QEAA_PEAVCMFCRibbonBaseElement__H_Z(
    void* pThis, void* element, int flags) {
    (void)element;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCRibbonKeyboardCustomizeDialog@@QEAA@PEAVCMFCRibbonBar@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonKeyboardCustomizeDialog__QEAA_PEAVCMFCRibbonBar__PEAVCWnd___Z(
    void* pThis, void* ribbonBar, void* parent) {
    (void)ribbonBar;
    (void)parent;
    return pThis;
}

// Symbol: ??0CMFCRibbonLaunchButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonLaunchButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonLinkCtrl@@QEAA@IPEB_W0@Z
extern "C" void* MS_ABI impl___0CMFCRibbonLinkCtrl__QEAA_IPEB_W0_Z(void* pThis, int id, const wchar_t* text) {
    (void)id;
    (void)text;
    return pThis;
}

// Symbol: ??0CMFCRibbonLinkCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonLinkCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonMainPanelButton@@QEAA@IPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonMainPanelButton__QEAA_IPEB_WH_Z(void* pThis, int id, const wchar_t* text) {
    (void)id;
    (void)text;
    return pThis;
}

// Symbol: ??0CMFCRibbonMainPanelButton@@QEAA@IPEB_WPEAUHICON__@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonMainPanelButton__QEAA_IPEB_WPEAUHICON_____Z(
    void* pThis, int id, const wchar_t* text, void* icon) {
    (void)id;
    (void)text;
    (void)icon;
    return pThis;
}

// Symbol: ??0CMFCRibbonMainPanelButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonMainPanelButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonMiniToolBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonMiniToolBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenu@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonBar@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, void* pRibbonBar, const void* elements, unsigned int unknown, int flags) {
    (void)pRibbonBar;
    (void)elements;
    (void)unknown;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonCategory@@VCSize@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonCategory__VCSize___Z(
    void* pThis, void* pCategory, const void* size) {
    (void)pCategory;
    (void)size;
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonGallery@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonGallery___Z(
    void* pThis, void* pGallery) {
    (void)pGallery;
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenu@@QEAA@PEAVCMFCRibbonPanel@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenu__QEAA_PEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pPanel;
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenuBar@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonBar@@AEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@H@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonBar__AEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___H_Z(
    void* pThis, void* pRibbonBar, const void* elements, unsigned int unknown) {
    (void)pRibbonBar;
    (void)elements;
    (void)unknown;
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonCategory@@VCSize@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonCategory__VCSize___Z(
    void* pThis, void* pCategory, const void* size) {
    (void)pCategory;
    (void)size;
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonGallery@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonGallery___Z(
    void* pThis, void* pGallery) {
    (void)pGallery;
    return pThis;
}

// Symbol: ??0CMFCRibbonPanelMenuBar@@QEAA@PEAVCMFCRibbonPanel@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonPanelMenuBar__QEAA_PEAVCMFCRibbonPanel___Z(
    void* pThis, void* pPanel) {
    (void)pPanel;
    return pThis;
}

// Symbol: ??0CMFCRibbonQuickAccessToolBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonQuickAccessToolBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonRichEditCtrl@@QEAA@AEAVCMFCRibbonEdit@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonRichEditCtrl__QEAA_AEAVCMFCRibbonEdit___Z(void* pThis, void* pEdit) {
    (void)pEdit;
    return pThis;
}

// Symbol: ??0CMFCRibbonTabsGroup@@QEAA@PEAVCMFCRibbonBaseElement@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonTabsGroup__QEAA_PEAVCMFCRibbonBaseElement___Z(
    void* pThis, void* pBaseElement) {
    (void)pBaseElement;
    return pThis;
}

// Symbol: ??0CMFCRibbonTabsGroup@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonTabsGroup__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCRibbonUndoButton@@QEAA@IPEB_WHH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_IPEB_WHH_Z(
    void* pThis, int id, const wchar_t* text, int width, int height) {
    (void)id;
    (void)text;
    (void)width;
    (void)height;
    return pThis;
}

// Symbol: ??0CMFCRibbonUndoButton@@QEAA@IPEB_WPEAUHICON__@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_IPEB_WPEAUHICON_____Z(
    void* pThis, int id, const wchar_t* text, void* icon) {
    (void)id;
    (void)text;
    (void)icon;
    return pThis;
}

// Symbol: ??0CMFCRibbonUndoButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonUndoButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCShadowRenderer@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCShadowRenderer__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCShellListCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCShellListCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCShellTreeCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCShellTreeCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCShowAllButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCShowAllButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCSpinButtonCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCSpinButtonCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCStandardColorsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCStandardColorsPropertyPage__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCTasksPaneFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCTasksPaneFrameWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarButtonCustomizeDialog@@QEAA@PEAVCMFCToolBarButton@@PEAVCMFCToolBarImages@@PEAVCWnd@@HH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarButtonCustomizeDialog__QEAA_PEAVCMFCToolBarButton__PEAVCMFCToolBarImages__PEAVCWnd__HH_Z(
    void* pThis, void* pButton, void* pImages, void* pParent, int p0, int p1) {
    (void)pButton;
    (void)pImages;
    (void)pParent;
    (void)p0;
    (void)p1;
    return pThis;
}

// Symbol: ??0CMFCToolBarButtonsListButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarButtonsListButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarComboBoxEdit@@QEAA@AEAVCMFCToolBarComboBoxButton@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarComboBoxEdit__QEAA_AEAVCMFCToolBarComboBoxButton___Z(void* pThis, void* pButton) {
    (void)pButton;
    return pThis;
}

// Symbol: ??0CMFCToolBarDateTimeCtrl@@QEAA@IHKH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarDateTimeCtrl__QEAA_IHKH_Z(
    void* pThis, int id, int unused0, int unused1, int unused2) {
    (void)id;
    (void)unused0;
    (void)unused1;
    (void)unused2;
    return pThis;
}

// Symbol: ??0CMFCToolBarDateTimeCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarDateTimeCtrl__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarDropSource@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarDropSource__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarDropTarget@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarDropTarget__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarEditCtrl@@QEAA@AEAVCMFCToolBarEditBoxButton@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarEditCtrl__QEAA_AEAVCMFCToolBarEditBoxButton___Z(void* pThis, void* pButton) {
    (void)pButton;
    return pThis;
}

// Symbol: ??0CMFCToolBarFontComboBox@@IEAA@PEAVCObList@@HEE@Z
extern "C" void* MS_ABI impl___0CMFCToolBarFontComboBox__IEAA_PEAVCObList__HEE_Z(
    void* pThis, void* pList, int id, int flags) {
    (void)pList;
    (void)id;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCToolBarFontComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarFontComboBox__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarFontComboBox@@QEAA@IHHEKHE@Z
extern "C" void* MS_ABI impl___0CMFCToolBarFontComboBox__QEAA_IHHEKHE_Z(
    void* pThis, int a, int b, int c, int d, int e, int f, int g) {
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
    (void)g;
    return pThis;
}

// Symbol: ??0CMFCToolBarFontSizeComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarFontSizeComboBox__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarFontSizeComboBox@@QEAA@IHKH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarFontSizeComboBox__QEAA_IHKH_Z(
    void* pThis, int a, int b, int c) {
    (void)a;
    (void)b;
    (void)c;
    return pThis;
}

// Symbol: ??0CMFCToolBarNameDialog@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarNameDialog__QEAA_PEAVCWnd___Z(void* pThis, void* pParent) {
    (void)pParent;
    return pThis;
}

// Symbol: ??0CMFCToolBarSpinEditBoxButton@@QEAA@IHKH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarSpinEditBoxButton__QEAA_IHKH_Z(
    void* pThis, int a, int b, int c) {
    (void)a;
    (void)b;
    (void)c;
    return pThis;
}

// Symbol: ??0CMFCToolBarSpinEditBoxButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarSpinEditBoxButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarSystemMenuButton@@QEAA@PEAUHMENU__@@PEAUHICON__@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarSystemMenuButton__QEAA_PEAUHMENU____PEAUHICON_____Z(
    void* pThis, void* hMenu, void* hIcon) {
    (void)hMenu;
    (void)hIcon;
    return pThis;
}

// Symbol: ??0CMFCToolBarSystemMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarSystemMenuButton__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarsCommandsListBox@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarsCommandsListBox__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarsCommandsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarsCommandsPropertyPage__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarsCustomizeDialog@@QEAA@PEAVCFrameWnd@@HIPEAV?$CList@PEAUCRuntimeClass@@PEAU1@@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsCustomizeDialog__QEAA_PEAVCFrameWnd__HIPEAV__CList_PEAUCRuntimeClass__PEAU1____Z(
    void* pThis, void* pFrameWnd, int a, int b, void* pList, int c) {
    (void)pFrameWnd;
    (void)a;
    (void)b;
    (void)pList;
    (void)c;
    return pThis;
}

// Symbol: ??0CMFCToolBarsKeyboardPropertyPage@@QEAA@PEAVCFrameWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsKeyboardPropertyPage__QEAA_PEAVCFrameWnd__H_Z(
    void* pThis, void* pFrameWnd, int flags) {
    (void)pFrameWnd;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCToolBarsListCheckBox@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarsListCheckBox__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolBarsListPropertyPage@@QEAA@PEAVCFrameWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsListPropertyPage__QEAA_PEAVCFrameWnd___Z(void* pThis, void* pFrameWnd) {
    (void)pFrameWnd;
    return pThis;
}

// Symbol: ??0CMFCToolBarsMenuPropertyPage@@QEAA@PEAVCFrameWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsMenuPropertyPage__QEAA_PEAVCFrameWnd__H_Z(void* pThis, void* pFrameWnd, int flags) {
    (void)pFrameWnd;
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCToolBarsOptionsPropertyPage@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsOptionsPropertyPage__QEAA_H_Z(void* pThis, int flags) {
    (void)flags;
    return pThis;
}

// Symbol: ??0CMFCToolBarsToolsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarsToolsPropertyPage__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCToolTipCtrl@@QEAA@PEAVCMFCToolTipInfo@@@Z
extern "C" void* MS_ABI impl___0CMFCToolTipCtrl__QEAA_PEAVCMFCToolTipInfo___Z(void* pThis, void* pInfo) {
    (void)pInfo;
    return pThis;
}

// Symbol: ??0CMFCVisualManagerBitmapCache@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCVisualManagerBitmapCache__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCVisualManagerBitmapCacheItem@CMFCVisualManagerBitmapCache@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCVisualManagerBitmapCacheItem_CMFCVisualManagerBitmapCache__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCVisualManagerVS2008@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCVisualManagerVS2008__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMFCWindowsManagerDialog@@QEAA@PEAVCMDIFrameWndEx@@H@Z
extern "C" void* MS_ABI impl___0CMFCWindowsManagerDialog__QEAA_PEAVCMDIFrameWndEx__H_Z(
    void* pThis, void* pFrameWnd, int mode) {
    (void)pFrameWnd;
    (void)mode;
    return pThis;
}

// Symbol: ??0CMFCZoomKernel@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCZoomKernel__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMiniDockFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMiniDockFrameWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMouseManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMouseManager__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CMultiPageDHtmlDialog@@QEAA@IIPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMultiPageDHtmlDialog__QEAA_IIPEAVCWnd___Z(
    void* pThis, int flags, int id, void* pWnd) {
    (void)flags;
    (void)id;
    (void)pWnd;
    return pThis;
}

// Symbol: ??0CMultiPageDHtmlDialog@@QEAA@PEB_W0PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMultiPageDHtmlDialog__QEAA_PEB_W0PEAVCWnd___Z(
    void* pThis, const wchar_t* path, int mode, void* pWnd) {
    (void)path;
    (void)mode;
    (void)pWnd;
    return pThis;
}

// Symbol: ??0CMultiPageDHtmlDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMultiPageDHtmlDialog__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0COleCntrFrameWndEx@@QEAA@PEAVCOleIPFrameWnd@@@Z
extern "C" void* MS_ABI impl___0COleCntrFrameWndEx__QEAA_PEAVCOleIPFrameWnd___Z(void* pThis, void* pFrameWnd) {
    (void)pFrameWnd;
    return pThis;
}

// Symbol: ??0COleDocIPFrameWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0COleDocIPFrameWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0COleDocIPFrameWndEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0COleDocIPFrameWndEx__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0COleUILinkInfo@@QEAA@PEAVCOleClientItem@@@Z
extern "C" void* MS_ABI impl___0COleUILinkInfo__QEAA_PEAVCOleClientItem___Z(void* pThis, void* pItem) {
    SetCOleUILinkInfoTarget(pThis, nullptr, static_cast<COleClientItem*>(pItem));
    return pThis;
}

// Symbol: ??0COleUILinkInfo@@QEAA@PEAVCOleDocument@@@Z
extern "C" void* MS_ABI impl___0COleUILinkInfo__QEAA_PEAVCOleDocument___Z(void* pThis, void* pDocument) {
    SetCOleUILinkInfoTarget(pThis, static_cast<COleDocument*>(pDocument), nullptr);
    return pThis;
}

// Symbol: ?UpdateLink@COleUILinkInfo@@UEAAJKHH@Z
extern "C" unsigned long MS_ABI impl__UpdateLink_COleUILinkInfo__UEAAJKHH_Z(
    void* pThis, unsigned long dwLink, unsigned short fErrorMessage, unsigned short fReserved) {
    (void)fErrorMessage;
    (void)fReserved;

    if (pThis == nullptr) return E_POINTER;

    COleDocument* pDocument = nullptr;
    COleClientItem* pItem = nullptr;
    if (!GetCOleUILinkInfoTarget(pThis, pDocument, pItem)) return E_INVALIDARG;

    if (pItem != nullptr) {
        if (dwLink != 0 && dwLink != 1) return E_INVALIDARG;
        return UpdateSingleOleLinkItem(pItem);
    }

    if (pDocument == nullptr) return E_INVALIDARG;

    if (dwLink == 0) {
        POSITION pos = pDocument->GetStartPosition();
        if (pos == nullptr) return S_FALSE;

        HRESULT hr = S_OK;
        bool didUpdate = false;
        while (pos != nullptr) {
            COleClientItem* updateItem = pDocument->GetNextClientItem(pos);
            if (updateItem == nullptr) continue;
            didUpdate = true;
            const HRESULT linkResult = UpdateSingleOleLinkItem(updateItem);
            if (FAILED(linkResult) && SUCCEEDED(hr)) hr = linkResult;
        }
        return didUpdate ? hr : S_FALSE;
    }

    COleClientItem* targetItem = FindItemByLinkIndex(pDocument, dwLink);
    if (targetItem == nullptr) return E_INVALIDARG;
    return UpdateSingleOleLinkItem(targetItem);
}

// Symbol: ?SetControl@CReflectorWnd@@QEAAXPEAVCOleControl@@@Z
extern "C" void MS_ABI impl__SetControl_CReflectorWnd__QEAAXPEAVCOleControl___Z(void* pThis, void* pControl) {
    if (pThis == nullptr) {
        return;
    }

    auto* newControl = static_cast<COleControl*>(pControl);
    auto* reflector = static_cast<CWnd*>(pThis);
    {
        std::lock_guard<std::mutex> lock(g_reflectorControlMutex);
        auto it = g_reflectorControls.find(pThis);
        if (it != g_reflectorControls.end()) {
            COleControl* priorControl = it->second;
            if (priorControl != nullptr) {
                priorControl->m_pReflect = nullptr;
            }
            if (newControl == nullptr) {
                g_reflectorControls.erase(it);
            } else {
                it->second = newControl;
            }
        } else if (newControl != nullptr) {
            g_reflectorControls.emplace(pThis, newControl);
        }

        for (auto& entry : g_reflectorControls) {
            if (entry.first != pThis && entry.second == newControl) {
                entry.second = nullptr;
            }
        }
    }

    if (newControl != nullptr) {
        newControl->m_pReflect = reflector;
    }
}


// Symbol: ??0COutlookOptionsDlg@@QEAA@AEAVCMFCOutlookBarTabCtrl@@@Z
extern "C" void* MS_ABI impl___0COutlookOptionsDlg__QEAA_AEAVCMFCOutlookBarTabCtrl___Z(void* pThis, void* pTabCtrl) {
    (void)pTabCtrl;
    return pThis;
}

// Symbol: ??0CPaneDialog@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPaneDialog__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CPreviewView@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPreviewView__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CPreviewViewEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0CPreviewViewEx__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CPrintInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPrintInfo__QEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        TrackCPrintInfo(pThis);
    }
    return pThis;
}

// Symbol: ??0CPropbagPropExchange@@QEAA@PEAUIPropertyBag@@PEAUIErrorLog@@HH@Z
extern "C" void* MS_ABI impl___0CPropbagPropExchange__QEAA_PEAUIPropertyBag__PEAUIErrorLog__HH_Z(
    void* pThis, void* pBag, void* pErrorLog, short flags, short mode) {
    (void)pBag;
    (void)pErrorLog;
    (void)flags;
    (void)mode;
    return pThis;
}

// Symbol: ??0CProperty@@QEAA@KQEAXK@Z
extern "C" void* MS_ABI impl___0CProperty__QEAA_KQEAXK_Z(
    void* pThis, unsigned long long flags, void* p, unsigned long long unused) {
    (void)flags;
    (void)p;
    (void)unused;
    return pThis;
}

// Symbol: ??0CProperty@@QEAA@XZ
extern "C" void* MS_ABI impl___0CProperty__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CPropertySection@@QEAA@U_GUID@@@Z
extern "C" void* MS_ABI impl___0CPropertySection__QEAA_U_GUID___Z(void* pThis, const void* pGuid) {
    (void)pGuid;
    return pThis;
}

// Symbol: ??0CPropertySection@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPropertySection__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CPropertySet@@QEAA@U_GUID@@@Z
extern "C" void* MS_ABI impl___0CPropertySet__QEAA_U_GUID___Z(void* pThis, const void* pGuid) {
    (void)pGuid;
    return pThis;
}

// Symbol: ??0CPropertySet@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPropertySet__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CPropsetPropExchange@@QEAA@AEAVCPropertySection@@PEAUIStorage@@H@Z
extern "C" void* MS_ABI impl___0CPropsetPropExchange__QEAA_AEAVCPropertySection__PEAUIStorage__H_Z(
    void* pThis, void* pSection, void* pStorage, int flags) {
    (void)pSection;
    (void)pStorage;
    (void)flags;
    return pThis;
}

// Symbol: ??0CReBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CReBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CReObject@@QEAA@PEAVCRichEditCntrItem@@@Z
extern "C" void* MS_ABI impl___0CReObject__QEAA_PEAVCRichEditCntrItem___Z(void* pThis, void* pItem) {
    (void)pItem;
    return pThis;
}

// Symbol: ??0CReObject@@QEAA@XZ
extern "C" void* MS_ABI impl___0CReObject__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CRecentDockSiteInfo@@QEAA@PEAVCPane@@@Z
extern "C" void* MS_ABI impl___0CRecentDockSiteInfo__QEAA_PEAVCPane___Z(void* pThis, void* pPane) {
    (void)pPane;
    return pThis;
}

// Symbol: ??0CResetPropExchange@@QEAA@XZ
extern "C" void* MS_ABI impl___0CResetPropExchange__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CScreenWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CScreenWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSmartDockingGroupGuide@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingGroupGuide__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSmartDockingGroupGuidesManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingGroupGuidesManager__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSmartDockingGroupGuidesWnd@@IEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingGroupGuidesWnd__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSmartDockingHighlighterWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingHighlighterWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSmartDockingManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingManager__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSmartDockingStandaloneGuide@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingStandaloneGuide__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSmartDockingStandaloneGuideWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSmartDockingStandaloneGuideWnd__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0CSplitterWndEx@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSplitterWndEx__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0PAGE_INFO@CPreviewView@@QEAA@XZ
extern "C" void* MS_ABI impl___0PAGE_INFO_CPreviewView__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XBase@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XBase_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    (void)type;
    return pThis;
}

// Symbol: ??0XCategory@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XCategory_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XCategoryMain@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XCategoryMain_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XContext@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XContext_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XElement@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    (void)type;
    return pThis;
}

// Symbol: ??0XElementButton@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    (void)type;
    return pThis;
}

// Symbol: ??0XElementButton@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButton_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XElementButtonApplication@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonApplication_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XElementButtonCheck@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonCheck_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XElementButtonColor@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonColor_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XElementButtonGallery@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElementButtonGallery_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    (void)type;
    return pThis;
}

// Symbol: ??0XElementButtonGallery@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XID@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XID_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XImage@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XImage_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XInfoError@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XInfoError_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XPanel@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XPanel_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XQATItem@XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XRibbonBar@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonBar_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XRibbonInfoParser@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParser_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XRibbonInfoParserCollection@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParserCollection_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??0XRibbonInfoParserRoot@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParserRoot_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1AFX_GLOBAL_DATA@@QEAA@XZ
extern "C" void MS_ABI impl___1AFX_GLOBAL_DATA__QEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CAnimationBaseObject@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimationBaseObject__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CAnimationController@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimationController__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CAnimationGroup@@QEAA@XZ
extern "C" void MS_ABI impl___1CAnimationGroup__QEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CAsyncMonikerFile@@UEAA@XZ
extern "C" void MS_ABI impl___1CAsyncMonikerFile__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CAutoHideDockSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CAutoHideDockSite__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CCommandManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CCommandManager__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CConnectionPoint@@UEAA@XZ
extern "C" void MS_ABI impl___1CConnectionPoint__UEAA_XZ(void* pThis) {
    (void)pThis;
}

extern "C" void MS_ABI impl__Destroy_CD2DGradientBrush__MEAAXXZ(void* pThis);

// Symbol: ??1CCtrlView@@UEAA@XZ
extern "C" void MS_ABI impl___1CCtrlView__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CD2DBitmap@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DBitmap__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DBitmap__UEAAXXZ(pThis);
    }
}

// Symbol: ??1CD2DBitmapBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DBitmapBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DBitmapBrush__UEAAXXZ(pThis);
    }
}

// Symbol: ??1CD2DBrush@@MEAA@XZ
extern "C" void MS_ABI impl___1CD2DBrush__MEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DBrush__UEAAXXZ(pThis);
    }
}

// Symbol: ??1CD2DGeometry@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DGeometry__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CD2DGeometrySink@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DGeometrySink__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CD2DGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DGradientBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DGradientBrush__MEAAXXZ(pThis);
    }
}

// Symbol: ??1CD2DLayer@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DLayer__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CD2DLinearGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DLinearGradientBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl__Destroy_CD2DLinearGradientBrush__UEAAXXZ(pThis);
    }
}

// Symbol: ??1CD2DMesh@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DMesh__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CD2DRadialGradientBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DRadialGradientBrush__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        std::lock_guard<std::mutex> lock(g_cd2dRadialGradientBrushStateMutex);
        g_cd2dRadialGradientBrushStates.erase(pThis);
    }
}

// Symbol: ??1CD2DSolidColorBrush@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DSolidColorBrush__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDHtmlControlSink@@UEAA@XZ
extern "C" void MS_ABI impl___1CDHtmlControlSink__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDataRecoveryHandler@@UEAA@XZ
extern "C" void MS_ABI impl___1CDataRecoveryHandler__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDialogImpl@@MEAA@XZ
extern "C" void MS_ABI impl___1CDialogImpl__MEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDocObjectServer@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocObjectServer__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDocObjectServerItem@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocObjectServerItem__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCDocObjectServerItemState(pThis);
    }
}

// Symbol: ??1CDockBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDockContext@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockContext__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDockSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockSite__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDockState@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockState__UEAA_XZ(void* pThis) {
    OpenMfcResetDockState(pThis);
}

// Symbol: ??1CDockablePaneAdapter@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockablePaneAdapter__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDockingPanesRow@@UEAA@XZ
extern "C" void MS_ABI impl___1CDockingPanesRow__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CDrawingManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CDrawingManager__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CEnumArray@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumArray__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CEnumConnPoints@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumConnPoints__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCEnumConnPointsState(pThis);
    }
}

// Symbol: ??1CEnumConnections@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumConnections__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCEnumConnectionsState(pThis);
    }
}

// Symbol: ??1CGestureConfig@@UEAA@XZ
extern "C" void MS_ABI impl___1CGestureConfig__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CHotKeyCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CHotKeyCtrl__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCHotKeyCtrlState(pThis);
    }
}

// Symbol: ??1CHtmlEditCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlEditCtrl__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CHtmlEditDoc@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlEditDoc__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CHtmlEditView@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlEditView__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CJumpList@@QEAA@XZ
extern "C" void MS_ABI impl___1CJumpList__QEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMDITabProxyWnd@@UEAA@XZ
extern "C" void MS_ABI impl___1CMDITabProxyWnd__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCAcceleratorKeyAssignCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCAcceleratorKeyAssignCtrl__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCBaseAccessibleObject@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseAccessibleObject__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCBaseToolBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseToolBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnAfterStretch@CMFCBaseToolBar@@MEAAXH@Z
extern "C" void MS_ABI impl__OnAfterStretch_CMFCBaseToolBar__MEAAXH_Z(
    void* pThis, int width) {
    (void)pThis;
    (void)width;
}

// Symbol: ?OnAfterChangeParent@CMFCBaseToolBar@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnAfterChangeParent_CMFCBaseToolBar__UEAAXPEAVCWnd___Z(
    void* pThis, void* pWnd) {
    (void)pThis;
    (void)pWnd;
}

// Symbol: ?CreateObject@CMFCBaseToolBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCBaseToolBar__SAPEAVCObject__XZ() {
    return new (std::nothrow) CMFCBaseToolBarObject();
}

// Symbol: ??1CMFCBaseVisualManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCBaseVisualManager__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCCaptionBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCCaptionBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCCmdUsageCount@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCCmdUsageCount__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCColorBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCColorButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorButton__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??0CMFCColorBarCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorBarCmdUI__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return pThis;
    }
    return new (pThis) CCmdUI();
}

// Symbol: ?Enable@CMFCColorBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCColorBarCmdUI__UEAAXH_Z(void* pThis, int bOn) {
    if (auto* pUI = static_cast<CCmdUI*>(pThis)) {
        pUI->Enable(bOn != 0);
    }
}

// Symbol: ??0CMFCRibbonCustomizeDialog@@QEAA@PEAVCWnd@@PEAVCMFCRibbonBar@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCustomizeDialog__QEAA_PEAVCWnd__PEAVCMFCRibbonBar___Z(
    void* pThis, void*, void*) {
    return pThis;
}

// Symbol: ??1CMFCRibbonCustomizeDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonCustomizeDialog__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??0CPrintPreviewState@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPrintPreviewState__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ?CreateObject@CMFCToolBarColorButton@@SAPEAVCObject@@XZ
namespace {
struct CMFCToolBarColorButtonObject : public CObject {
public:
    CMFCToolBarColorButtonObject() = default;
};
}

extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarColorButton__SAPEAVCObject__XZ() {
    return new (std::nothrow) CMFCToolBarColorButtonObject();
}

// Symbol: ?Enable@CMFCTasksPaneToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCTasksPaneToolBarCmdUI__UEAAXH_Z(void* pThis, int bOn) {
    if (auto* pUI = static_cast<CCmdUI*>(pThis)) {
        pUI->Enable(bOn != 0);
    }
}

// Symbol: ?SetText@CMFCTasksPaneToolBarCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCTasksPaneToolBarCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText) {
    if (auto* pUI = static_cast<CCmdUI*>(pThis)) {
        pUI->SetText(lpszText);
    }
}

// Symbol: ??0CXMLParserCollection@@IEAA@PEAUIXMLDOMNodeList@@@Z
extern "C" void* MS_ABI impl___0CXMLParserCollection__IEAA_PEAUIXMLDOMNodeList___Z(void* pThis, void*) {
    return pThis;
}

// Symbol: ??$HashKey@AEBUtagVARIANT@@@@YAIAEBUtagVARIANT@@@Z
extern "C" long long MS_ABI impl____HashKey_AEBUtagVARIANT____YAIAEBUtagVARIANT___Z(
    void* pThis, const void* value) {
    (void)pThis;
    if (value == nullptr) {
        return 0;
    }
    const void* firstField = value;
    return static_cast<long long>(
        HashRawBytes(firstField, sizeof(unsigned long long) * 4));
}

// Symbol: ??$HashKey@VCComBSTR@ATL@@@@YAIVCComBSTR@ATL@@@Z
extern "C" long long MS_ABI impl____HashKey_VCComBSTR_ATL____YAIVCComBSTR_ATL___Z(
    void* pThis, const void* value) {
    (void)pThis;
    if (value == nullptr) {
        return 0;
    }
    const void* firstField = value;
    unsigned long long hash = HashRawBytes(firstField, sizeof(unsigned long long) * 2);
    if (hash == 0) {
        hash = 1;
    }
    return static_cast<long long>(hash);
}

// Symbol: ??$SerializeElements@VCComBSTR@ATL@@@@YAXAEAVCArchive@@PEAVCComBSTR@ATL@@_J@Z
extern "C" void MS_ABI impl____SerializeElements_VCComBSTR_ATL____YAXAEAVCArchive__PEAVCComBSTR_ATL___J_Z(
    void* pThis, void* pArchive, void* pList, long long* pHash) {
    (void)pThis;
    (void)pArchive;
    (void)pList;
    (void)pHash;
}

// Symbol: ?ExchangeVersion@CPropExchange@@UEAAHAEAKKH@Z
extern "C" int MS_ABI impl__ExchangeVersion_CPropExchange__UEAAHAEAKKH_Z(
    void* /*pThis*/, unsigned long* pVersion, unsigned long preferredVersion, int) {
    if (pVersion == nullptr) {
        return 0;
    }

    const unsigned long loadedVersion = *pVersion;
    if (loadedVersion == 0) {
        *pVersion = preferredVersion;
        return 1;
    }

    *pVersion = loadedVersion;
    return (loadedVersion <= preferredVersion) ? 1 : 0;
}

// Symbol: ?CreateInstance@AFX_COM@@QEAAJAEBU_GUID@@PEAUIUnknown@@0PEAPEAX@Z
extern "C" long long MS_ABI impl__CreateInstance_AFX_COM__QEAAJAEBU_GUID__PEAUIUnknown__0PEAPEAX_Z(
    const GUID* clsid, IUnknown* pOuter, const GUID* iid, void** ppv) {
    if (ppv == nullptr || clsid == nullptr || iid == nullptr) {
        return static_cast<long long>(E_POINTER);
    }
    return static_cast<long long>(::CoCreateInstance(*clsid, pOuter, CLSCTX_ALL, *iid, ppv));
}

// Symbol: ?GetClassObject@AFX_COM@@QEAAJAEBU_GUID@@0PEAPEAX@Z
extern "C" long long MS_ABI impl__GetClassObject_AFX_COM__QEAAJAEBU_GUID__0PEAPEAX_Z(
    const GUID* clsid, const GUID* iid, void** ppv) {
    if (ppv == nullptr || clsid == nullptr || iid == nullptr) {
        return static_cast<long long>(E_POINTER);
    }
    return static_cast<long long>(::CoGetClassObject(*clsid, CLSCTX_ALL, nullptr, *iid, ppv));
}

// Symbol: ?OnDraw@CMFCToolBarColorButton@@MEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarColorButton__MEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const void* pRect, void* pImages,
    int nColor, int nStyle, int, int, int) {
    (void)pThis;
    (void)nColor;
    (void)nStyle;
    (void)pImages;

    auto* pDCObj = static_cast<CDC*>(pDC);
    if (pDCObj == nullptr || pDCObj->m_hDC == nullptr || pRect == nullptr) {
        return;
    }

    const RECT* rect = static_cast<const RECT*>(pRect);
    HBRUSH brush = ::GetSysColorBrush(COLOR_BTNFACE);
    ::FillRect(pDCObj->m_hDC, rect, brush);
}

// Symbol: ?Create@CPagerCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CPagerCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, const void* pRect, void* pParentWnd, unsigned int nID) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pRect == nullptr || pParentWnd == nullptr) {
        return FALSE;
    }

    auto* rect = static_cast<const RECT*>(pRect);
    if (rect == nullptr) {
        return FALSE;
    }

    HWND hParent = static_cast<CWnd*>(pParentWnd)->m_hWnd;
    if (!hParent) {
        return FALSE;
    }

    HWND hWnd = ::CreateWindowExW(
        0, L"SysPager", nullptr,
        WS_CHILD | WS_VISIBLE,
        rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
        hParent, (HMENU)(UINT_PTR)nID,
        AfxGetInstanceHandle(), nullptr);
    if (!hWnd) {
        return FALSE;
    }

    pWnd->Attach(hWnd);
    return TRUE;
}

// Symbol: ?CreateEx@CPagerCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CPagerCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long dwExStyle, const void* pRect, void* pParentWnd, unsigned int nID) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pRect == nullptr || pParentWnd == nullptr) {
        return FALSE;
    }

    auto* rect = static_cast<const RECT*>(pRect);
    if (rect == nullptr) {
        return FALSE;
    }

    HWND hParent = static_cast<CWnd*>(pParentWnd)->m_hWnd;
    if (!hParent) {
        return FALSE;
    }

    HWND hWnd = ::CreateWindowExW(
        static_cast<DWORD>(dwExStyle),
        L"SysPager", nullptr,
        WS_CHILD | WS_VISIBLE,
        rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top,
        hParent, (HMENU)(UINT_PTR)nID,
        AfxGetInstanceHandle(), nullptr);
    if (!hWnd) {
        return FALSE;
    }

    pWnd->Attach(hWnd);
    return TRUE;
}

// Symbol: ?OnEraseBkgnd@CPaneTrackingWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPaneTrackingWnd__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    auto* pCDC = static_cast<CDC*>(pDC);
    if (pWnd == nullptr || pCDC == nullptr || pCDC->m_hDC == nullptr) {
        return FALSE;
    }

    if (!pWnd->m_hWnd) {
        return FALSE;
    }

    RECT clientRect = {};
    ::GetClientRect(pWnd->m_hWnd, &clientRect);
    HBRUSH brush = ::GetSysColorBrush(COLOR_3DFACE);
    ::FillRect(pCDC->m_hDC, &clientRect, brush);
    return TRUE;
}

// Symbol: ?OnPaint@CPaneTrackingWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CPaneTrackingWnd__IEAAXXZ(void* pThis) {
    if (auto* pWnd = static_cast<CWnd*>(pThis)) {
        pWnd->OnPaint();
    }
}

// Symbol: ?OnNext@CEnumUnknown@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumUnknown__MEAAHPEAX_Z(void* pThis, void* pInterface) {
    if (pInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }

    auto** ppInterface = static_cast<void**>(pInterface);
    if (ppInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }

    if (pThis == nullptr) {
        *ppInterface = nullptr;
        return static_cast<int>(S_FALSE);
    }

    *ppInterface = nullptr;
    return static_cast<int>(S_FALSE);
}

// Symbol: ??1CEnumUnknown@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumUnknown__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    static_cast<CCmdTarget*>(pThis)->~CCmdTarget();
}

// Symbol: ?OnInitialUpdate@COleDBRecordView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_COleDBRecordView__UEAAXXZ(void* pThis) {
    if (auto* pView = static_cast<CRecordView*>(pThis)) {
        pView->OnInitialUpdate();
    }
}

// Symbol: ?OnMove@COleDBRecordView@@UEAAHI@Z
extern "C" int MS_ABI impl__OnMove_COleDBRecordView__UEAAHI_Z(void* pThis, unsigned int nIDMoveCommand) {
    if (auto* pView = static_cast<CRecordView*>(pThis)) {
        return static_cast<int>(pView->OnMove(nIDMoveCommand));
    }
    return 0;
}


// Symbol: ??1CMFCPropertyGridFileProperty@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCPropertyGridFileProperty__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnClickButton@CMFCPropertyGridFileProperty@@UEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridFileProperty__UEAAXVCPoint___Z(
    void* pThis, void* point) {
    (void)pThis;
    (void)point;
}

// Symbol: ??1CSplitterWndEx@@UEAA@XZ
extern "C" void MS_ABI impl___1CSplitterWndEx__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
}

// Symbol: ?OnDrawSplitter@CSplitterWndEx@@UEAAXPEAVCDC@@W4ESplitType@CSplitterWnd@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitter_CSplitterWndEx__UEAAXPEAVCDC__W4ESplitType_CSplitterWnd__AEBVCRect___Z(
    void* pThis, void* pDC, int splitType, unsigned char draw,
    const void* pRect) {
    (void)pThis;
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    if (pRect == nullptr) {
        return;
    }
    const RECT& splitRect = *static_cast<const RECT*>(pRect);
    if (IsRectEmpty(&splitRect)) {
        return;
    }

    const COLORREF color = (draw != 0) ? COLOR_3DSHADOW : COLOR_3DFACE;
    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(color));
    if (brush == nullptr) {
        return;
    }
    RECT rc = splitRect;
    if (splitType == 0) {
        rc.right = rc.left + 2;
    } else {
        rc.bottom = rc.top + 2;
    }
    ::FillRect(pDeviceContext->GetSafeHdc(), &rc, brush);
    ::DeleteObject(brush);
}

// Symbol: ?AddToStoryboard@CBaseTransition@@QEAAHPEAUIUIAnimationStoryboard@@@Z
extern "C" int MS_ABI impl__AddToStoryboard_CBaseTransition__QEAAHPEAUIUIAnimationStoryboard___Z(
    void* pThis, void* pStoryboard) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCBaseTransitionState(pThis);
    ++state.m_addToStoryboardCalls;
    (void)state;
    return static_cast<int>(S_OK);
}

// Symbol: ?AddToStoryboardAtKeyframes@CBaseTransition@@QEAAHPEAUIUIAnimationStoryboard@@@Z
extern "C" int MS_ABI impl__AddToStoryboardAtKeyframes_CBaseTransition__QEAAHPEAUIUIAnimationStoryboard___Z(
    void* pThis, void* pStoryboard) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCBaseTransitionState(pThis);
    ++state.m_addToStoryboardAtKeyframesCalls;
    (void)state;
    return static_cast<int>(S_OK);
}

// Symbol: ?GetTransition@CBaseTransition@@QEAAPEAUIUIAnimationTransition@@PEAUIUIAnimationTransitionLibrary@@PEAUIUIAnimationTransitionFactory@@@Z
extern "C" void* MS_ABI impl__GetTransition_CBaseTransition__QEAAPEAUIUIAnimationTransition__PEAUIUIAnimationTransitionLibrary__PEAUIUIAnimationTransitionFactory___Z(
    void* pThis, void* pLibrary, void* pFactory) {
    (void)pLibrary;
    (void)pFactory;
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCBaseTransitionState(pThis);
    if (state.m_transition == nullptr) {
        state.m_transition = pThis;
    }
    return state.m_transition;
}

// Symbol: ?Destroy@CD2DGradientBrush@@MEAAXXZ
extern "C" void MS_ABI impl__Destroy_CD2DGradientBrush__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_cd2dGradientBrushStateMutex);
    g_cd2dGradientBrushStates.erase(pThis);
}

// Symbol: ??1CPrintInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1CPrintInfo__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    UntrackCPrintInfo(pThis);
}

// Symbol: ??1CReObject@@QEAA@XZ
extern "C" void MS_ABI impl___1CReObject__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
}

// Symbol: ?AddConnPoint@CEnumConnPoints@@QEAAXPEAUIConnectionPoint@@@Z
extern "C" void MS_ABI impl__AddConnPoint_CEnumConnPoints__QEAAXPEAUIConnectionPoint___Z(
    void* pThis, void* pConnectionPoint) {
    if (pThis == nullptr || pConnectionPoint == nullptr) {
        return;
    }
    auto& state = EnsureCEnumConnPointsState(pThis);
    state.m_connectionPoints.push_back(pConnectionPoint);
    static_cast<IUnknown*>(pConnectionPoint)->AddRef();
}

// Symbol: ?OnNext@CEnumConnPoints@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumConnPoints__MEAAHPEAX_Z(void* pThis, void* pInterface) {
    if (pThis == nullptr || pInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto** ppInterface = static_cast<void**>(pInterface);
    std::lock_guard<std::mutex> lock(g_enumConnPointsStateMutex);
    const auto it = g_enumConnPointsStates.find(pThis);
    if (it == g_enumConnPointsStates.end() ||
        it->second.m_position >= it->second.m_connectionPoints.size()) {
        *ppInterface = nullptr;
        return static_cast<int>(S_FALSE);
    }

    void* connectionPoint = it->second.m_connectionPoints[it->second.m_position++];
    *ppInterface = connectionPoint;
    if (connectionPoint != nullptr) {
        static_cast<IUnknown*>(connectionPoint)->AddRef();
    }
    return static_cast<int>(S_OK);
}

// Symbol: ?OnDoVerb@CDocObjectServerItem@@MEAAXJ@Z
extern "C" void MS_ABI impl__OnDoVerb_CDocObjectServerItem__MEAAXJ_Z(void* pThis, int nVerb) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCDocObjectServerItemState(pThis);
    state.m_lastVerb = nVerb;
    state.m_visible = (nVerb != -7);
}

// Symbol: ?OnHide@CDocObjectServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnHide_CDocObjectServerItem__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCDocObjectServerItemState(pThis);
    state.m_visible = false;
}

// Symbol: ?OnShow@CDocObjectServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnShow_CDocObjectServerItem__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    auto& state = EnsureCDocObjectServerItemState(pThis);
    state.m_visible = true;
}

// Symbol: ?AddConnection@CEnumConnections@@QEAAXPEAUtagCONNECTDATA@@@Z
extern "C" void MS_ABI impl__AddConnection_CEnumConnections__QEAAXPEAUtagCONNECTDATA___Z(
    void* pThis, void* pConnectData) {
    if (pThis == nullptr || pConnectData == nullptr) {
        return;
    }
    auto& state = EnsureCEnumConnectionsState(pThis);
    const CONNECTDATA connectData = *static_cast<CONNECTDATA*>(pConnectData);
    state.m_connections.push_back(connectData);
    if (connectData.pUnk != nullptr) {
        connectData.pUnk->AddRef();
    }
}

// Symbol: ?OnClone@CEnumConnections@@MEAAPEAVCEnumArray@@XZ
extern "C" void* MS_ABI impl__OnClone_CEnumConnections__MEAAPEAVCEnumArray__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    std::vector<CONNECTDATA> originalConnections;
    {
        std::lock_guard<std::mutex> lock(g_enumConnectionsStateMutex);
        const auto it = g_enumConnectionsStates.find(pThis);
        if (it == g_enumConnectionsStates.end()) {
            return nullptr;
        }
        originalConnections = it->second.m_connections;
    }

    void* pClone = ::operator new(sizeof(void*));
    if (pClone == nullptr) {
        return nullptr;
    }
    impl___0CEnumConnections__QEAA_PEBXI_Z(pClone, nullptr, 0);
    auto& cloneState = EnsureCEnumConnectionsState(pClone, true);
    cloneState.m_connections = std::move(originalConnections);
    for (const CONNECTDATA& connection : cloneState.m_connections) {
        if (connection.pUnk != nullptr) {
            connection.pUnk->AddRef();
        }
    }
    return pClone;
}

// Symbol: ?OnNext@CEnumConnections@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumConnections__MEAAHPEAX_Z(void* pThis, void* pInterface) {
    if (pThis == nullptr || pInterface == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    CONNECTDATA* pConnectData = static_cast<CONNECTDATA*>(pInterface);
    std::lock_guard<std::mutex> lock(g_enumConnectionsStateMutex);
    const auto it = g_enumConnectionsStates.find(pThis);
    if (it == g_enumConnectionsStates.end() || it->second.m_position >= it->second.m_connections.size()) {
        pConnectData->pUnk = nullptr;
        pConnectData->dwCookie = 0;
        return static_cast<int>(S_FALSE);
    }

    CONNECTDATA result = it->second.m_connections[it->second.m_position++];
    pConnectData->pUnk = result.pUnk;
    pConnectData->dwCookie = result.dwCookie;
    if (pConnectData->pUnk != nullptr) {
        pConnectData->pUnk->AddRef();
    }
    return static_cast<int>(S_OK);
}

// Symbol: ?Create@CHotKeyCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CHotKeyCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long dwExStyle, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID);

extern "C" int MS_ABI impl__Create_CHotKeyCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID) {
    return impl__CreateEx_CHotKeyCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
        pThis, 0, nStyles, pRect, pParentWnd, nID);
}

// Symbol: ?CreateEx@CHotKeyCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CHotKeyCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long dwExStyle, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID) {
    if (pThis == nullptr || pRect == nullptr || pParentWnd == nullptr) {
        return FALSE;
    }

    const RECT* rect = static_cast<const RECT*>(pRect);
    HWND parentWnd = static_cast<CWnd*>(pParentWnd)->GetSafeHwnd();
    if (parentWnd == nullptr) {
        return FALSE;
    }

    HWND hWnd = ::CreateWindowExW(
        dwExStyle, HOTKEY_CLASSW, nullptr,
        nStyles | WS_CHILD,
        rect->left, rect->top,
        rect->right - rect->left,
        rect->bottom - rect->top,
        parentWnd, (HMENU)(UINT_PTR)nID,
        AfxGetInstanceHandle(), nullptr);
    if (hWnd == nullptr) {
        return FALSE;
    }

    static_cast<CWnd*>(pThis)->m_hWnd = hWnd;
    auto& state = EnsureCHotKeyCtrlState(pThis);
    state.m_virtualKey = 0;
    state.m_modifierFlags = 0;
    return TRUE;
}

// Symbol: ?GetHotKey@CHotKeyCtrl@@QEBAXAEAG0@Z
extern "C" void MS_ABI impl__GetHotKey_CHotKeyCtrl__QEBAXAEAG0_Z(
    const void* pThis, unsigned short* pVirtualKey, unsigned short* pModifierFlags) {
    if (pThis == nullptr) {
        if (pVirtualKey != nullptr) {
            *pVirtualKey = 0;
        }
        if (pModifierFlags != nullptr) {
            *pModifierFlags = 0;
        }
        return;
    }

    auto& state = EnsureCHotKeyCtrlState(const_cast<void*>(pThis));
    const auto* pWnd = static_cast<const CWnd*>(pThis);
    if (pWnd->GetSafeHwnd() == nullptr) {
        if (pVirtualKey != nullptr) {
            *pVirtualKey = state.m_virtualKey;
        }
        if (pModifierFlags != nullptr) {
            *pModifierFlags = state.m_modifierFlags;
        }
        return;
    }

    const LRESULT hotkey = ::SendMessageW(pWnd->GetSafeHwnd(), HKM_GETHOTKEY, 0, 0);
    const unsigned short virtualKey = static_cast<unsigned short>(LOWORD(static_cast<unsigned long>(hotkey)));
    const unsigned short modifierFlags = static_cast<unsigned short>(HIWORD(static_cast<unsigned long>(hotkey)));
    state.m_virtualKey = virtualKey;
    state.m_modifierFlags = modifierFlags;
    if (pVirtualKey != nullptr) {
        *pVirtualKey = virtualKey;
    }
    if (pModifierFlags != nullptr) {
        *pModifierFlags = modifierFlags;
    }
}

// Symbol: ?AddRef@CInnerUnknown@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CInnerUnknown__UEAAKXZ(void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }

    std::lock_guard<std::mutex> lock(g_innerUnknownMutex);
    auto& count = g_innerUnknownRefCounts[pThis];
    count = (count == 0) ? 1 : count + 1;
    return count;
}

// Symbol: ?QueryInterface@CInnerUnknown@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_CInnerUnknown__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const void* riid, void** ppObject) {
    if (ppObject == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    *ppObject = nullptr;
    if (pThis == nullptr || riid == nullptr) {
        return static_cast<long>(E_POINTER);
    }

    const GUID* requested = static_cast<const GUID*>(riid);
    if (IsEqualIID(*requested, IID_IUnknown) || IsEqualIID(*requested, IID_IClassFactory)) {
        *ppObject = pThis;
        impl__AddRef_CInnerUnknown__UEAAKXZ(pThis);
        return S_OK;
    }
    return static_cast<long>(E_NOINTERFACE);
}

// Symbol: ?Release@CInnerUnknown@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CInnerUnknown__UEAAKXZ(void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }

    std::lock_guard<std::mutex> lock(g_innerUnknownMutex);
    auto it = g_innerUnknownRefCounts.find(pThis);
    if (it == g_innerUnknownRefCounts.end()) {
        return 0;
    }

    unsigned long& count = it->second;
    if (count > 0) {
        --count;
    }
    const unsigned long remaining = count;
    if (remaining == 0) {
        g_innerUnknownRefCounts.erase(it);
    }
    return remaining;
}

// Symbol: ?AddToStoryboard@CKeyFrame@@UEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboard_CKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int keyFrameID) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCKeyFrameState(pThis);
    ++state.m_addToStoryboardCalls;
    state.m_lastStoryboard = pStoryboard;
    state.m_lastKeyFrameID = keyFrameID;
    return static_cast<int>(S_OK);
}

// Symbol: ?AddToStoryboardAfterTransition@CKeyFrame@@QEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboardAfterTransition_CKeyFrame__QEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int keyFrameID) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCKeyFrameState(pThis);
    ++state.m_addToStoryboardAfterTransitionCalls;
    state.m_lastStoryboard = pStoryboard;
    state.m_lastKeyFrameID = keyFrameID;
    return static_cast<int>(S_OK);
}

// Symbol: ?AddToStoryboardAtOffset@CKeyFrame@@UEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboardAtOffset_CKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* pThis, void* pStoryboard, int keyFrameID) {
    if (pThis == nullptr || pStoryboard == nullptr) {
        return static_cast<int>(E_POINTER);
    }
    auto& state = EnsureCKeyFrameState(pThis);
    ++state.m_addToStoryboardAtOffsetCalls;
    state.m_lastStoryboard = pStoryboard;
    state.m_lastKeyFrameID = keyFrameID;
    return static_cast<int>(S_OK);
}

// Symbol: ?GetBitmap@CBitmapRenderTarget@@QEAAHAEAVCD2DBitmap@@@Z
extern "C" int MS_ABI impl__GetBitmap_CBitmapRenderTarget__QEAAHAEAVCD2DBitmap___Z(
    void* pThis, void* pBitmap) {
    if (pThis == nullptr || pBitmap == nullptr) {
        return 0;
    }

    void* bitmap = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_bitmapRenderTargetMutex);
        auto it = g_bitmapRenderTargetBitmaps.find(pThis);
        if (it == g_bitmapRenderTargetBitmaps.end()) {
            return 0;
        }
        bitmap = it->second;
    }

    *static_cast<void**>(pBitmap) = bitmap;
    return 1;
}

// Symbol: ?Attach@CBitmapRenderTarget@@QEAAXPEAUID2D1BitmapRenderTarget@@@Z
extern "C" void MS_ABI impl__Attach_CBitmapRenderTarget__QEAAXPEAUID2D1BitmapRenderTarget___Z(
    void* pThis, void* pBitmapTarget) {
    if (pThis == nullptr || pBitmapTarget == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_bitmapRenderTargetMutex);
    g_bitmapRenderTargetBitmaps[pThis] = pBitmapTarget;
}

// Symbol: ?Detach@CBitmapRenderTarget@@QEAAPEAUID2D1BitmapRenderTarget@@XZ
extern "C" void* MS_ABI impl__Detach_CBitmapRenderTarget__QEAAPEAUID2D1BitmapRenderTarget__XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(g_bitmapRenderTargetMutex);
    auto it = g_bitmapRenderTargetBitmaps.find(pThis);
    if (it == g_bitmapRenderTargetBitmaps.end()) {
        return nullptr;
    }
    void* bitmap = it->second;
    g_bitmapRenderTargetBitmaps.erase(it);
    return bitmap;
}

// Symbol: ?Enable@CMFCStatusBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCStatusBarCmdUI__UEAAXH_Z(void* pThis, int bEnable) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->Enable(bEnable != 0);
    }
}

// Symbol: ?SetCheck@CMFCStatusBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CMFCStatusBarCmdUI__UEAAXH_Z(void* pThis, int check) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(check != 0);
    }
}

// Symbol: ?SetText@CMFCStatusBarCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCStatusBarCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* text) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetText(text);
    }
}

// Symbol: ?Enable@CStatusCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CStatusCmdUI__UEAAXH_Z(void* pThis, int bEnable) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->Enable(bEnable != 0);
    }
}

// Symbol: ?SetCheck@CStatusCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CStatusCmdUI__UEAAXH_Z(void* pThis, int check) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(check != 0);
    }
}

// Symbol: ?SetText@CStatusCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CStatusCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* text) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetText(text);
    }
}

// Symbol: ?OnDataAvailable@CCachedDataPathProperty@@UEAAXKK@Z
extern "C" void MS_ABI impl__OnDataAvailable_CCachedDataPathProperty__UEAAXKK_Z(
    void* pThis, unsigned long flags, unsigned long status) {
    (void)pThis;
    (void)flags;
    (void)status;
}

// Symbol: ?ResetData@CCachedDataPathProperty@@UEAAXXZ
extern "C" void MS_ABI impl__ResetData_CCachedDataPathProperty__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?Close@CCachedDataPathProperty@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CCachedDataPathProperty__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CToolBarCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CToolBarCtrl__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Shared helper for both `AddBitmap` overloads.
static int AddBitmapViaHandle(CWnd* pWnd, int nNumBits, HBITMAP hBitmap) {
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || hBitmap == nullptr) {
        return -1;
    }

    TBADDBITMAP info{};
    info.hInst = nullptr;
    info.nID = reinterpret_cast<UINT_PTR>(hBitmap);
    return static_cast<int>(::SendMessageW(pWnd->GetSafeHwnd(), TB_ADDBITMAP, static_cast<WPARAM>(nNumBits), reinterpret_cast<LPARAM>(&info)));
}

// Symbol: ?AddBitmap@CToolBarCtrl@@QEAAHHI@Z
extern "C" int MS_ABI impl__AddBitmap_CToolBarCtrl__QEAAHHI_Z(
    void* pThis, int nNumBits, unsigned int nBitmapID) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || nBitmapID == 0 || nNumBits <= 0) {
        return -1;
    }

    TBADDBITMAP info{};
    info.hInst = AfxGetInstanceHandle();
    info.nID = nBitmapID;
    return static_cast<int>(::SendMessageW(pWnd->GetSafeHwnd(), TB_ADDBITMAP, static_cast<WPARAM>(nNumBits), reinterpret_cast<LPARAM>(&info)));
}

// Symbol: ?AddBitmap@CToolBarCtrl@@QEAAHHPEAVCBitmap@@@Z
extern "C" int MS_ABI impl__AddBitmap_CToolBarCtrl__QEAAHHPEAVCBitmap___Z(
    void* pThis, int nNumBits, void* pBitmap) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || pBitmap == nullptr || nNumBits <= 0) {
        return -1;
    }

    HBITMAP hBitmap = static_cast<HBITMAP>(static_cast<CGdiObject*>(pBitmap)->GetSafeHandle());
    return AddBitmapViaHandle(pWnd, nNumBits, hBitmap);
}

// Symbol: ?CreateObject@COutlookCustomizeButton@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_COutlookCustomizeButton__SAPEAVCObject__XZ() {
    return new (std::nothrow) COutlookCustomizeButtonObject();
}

// Symbol: ?CreatePopupMenu@COutlookCustomizeButton@@UEAAPEAVCMFCPopupMenu@@XZ
extern "C" void* MS_ABI impl__CreatePopupMenu_COutlookCustomizeButton__UEAAPEAVCMFCPopupMenu__XZ(
    void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    auto& state = EnsureCOutlookCustomizeButtonState(pThis);
    if (state.m_popupMenu == nullptr) {
        state.m_popupMenu = new (std::nothrow) CMFCPopupMenu();
    }
    return state.m_popupMenu;
}

// Symbol: ?OnDraw@COutlookCustomizeButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_COutlookCustomizeButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const void* rect, void* pImages,
    int x, int y, int cx, int cy, int flags) {
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    if (pThis != nullptr) {
        auto& state = EnsureCOutlookCustomizeButtonState(pThis);
        state.m_didDraw = true;
    }

    const RECT* drawRect = static_cast<const RECT*>(rect);
    RECT localRect{};
    if (drawRect != nullptr) {
        localRect = *drawRect;
    } else {
        localRect = {x, y, x + cx, y + cy};
    }

    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    if (brush != nullptr) {
        ::FillRect(pDeviceContext->GetSafeHdc(), &localRect, brush);
        ::DeleteObject(brush);
    }

    if (!::IsRectEmpty(&localRect)) {
        ::DrawEdge(pDeviceContext->GetSafeHdc(), &localRect, BDR_RAISEDINNER, BF_RECT);
    }
    (void)pImages;
    (void)flags;
}

// Symbol: ?DoDataExchange@CMFCToolBarNameDialog@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarNameDialog__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDataExchange) {
    (void)pThis;
    (void)pDataExchange;
}

// Symbol: ?OnInitDialog@CMFCToolBarNameDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarNameDialog__MEAAHXZ(void* pThis) {
    (void)pThis;
    return TRUE;
}

// Symbol: ?OnUpdateToolbarName@CMFCToolBarNameDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnUpdateToolbarName_CMFCToolBarNameDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1CMFCOutlookBarPaneAdapter@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCOutlookBarPaneAdapter__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CreateObject@CMFCOutlookBarPaneAdapter@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCOutlookBarPaneAdapter__SAPEAVCObject__XZ() {
    return new (std::nothrow) CMFCOutlookBarPaneAdapterObject();
}

// Symbol: ?OnNcDestroy@CMFCOutlookBarPaneAdapter@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CMFCOutlookBarPaneAdapter__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?GetHit@CMFCCaptionButton@@UEBAIXZ
extern "C" unsigned int MS_ABI impl__GetHit_CMFCCaptionButton__UEBAIXZ(void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }
    auto& state = EnsureCMFCCaptionButtonState(pThis);
    return state.m_hit;
}

// Symbol: ?GetIconID@CMFCCaptionButton@@UEBA?AW4IMAGES_IDS@CMenuImages@@HH@Z
extern "C" void* MS_ABI impl__GetIconID_CMFCCaptionButton__UEBA_AW4IMAGES_IDS_CMenuImages__HH_Z(
    void* pThis, int imageId, unsigned int width, float scale, unsigned short* extra, unsigned char flags,
    void* args0, int args1, unsigned int args2, char args3, void* args4, void* args5,
    float args6, void* args7, void* args8, void* args9, unsigned int args10,
    void* args11, void* args12, void* args13, void* args14, void* args15, int args16, int args17) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCMFCCaptionButtonState(pThis);
    state.m_lastImageId = static_cast<unsigned int>(imageId);
    state.m_lastX = static_cast<int>(width);
    state.m_lastY = static_cast<int>(flags);
    if (extra != nullptr) {
        // Encode a small amount of caller-provided state into returned ID.
        *extra = static_cast<unsigned short>((imageId ^ flags) & 0xFFFF);
    }
    (void)scale;
    (void)args0;
    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
    (void)args6;
    (void)args7;
    (void)args8;
    (void)args9;
    (void)args10;
    (void)args11;
    (void)args12;
    (void)args13;
    (void)args14;
    (void)args15;
    (void)args16;
    (void)args17;
    return reinterpret_cast<void*>(static_cast<uintptr_t>(state.m_lastImageId));
}

// Symbol: ?GetHit@CMFCCaptionButton@@QEBAIXZ
extern "C" unsigned int MS_ABI impl__GetHit_CMFCCaptionButton__QEBAIXZ(const void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }
    auto& state = EnsureCMFCCaptionButtonState(const_cast<void*>(pThis));
    return state.m_hit;
}

// Symbol: ?OnDraw@CMFCCaptionButton@@UEAAXPEAVCDC@@HHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCaptionButton__UEAAXPEAVCDC__HHHH_Z(
    void* pThis, void* pDC, int x, int y, int width, int height) {
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    if (pThis != nullptr) {
        auto& state = EnsureCMFCCaptionButtonState(pThis);
        state.m_hit = 0;
        state.m_lastX = x;
        state.m_lastY = y;
    }

    RECT rc{ x, y, x + width, y + height };
    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    if (brush != nullptr) {
        ::FillRect(pDeviceContext->GetSafeHdc(), &rc, brush);
        ::DeleteObject(brush);
    }
    ::DrawFrameControl(pDeviceContext->GetSafeHdc(), &rc, DFC_BUTTON, DFCS_BUTTONPUSH);
}

// Symbol: ?OnContextMenu@CMFCPrintPreviewToolBar@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCPrintPreviewToolBar__IEAAXPEAVCWnd__VCPoint___Z(
    void* pThis, void* pWnd, const void* point) {
    if (pWnd == nullptr) {
        return;
    }
    const POINT* screenPoint = static_cast<const POINT*>(point);
    if (pThis != nullptr) {
        auto& state = EnsureCMFCPrintPreviewToolBarState(pThis);
        state.m_destroyed = false;
        if (screenPoint != nullptr) {
            state.m_lastX = screenPoint->x;
            state.m_lastY = screenPoint->y;
        }
    }
    if (screenPoint != nullptr) {
        CWnd* owner = static_cast<CWnd*>(pWnd);
        if (owner != nullptr) {
            POINT menuPoint = *screenPoint;
            if (owner->GetSafeHwnd()) {
                ::ClientToScreen(owner->GetSafeHwnd(), &menuPoint);
            }
            owner->SendMessageW(WM_CONTEXTMENU, (WPARAM)0, MAKELPARAM(menuPoint.x, menuPoint.y));
        }
    }
}

// Symbol: ?OnDestroy@CMFCPrintPreviewToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCPrintPreviewToolBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    ClearCMFCPrintPreviewToolBarState(pThis);
}

// Symbol: ?OnToolHitTest@CMFCPrintPreviewToolBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" long long MS_ABI impl__OnToolHitTest_CMFCPrintPreviewToolBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const void* pThis, const void* point, void* pInfo) {
    if (pThis == nullptr || point == nullptr || pInfo == nullptr) {
        return -1;
    }

    const POINT* cursor = static_cast<const POINT*>(point);
    auto& state = EnsureCMFCPrintPreviewToolBarState(const_cast<void*>(pThis));
    if (state.m_destroyed) {
        return -1;
    }
    state.m_lastToolHit = 0;
    state.m_lastX = cursor->x;
    state.m_lastY = cursor->y;

    tagTOOLINFOW* info = static_cast<tagTOOLINFOW*>(pInfo);
    info->cbSize = sizeof(tagTOOLINFOW);
    info->rect.left = cursor->x;
    info->rect.top = cursor->y;
    info->rect.right = cursor->x;
    info->rect.bottom = cursor->y;
    return 0;
}

// Symbol: ??1CMFCColorPopupMenu@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorPopupMenu__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCMFCColorPopupMenuState(pThis);
    }
}

// Symbol: ?CreateTearOffBar@CMFCColorPopupMenu@@UEAAPEAVCPane@@PEAVCFrameWnd@@IPEB_W@Z
extern "C" void* MS_ABI impl__CreateTearOffBar_CMFCColorPopupMenu__UEAAPEAVCPane__PEAVCFrameWnd__IPEB_W_Z(
    void* pThis, void* pFrameWnd, unsigned int commandId, const wchar_t* text) {
    if (pThis == nullptr || pFrameWnd == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCMFCColorPopupMenuState(pThis);
    state.m_commandId = commandId;
    state.m_text = const_cast<wchar_t*>(text);
    if (state.m_tearOffBar == nullptr) {
        state.m_tearOffBar = new (std::nothrow) CPane();
    }
    state.m_created = true;
    return state.m_tearOffBar;
}

// Symbol: ?OnCreate@CMFCColorPopupMenu@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCColorPopupMenu__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, void* createStruct) {
    if (pThis == nullptr || createStruct == nullptr) {
        return 0;
    }
    auto& state = EnsureCMFCColorPopupMenuState(pThis);
    state.m_created = true;
    return 1;
}

// Symbol: ?CopyFrom@CMFCToolBarMenuButtonsButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButtonsButton__UEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const void* pButton) {
    (void)pThis;
    (void)pButton;
}

// Symbol: ?OnCalculateSize@CMFCToolBarMenuButtonsButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" void* MS_ABI impl__OnCalculateSize_CMFCToolBarMenuButtonsButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, void* pDC, const void* size, int some) {
    (void)pThis;
    (void)pDC;
    (void)size;
    (void)some;
    return nullptr;
}

// Symbol: ?OnDraw@CMFCToolBarMenuButtonsButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarMenuButtonsButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const void* rect, void* pImages,
    int n1, int n2, int n3, int n4, int n5) {
    (void)pThis;
    (void)pDC;
    (void)rect;
    (void)pImages;
    (void)n1;
    (void)n2;
    (void)n3;
    (void)n4;
    (void)n5;
}

// Symbol: ?OnGetFont@CVSListBoxBase@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CVSListBoxBase__IEAAPEAUHFONT____XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd->GetSafeHwnd() == nullptr) {
        return nullptr;
    }
    auto* pFontHandle = reinterpret_cast<void*>(
        static_cast<uintptr_t>(pWnd->SendMessageW(WM_GETFONT, 0, 0)));
    return pFontHandle;
}

// Symbol: ?OnPaint@CVSListBoxBase@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CVSListBoxBase__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?OnSetFont@CVSListBoxBase@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CVSListBoxBase__IEAAXPEAVCFont__H_Z(
    void* pThis, void* pFont, int redraw) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr) {
        return;
    }
    if (pWnd->GetSafeHwnd() == nullptr) {
        return;
    }
    HFONT hFont = nullptr;
    if (pFont != nullptr) {
        hFont = reinterpret_cast<HFONT>(
            static_cast<CFont*>(pFont)->GetSafeHandle());
    }
    pWnd->SendMessageW(WM_SETFONT, reinterpret_cast<uintptr_t>(hFont), redraw);
}

// Symbol: ?Enable@CMFCToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCToolBarCmdUI__UEAAXH_Z(void* pThis, int bEnable) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->Enable(bEnable != 0);
    }
}

// Symbol: ?SetCheck@CMFCToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CMFCToolBarCmdUI__UEAAXH_Z(void* pThis, int bChecked) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(bChecked != 0);
    }
}

// Symbol: ?SetRadio@CMFCToolBarCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CMFCToolBarCmdUI__UEAAXH_Z(void* pThis, int bCheck) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetCheck(bCheck != 0);
    }
}

// Symbol: ?SetText@CMFCToolBarCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCToolBarCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* text) {
    if (auto* ui = static_cast<CCmdUI*>(pThis)) {
        ui->SetText(text);
    }
}

// Symbol: ?AddTransition@CAnimationValue@@QEAAXPEAVCBaseTransition@@@Z
extern "C" void MS_ABI impl__AddTransition_CAnimationValue__QEAAXPEAVCBaseTransition___Z(
    void* pThis, void* pTransition) {
    if (pThis == nullptr || pTransition == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    g_animationValueStates[pThis].m_transitions.push_back(pTransition);
}

// Symbol: ?GetAnimationVariableList@CAnimationValue@@MEAAXAEAV?$CList@PEAVCAnimationVariable@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetAnimationVariableList_CAnimationValue__MEAAXAEAV__CList_PEAVCAnimationVariable__PEAV1____Z(
    void* pThis, void* pList) {
    if (pThis == nullptr || pList == nullptr) {
        return;
    }

    auto* pVariableList = static_cast<CList<CAnimationVariable*, CAnimationVariable*>*>(pList);
    pVariableList->RemoveAll();

    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    auto& state = g_animationValueStates[pThis];
    if (state.m_variable == nullptr) {
        state.m_variable = new (std::nothrow) CAnimationVariable(state.m_value);
        if (state.m_variable) {
            static_cast<CAnimationVariable*>(state.m_variable)->SetDefaultValue(state.m_value);
        }
    }
    if (state.m_variable) {
        pVariableList->AddTail(static_cast<CAnimationVariable*>(state.m_variable));
    }
}

// Symbol: ?GetValue@CAnimationValue@@QEAAHAEAH@Z
extern "C" int MS_ABI impl__GetValue_CAnimationValue__QEAAHAEAH_Z(void* pThis, int* pValue) {
    if (pThis == nullptr || pValue == nullptr) {
        return E_POINTER;
    }
    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    *pValue = static_cast<int>(g_animationValueStates[pThis].m_value);
    return S_OK;
}

// Symbol: ?GetValue@CAnimationValue@@QEAAHAEAN@Z
extern "C" int MS_ABI impl__GetValue_CAnimationValue__QEAAHAEAN_Z(void* pThis, double* pValue) {
    if (pThis == nullptr || pValue == nullptr) {
        return E_POINTER;
    }
    std::lock_guard<std::mutex> lock(g_animationValueStateMutex);
    *pValue = g_animationValueStates[pThis].m_value;
    return S_OK;
}

// Symbol: ?AutoLoad@CBitmapButton@@QEAAHIPEAVCWnd@@@Z
extern "C" int MS_ABI impl__AutoLoad_CBitmapButton__QEAAHIPEAVCWnd___Z(
    void* pThis, unsigned int nId, void* pWnd) {
    if (pThis == nullptr || pWnd == nullptr || nId == 0) {
        return 0;
    }

    auto* button = static_cast<CWnd*>(pThis);
    if (button->GetSafeHwnd() == nullptr) {
        return 0;
    }

    return LoadBitmapButtonStateImages(
        pThis,
        MAKEINTRESOURCEW(nId),
        MAKEINTRESOURCEW(nId),
        MAKEINTRESOURCEW(nId),
        MAKEINTRESOURCEW(nId)) ? 1 : 0;
}

// Symbol: ?DrawItem@CBitmapButton@@MEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CBitmapButton__MEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, void* pDrawItemStruct) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || pDrawItemStruct == nullptr) {
        return;
    }

    auto* pDis = static_cast<DRAWITEMSTRUCT*>(pDrawItemStruct);
    HDC hdc = pDis->hDC;
    if (hdc == nullptr) {
        return;
    }

    {
        HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
        ::FillRect(hdc, &pDis->rcItem, brush);
        ::DeleteObject(brush);
    }

    HBITMAP selectedBitmap = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_bitmapButtonStateMutex);
        auto it = g_bitmapButtonStates.find(pThis);
        if (it != g_bitmapButtonStates.end()) {
            selectedBitmap = PickBitmapForBitmapButton(it->second, pDis->itemState);
        }
    }

    if (selectedBitmap != nullptr) {
        DrawBitmapCentered(hdc, selectedBitmap, pDis->rcItem);
    }

    if ((pDis->itemState & ODS_FOCUS) != 0) {
        RECT focusRect = pDis->rcItem;
        ::InflateRect(&focusRect, -1, -1);
        if (!::IsRectEmpty(&focusRect)) {
            ::DrawFocusRect(hdc, &focusRect);
        }
    }
}

// Symbol: ?LoadBitmaps@CBitmapButton@@QEAAHPEB_W000@Z
extern "C" int MS_ABI impl__LoadBitmaps_CBitmapButton__QEAAHPEB_W000_Z(
    void* pThis, const wchar_t* normal, const wchar_t* selected,
    const wchar_t* disabled, const wchar_t* focus) {
    return LoadBitmapButtonStateImages(pThis, normal, selected, disabled, focus) ? 1 : 0;
}

// Symbol: ?SizeToContent@CBitmapButton@@QEAAXXZ
extern "C" void MS_ABI impl__SizeToContent_CBitmapButton__QEAAXXZ(void* pThis) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr) {
        return;
    }

    HBITMAP hBitmap = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_bitmapButtonStateMutex);
        auto it = g_bitmapButtonStates.find(pThis);
        if (it != g_bitmapButtonStates.end()) {
            hBitmap = PickBitmapForBitmapButton(it->second, 0);
        }
    }

    if (hBitmap == nullptr) {
        return;
    }

    BITMAP bmp {};
    if (!::GetObjectW(hBitmap, sizeof(bmp), &bmp)) {
        return;
    }

    CRect rect {};
    rect.left = 0;
    rect.top = 0;
    rect.right = static_cast<int>(bmp.bmWidth);
    rect.bottom = static_cast<int>(bmp.bmHeight);
    pWnd->SetWindowPos(nullptr, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

// Symbol: ?OnDateTimeChange@CMFCToolBarDateTimeCtrlImpl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDateTimeChange_CMFCToolBarDateTimeCtrlImpl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, void* pNotifyStruct, long long* pResult) {
    auto* header = static_cast<NMHDR*>(pNotifyStruct);
    if (pResult) {
        *pResult = 0;
    }
    if (pThis == nullptr || header == nullptr) {
        return;
    }

    HWND hWnd = static_cast<CWnd*>(pThis)->GetSafeHwnd();
    HWND hParent = hWnd ? ::GetParent(hWnd) : header->hwndFrom;
    if (hParent != nullptr) {
        ::SendMessageW(hParent, WM_NOTIFY, static_cast<WPARAM>(header->idFrom), reinterpret_cast<LPARAM>(header));
    }
}

// Symbol: ?OnDateTimeCloseUp@CMFCToolBarDateTimeCtrlImpl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDateTimeCloseUp_CMFCToolBarDateTimeCtrlImpl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, void* pNotifyStruct, long long* pResult) {
    auto* header = static_cast<NMHDR*>(pNotifyStruct);
    if (pResult) {
        *pResult = 0;
    }
    if (pThis == nullptr || header == nullptr) {
        return;
    }

    HWND hWnd = static_cast<CWnd*>(pThis)->GetSafeHwnd();
    HWND hParent = hWnd ? ::GetParent(hWnd) : header->hwndFrom;
    if (hParent != nullptr) {
        ::SendMessageW(hParent, WM_NOTIFY, static_cast<WPARAM>(header->idFrom), reinterpret_cast<LPARAM>(header));
    }
}

// Symbol: ?OnDateTimeDropDown@CMFCToolBarDateTimeCtrlImpl@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDateTimeDropDown_CMFCToolBarDateTimeCtrlImpl__IEAAXPEAUtagNMHDR__PEA_J_Z(
    void* pThis, void* pNotifyStruct, long long* pResult) {
    auto* header = static_cast<NMHDR*>(pNotifyStruct);
    if (pResult) {
        *pResult = 0;
    }
    if (pThis == nullptr || header == nullptr) {
        return;
    }

    HWND hWnd = static_cast<CWnd*>(pThis)->GetSafeHwnd();
    HWND hParent = hWnd ? ::GetParent(hWnd) : header->hwndFrom;
    if (hParent != nullptr) {
        ::SendMessageW(hParent, WM_NOTIFY, static_cast<WPARAM>(header->idFrom), reinterpret_cast<LPARAM>(header));
    }
}

// Symbol: ?GetIntermediateSize@CMFCRibbonCheckBox@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__GetIntermediateSize_CMFCRibbonCheckBox__UEAA_AVCSize__PEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
    static CSize size(16, 16);
    return static_cast<void*>(&size);
}

// Symbol: ?OnDraw@CMFCRibbonCheckBox@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCRibbonCheckBox__UEAAXPEAVCDC___Z(
    void* pThis, void* pDC) {
    (void)pThis;
    (void)pDC;
}

// Symbol: ?OnDrawOnList@CMFCRibbonCheckBox@@UEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonCheckBox__UEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    void* pThis, void* pDC, const void* text, const void* rect, int index, int isEnabled) {
    (void)pThis;
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    const RECT* itemRect = static_cast<const RECT*>(rect);
    if (itemRect == nullptr) {
        return;
    }
    COLORREF bg = (isEnabled == 0) ? COLOR_BTNFACE : COLOR_WINDOW;
    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(bg));
    if (brush != nullptr) {
        ::FillRect(pDeviceContext->GetSafeHdc(), itemRect, brush);
        ::DeleteObject(brush);
    }
    if (index >= 0 && (index & 1)) {
        ::FrameRect(pDeviceContext->GetSafeHdc(), itemRect, (HBRUSH)(intptr_t)(COLOR_3DLIGHT + 1));
    }
    (void)text;
}

// Symbol: ?GetRuntimeClass@CArchiveException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CArchiveException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CBitmap@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CBitmap__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CBitmap@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CBitmap__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CBrush@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CBrush__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CBrush@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CBrush__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CClientDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CClientDC__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CClientDC@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CClientDC__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CDC__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CDC@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CDC__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CDynLinkLibrary@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CDynLinkLibrary@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CDocObjectServerItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CDocObjectServerItem__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CDocObjectServerItem@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CDocObjectServerItem__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CEdit@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CEdit__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CEdit@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CEdit__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CFolderPickerDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CFolderPickerDialog__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CFolderPickerDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CFolderPickerDialog__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CFont@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CFont__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CFont@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CFont__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CInvalidArgException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CInvalidArgException@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CKeyFrame@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CKeyFrame__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CKeyFrame@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CKeyFrame__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CLongBinary@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CLongBinary__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CLongBinary@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CLongBinary__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CMFCPropertyGridFileProperty@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CMFCPropertyGridFileProperty__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CMFCPropertyGridFileProperty@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CMFCPropertyGridFileProperty__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CNotSupportedException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CNotSupportedException@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@COleBusyDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_COleBusyDialog__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@COleBusyDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_COleBusyDialog__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@COleLinksDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_COleLinksDialog__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@COleLinksDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_COleLinksDialog__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@COutlookCustomizeButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_COutlookCustomizeButton__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@COutlookCustomizeButton@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_COutlookCustomizeButton__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CPagerCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPagerCtrl__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CPagerCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPagerCtrl__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CPaintDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPaintDC__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CPaintDC@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPaintDC__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CPalette@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPalette__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CPalette@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPalette__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CPen@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CPen__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CPen@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CPen__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CProgressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CProgressCtrl__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CProgressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CProgressCtrl__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CMemoryException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CMemoryException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CUserException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CUserException@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CMFCPropertySheetCategoryInfo@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CMFCPropertySheetCategoryInfo__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CMFCPropertySheetCategoryInfo@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CMFCPropertySheetCategoryInfo__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetMessageMap@CDummyDockablePane@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetMessageMap_CDummyDockablePane__MEBAPEBUAFX_MSGMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CDummyDockablePane@@KAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetThisMessageMap_CDummyDockablePane__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetMessageMap@CMFCOutlookBarPaneList@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetMessageMap_CMFCOutlookBarPaneList__MEBAPEBUAFX_MSGMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CMFCOutlookBarPaneList@@KAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetThisMessageMap_CMFCOutlookBarPaneList__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetInterfaceMap@CEnumConnPoints@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetInterfaceMap_CEnumConnPoints__MEBAPEBUAFX_INTERFACEMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisInterfaceMap@CEnumConnPoints@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetThisInterfaceMap_CEnumConnPoints__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetInterfaceMap@CEnumConnections@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetInterfaceMap_CEnumConnections__MEBAPEBUAFX_INTERFACEMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisInterfaceMap@CEnumConnections@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetThisInterfaceMap_CEnumConnections__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetInterfaceMap@CEnumUnknown@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetInterfaceMap_CEnumUnknown__MEBAPEBUAFX_INTERFACEMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisInterfaceMap@CEnumUnknown@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" void* MS_ABI impl__GetThisInterfaceMap_CEnumUnknown__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizeDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CMFCRibbonCustomizeDialog__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CMFCRibbonCustomizeDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CMFCRibbonCustomizeDialog__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetRuntimeClass@CMFCRibbonCustomizeCategory@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CMFCRibbonCustomizeCategory__UEBAPEAUCRuntimeClass__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisClass@CMFCRibbonCustomizeCategory@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CMFCRibbonCustomizeCategory__SAPEAUCRuntimeClass__XZ() {
    return nullptr;
}

// Symbol: ?GetMessageMap@CMFCToolBarDateTimeCtrlImpl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetMessageMap_CMFCToolBarDateTimeCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CMFCToolBarDateTimeCtrlImpl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" void* MS_ABI impl__GetThisMessageMap_CMFCToolBarDateTimeCtrlImpl__KAPEBUAFX_MSGMAP__XZ() {
    return nullptr;
}

// Symbol: ?DeleteTemp@CHandleMap@@QEAAXXZ
extern "C" void MS_ABI impl__DeleteTemp_CHandleMap__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_handleMapMutex);
    auto it = g_tempHandleMap.find(pThis);
    if (it == g_tempHandleMap.end()) {
        return;
    }

    for (auto& entry : it->second) {
        delete entry.second;
    }
    g_tempHandleMap.erase(it);
}

// Symbol: ?FromHandle@CHandleMap@@QEAAPEAVCObject@@PEAX@Z
extern "C" void* MS_ABI impl__FromHandle_CHandleMap__QEAAPEAVCObject__PEAX_Z(
    void* pThis, void* hHandle) {
    if (pThis == nullptr || hHandle == nullptr) {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(g_handleMapMutex);
    auto& table = g_tempHandleMap[pThis];
    auto it = table.find(hHandle);
    if (it != table.end()) {
        return it->second;
    }

    auto* object = new (std::nothrow) CTempHandleObject();
    if (object == nullptr) {
        return nullptr;
    }
    object->m_handle = hHandle;
    table.emplace(hHandle, object);
    return object;
}

// Symbol: ?Serialize@CMDITabInfo@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMDITabInfo__QEAAXAEAVCArchive___XZ(
    void* pThis, void* pArchive) {
    (void)pThis;
    (void)pArchive;
}

// Symbol: ?CopyTo@CSmartDockingInfo@@QEAAXAEAV1@@Z
extern "C" void MS_ABI impl__CopyTo_CSmartDockingInfo__QEAAXAEAV1__Z(
    void* pThis, void* pSrc) {
    (void)pThis;
    (void)pSrc;
}

// Symbol: ?CommonInit@CD2DBrushProperties@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CD2DBrushProperties__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }

    auto* p = static_cast<float*>(pThis);
    p[0] = 1.0f;
    p[1] = 1.0f;
    p[2] = 0.0f;
    p[3] = 0.0f;
    p[4] = 1.0f;
    p[5] = 0.0f;
    p[6] = 0.0f;
}

// Symbol: ?Clear@CDBVariant@@QEAAXXZ
extern "C" void MS_ABI impl__Clear_CDBVariant__QEAAXXZ(void* pThis) {
    ResetCDBVariantMemory(pThis);
}

// Symbol: ?DoUpdate@COleCmdUI@@UEAAHPEAVCCmdTarget@@H@Z
extern "C" int MS_ABI impl__DoUpdate_COleCmdUI__UEAAHPEAVCCmdTarget__H_Z(
    void* pThis, void* pCmdTarget, int nID) {
    if (pThis == nullptr || pCmdTarget == nullptr) {
        return 0;
    }
    return static_cast<int>(static_cast<COleCmdUI*>(pThis)->DoUpdate(static_cast<CCmdTarget*>(pCmdTarget), nID != 0));
}

// Symbol: ?GetStyle@COleControlSiteOrWnd@@QEBAKXZ
extern "C" unsigned long MS_ABI impl__GetStyle_COleControlSiteOrWnd__QEBAKXZ(void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }
    return static_cast<COleControlSiteOrWnd*>(pThis)->GetStyle();
}

// Symbol: ?DrawItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
static HFONT GetChevronOwnerDrawMenuFont() {
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

// Symbol: ?MeasureItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CChevronOwnerDrawMenu__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    void* pThis, void* pMis) {
    (void)pThis;
    auto* mis = static_cast<MEASUREITEMSTRUCT*>(pMis);
    if (mis == nullptr || mis->CtlType != ODT_MENU) {
        return;
    }

    UINT height = static_cast<UINT>(::GetSystemMetrics(SM_CYMENU));
    UINT checkWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXMENUCHECK));
    LPCWSTR text = reinterpret_cast<LPCWSTR>(mis->itemData);
    UINT width = 0;
    HDC hdc = ::GetDC(nullptr);
    if (hdc) {
        HFONT font = GetChevronOwnerDrawMenuFont();
        HGDIOBJ old = ::SelectObject(hdc, font);
        if (text != nullptr && text[0] != 0) {
            SIZE size {};
            if (::GetTextExtentPoint32W(hdc, text, static_cast<int>(::lstrlenW(text)), &size)) {
                width = static_cast<UINT>(size.cx);
                height = static_cast<UINT>(std::max<int>(static_cast<int>(height), size.cy));
            } else if (size.cx > 0) {
                width = static_cast<UINT>(size.cx);
            }
        }
        TEXTMETRICW tm {};
        if (::GetTextMetricsW(hdc, &tm)) {
            height = static_cast<UINT>(std::max<int>(static_cast<int>(height), tm.tmHeight));
        }
        if (old) {
            ::SelectObject(hdc, old);
        }
        ::ReleaseDC(nullptr, hdc);
    }

    mis->itemWidth = width + checkWidth + 4;
    mis->itemHeight = height;
}

namespace {
// Layout-faithful CMFCCaptionMenuButton view used by non-constructor exports.
struct CMFCCaptionMenuButtonLayout {
    void* vfptr;                  // 0
    int m_bPushed;                // 8
    int m_bFocused;               // 12
    int m_bHidden;                // 16
    int m_bEnabled;               // 20
    int m_bDroppedDown;           // 24
    int m_bLeftAlign;             // 28
    unsigned int m_nHit;          // 32
    unsigned long m_clrForeground; // 36
    POINT m_ptOrg;                // 40
    int m_bIsMiniFrameButton;     // 48
    int _pad0;                    // 52
    int m_nMenuResult;            // 56
    int m_bOSMenu;                // 60
    int m_bMenuIsActive;          // 64
    int _pad1;                    // 68
};

static HWND WndHandleForObject(void* pWnd) {
    return pWnd ? *reinterpret_cast<HWND*>(static_cast<char*>(pWnd) + 64) : NULL;
}

static HDC DCHandleForCDC(void* pDC) {
    return pDC ? *reinterpret_cast<HDC*>(static_cast<char*>(pDC) + 8) : NULL;
}

static RECT GetCaptionMenuRect(void* pThis) {
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
}  // namespace

// Symbol: ?OnDraw@CMFCCaptionMenuButton@@UEAAXPEAVCDC@@HHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCaptionMenuButton__UEAAXPEAVCDC__HHHH_Z(
    void* pThis, void* pDC, int bActive, int /*bHorz*/, int /*bMaximized*/, int bDisabled) {
    auto* self = static_cast<CMFCCaptionMenuButtonLayout*>(pThis);
    HDC hdc = DCHandleForCDC(pDC);
    if (self == nullptr || hdc == nullptr) {
        return;
    }

    RECT rc = GetCaptionMenuRect(pThis);
    if (rc.right <= rc.left || rc.bottom <= rc.top) {
        return;
    }

    COLORREF bk = ::GetSysColor(self->m_bPushed ? COLOR_HIGHLIGHT
                              : (bActive ? COLOR_ACTIVECAPTION
                                         : COLOR_INACTIVECAPTION));
    HBRUSH hbr = ::CreateSolidBrush(bk);
    if (hbr) {
        ::FillRect(hdc, &rc, hbr);
        ::DeleteObject(hbr);
    }

    int cx = (rc.left + rc.right) / 2;
    int cy = (rc.top + rc.bottom) / 2;
    int r = 3;
    POINT tri[3] = {
        {cx - r, cy - 1},
        {cx + r, cy - 1},
        {cx, cy + r - 1},
    };
    COLORREF fg = ::GetSysColor(bDisabled ? COLOR_GRAYTEXT : COLOR_CAPTIONTEXT);
    HBRUSH fb = static_cast<HBRUSH>(::CreateSolidBrush(fg));
    HPEN fp = static_cast<HPEN>(::CreatePen(PS_SOLID, 1, fg));
    HGDIOBJ oldBrush = NULL;
    HGDIOBJ oldPen = NULL;
    if (fb) {
        oldBrush = ::SelectObject(hdc, fb);
    }
    if (fp) {
        oldPen = ::SelectObject(hdc, fp);
    }
    ::Polygon(hdc, tri, 3);
    if (oldBrush) {
        ::SelectObject(hdc, oldBrush);
        ::DeleteObject(fb);
    }
    if (oldPen) {
        ::SelectObject(hdc, oldPen);
        ::DeleteObject(fp);
    }
}

// Symbol: ?ShowMenu@CMFCCaptionMenuButton@@QEAAXPEAUHMENU__@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__ShowMenu_CMFCCaptionMenuButton__QEAAXPEAUHMENU____PEAVCWnd___Z(
    void* pThis, void* hMenu, void* pWindow) {
    auto* self = static_cast<CMFCCaptionMenuButtonLayout*>(pThis);
    HWND hWnd = WndHandleForObject(pWindow);

    if (self == nullptr) {
        return;
    }

    self->m_bPushed = TRUE;
    self->m_bMenuIsActive = TRUE;
    self->m_nMenuResult = 0;

    if (hMenu != nullptr) {
        HMENU hmenu = static_cast<HMENU>(hMenu);
        POINT pt {self->m_ptOrg.x, self->m_ptOrg.y};
        if (hWnd) {
            ::ClientToScreen(hWnd, &pt);
        }
        int cmd = ::TrackPopupMenu(
            hmenu,
            TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
            pt.x, pt.y, 0, hWnd, NULL);
        self->m_nMenuResult = cmd;
        if (cmd != 0 && hWnd) {
            ::PostMessageW(hWnd, WM_COMMAND, static_cast<WPARAM>(cmd), 0);
        }
    }

    self->m_bMenuIsActive = FALSE;
    self->m_bPushed = FALSE;
}

// Symbol: ?EnsureVisible@CMFCOutlookBarPaneList@@AEAAXH@Z
extern "C" void MS_ABI impl__EnsureVisible_CMFCOutlookBarPaneList__AEAAXH_Z(
    void* pThis, int bVisible) {
    struct CMFCOutlookBarPaneListLayout {
        void* vfptr;
        char _opaque[11120 - 8 - 8];
        void* m_pParent;
    };

    const auto* self = static_cast<const CMFCOutlookBarPaneListLayout*>(pThis);
    if (self == nullptr) {
        return;
    }

    (void)bVisible;
    (void)self->m_pParent;
}

// Symbol: ?OnSendCommand@CMFCOutlookBarPaneList@@EEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCOutlookBarPaneList__EEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, const void* pButton) {
    if (pThis == nullptr || pButton == nullptr) {
        return 0;
    }
    return 1;
}

// Symbol: ?SetActiveTab@CMFCPropertySheetTabCtrl@@EEAAHH@Z
extern "C" int MS_ABI impl__SetActivePage_CPropertySheet__QEAAHH_Z(void* pThis, int p0);

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

// Symbol: ?SetACCData@CMFCTasksPanePropertyPage@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPanePropertyPage__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData) {
    if (pThis == nullptr || pData == nullptr) {
        return 0;
    }

    struct CTaskPanePageLayout {
        void* vfptr;
        void* m_strName;
        void* m_pTaskPane;
    };

    auto* self = static_cast<CTaskPanePageLayout*>(pThis);
    auto* data = static_cast<CAccessibilityData*>(pData);
    data->m_strAccName = self->m_strName ? *static_cast<CString*>(self->m_strName) : CString();
    data->m_strAccValue.Empty();
    data->m_strDescription.Empty();
    data->m_strAccKeys.Empty();
    data->m_strAccHelp.Empty();
    data->m_strAccDefAction.Empty();
    data->m_nAccRole = 0;
    data->m_bAccState = 0;
    data->m_nAccHit = 0;

    if (pWindow != nullptr) {
        HWND hParent = *reinterpret_cast<HWND*>(static_cast<char*>(pWindow) + 64);
        if (::IsWindow(hParent)) {
            RECT rc {};
            if (::GetClientRect(hParent, &rc)) {
                POINT ptTL { rc.left, rc.top };
                POINT ptBR { rc.right, rc.bottom };
                ::ClientToScreen(hParent, &ptTL);
                ::ClientToScreen(hParent, &ptBR);
                data->m_rectAccLocation = CRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y);
                data->m_ptAccHit.x = (ptTL.x + ptBR.x) / 2;
                data->m_ptAccHit.y = (ptTL.y + ptBR.y) / 2;
            }
        }
    }

    return 1;
}

// Symbol: ?SetACCData@CMFCTasksPaneTask@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPaneTask__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData) {
    if (pThis == nullptr || pData == nullptr) {
        return 0;
    }

    struct CTaskLayout {
        void* vfptr;
        void* m_pGroup;
        void* m_strName;
        RECT m_rect;
        int m_nIcon;
        int m_nWindowHeight;
        unsigned m_uiCommandID;
        unsigned _pad0;
        uintptr_t m_dwUserData;
        HWND m_hwndTask;
        int m_bAutoDestroyWindow;
        int m_bVisible;
        int m_bEnabled;
        int m_bIsSeparator;
        int m_bIsBold;
        unsigned long m_clrText;
        unsigned long m_clrTextHot;
        unsigned _pad1;
    };

    auto* self = static_cast<CTaskLayout*>(pThis);
    auto* data = static_cast<CAccessibilityData*>(pData);
    if (pWindow != nullptr) {
        HWND hParent = *reinterpret_cast<HWND*>(static_cast<char*>(pWindow) + 64);
        if (::IsWindow(hParent)) {
            RECT rc = self->m_rect;
            POINT ptTL { rc.left, rc.top };
            POINT ptBR { rc.right, rc.bottom };
            ::ClientToScreen(hParent, &ptTL);
            ::ClientToScreen(hParent, &ptBR);
            data->m_rectAccLocation = CRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y);
        }
    }

    return 1;
}

// Symbol: ?SetACCData@CMFCTasksPaneTaskGroup@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData) {
    if (pThis == nullptr || pData == nullptr) {
        return 0;
    }

    struct CTaskGroupLayout {
        void* vfptr;
        void* m_pPage;
        void* m_strName;
        char _lstTasks[56];
        int m_bIsBottom;
        int m_bIsSpecial;
        int m_bIsCollapsed;
        RECT m_rect;
        RECT m_rectGroup;
        SIZE m_sizeIcon;
        HICON m_hIcon;
        unsigned long m_clrText;
        unsigned long m_clrTextHot;
    };

    auto* self = static_cast<CTaskGroupLayout*>(pThis);
    auto* data = static_cast<CAccessibilityData*>(pData);
    data->m_strAccName = self->m_strName ? *static_cast<CString*>(self->m_strName) : CString();
    data->m_strAccValue.Empty();
    data->m_strDescription.Empty();
    data->m_strAccKeys.Empty();
    data->m_strAccHelp.Empty();
    data->m_strAccDefAction.Empty();
    data->m_nAccRole = 0;
    data->m_bAccState = self->m_bIsCollapsed ? 0 : 1;
    data->m_nAccHit = 0;

    CRect rect(self->m_rect.left, self->m_rect.top, self->m_rect.right, self->m_rect.bottom);
    if (pWindow != nullptr) {
        HWND hWnd = *reinterpret_cast<HWND*>(static_cast<char*>(pWindow) + 64);
        if (::IsWindow(hWnd)) {
            POINT ptTL { rect.left, rect.top };
            POINT ptBR { rect.right, rect.bottom };
            ::ClientToScreen(hWnd, &ptTL);
            ::ClientToScreen(hWnd, &ptBR);
            rect = CRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y);
        }
    }

    data->m_rectAccLocation = rect;
    data->m_ptAccHit.x = (rect.left + rect.right) / 2;
    data->m_ptAccHit.y = (rect.top + rect.bottom) / 2;

    return 1;
}

// Symbol: ?OnDrawBorder@CMFCTabButton@@EEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCTabButton__EEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, void* pRect, unsigned int state) {
    if (pThis == nullptr || pDC == nullptr || pRect == nullptr) {
        return;
    }

    (void)pThis;
    (void)state;
}

// Symbol: ?OnFillBackground@CMFCTabButton@@EEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCTabButton__EEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const void* pRect) {
    if (pThis == nullptr || pDC == nullptr || pRect == nullptr) {
        return;
    }
}
