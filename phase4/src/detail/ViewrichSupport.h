#pragma once
// Shared internals of the former viewrich.cpp translation unit.
// Definitions live in detail/ViewrichSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace viewrich {} } }
using namespace openmfc::detail::viewrich;
// CRichEditView & CRichEditCtrl Implementation
// Rich Edit 2.0 view with formatting and printing support
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <richedit.h>
#include <commctrl.h>
#include <cstring>
#include <cwchar>
#include <cstdio>
#include <mshtml.h>
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CCmdUI export helpers (defined in olecore.cpp).
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);
extern "C" void MS_ABI impl__SetCheck_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck);

// MinGW compat: rich edit 2.0 class name may be missing
#ifndef MSFTEDIT_CLASS
#define MSFTEDIT_CLASS L"RICHEDIT50W"
#endif

#ifndef RICHEDIT_CLASS
#define RICHEDIT_CLASS L"RichEdit20W"
#endif

// Rich edit 2.0 class (prefer 4.1 if available)
static const wchar_t* g_pszRichEditClass = RICHEDIT_CLASS;

namespace openmfc { namespace detail { namespace viewrich {
wchar_t* CoTaskMemDuplicateW(const wchar_t* pszText);
} } }

//=============================================================================
// CRichEditCtrl
//=============================================================================

















































//=============================================================================
// CRichEditView
//=============================================================================





































//=============================================================================
// CHtmlView - WebBrowser-based HTML View
//=============================================================================

// Minimal IOleClientSite for hosting the WebBrowser control
class CHtmlViewClientSite : public IOleClientSite, public IOleInPlaceSite {
public:
    CHtmlViewClientSite(HWND hWnd) : m_hWnd(hWnd), m_refCount(1), m_pBrowser(nullptr) {}
    virtual ~CHtmlViewClientSite() {}

    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override {
        if (riid == IID_IUnknown || riid == IID_IOleClientSite) {
            *ppv = static_cast<IOleClientSite*>(this);
        } else if (riid == IID_IOleInPlaceSite) {
            *ppv = static_cast<IOleInPlaceSite*>(this);
        } else {
            *ppv = nullptr;
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }
    STDMETHOD_(ULONG, AddRef)() override { return ++m_refCount; }
    STDMETHOD_(ULONG, Release)() override {
        if (--m_refCount == 0) { delete this; return 0; }
        return m_refCount;
    }

    // IOleClientSite
    STDMETHOD(SaveObject)() override { return S_OK; }
    STDMETHOD(GetMoniker)(DWORD, DWORD, IMoniker** ppmk) override {
        if (ppmk) {
            *ppmk = nullptr;
        }
        return S_FALSE;
    }
    STDMETHOD(GetContainer)(IOleContainer** ppContainer) override { *ppContainer = nullptr; return E_NOINTERFACE; }
    STDMETHOD(ShowObject)() override { return S_OK; }
    STDMETHOD(OnShowWindow)(BOOL) override { return S_OK; }
    STDMETHOD(RequestNewObjectLayout)() override { return S_OK; }

    // IOleInPlaceSite
    STDMETHOD(GetWindow)(HWND* phWnd) override { *phWnd = m_hWnd; return S_OK; }
    STDMETHOD(ContextSensitiveHelp)(BOOL) override { return S_OK; }
    STDMETHOD(CanInPlaceActivate)() override { return S_OK; }
    STDMETHOD(OnInPlaceActivate)() override { return S_OK; }
    STDMETHOD(OnUIActivate)() override { return S_OK; }
    STDMETHOD(GetWindowContext)(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc,
                                 LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo) override {
        *ppFrame = nullptr;
        *ppDoc = nullptr;
        ::GetClientRect(m_hWnd, lprcPosRect);
        ::GetClientRect(m_hWnd, lprcClipRect);
        if (lpFrameInfo) {
            lpFrameInfo->cb = sizeof(OLEINPLACEFRAMEINFO);
            lpFrameInfo->fMDIApp = FALSE;
            lpFrameInfo->hwndFrame = ::GetParent(m_hWnd);
            lpFrameInfo->haccel = nullptr;
            lpFrameInfo->cAccelEntries = 0;
        }
        return S_OK;
    }
    STDMETHOD(Scroll)(SIZE) override { return S_OK; }
    STDMETHOD(OnUIDeactivate)(BOOL) override { return S_OK; }
    STDMETHOD(OnInPlaceDeactivate)() override { return S_OK; }
    STDMETHOD(DiscardUndoState)() override { return S_OK; }
    STDMETHOD(DeactivateAndUndo)() override { return S_OK; }
    STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect) override {
        if (m_pBrowser) {
            m_pBrowser->put_Left(lprcPosRect->left);
            m_pBrowser->put_Top(lprcPosRect->top);
            m_pBrowser->put_Width(lprcPosRect->right - lprcPosRect->left);
            m_pBrowser->put_Height(lprcPosRect->bottom - lprcPosRect->top);
        }
        return S_OK;
    }

    void SetBrowser(IWebBrowser2* pBrowser) { m_pBrowser = pBrowser; }

private:
    HWND m_hWnd;
    ULONG m_refCount;
    IWebBrowser2* m_pBrowser;
};






















// Event stubs (override in derived class)
// Note: OnBeforeNavigate2/OnNavigateComplete2/OnDocumentComplete take decoded string params

// IWebBrowser2 property accessors


//=============================================================================
// CDHtmlDialog - DHTML-based Dialog
//=============================================================================













// Legacy overload


















// Navigation event stubs

// Internal helpers

// IDocHostUIHandler stubs




//=============================================================================
// CHtmlView + CDHtmlDialog extern "C" MS_ABI wrappers
// These allow MSVC-mangled exports while using MinGW C++ class implementations.
//=============================================================================

namespace openmfc { namespace detail { namespace viewrich {
struct CDHtmlDialogAccess : CDHtmlDialog {
    using CDHtmlDialog::FindSinkForObject;
    using CDHtmlDialog::OnDDXError;
    using CDHtmlDialog::OnDestroy;
    using CDHtmlDialog::Select_FindString;
    using CDHtmlDialog::SetFocusToElement;
};
} } }

namespace openmfc { namespace detail { namespace viewrich {
struct DHtmlDataExchange {
    int m_bSaveAndValidate;
};
} } }

namespace openmfc { namespace detail { namespace viewrich {
void DDXDHtmlText(CDHtmlDialog* pThis, const wchar_t* lpszId, CString* pValue, int bSaveAndValidate);
} } }

namespace openmfc { namespace detail { namespace viewrich {
int DHtmlSaveAndValidate(void* pDX);
} } }

// ---------------------------------------------------------------------------
// CHtmlView wrappers
// ---------------------------------------------------------------------------





















































// ---------------------------------------------------------------------------
// CDHtmlDialog wrappers
// ---------------------------------------------------------------------------






























































//=============================================================================
// CRichEditView OnUpdate* command-UI handlers
//=============================================================================


















//=============================================================================
// CHtmlView OnUpdate* command-UI handlers
//=============================================================================




//=============================================================================
// CPreviewView / CPreviewViewEx OnUpdate* handlers (no header class yet)
//=============================================================================






