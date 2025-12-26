// Test that InvokeHelperV frees EXCEPINFO BSTRs on DISP_E_EXCEPTION.
#include <cstdio>
#include <string>

#define _OLEAUT32_
#define SysFreeString TrackedSysFreeString
#include "openmfc/afxdisp.h"
#undef SysFreeString
#undef _OLEAUT32_

extern "C" void __stdcall SysFreeString(BSTR bstr);

static int g_sysFreeCount = 0;

extern "C" void __stdcall TrackedSysFreeString(BSTR bstr) {
    ++g_sysFreeCount;
    SysFreeString(bstr);
}

struct DispatchException {
    WORD code;
    std::wstring description;
    DWORD helpContext;
};

struct OleException {
    LONG status;
};

class ErrorDispatch : public IDispatch {
public:
    ErrorDispatch() : m_ref(1) {}

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

    HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*) override {
        return E_NOTIMPL;
    }

    HRESULT __stdcall Invoke(DISPID, REFIID, LCID, WORD,
                             DISPPARAMS*, VARIANT*, EXCEPINFO* pExcepInfo, UINT*) override {
        if (!pExcepInfo) {
            return E_POINTER;
        }
        pExcepInfo->wCode = 123;
        pExcepInfo->dwHelpContext = 77;
        pExcepInfo->bstrDescription = SysAllocString(L"boom");
        pExcepInfo->bstrSource = SysAllocString(L"source");
        pExcepInfo->bstrHelpFile = SysAllocString(L"help");
        return DISP_E_EXCEPTION;
    }

private:
    ~ErrorDispatch() = default;
    LONG m_ref;
};

void AFXAPI AfxThrowOleException(LONG sc) {
    throw OleException{sc};
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, UINT nDescriptionID, UINT nHelpID) {
    (void)nDescriptionID;
    throw DispatchException{wCode, L"", nHelpID};
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID) {
    std::wstring desc = lpszDescription ? lpszDescription : L"";
    throw DispatchException{wCode, desc, nHelpID};
}

int main() {
    ErrorDispatch* dispatch = new ErrorDispatch();
    COleDispatchDriver driver(dispatch, FALSE);

    bool caught = false;
    try {
        driver.InvokeHelper(1, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
    } catch (const DispatchException& ex) {
        caught = true;
        if (ex.code != 123 || ex.helpContext != 77 || ex.description != L"boom") {
            std::printf("FAIL: DispatchException mismatch\n");
            dispatch->Release();
            return 1;
        }
    } catch (const OleException&) {
        std::printf("FAIL: unexpected OleException\n");
        dispatch->Release();
        return 1;
    }

    if (!caught) {
        std::printf("FAIL: expected DispatchException\n");
        dispatch->Release();
        return 1;
    }

    if (g_sysFreeCount != 3) {
        std::printf("FAIL: expected 3 SysFreeString calls, got %d\n", g_sysFreeCount);
        dispatch->Release();
        return 1;
    }

    std::printf("OK: InvokeHelperV frees EXCEPINFO BSTRs on exception\n");
    dispatch->Release();
    return 0;
}
