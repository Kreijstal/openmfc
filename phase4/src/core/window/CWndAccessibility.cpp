// Default CWnd accessibility backed by the standard Windows client object.
#include "detail/CWndAccessibilitySupport.h"
#include <oleacc.h>
#include <cstddef>

static_assert(sizeof(CWnd) == 232, "retail x64 CWnd size");
static_assert(offsetof(CWnd, m_pStdObject) == 0x50, "standard accessibility object");
static_assert(offsetof(CWnd, m_pProxy) == 0x58, "accessibility proxy");
static_assert(offsetof(CWnd, m_nFlags) == 0xa8, "window flags");
static_assert(offsetof(CWnd, m_pfnSuper) == 0xb0, "superclass procedure");
static_assert(offsetof(CWnd, m_pRenderTarget) == 0xe0, "Direct2D target");

namespace {
constexpr IID kAccessible = {0x618736e0, 0x3c3d, 0x11cf,
    {0x81, 0x0c, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};
}

void CWnd::ReleaseAccessibility() {
    // Detach before Release: releasing a COM object can reenter the window.
    IAccessible* object = m_pStdObject;
    m_pStdObject = nullptr;
    // SetProxy installs a borrowed pointer; it does not transfer ownership.
    m_pProxy = nullptr;
    if (object) object->Release();
}

// Symbol: ?EnsureStdObj@CWnd@@UEAAJXZ
extern "C" long MS_ABI impl__EnsureStdObj_CWnd__UEAAJXZ(CWnd* pThis) {
    if (!pThis) return E_POINTER;
    if (pThis->m_pStdObject) return S_OK;
    return CreateStdAccessibleObject(pThis->m_hWnd, OBJID_CLIENT,
        kAccessible, reinterpret_cast<void**>(&pThis->m_pStdObject));
}

// Symbol: ?accNavigate@CWnd@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CWnd__UEAAJJUtagVARIANT__PEAU2__Z(void* pThis, long p0, void* p1, void* p2) {
    if (!pThis || !(p1 && p2)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->accNavigate(p0, *static_cast<VARIANT*>(p1), static_cast<VARIANT*>(p2));
}

// Symbol: ?accDoDefaultAction@CWnd@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CWnd__UEAAJUtagVARIANT___Z(void* pThis, void* p0) {
    if (!pThis || !(p0)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->accDoDefaultAction(*static_cast<VARIANT*>(p0));
}

// Symbol: ?accHitTest@CWnd@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CWnd__UEAAJJJPEAUtagVARIANT___Z(void* pThis, long p0, long p1, void* p2) {
    if (!pThis || !(p2)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->accHitTest(p0, p1, static_cast<VARIANT*>(p2));
}

// Symbol: ?accLocation@CWnd@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CWnd__UEAAJPEAJ000UtagVARIANT___Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4) {
    if (!pThis || !(p0 && p1 && p2 && p3 && p4)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->accLocation(static_cast<long*>(p0), static_cast<long*>(p1), static_cast<long*>(p2), static_cast<long*>(p3), *static_cast<VARIANT*>(p4));
}

// Symbol: ?accSelect@CWnd@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accSelect_CWnd__UEAAJJUtagVARIANT___Z(void* pThis, long p0, void* p1) {
    if (!pThis || !(p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->accSelect(p0, *static_cast<VARIANT*>(p1));
}

// Symbol: ?get_accChildCount@CWnd@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(void* pThis, void* p0) {
    if (!pThis || !(p0)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accChildCount(static_cast<long*>(p0));
}

// Symbol: ?get_accChild@CWnd@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accChild(*static_cast<VARIANT*>(p0), static_cast<IDispatch**>(p1));
}

// Symbol: ?get_accDefaultAction@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accDefaultAction(*static_cast<VARIANT*>(p0), static_cast<BSTR*>(p1));
}

// Symbol: ?get_accDescription@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDescription_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accDescription(*static_cast<VARIANT*>(p0), static_cast<BSTR*>(p1));
}

// Symbol: ?get_accFocus@CWnd@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accFocus_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, void* p0) {
    if (!pThis || !(p0)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accFocus(static_cast<VARIANT*>(p0));
}

// Symbol: ?get_accHelp@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accHelp_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accHelp(*static_cast<VARIANT*>(p0), static_cast<BSTR*>(p1));
}

// Symbol: ?get_accHelpTopic@CWnd@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI impl__get_accHelpTopic_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(void* pThis, void* p0, void* p1, void* p2) {
    if (!pThis || !(p0 && p1 && p2)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accHelpTopic(static_cast<BSTR*>(p0), *static_cast<VARIANT*>(p1), static_cast<long*>(p2));
}

// Symbol: ?get_accKeyboardShortcut@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accKeyboardShortcut_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accKeyboardShortcut(*static_cast<VARIANT*>(p0), static_cast<BSTR*>(p1));
}

// Symbol: ?get_accName@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accName(*static_cast<VARIANT*>(p0), static_cast<BSTR*>(p1));
}

// Symbol: ?get_accParent@CWnd@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CWnd__UEAAJPEAPEAUIDispatch___Z(void* pThis, void* p0) {
    if (!pThis || !(p0)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accParent(static_cast<IDispatch**>(p0));
}

// Symbol: ?get_accRole@CWnd@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CWnd__UEAAJUtagVARIANT__PEAU2__Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accRole(*static_cast<VARIANT*>(p0), static_cast<VARIANT*>(p1));
}

// Symbol: ?get_accSelection@CWnd@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accSelection_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, void* p0) {
    if (!pThis || !(p0)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accSelection(static_cast<VARIANT*>(p0));
}

// Symbol: ?get_accState@CWnd@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CWnd__UEAAJUtagVARIANT__PEAU2__Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accState(*static_cast<VARIANT*>(p0), static_cast<VARIANT*>(p1));
}

// Symbol: ?get_accValue@CWnd@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0 && p1)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->get_accValue(*static_cast<VARIANT*>(p0), static_cast<BSTR*>(p1));
}

// Symbol: ?put_accName@CWnd@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accName_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->put_accName(*static_cast<VARIANT*>(p0), static_cast<BSTR>(p1));
}

// Symbol: ?put_accValue@CWnd@@UEAAJUtagVARIANT@@PEA_W@Z
extern "C" long MS_ABI impl__put_accValue_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, void* p0, void* p1) {
    if (!pThis || !(p0)) return E_POINTER;
    CWnd* window = static_cast<CWnd*>(pThis);
    HRESULT hr = impl__EnsureStdObj_CWnd__UEAAJXZ(window);
    if (FAILED(hr)) return hr;
    return window->m_pStdObject->put_accValue(*static_cast<VARIANT*>(p0), static_cast<BSTR>(p1));
}

