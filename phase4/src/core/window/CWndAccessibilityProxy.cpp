// Adapt the MFC embedded interfaces to the standard COM accessibility ABI.
#include "detail/CWndAccessibilityProtocol.h"
#include <atomic>
#include <new>

using namespace openmfc::detail::accessibility;
namespace {
class AccessibleProxy final : public IAccessible, public IAccessibleProxy, public IOleWindow {
    std::atomic<ULONG> references{1};
    WindowAccessible* accessible = nullptr;
    WindowAccessibleServer* server = nullptr;
public:
    ~AccessibleProxy() {
        // The window's proxy pointer is borrowed. Clear it before freeing this
        // object, unless window cleanup has already disconnected the server.
        if (server) server->SetProxy(nullptr);
    }
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** out) override {
        if (!out) return E_POINTER;
        *out = nullptr;
        if (iid == IID_IUnknown || iid == proxyIID)
            *out = static_cast<IAccessibleProxy*>(this);
        else if (iid == accessibleIID || iid == IID_IDispatch)
            *out = static_cast<IAccessible*>(this);
        else if (iid == IID_IOleWindow)
            *out = static_cast<IOleWindow*>(this);
        else return E_NOINTERFACE;
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++references; }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG left = --references;
        if (!left) delete this;
        return left;
    }
    HRESULT STDMETHODCALLTYPE SetServer(IAccessible* acc, IAccessibleServer* srv) override {
        accessible = reinterpret_cast<WindowAccessible*>(acc);
        server = reinterpret_cast<WindowAccessibleServer*>(srv);
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetWindow(HWND* out) override {
        if (!out) return E_POINTER;
        *out = nullptr;
        return server ? server->GetHWND(out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* out) override {
        return accessible ? accessible->GetTypeInfoCount(out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT index, LCID lcid, ITypeInfo** out) override {
        return accessible ? accessible->GetTypeInfo(index, lcid, reinterpret_cast<void**>(out)) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID iid, LPOLESTR* names, UINT count, LCID lcid, DISPID* ids) override {
        if (count && (!names || !ids)) return E_POINTER;
        return accessible ? accessible->GetIDsOfNames(&iid, names, count, lcid, ids) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE Invoke(DISPID id, REFIID iid, LCID lcid, WORD flags, DISPPARAMS* args,
        VARIANT* result, EXCEPINFO* exception, UINT* error) override {
        return accessible ? accessible->Invoke(id, &iid, lcid, flags, args, result, exception, error) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE get_accParent(IDispatch** out) override {
        return accessible ? accessible->get_accParent(reinterpret_cast<void**>(out)) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE get_accChildCount(long* out) override {
        return accessible ? accessible->get_accChildCount(out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE get_accChild(VARIANT child, IDispatch** out) override {
        return accessible ? accessible->get_accChild(child, reinterpret_cast<void**>(out)) : RPC_E_DISCONNECTED;
    }
#define ACCESSIBLE_STRING(method) \
    HRESULT STDMETHODCALLTYPE method(VARIANT child, BSTR* out) override { \
        return accessible ? accessible->method(child, out) : RPC_E_DISCONNECTED; \
    }
    ACCESSIBLE_STRING(get_accName)
    ACCESSIBLE_STRING(get_accValue)
    ACCESSIBLE_STRING(get_accDescription)
    ACCESSIBLE_STRING(get_accHelp)
    ACCESSIBLE_STRING(get_accKeyboardShortcut)
    ACCESSIBLE_STRING(get_accDefaultAction)
#undef ACCESSIBLE_STRING
    HRESULT STDMETHODCALLTYPE get_accRole(VARIANT child, VARIANT* out) override {
        return accessible ? accessible->get_accRole(child, out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE get_accState(VARIANT child, VARIANT* out) override {
        return accessible ? accessible->get_accState(child, out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE get_accHelpTopic(BSTR* file, VARIANT child, long* topic) override {
        return accessible ? accessible->get_accHelpTopic(file, child, topic) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE get_accFocus(VARIANT* out) override {
        return accessible ? accessible->get_accFocus(out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE get_accSelection(VARIANT* out) override {
        return accessible ? accessible->get_accSelection(out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE accSelect(long flags, VARIANT child) override {
        return accessible ? accessible->accSelect(flags, child) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE accLocation(long* x, long* y, long* w, long* h, VARIANT child) override {
        return accessible ? accessible->accLocation(x, y, w, h, child) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE accNavigate(long direction, VARIANT start, VARIANT* out) override {
        return accessible ? accessible->accNavigate(direction, start, out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE accHitTest(long x, long y, VARIANT* out) override {
        return accessible ? accessible->accHitTest(x, y, out) : RPC_E_DISCONNECTED;
    }
    HRESULT STDMETHODCALLTYPE accDoDefaultAction(VARIANT child) override {
        return accessible ? accessible->accDoDefaultAction(child) : RPC_E_DISCONNECTED;
    }
    // MFC's public proxy rejects these obsolete setters even while connected.
    HRESULT STDMETHODCALLTYPE put_accName(VARIANT, BSTR) override { return accessible ? E_NOTIMPL : RPC_E_DISCONNECTED; }
    HRESULT STDMETHODCALLTYPE put_accValue(VARIANT, BSTR) override { return accessible ? E_NOTIMPL : RPC_E_DISCONNECTED; }
};
}

void CWnd::ReleaseAccessibility() {
    IAccessible* object = m_pStdObject;
    IAccessibleProxy* proxy = m_pProxy;
    m_pStdObject = nullptr;
    m_pProxy = nullptr;
    // Detach before calling COM: clients may outlive the window, and callbacks
    // must not see stale window members. SetProxy does not transfer a reference.
    if (proxy) proxy->SetServer(nullptr, nullptr);
    if (object) object->Release();
}

// Symbol: ?SetProxy@CWnd@@UEAAJPEAUIAccessibleProxy@@@Z
extern "C" long MS_ABI impl__SetProxy_CWnd__UEAAJPEAUIAccessibleProxy___Z(CWnd* window, void* proxy) {
    if (!window) return E_POINTER;
    window->m_pProxy = static_cast<IAccessibleProxy*>(proxy);
    return S_OK;
}

// Symbol: ?CreateAccessibleProxy@CWnd@@UEAAJ_K_JPEA_J@Z
extern "C" long MS_ABI impl__CreateAccessibleProxy_CWnd__UEAAJ_K_JPEA_J_Z(
    CWnd* window, unsigned __int64 wParam, __int64 objectID, __int64* result) {
    if (!window || !result) return E_POINTER;
    *result = 0;
    // Windows can zero-extend the 32-bit object ID into LPARAM on x64.
    if (static_cast<LONG>(objectID) != OBJID_CLIENT) return E_FAIL;
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    IAccessibleProxy* proxy = window->m_pProxy;
    if (proxy) proxy->AddRef();
    else {
        auto* created = new(std::nothrow) AccessibleProxy;
        if (!created) return E_OUTOFMEMORY;
        proxy = static_cast<IAccessibleProxy*>(created);
        proxy->SetServer(reinterpret_cast<IAccessible*>(&window->m_xAccessible),
            reinterpret_cast<IAccessibleServer*>(&window->m_xAccessibleServer));
        window->m_pProxy = proxy;
    }
    LRESULT token = LresultFromObject(accessibleIID, static_cast<WPARAM>(wParam), proxy);
    *result = static_cast<__int64>(token);
    // The marshaled object or client owns the remaining references. The
    // window's borrowed pointer is cleared by AccessibleProxy's destructor.
    proxy->Release();
    return FAILED(static_cast<HRESULT>(token)) ? static_cast<HRESULT>(token) : S_OK;
}

LONGLONG CWnd::OnGetObject(ULONGLONG wParam, LONGLONG objectID) {
    if (!m_hWnd) return 0;
    if (m_bEnableActiveAccessibility) {
        __int64 result = 0;
        if (SUCCEEDED(impl__CreateAccessibleProxy_CWnd__UEAAJ_K_JPEA_J_Z(this, wParam, objectID, &result)))
            return result;
    }
    return static_cast<LONGLONG>(::DefWindowProcW(m_hWnd, WM_GETOBJECT,
        static_cast<WPARAM>(wParam), static_cast<LPARAM>(objectID)));
}

// Symbol: ?OnGetObject@CWnd@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetObject_CWnd__IEAA_J_K_J_Z(CWnd* window, unsigned __int64 wParam, __int64 objectID) {
    return window ? window->OnGetObject(wParam, objectID) : 0;
}
