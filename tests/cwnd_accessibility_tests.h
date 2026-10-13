#pragma once
#include "detail/CWndAccessibilitySupport.h"
#include <oleacc.h>

namespace accessibility_tests {
struct AccessibleProbe : IAccessible {
    int calls = 0, releases = 0;
    ULONG STDMETHODCALLTYPE AddRef() override { return 2; }
    ULONG STDMETHODCALLTYPE Release() override { ++releases; return 1; }
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) override { return E_NOINTERFACE; }
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT, LCID, ITypeInfo**) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*) override { return E_NOTIMPL; }
    HRESULT child(VARIANT value) {
        ++calls;
        return value.vt == VT_I4 && value.lVal == 17 ? S_FALSE : E_INVALIDARG;
    }
    HRESULT STDMETHODCALLTYPE get_accParent(IDispatch** out) override { ++calls; *out = this; return S_FALSE; }
    HRESULT STDMETHODCALLTYPE get_accChildCount(long* out) override { ++calls; *out = 17; return S_FALSE; }
    HRESULT STDMETHODCALLTYPE get_accChild(VARIANT v, IDispatch** out) override { *out = this; return child(v); }
#define PROBE_STRING(name) HRESULT STDMETHODCALLTYPE name(VARIANT v, BSTR* out) override { *out = SysAllocString(L"probe"); return child(v); }
    PROBE_STRING(get_accName)
    PROBE_STRING(get_accValue)
    PROBE_STRING(get_accDescription)
    PROBE_STRING(get_accHelp)
    PROBE_STRING(get_accKeyboardShortcut)
    PROBE_STRING(get_accDefaultAction)
#undef PROBE_STRING
#define PROBE_VARIANT(name) HRESULT STDMETHODCALLTYPE name(VARIANT v, VARIANT* out) override { out->vt = VT_I4; out->lVal = 17; return child(v); }
    PROBE_VARIANT(get_accRole)
    PROBE_VARIANT(get_accState)
#undef PROBE_VARIANT
    HRESULT STDMETHODCALLTYPE get_accHelpTopic(BSTR* out, VARIANT v, long* topic) override { *out = SysAllocString(L"probe"); *topic = 17; return child(v); }
    HRESULT STDMETHODCALLTYPE get_accFocus(VARIANT* out) override { ++calls; out->vt = VT_I4; out->lVal = 17; return S_FALSE; }
    HRESULT STDMETHODCALLTYPE get_accSelection(VARIANT* out) override { return get_accFocus(out); }
    HRESULT STDMETHODCALLTYPE accSelect(long flags, VARIANT v) override { return flags == 9 ? child(v) : E_INVALIDARG; }
    HRESULT STDMETHODCALLTYPE accLocation(long* x, long* y, long* w, long* h, VARIANT v) override { *x=1; *y=2; *w=3; *h=4; return child(v); }
    HRESULT STDMETHODCALLTYPE accNavigate(long direction, VARIANT v, VARIANT* out) override { out->vt=VT_I4; out->lVal=17; return direction == 9 ? child(v) : E_INVALIDARG; }
    HRESULT STDMETHODCALLTYPE accHitTest(long x, long y, VARIANT* out) override { ++calls; out->vt=VT_I4; out->lVal=x+y; return E_FAIL; }
    HRESULT STDMETHODCALLTYPE accDoDefaultAction(VARIANT v) override { return child(v); }
    HRESULT STDMETHODCALLTYPE put_accName(VARIANT v, BSTR value) override { return value && value[0] == L'p' ? child(v) : E_INVALIDARG; }
    HRESULT STDMETHODCALLTYPE put_accValue(VARIANT v, BSTR value) override { return put_accName(v, value); }
};

