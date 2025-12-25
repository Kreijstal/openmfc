// Integration test for COleDispatchDriver using a real COM automation object.
#include "openmfc/afxdisp.h"
#include <cstdio>
#include <cstdlib>

// Fail-fast stubs for OpenMFC exception helpers.
void AFXAPI AfxThrowOleException(LONG sc) {
    std::fprintf(stderr, "AfxThrowOleException called: 0x%lx\n", static_cast<unsigned long>(sc));
    std::exit(1);
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, UINT nDescriptionID, UINT nHelpID) {
    (void)nDescriptionID;
    std::fprintf(stderr, "AfxThrowOleDispatchException called: %u help=%u\n", wCode, nHelpID);
    std::exit(1);
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID) {
    (void)lpszDescription;
    std::fprintf(stderr, "AfxThrowOleDispatchException called: %u help=%u\n", wCode, nHelpID);
    std::exit(1);
}

int main() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        std::printf("FAIL: CoInitializeEx 0x%lx\n", static_cast<unsigned long>(hr));
        return 1;
    }

    CLSID clsid;
    hr = CLSIDFromProgID(L"Scripting.Dictionary", &clsid);
    if (FAILED(hr)) {
        std::printf("FAIL: CLSIDFromProgID Scripting.Dictionary 0x%lx\n", static_cast<unsigned long>(hr));
        CoUninitialize();
        return 1;
    }

    LPDISPATCH disp = nullptr;
    hr = CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_IDispatch,
                          reinterpret_cast<void**>(&disp));
    if (FAILED(hr) || !disp) {
        std::printf("FAIL: CoCreateInstance 0x%lx\n", static_cast<unsigned long>(hr));
        CoUninitialize();
        return 1;
    }

    COleDispatchDriver driver(disp, TRUE);

    OLECHAR* addName = const_cast<OLECHAR*>(L"Add");
    OLECHAR* countName = const_cast<OLECHAR*>(L"Count");
    DISPID dispidAdd = DISPID_UNKNOWN;
    DISPID dispidCount = DISPID_UNKNOWN;

    hr = disp->GetIDsOfNames(IID_NULL, &addName, 1, LOCALE_USER_DEFAULT, &dispidAdd);
    if (FAILED(hr)) {
        std::printf("FAIL: GetIDsOfNames(Add) 0x%lx\n", static_cast<unsigned long>(hr));
        CoUninitialize();
        return 1;
    }
    hr = disp->GetIDsOfNames(IID_NULL, &countName, 1, LOCALE_USER_DEFAULT, &dispidCount);
    if (FAILED(hr)) {
        std::printf("FAIL: GetIDsOfNames(Count) 0x%lx\n", static_cast<unsigned long>(hr));
        CoUninitialize();
        return 1;
    }

    COleVariant value(42L);
    driver.InvokeHelper(dispidAdd, DISPATCH_METHOD, VT_EMPTY, nullptr,
                        reinterpret_cast<const unsigned char*>(VTS_BSTR VTS_VARIANT),
                        L"answer", static_cast<VARIANT*>(&value));

    long count = 0;
    driver.InvokeHelper(dispidCount, DISPATCH_PROPERTYGET, VT_I4, &count, nullptr);
    if (count != 1) {
        std::printf("FAIL: Count expected 1, got %ld\n", count);
        CoUninitialize();
        return 1;
    }

    std::printf("OK: COM dispatch integration test passed\n");
    CoUninitialize();
    return 0;
}
