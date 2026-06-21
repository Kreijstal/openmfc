// OpenMFC: CGlobalUtils — currency/decimal string conversions (oleaut32-backed).
//
// CGlobalUtils is an MFC feature-pack utility class; these two methods are
// stateless numeric parsers that wrap the OLE Automation Variant conversion
// APIs (fully available under Wine). The remaining CGlobalUtils exports are
// docking/pane geometry helpers (CWnd/CDockablePane/CPaneContainerManager) and
// the StringFromCy/StringFromDecimal pair (CStringT-by-reference out params);
// those are left for a GUI/string-coupled tranche.
//
// pThis (the CGlobalUtils*) is accepted to match the member-function ABI but is
// unused — both conversions are pure functions of their inputs.

#include <windows.h>
#include <oleauto.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ?CyFromString@CGlobalUtils@@QEAAHAEATtagCY@@PEB_W@Z
// int CyFromString(CY& cyResult, const wchar_t* lpszValue)
// Parses a currency string into a CY (scaled by 10000). Returns TRUE on success.
// Symbol: ?CyFromString@CGlobalUtils@@QEAAHAEATtagCY@@PEB_W@Z
extern "C" int MS_ABI impl__CyFromString_CGlobalUtils__QEAAHAEATtagCY__PEB_W_Z(
    void* /*pThis*/, CY* pcyResult, const wchar_t* lpszValue)
{
    if (!pcyResult) return 0;
    pcyResult->int64 = 0;
    if (!lpszValue) return 0;
    HRESULT hr = VarCyFromStr(const_cast<wchar_t*>(lpszValue), GetThreadLocale(), 0, pcyResult);
    return SUCCEEDED(hr) ? 1 : 0;
}

// ?DecimalFromString@CGlobalUtils@@QEAAHAEAUtagDEC@@PEB_W@Z
// int DecimalFromString(DECIMAL& decResult, const wchar_t* lpszValue)
// Parses a decimal string into a DECIMAL. Returns TRUE on success.
// Symbol: ?DecimalFromString@CGlobalUtils@@QEAAHAEAUtagDEC@@PEB_W@Z
extern "C" int MS_ABI impl__DecimalFromString_CGlobalUtils__QEAAHAEAUtagDEC__PEB_W_Z(
    void* /*pThis*/, DECIMAL* pdecResult, const wchar_t* lpszValue)
{
    if (!pdecResult) return 0;
    *pdecResult = DECIMAL();
    if (!lpszValue) return 0;
    HRESULT hr = VarDecFromStr(const_cast<wchar_t*>(lpszValue), GetThreadLocale(), 0, pdecResult);
    return SUCCEEDED(hr) ? 1 : 0;
}