static void forwarding() {
    AccessibleProbe probe;
    CWnd window;
    window.m_pStdObject = &probe;
    VARIANT child{}; child.vt=VT_I4; child.lVal=17;
    VARIANT out{};
    long count=0;
    IDispatch* dispatch=nullptr;
    check(impl__EnsureStdObj_CWnd__UEAAJXZ(&window) == S_OK && window.m_pStdObject == &probe,
        "accessibility cache preserves an existing COM object");
    check(impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(&window, &count)==S_FALSE && count==17,
        "accessibility child count writes output and propagates S_FALSE");
    check(impl__get_accParent_CWnd__UEAAJPEAPEAUIDispatch___Z(&window, &dispatch)==S_FALSE && dispatch==&probe &&
        impl__get_accChild_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(&window, &child, &dispatch)==S_FALSE,
        "accessibility parent and child forward COM outputs and the by-value VARIANT ABI");
    using StringGetter=long (*)(void*,void*,void*);
    StringGetter strings[]={impl__get_accName_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z,
        impl__get_accValue_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z,
        impl__get_accDescription_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z,
        impl__get_accHelp_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z,
        impl__get_accKeyboardShortcut_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z,
        impl__get_accDefaultAction_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z};
    for (auto getter:strings) {
        BSTR text=nullptr;
        HRESULT hr=getter(&window,&child,&text);
        check(hr==S_FALSE && text && wcscmp(text,L"probe")==0,
            "accessibility string getter returns the COM BSTR");
        SysFreeString(text);
    }
    BSTR help=nullptr; long topic=0;
    check(impl__get_accHelpTopic_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(&window,&help,&child,&topic)==S_FALSE && topic==17,
        "accessibility help topic preserves argument order");
    SysFreeString(help);
    check(impl__get_accRole_CWnd__UEAAJUtagVARIANT__PEAU2__Z(&window,&child,&out)==S_FALSE && out.lVal==17 &&
        impl__get_accState_CWnd__UEAAJUtagVARIANT__PEAU2__Z(&window,&child,&out)==S_FALSE &&
        impl__get_accFocus_CWnd__UEAAJPEAUtagVARIANT___Z(&window,&out)==S_FALSE &&
        impl__get_accSelection_CWnd__UEAAJPEAUtagVARIANT___Z(&window,&out)==S_FALSE,
        "role, state, focus and selection return COM variants");
    long x=0,y=0,w=0,h=0;
    check(impl__accLocation_CWnd__UEAAJPEAJ000UtagVARIANT___Z(&window,&x,&y,&w,&h,&child)==S_FALSE && x==1 && y==2 && w==3 && h==4,
        "accessibility location forwards all five arguments");
    check(impl__accSelect_CWnd__UEAAJJUtagVARIANT___Z(&window,9,&child)==S_FALSE &&
        impl__accNavigate_CWnd__UEAAJJUtagVARIANT__PEAU2__Z(&window,9,&child,&out)==S_FALSE &&
        impl__accDoDefaultAction_CWnd__UEAAJUtagVARIANT___Z(&window,&child)==S_FALSE,
        "accessibility actions forward flags, direction and child");
    check(impl__accHitTest_CWnd__UEAAJJJPEAUtagVARIANT___Z(&window,4,5,&out)==E_FAIL && out.lVal==9,
        "accessibility hit test propagates failures and output changes");
    wchar_t value[]=L"probe";
    check(impl__put_accName_CWnd__UEAAJUtagVARIANT__PEA_W_Z(&window,&child,value)==S_FALSE &&
        impl__put_accValue_CWnd__UEAAJUtagVARIANT__PEA_W_Z(&window,&child,value)==S_FALSE,
        "accessibility setters forward values");
    check(probe.calls==21 && impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(&window,nullptr)==E_POINTER,
        "all 21 default accessibility methods call their COM implementation");
    window.ReleaseAccessibility(); window.ReleaseAccessibility();
    check(!window.m_pStdObject && probe.releases==1, "accessibility cleanup detaches and releases once");
}

static void nativeWindow() {
    CWnd window;
    HWND hwnd=CreateWindowExW(0,L"STATIC",L"OpenMFC accessibility",WS_OVERLAPPEDWINDOW,
        10,20,120,100,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    check(hwnd!=nullptr, "create native accessibility test window");
    if (!hwnd) return;
    window.m_hWnd=hwnd;
    window.m_nFlags=0x1234;
    window.m_pfnSuper=DefWindowProcW;
    HRESULT hr=impl__EnsureStdObj_CWnd__UEAAJXZ(&window);
    check(SUCCEEDED(hr) && window.m_pStdObject, "EnsureStdObj creates the native Windows accessibility object");
    if (SUCCEEDED(hr) && window.m_pStdObject) {
        IAccessible* cached=window.m_pStdObject;
        check(impl__EnsureStdObj_CWnd__UEAAJXZ(&window)==S_OK && cached==window.m_pStdObject &&
            window.m_nFlags==0x1234 && window.m_pfnSuper==DefWindowProcW,
            "accessibility caching preserves native window flags and superclass procedure");
        VARIANT self{}; self.vt=VT_I4; self.lVal=CHILDID_SELF;
        VARIANT role{};
        BSTR name=nullptr;
        check(impl__get_accName_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(&window,&self,&name)==S_OK &&
            name && wcscmp(name,L"OpenMFC accessibility")==0,
            "native accessibility returns the actual window name");
        SysFreeString(name);
        // Standard objects specialize the role by window class. Windows uses
        // a STATIC-specific role where Wine currently uses ROLE_SYSTEM_CLIENT.
        constexpr IID accessibleIID = {0x618736e0, 0x3c3d, 0x11cf,
            {0x81, 0x0c, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
        IAccessible* reference = nullptr;
        HRESULT referenceHR = CreateStdAccessibleObject(hwnd, OBJID_CLIENT,
            accessibleIID, reinterpret_cast<void**>(&reference));
        VARIANT expectedRole{};
        if (SUCCEEDED(referenceHR) && reference)
            referenceHR = reference->get_accRole(self, &expectedRole);
        HRESULT roleHR = impl__get_accRole_CWnd__UEAAJUtagVARIANT__PEAU2__Z(&window,&self,&role);
        check(reference && referenceHR == S_OK && roleHR == referenceHR &&
            role.vt == VT_I4 && expectedRole.vt == VT_I4 && role.lVal == expectedRole.lVal,
            "native accessibility role matches the independently created standard object");
        VariantClear(&expectedRole); VariantClear(&role);
        if (reference) reference->Release();
        long x=0,y=0,w=0,h=0; RECT rect{}; GetClientRect(hwnd,&rect);
        check(impl__accLocation_CWnd__UEAAJPEAJ000UtagVARIANT___Z(&window,&x,&y,&w,&h,&self)==S_OK &&
            w==rect.right && h==rect.bottom,
            "native accessibility returns the actual client extent");
    }
    check(window.DestroyWindow() && !window.m_hWnd && !window.m_pStdObject,
        "destroying the window releases its cached accessibility object");
    check(FAILED(impl__EnsureStdObj_CWnd__UEAAJXZ(&window)) && !window.m_pStdObject,
        "invalid windows cannot report accessibility success");
    long count = -1;
    check(FAILED(impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(&window, &count)) && count == -1,
        "accessibility getters propagate creation failure");
}
}
static void windowAccessibility() { accessibility_tests::forwarding(); accessibility_tests::nativeWindow(); }
