// Minimal test for COleDispatchDriver InvokeHelper behavior.
#include "openmfc/afxdisp.h"
#include <cstdio>
#include <cstdlib>

class FakeDispatch : public IDispatch {
public:
    FakeDispatch() : m_ref(1), m_value(0) {}

    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override {
        if (!ppvObject) {
            return E_POINTER;
        }
        if (riid == IID_IUnknown || riid == IID_IDispatch) {
            *ppvObject = static_cast<IDispatch*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG __stdcall AddRef() override {
        return static_cast<ULONG>(InterlockedIncrement(&m_ref));
    }

    ULONG __stdcall Release() override {
        ULONG ref = static_cast<ULONG>(InterlockedDecrement(&m_ref));
        if (ref == 0) {
            delete this;
        }
        return ref;
    }

    HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo) override {
        if (!pctinfo) {
            return E_POINTER;
        }
        *pctinfo = 0;
        return S_OK;
    }

    HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo**) override {
        return E_NOTIMPL;
    }

    HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR* rgszNames, UINT cNames,
                                    LCID, DISPID* rgDispId) override {
        if (!rgszNames || !rgDispId) {
            return E_POINTER;
        }
        for (UINT i = 0; i < cNames; ++i) {
            if (wcscmp(rgszNames[i], L"Add") == 0) {
                rgDispId[i] = 1;
            } else if (wcscmp(rgszNames[i], L"Value") == 0) {
                rgDispId[i] = 2;
            } else {
                return DISP_E_UNKNOWNNAME;
            }
        }
        return S_OK;
    }

    HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID, LCID, WORD wFlags,
                             DISPPARAMS* pDispParams, VARIANT* pVarResult,
                             EXCEPINFO*, UINT*) override {
        if (!pDispParams) {
            return E_POINTER;
        }

        if (dispIdMember == 1 && (wFlags & DISPATCH_METHOD)) {
            if (pDispParams->cArgs != 2) {
                return DISP_E_BADPARAMCOUNT;
            }
            const VARIANTARG& rhs = pDispParams->rgvarg[0];
            const VARIANTARG& lhs = pDispParams->rgvarg[1];
            if (lhs.vt != VT_I4 || rhs.vt != VT_I4) {
                return DISP_E_TYPEMISMATCH;
            }
            if (pVarResult) {
                VariantInit(pVarResult);
                pVarResult->vt = VT_I4;
                pVarResult->lVal = lhs.lVal + rhs.lVal;
            }
            return S_OK;
        }

        if (dispIdMember == 2 && (wFlags & DISPATCH_PROPERTYGET)) {
            if (pVarResult) {
                VariantInit(pVarResult);
                pVarResult->vt = VT_I4;
                pVarResult->lVal = m_value;
            }
            return S_OK;
        }

        if (dispIdMember == 2 && (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF))) {
            if (pDispParams->cArgs != 1) {
                return DISP_E_BADPARAMCOUNT;
            }
            const VARIANTARG& val = pDispParams->rgvarg[0];
            if (val.vt != VT_I4) {
                return DISP_E_TYPEMISMATCH;
            }
            m_value = val.lVal;
            return S_OK;
        }

        return DISP_E_MEMBERNOTFOUND;
    }

private:
    ~FakeDispatch() = default;

    LONG m_ref;
    long m_value;
};

// Test-only stubs: the helper calls these even though the happy path won't.
void AFXAPI AfxThrowOleException(LONG sc) {
    std::fprintf(stderr, "AfxThrowOleException called: 0x%lx\n", static_cast<unsigned long>(sc));
    std::abort();
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, UINT nDescriptionID, UINT nHelpID) {
    (void)nDescriptionID;
    std::fprintf(stderr, "AfxThrowOleDispatchException called: %u help=%u\n", wCode, nHelpID);
    std::abort();
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID) {
    (void)lpszDescription;
    std::fprintf(stderr, "AfxThrowOleDispatchException called: %u help=%u\n", wCode, nHelpID);
    std::abort();
}

int main() {
    FakeDispatch* dispatch = new FakeDispatch();
    COleDispatchDriver driver(dispatch, FALSE);

    long sum = 0;
    driver.InvokeHelper(1, DISPATCH_METHOD, VT_I4, &sum,
                        reinterpret_cast<const unsigned char*>(VTS_I4 VTS_I4),
                        10L, 32L);
    if (sum != 42) {
        std::printf("FAIL: Add expected 42, got %ld\n", sum);
        dispatch->Release();
        return 1;
    }

    long value = 0;
    driver.InvokeHelper(2, DISPATCH_PROPERTYGET, VT_I4, &value, nullptr);
    if (value != 0) {
        std::printf("FAIL: initial Value expected 0, got %ld\n", value);
        dispatch->Release();
        return 1;
    }

    driver.InvokeHelper(2, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr,
                        reinterpret_cast<const unsigned char*>(VTS_I4),
                        7L);
    driver.InvokeHelper(2, DISPATCH_PROPERTYGET, VT_I4, &value, nullptr);
    if (value != 7) {
        std::printf("FAIL: Value expected 7, got %ld\n", value);
        dispatch->Release();
        return 1;
    }

    std::printf("OK: COleDispatchDriver InvokeHelper tests passed\n");
    dispatch->Release();
    return 0;
}
