#pragma once
#include "detail/CWndAccessibilitySupport.h"
#include <oleacc.h>
#include <oleidl.h>

// ATL's proxy protocol, kept independent of the installed MFC headers.
class IAccessibleServer;
class IAccessibleProxy : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE SetServer(IAccessible*, IAccessibleServer*) = 0;
};
class IAccessibleServer : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE SetProxy(IAccessibleProxy*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHWND(HWND*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetEnumVariant(IEnumVARIANT**) = 0;
};

namespace openmfc { namespace detail { namespace accessibility {
inline constexpr IID proxyIID = {0x7a7d9dcf, 0xb7a1, 0x4019,
    {0x90,0x31,0x25,0x82,0x68,0x84,0x69,0x80}};
inline constexpr IID accessibleIID = {0x618736e0,0x3c3d,0x11cf,
    {0x81,0x0c,0x00,0xaa,0x00,0x38,0x9b,0x71}};

// MFC embeds these interfaces with AddRef/Release/QueryInterface first,
// followed by reversed IDispatch methods. They must be adapted before they
// can be handed to COM as a standard IAccessible or IAccessibleServer.
class WindowAccessible {
public:
    virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
    virtual ULONG STDMETHODCALLTYPE Release() = 0;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) = 0;
    virtual HRESULT STDMETHODCALLTYPE Invoke(long dispIdMember, const GUID* riid, unsigned long lcid, unsigned short wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(const GUID* riid, wchar_t** rgszNames, unsigned int cNames, unsigned long lcid, long* rgDispId) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(unsigned int* pctinfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int iTInfo, unsigned long lcid, void** ppTInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accParent(void** ppdispParent) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accChildCount(long* pcountChildren) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accChild(VARIANT varChild, void** ppdispChild) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accName(VARIANT varChild, wchar_t** pszName) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accValue(VARIANT varChild, wchar_t** pszValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accDescription(VARIANT varChild, wchar_t** pszDescription) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accRole(VARIANT varChild, VARIANT* pvarRole) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accState(VARIANT varChild, VARIANT* pvarState) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accHelp(VARIANT varChild, wchar_t** pszHelp) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accHelpTopic(wchar_t** pszHelpFile, VARIANT varChild, long* pidTopic) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accKeyboardShortcut(VARIANT varChild, wchar_t** pszKeyboardShortcut) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accFocus(VARIANT* pvarChild) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accSelection(VARIANT* pvarChildren) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_accDefaultAction(VARIANT varChild, wchar_t** pszDefaultAction) = 0;
    virtual HRESULT STDMETHODCALLTYPE accSelect(long flagsSelect, VARIANT varChild) = 0;
    virtual HRESULT STDMETHODCALLTYPE accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild) = 0;
    virtual HRESULT STDMETHODCALLTYPE accNavigate(long navDir, VARIANT varStart, VARIANT* pvarEndUpAt) = 0;
    virtual HRESULT STDMETHODCALLTYPE accHitTest(long xLeft, long yTop, VARIANT* pvarChild) = 0;
    virtual HRESULT STDMETHODCALLTYPE accDoDefaultAction(VARIANT varChild) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_accName(VARIANT varChild, wchar_t* szName) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_accValue(VARIANT varChild, wchar_t* szValue) = 0;
};
class WindowAccessibleServer {
public:
    virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
    virtual ULONG STDMETHODCALLTYPE Release() = 0;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetProxy(void*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHWND(HWND*) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetEnumVariant(void**) = 0;
};
}}}
