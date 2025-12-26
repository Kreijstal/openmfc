#pragma once
#include "afxwin.h"
#include <cstdarg>
#include <string>
#include <vector>

// OLE/COM Dispatch support (minimal)
#include <objbase.h>
#include <oleauto.h>

// OLE exception classes (ABI-aligned with current OpenMFC implementation)
class COleException : public CException {
public:
    COleException() : CException(1), m_sc(0) {}
    LONG m_sc;
};

class COleDispatchException : public CException {
public:
    COleDispatchException() : CException(1), m_wCode(0), m_strDescription(nullptr), m_dwHelpContext(0) {}
    WORD m_wCode;
    const wchar_t* m_strDescription;
    DWORD m_dwHelpContext;
};

// OLE exception helpers
void AFXAPI AfxThrowOleException(LONG sc);
void AFXAPI AfxThrowOleDispatchException(WORD wCode, UINT nDescriptionID, UINT nHelpID);
void AFXAPI AfxThrowOleDispatchException(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID);

// MFC-style parameter type descriptors for InvokeHelper
#ifndef VTS_I2
#define VTS_I2 "\x02"
#define VTS_I4 "\x03"
#define VTS_R4 "\x04"
#define VTS_R8 "\x05"
#define VTS_BOOL "\x0B"
#define VTS_BSTR "\x08"
#define VTS_DISPATCH "\x09"
#define VTS_UNKNOWN "\x0D"
#define VTS_VARIANT "\x0C"
#define VTS_UI1 "\x11"
#define VTS_UI2 "\x12"
#define VTS_UI4 "\x13"
#endif

class COleVariant : public VARIANT {
public:
    COleVariant() { VariantInit(this); }
    explicit COleVariant(const wchar_t* value) {
        VariantInit(this);
        vt = VT_BSTR;
        bstrVal = SysAllocString(value ? value : L"");
    }
    explicit COleVariant(long value) {
        VariantInit(this);
        vt = VT_I4;
        lVal = value;
    }
    explicit COleVariant(bool value) {
        VariantInit(this);
        vt = VT_BOOL;
        boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
    }
    ~COleVariant() { VariantClear(this); }
};

class COleDispatchDriver {
public:
    COleDispatchDriver() : m_lpDispatch(nullptr), m_bAutoRelease(TRUE) {}
    explicit COleDispatchDriver(LPDISPATCH lpDispatch, BOOL bAutoRelease = TRUE)
        : m_lpDispatch(lpDispatch), m_bAutoRelease(bAutoRelease) {}
    ~COleDispatchDriver() { ReleaseDispatch(); }

    void AttachDispatch(LPDISPATCH lpDispatch, BOOL bAutoRelease = TRUE) {
        ReleaseDispatch();
        m_lpDispatch = lpDispatch;
        m_bAutoRelease = bAutoRelease;
    }

    LPDISPATCH DetachDispatch() {
        LPDISPATCH lpDispatch = m_lpDispatch;
        m_lpDispatch = nullptr;
        return lpDispatch;
    }

    void ReleaseDispatch() {
        if (m_lpDispatch && m_bAutoRelease) {
            m_lpDispatch->Release();
        }
        m_lpDispatch = nullptr;
    }

    LPDISPATCH GetIDispatch(BOOL bAddRef = FALSE) const {
        if (m_lpDispatch && bAddRef) {
            m_lpDispatch->AddRef();
        }
        return m_lpDispatch;
    }

    BOOL CreateDispatch(const wchar_t* lpszProgID, CException* /*pError*/ = nullptr) {
        CLSID clsid;
        HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
        if (FAILED(hr)) {
            AfxThrowOleException(hr);
        }
        return CreateDispatch(clsid);
    }

    BOOL CreateDispatch(REFCLSID clsid, CException* /*pError*/ = nullptr) {
        ReleaseDispatch();
        LPDISPATCH lpDispatch = nullptr;
        HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL, IID_IDispatch,
                                      reinterpret_cast<void**>(&lpDispatch));
        if (FAILED(hr)) {
            AfxThrowOleException(hr);
        }
        m_lpDispatch = lpDispatch;
        m_bAutoRelease = TRUE;
        return TRUE;
    }

    void InvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet, void* pvRet,
                      const unsigned char* pbParamInfo, ...) {
        va_list args;
        va_start(args, pbParamInfo);
        InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
        va_end(args);
    }

    void InvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet, void* pvRet,
                       const unsigned char* pbParamInfo, va_list args) {
        if (!m_lpDispatch) {
            AfxThrowOleException(E_POINTER);
        }

        int cParams = CountParams(pbParamInfo);
        std::vector<VARIANTARG> params;
        params.reserve(cParams);

        // Build parameters in reverse order for IDispatch::Invoke.
        for (int i = 0; i < cParams; ++i) {
            VARTYPE vt = static_cast<VARTYPE>(pbParamInfo[i]);
            VARIANTARG var = MakeVariant(vt, &args);
            params.push_back(var);
        }

        std::vector<VARIANTARG> reversed;
        reversed.reserve(cParams);
        for (int i = cParams - 1; i >= 0; --i) {
            reversed.push_back(params[static_cast<size_t>(i)]);
            if (i == 0) {
                break;
            }
        }

        DISPPARAMS dispParams = {};
        dispParams.cArgs = cParams;
        dispParams.rgvarg = cParams ? reversed.data() : nullptr;

        DISPID dispidNamed = DISPID_PROPERTYPUT;
        if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF)) {
            dispParams.cNamedArgs = 1;
            dispParams.rgdispidNamedArgs = &dispidNamed;
        }

        VARIANT result;
        VariantInit(&result);

        EXCEPINFO excep = {};
        UINT argErr = 0;
        HRESULT hr = m_lpDispatch->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT, wFlags,
                                          &dispParams, &result, &excep, &argErr);

        // Clean up argument variants.
        for (int i = 0; i < cParams; ++i) {
            VariantClear(&params[static_cast<size_t>(i)]);
        }

        if (FAILED(hr)) {
            if (hr == DISP_E_EXCEPTION) {
                std::wstring desc;
                if (excep.bstrDescription) {
                    desc.assign(excep.bstrDescription, SysStringLen(excep.bstrDescription));
                    SysFreeString(excep.bstrDescription);
                    excep.bstrDescription = nullptr;
                }
                if (excep.bstrSource) {
                    SysFreeString(excep.bstrSource);
                    excep.bstrSource = nullptr;
                }
                if (excep.bstrHelpFile) {
                    SysFreeString(excep.bstrHelpFile);
                    excep.bstrHelpFile = nullptr;
                }
                const wchar_t* descPtr = desc.empty() ? L"" : desc.c_str();
                AfxThrowOleDispatchException(excep.wCode, descPtr, excep.dwHelpContext);
            }
            AfxThrowOleException(hr);
        }

        if (vtRet != VT_EMPTY && pvRet) {
            CopyResult(vtRet, pvRet, &result);
        }
        if (vtRet != VT_BSTR && vtRet != VT_DISPATCH && vtRet != VT_UNKNOWN &&
            vtRet != VT_VARIANT && vtRet != VT_BYREF) {
            VariantClear(&result);
        } else {
            VariantInit(&result);
        }
    }

