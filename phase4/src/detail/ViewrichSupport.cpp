#include "ViewrichSupport.h"

IMPLEMENT_DYNAMIC(CRichEditCtrl, CWnd)
IMPLEMENT_DYNCREATE(CRichEditView, CView)
IMPLEMENT_DYNCREATE(CHtmlView, CView)
namespace openmfc { namespace detail { namespace viewrich {
wchar_t* CoTaskMemDuplicateW(const wchar_t* pszText) {
    if (!pszText) {
        return nullptr;
    }
    const size_t cch = wcslen(pszText) + 1;
    wchar_t* p = static_cast<wchar_t*>(CoTaskMemAlloc(cch * sizeof(wchar_t)));
    if (!p) {
        return nullptr;
    }
    memcpy(p, pszText, cch * sizeof(wchar_t));
    return p;
}
void DDXDHtmlText(CDHtmlDialog* pThis, const wchar_t* lpszId, CString* pValue, int bSaveAndValidate) {
    if (!pThis || !pValue) return;
    if (bSaveAndValidate) {
        wchar_t* text = pThis->GetElementText(lpszId);
        *pValue = text ? text : L"";
        if (text) ::SysFreeString(text);
    } else {
        pThis->SetElementText(lpszId, const_cast<wchar_t*>(static_cast<const wchar_t*>(*pValue)));
    }
}
int DHtmlSaveAndValidate(void* pDX) {
    return pDX ? static_cast<DHtmlDataExchange*>(pDX)->m_bSaveAndValidate : FALSE;
}
} } }  // namespace openmfc::detail::viewrich
