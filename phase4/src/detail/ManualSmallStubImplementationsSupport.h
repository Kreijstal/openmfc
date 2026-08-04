#pragma once
// Shared internals of the former manual_small_stub_implementations.cpp translation unit.
// Definitions live in detail/ManualSmallStubImplementationsSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {} } }
using namespace openmfc::detail::manualsmallstubimplementations;
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

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CMFCBaseToolBarObject : public CObject {};
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct COutlookCustomizeButtonObject : public CObject {};
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CMFCOutlookBarPaneAdapterObject : public CObject {};
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CBitmapButtonState {
    HBITMAP normal = nullptr;
    HBITMAP selected = nullptr;
    HBITMAP disabled = nullptr;
    HBITMAP focus = nullptr;
};
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CMFCToolBarColorButtonObject : public CObject {
public:
    CMFCToolBarColorButtonObject() = default;
};
} } }

// Menu font used by the CChevronOwnerDrawMenu thunks, here and in the class file.
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
HFONT GetChevronOwnerDrawMenuFont();
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CTempHandleObject_ManualSmallStubImplementations : public CObject {
    void* m_handle = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DSizeF_ManualSmallStubImplementations {
    float width;
    float height;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DPointU {
    uint32_t x;
    uint32_t y;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DPointF {
    float x;
    float y;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DSizeU {
    uint32_t width;
    uint32_t height;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DRectU {
    uint32_t left;
    uint32_t top;
    uint32_t right;
    uint32_t bottom;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct D2D1_PIXEL_FORMAT {
    uint32_t format;
    uint32_t alphaMode;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DBitmapState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    CD2DSizeF_ManualSmallStubImplementations m_dpi = CD2DSizeF_ManualSmallStubImplementations{96.0f, 96.0f};
    CD2DSizeU m_pixelSize = CD2DSizeU{0, 0};
    D2D1_PIXEL_FORMAT m_pixelFormat = D2D1_PIXEL_FORMAT{0, 0};
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
typedef uint32_t D2D1_BITMAP_INTERPOLATION_MODE;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
typedef uint32_t D2D1_EXTEND_MODE;
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DBitmapBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    void* m_bitmap = nullptr;
    void* m_bitmapObject = nullptr;
    D2D1_EXTEND_MODE m_extendModeX = 0;
    D2D1_EXTEND_MODE m_extendModeY = 0;
    D2D1_BITMAP_INTERPOLATION_MODE m_interpolationMode = 0;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct D2D_MATRIX_3X2_F {
    float _11;
    float _12;
    float _21;
    float _22;
    float _31;
    float _32;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    float m_opacity = 1.0f;
    D2D_MATRIX_3X2_F m_transform = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DLinearGradientBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    CD2DPointF m_startPoint = {0.0f, 0.0f};
    CD2DPointF m_endPoint = {0.0f, 0.0f};
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DGradientBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
    float m_opacity = 1.0f;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CD2DRadialGradientBrushState {
    void* m_resource = nullptr;
    void* m_parentRenderTarget = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CBaseTransitionState {
    void* m_transition = nullptr;
    int m_addToStoryboardCalls = 0;
    int m_addToStoryboardAtKeyframesCalls = 0;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CAnimationValueState {
    double m_value = 0.0;
    void* m_variable = nullptr;
    std::vector<void*> m_transitions;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CEnumConnPointsState {
    std::vector<void*> m_connectionPoints;
    std::size_t m_position = 0;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CEnumConnectionsState {
    std::vector<CONNECTDATA> m_connections;
    std::size_t m_position = 0;
    bool m_synthetic = false;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CDocObjectServerItemState {
    int m_lastVerb = 0;
    bool m_visible = true;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
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
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CHotKeyCtrlState {
    unsigned short m_virtualKey = 0;
    unsigned short m_modifierFlags = 0;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct COutlookCustomizeButtonState {
    void* m_popupMenu = nullptr;
    bool m_didDraw = false;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CMFCCaptionButtonState {
    unsigned int m_hit = 0;
    int m_lastX = 0;
    int m_lastY = 0;
    unsigned int m_lastImageId = 0;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CMFCPrintPreviewToolBarState {
    bool m_destroyed = false;
    int m_lastToolHit = -1;
    int m_lastX = 0;
    int m_lastY = 0;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct CMFCColorPopupMenuState {
    void* m_tearOffBar = nullptr;
    unsigned int m_commandId = 0;
    void* m_text = nullptr;
    bool m_created = false;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
unsigned long long HashRawBytes(const void* data, std::size_t bytes);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_cd2dBitmapStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CD2DBitmapState> g_cd2dbitmapStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_cd2dBitmapBrushStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CD2DBitmapBrushState> g_cd2dbitmapBrushStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_cd2dBrushStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CD2DBrushState> g_cd2dBrushStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_cd2dLinearGradientBrushStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CD2DLinearGradientBrushState> g_cd2dLinearGradientBrushStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_cd2dGradientBrushStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CD2DGradientBrushState> g_cd2dGradientBrushStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_cd2dRadialGradientBrushStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CD2DRadialGradientBrushState> g_cd2dRadialGradientBrushStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_baseTransitionStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CBaseTransitionState> g_baseTransitionStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_enumConnPointsStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CEnumConnPointsState> g_enumConnPointsStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_enumConnectionsStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CEnumConnectionsState> g_enumConnectionsStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_docObjectServerItemStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CDocObjectServerItemState> g_docObjectServerItemStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_printInfoStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, bool> g_printInfoStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_keyFrameStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CKeyFrameState> g_keyFrameStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_hotKeyCtrlStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CHotKeyCtrlState> g_hotKeyCtrlStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_outlookCustomizeButtonStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, COutlookCustomizeButtonState> g_outlookCustomizeButtonStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_cmfccaptionButtonStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CMFCCaptionButtonState> g_cmfccaptionButtonStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_printPreviewToolBarStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CMFCPrintPreviewToolBarState> g_printPreviewToolBarStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_colorPopupMenuStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CMFCColorPopupMenuState> g_colorPopupMenuStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_animationValueStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CAnimationValueState> g_animationValueStates;
} } }

extern "C" void MS_ABI impl__CommonInit_CD2DBitmap__IEAAXXZ(void* pThis);

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ApplyBitmapBrushDefaults(CD2DBitmapBrushState& state);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CD2DSizeU CalcSizeFromRect(const CD2DRectU* pRect);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ApplyBitmapDefaults(CD2DBitmapState& state);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CD2DBitmapState& EnsureCD2DBitmapState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool TryLoadCD2DBitmapState(void* pThis, CD2DBitmapState& outState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CD2DBitmapBrushState& EnsureCD2DBitmapBrushState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool TryLoadCD2DBitmapBrushState(void* pThis, CD2DBitmapBrushState& outState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CKeyFrameState& EnsureCKeyFrameState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool TryLoadCKeyFrameState(void* pThis, CKeyFrameState& outState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CHotKeyCtrlState& EnsureCHotKeyCtrlState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool TryLoadCHotKeyCtrlState(void* pThis, CHotKeyCtrlState& outState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ClearCHotKeyCtrlState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
COutlookCustomizeButtonState& EnsureCOutlookCustomizeButtonState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ClearCOutlookCustomizeButtonState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CMFCCaptionButtonState& EnsureCMFCCaptionButtonState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CMFCPrintPreviewToolBarState& EnsureCMFCPrintPreviewToolBarState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ClearCMFCPrintPreviewToolBarState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CMFCColorPopupMenuState& EnsureCMFCColorPopupMenuState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ClearCMFCColorPopupMenuState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ApplyCD2DBrushDefaults(CD2DBrushState& state);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CD2DBrushState& EnsureCD2DBrushState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool TryLoadCD2DBrushState(void* pThis, CD2DBrushState& outState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ApplyTransformFromBrushProperties(CD2DBrushState& state, const float* pBrushProps);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ApplyLinearGradientBrushDefaults(CD2DLinearGradientBrushState& state);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CD2DLinearGradientBrushState& EnsureCD2DLinearGradientBrushState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool TryLoadCD2DLinearGradientBrushState(void* pThis, CD2DLinearGradientBrushState& outState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ApplyLinearGradientBrushProperties(
    CD2DLinearGradientBrushState& state, const void* pProperties);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ApplyCD2DRadialGradientBrushDefaults(CD2DRadialGradientBrushState& state);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CD2DRadialGradientBrushState& EnsureCD2DRadialGradientBrushState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool TryLoadCD2DRadialGradientBrushState(void* pThis, CD2DRadialGradientBrushState& outState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CD2DGradientBrushState& EnsureCD2DGradientBrushState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CBaseTransitionState& EnsureCBaseTransitionState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CEnumConnPointsState& EnsureCEnumConnPointsState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CEnumConnectionsState& EnsureCEnumConnectionsState(void* pThis, bool synthetic = false);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ClearCEnumConnPointsState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ClearCEnumConnectionsState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
CDocObjectServerItemState& EnsureCDocObjectServerItemState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ClearCDocObjectServerItemState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void TrackCPrintInfo(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void UntrackCPrintInfo(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_bitmapButtonStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, CBitmapButtonState> g_bitmapButtonStates;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_bitmapRenderTargetMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, void*> g_bitmapRenderTargetBitmaps;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_handleMapMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, std::unordered_map<void*, CTempHandleObject_ManualSmallStubImplementations*>> g_tempHandleMap;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_innerUnknownMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, unsigned long> g_innerUnknownRefCounts;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_coleUIlinkInfoMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, std::pair<COleDocument*, COleClientItem*>> g_coleUILinkInfoTargets;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_reflectorControlMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, COleControl*> g_reflectorControls_ManualSmallStubImplementations;
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
HBITMAP LoadBitmapFromResource(const wchar_t* resource);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
HBITMAP PickBitmapForBitmapButton(const CBitmapButtonState& state, unsigned int itemState);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool LoadBitmapButtonStateImages(
    void* pThis, const wchar_t* normal, const wchar_t* selected,
    const wchar_t* disabled, const wchar_t* focus);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void SetCOleUILinkInfoTarget(void* pThis, COleDocument* document, COleClientItem* item);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
bool GetCOleUILinkInfoTarget(void* pThis, COleDocument*& pDocument, COleClientItem*& pItem);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
COleClientItem* FindItemByLinkIndex(const COleDocument* pDocument, unsigned long link);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
HRESULT UpdateSingleOleLinkItem(COleClientItem* pItem);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void DrawBitmapCentered(HDC hdc, HBITMAP hBitmap, const RECT& rect);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void ResetCDBVariantMemory(void* pThis);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct LocalAFX_MODULE_STATE {
    HINSTANCE m_hCurrentInstanceHandle{};
    HINSTANCE m_hCurrentResourceHandle{};
    void* m_pCurrentWinApp{};
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct AFX_MODULE_THREAD_STATE_ManualSmallStubImplementations {
    int m_nTempMapLock{};
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
struct MaintainStateRecord {
    LocalAFX_MODULE_STATE* prevModuleState = nullptr;
    bool trackedThreadLock = false;
    int prevThreadLock = 0;
};
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::mutex g_maintainStateMutex;
} } }
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern std::unordered_map<void*, MaintainStateRecord> g_maintainStateMap;
} } }

extern "C" void* MS_ABI impl__AfxSetModuleState__YAPEAVAFX_MODULE_STATE__PEAV1__Z(void*);
extern "C" LocalAFX_MODULE_STATE* MS_ABI impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
extern "C" AFX_MODULE_THREAD_STATE_ManualSmallStubImplementations* MS_ABI impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void* MS_ABI enter_maintain_state(void* pThis, void* pState, bool useThreadLock);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
void MS_ABI leave_maintain_state(void* pThis);
} } }






















































































































































































































































































































































extern "C" void MS_ABI impl__Destroy_CD2DGradientBrush__MEAAXXZ(void* pThis);




















































namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern "C" CObject* MS_ABI impl__CreateObject_CMFCToolBarColorButton__SAPEAVCObject__XZ();
} } }







































namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern "C" int MS_ABI impl__Create_CHotKeyCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis, unsigned long nStyles, const void* pRect, void* pParentWnd, unsigned int nID);
} } }






















// Shared helper for both `AddBitmap` overloads.
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
int AddBitmapViaHandle(CWnd* pWnd, int nNumBits, HBITMAP hBitmap);
} } }

























































































































namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern "C" void MS_ABI impl__DrawItem_CChevronOwnerDrawMenu__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, void* pDis);
} } }


// Layout-faithful CMFCCaptionMenuButton view used by non-constructor exports.
namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
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
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
HWND WndHandleForObject(void* pWnd);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
HDC DCHandleForCDC(void* pDC);
} } }

namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
RECT GetCaptionMenuRect(void* pThis);
} } }






namespace openmfc { namespace detail { namespace manualsmallstubimplementations {
extern "C" int MS_ABI impl__SetActiveTab_CMFCPropertySheetTabCtrl__EEAAHH_Z(
    void* pThis, int nTab);
} } }