private:
    static int CountParams(const unsigned char* pbParamInfo) {
        if (!pbParamInfo) {
            return 0;
        }
        int count = 0;
        while (pbParamInfo[count] != 0) {
            ++count;
        }
        return count;
    }

    static VARIANTARG MakeVariant(VARTYPE vt, va_list* args) {
        VARIANTARG var;
        VariantInit(&var);

        switch (vt) {
            case VT_I2:
                var.vt = VT_I2;
                var.iVal = static_cast<SHORT>(va_arg(*args, int));
                break;
            case VT_I4:
                var.vt = VT_I4;
                var.lVal = va_arg(*args, long);
                break;
            case VT_R4:
                var.vt = VT_R4;
                var.fltVal = static_cast<float>(va_arg(*args, double));
                break;
            case VT_R8:
                var.vt = VT_R8;
                var.dblVal = va_arg(*args, double);
                break;
            case VT_BOOL:
                var.vt = VT_BOOL;
                var.boolVal = va_arg(*args, int) ? VARIANT_TRUE : VARIANT_FALSE;
                break;
            case VT_BSTR: {
                var.vt = VT_BSTR;
                const wchar_t* text = va_arg(*args, const wchar_t*);
                var.bstrVal = SysAllocString(text ? text : L"");
                break;
            }
            case VT_DISPATCH:
                var.vt = VT_DISPATCH;
                var.pdispVal = va_arg(*args, LPDISPATCH);
                break;
            case VT_UNKNOWN:
                var.vt = VT_UNKNOWN;
                var.punkVal = va_arg(*args, LPUNKNOWN);
                break;
            case VT_VARIANT: {
                VARIANT* src = va_arg(*args, VARIANT*);
                if (src) {
                    VariantCopy(&var, src);
                }
                break;
            }
            case VT_UI1:
                var.vt = VT_UI1;
                var.bVal = static_cast<BYTE>(va_arg(*args, int));
                break;
            case VT_UI2:
                var.vt = VT_UI2;
                var.uiVal = static_cast<USHORT>(va_arg(*args, int));
                break;
            case VT_UI4:
                var.vt = VT_UI4;
                var.ulVal = va_arg(*args, unsigned long);
                break;
            default:
                var.vt = VT_EMPTY;
                AfxThrowOleException(DISP_E_TYPEMISMATCH);
                break;
        }

        return var;
    }

    static void CopyResult(VARTYPE vt, void* pvRet, const VARIANT* result) {
        switch (vt) {
            case VT_I2:
                *static_cast<short*>(pvRet) = result->iVal;
                break;
            case VT_I4:
                *static_cast<long*>(pvRet) = result->lVal;
                break;
            case VT_R4:
                *static_cast<float*>(pvRet) = result->fltVal;
                break;
            case VT_R8:
                *static_cast<double*>(pvRet) = result->dblVal;
                break;
            case VT_BOOL:
                *static_cast<BOOL*>(pvRet) = (result->boolVal == VARIANT_TRUE);
                break;
            case VT_BSTR:
                *static_cast<BSTR*>(pvRet) = result->bstrVal;
                break;
            case VT_DISPATCH:
                *static_cast<LPDISPATCH*>(pvRet) = result->pdispVal;
                break;
            case VT_UNKNOWN:
                *static_cast<LPUNKNOWN*>(pvRet) = result->punkVal;
                break;
            case VT_VARIANT:
                VariantCopy(static_cast<VARIANT*>(pvRet), const_cast<VARIANT*>(result));
                break;
            case VT_UI1:
                *static_cast<BYTE*>(pvRet) = result->bVal;
                break;
            case VT_UI2:
                *static_cast<USHORT*>(pvRet) = result->uiVal;
                break;
            case VT_UI4:
                *static_cast<ULONG*>(pvRet) = result->ulVal;
                break;
            default:
                break;
        }
    }

    LPDISPATCH m_lpDispatch;
    BOOL m_bAutoRelease;
};
