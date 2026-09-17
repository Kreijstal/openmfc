// CDataExchange — OpenMFC implementation.
// Sources: global_cdataexchange.cpp
//
// The DDX_/DDV_/Afx* bodies below were transcribed from the retail export
// bodies (disas.py --u, mfc140u.dll RVAs quoted per function), the method
// described in the header of core/ole/COleControl.cpp.  Member offsets are
// those of the shadow struct in detail/CDataExchangeSupport.h:
//   +0x00 m_bSaveAndValidate, +0x08 m_pDlgWnd, +0x10 m_idLastControl,
//   +0x14 m_bEditLastControl.
// Retail's CDataExchange::PrepareCtrl is 0x209050 (mfc140u) and
// PrepareEditCtrl (0x209030) is PrepareCtrl followed by
// `movl $1, 0x14(%rbx)`; several bodies inline that pair, which is written
// here as a call to this file's PrepareEditCtrl thunk.

#include "detail/CDataExchangeSupport.h"
#include <commctrl.h>
#include <oleauto.h>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>

// ---------------------------------------------------------------------------
// Thunks defined elsewhere in the tree (every one grep-verified to have a
// `// Symbol:` definition).  Declared with ABI-equivalent parameter types.
// ---------------------------------------------------------------------------
extern "C" void  MS_ABI impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(
    void* rString, UINT nIDS, const wchar_t* lpsz1);
extern "C" void  MS_ABI impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z(
    void* rString, UINT nIDS, const wchar_t* const* rglpsz, int nString);
extern "C" int   MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, UINT nType, UINT nIDHelp);
extern "C" int   MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType, UINT nIDHelp);
extern "C" void  MS_ABI impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(HWND hWnd, const wchar_t* lpszText);
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(void* pThis, UINT nID);
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void  MS_ABI impl__AfxThrowNotSupportedException__YAXXZ();
extern "C" void  MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void  MS_ABI impl__GetDlgItem_CWnd__QEBAXHPEAPEAUHWND_____Z(const void* pThis, int nID, HWND* pWnd);
extern "C" void* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const void* pThis, int nID);
extern "C" int   MS_ABI impl__SetRange_CDateTimeCtrl__QEAAHPEBVCOleDateTime_ATL__0_Z(void* pThis, const void* pMin, const void* pMax);
extern "C" int   MS_ABI impl__SetRange_CDateTimeCtrl__QEAAHPEBVCTime_ATL__0_Z(void* pThis, const void* pMin, const void* pMax);
extern "C" int   MS_ABI impl__SetRange_CMonthCalCtrl__QEAAHPEBVCOleDateTime_ATL__0_Z(void* pThis, const void* pMin, const void* pMax);
extern "C" int   MS_ABI impl__SetRange_CMonthCalCtrl__QEAAHPEBVCTime_ATL__0_Z(void* pThis, const void* pMin, const void* pMax);
extern "C" int   MS_ABI impl__IsOpen_CRecordset__QEBAHXZ(const void* pThis);
extern "C" int   MS_ABI impl__IsFieldNull_CRecordset__QEAAHPEAX_Z(void* pThis, void* pv);
extern "C" int   MS_ABI impl__IsFieldNullable_CRecordset__QEAAHPEAX_Z(void* pThis, void* pv);
extern "C" void  MS_ABI impl__SetFieldNull_CRecordset__QEAAXPEAXH_Z(void* pThis, void* pv, int bNull);
extern "C" int   MS_ABI impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(void* pThis, const wchar_t* lpsz, unsigned long dwFlags, unsigned long lcid);
extern "C" void  MS_ABI impl__Format_COleCurrency__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__KK_Z(
    void* pRet, const void* pThis, unsigned long dwFormat, unsigned long lcid);
extern "C" void* MS_ABI impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(void* pThis);
extern "C" void* MS_ABI impl__FindItem_COleControlContainer__UEBAPEAVCOleControlSite__I_Z(const void* pThis, unsigned int nID);
extern "C" void  MS_ABI impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(void* pThis, long dispid, unsigned short vtProp, void* pvProp);
extern "C" void  MS_ABI impl__SetPropertyV_COleControlSite__UEAAXJGPEAD_Z(void* pThis, long dispid, unsigned short vtProp, char* pArgList);

// CStringT / CSimpleStringT members, reached exactly as retail reaches them:
// through the exports.  (The OpenMFC C++ CString header cannot be used from
// this unit -- see CStr below.)
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(void* pThis, const wchar_t* psz);
extern "C" wchar_t* MS_ABI impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(void* pThis, int nLength);
extern "C" void  MS_ABI impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z(void* pThis, int nNewLength);
extern "C" void  MS_ABI impl__Empty___CSimpleStringT__W_00_ATL__QEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__GetLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const void* pThis);

// Thunks defined later in this file.
extern "C" HWND  MS_ABI impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC);
extern "C" HWND  MS_ABI impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC);
extern "C" void* MS_ABI impl__PrepareOleCtrl_CDataExchange__QEAAPEAVCOleControlSite__H_Z(void* pThis, int nIDC);
extern "C" void  MS_ABI impl__Fail_CDataExchange__QEAAXXZ(void* pThis);

namespace {

// ---- a CString handled through the exported CStringT members --------------
// The MFC/ATL CStringT is one pointer (m_pszData) to a ref-counted buffer
// whose CStringData header sits 24 bytes before it.  This unit drives it
// through the DLL's own CStringT/CSimpleStringT exports (which is what the
// retail bodies do) rather than the C++ CString in openmfc/afxstr.h: including
// any of the openmfc/afx*.h headers emits CObject's vtable and the string
// manager's RTTI into this object, and the per-file link audit rejects the
// resulting undefined toolchain symbols.
struct CStr {
    wchar_t* m_pszData;
    CStr() { impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(this); }
    ~CStr() { impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(this); }
    CStr(const CStr&) = delete;
    CStr& operator=(const CStr&) = delete;
    void Assign(const wchar_t* psz) { impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(this, psz); }
    const wchar_t* c_str() const { return m_pszData; }
    int GetLength() const { return impl__GetLength___CSimpleStringT__W_00_ATL__QEBAHXZ(this); }
    void Empty() { impl__Empty___CSimpleStringT__W_00_ATL__QEAAXXZ(this); }
    wchar_t* GetBufferSetLength(int n) { return impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(this, n); }
    void ReleaseBuffer(int n = -1) { impl__ReleaseBuffer___CSimpleStringT__W_00_ATL__QEAAXH_Z(this, n); }
    bool LoadString(UINT nID) { return impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(this, nID) != 0; }
};
static_assert(sizeof(CStr) == sizeof(void*), "CStringT is a single pointer");
// Storage for a CString that a callee constructs in place (the by-value
// CString return of COleCurrency::Format): only the destructor runs here.
struct CStrSlot {
    wchar_t* m_pszData;
    CStrSlot() : m_pszData(nullptr) {}
    ~CStrSlot() { if (m_pszData) impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(this); }
    CStrSlot(const CStrSlot&) = delete;
    CStrSlot& operator=(const CStrSlot&) = delete;
    const wchar_t* c_str() const { return m_pszData ? m_pszData : L""; }
};

// ---- value records as the MSVC client lays them out ----------------------
// ATL::COleDateTime (atlcomtime.h): DATE m_dt @0, DateTimeStatus m_status @8,
// sizeof 16.
struct OleDT { double m_dt; int m_status; };
static_assert(sizeof(OleDT) == 16 && offsetof(OleDT, m_status) == 8, "ATL COleDateTime layout");
enum { OLEDT_VALID = 0, OLEDT_INVALID = 1, OLEDT_NULL = 2 };
// ATL::CTime: a single __time64_t.
struct AtlCTime { long long m_time; };
static_assert(sizeof(AtlCTime) == 8, "ATL CTime layout");
// OLE DB value types (oledb.h), spelled locally to keep that header out.
struct S_DBDATE { SHORT year; USHORT month; USHORT day; };
struct S_DBTIME { USHORT hour; USHORT minute; USHORT second; };
struct S_DB_NUMERIC { BYTE precision; BYTE scale; BYTE sign; BYTE val[16]; };
static_assert(sizeof(S_DBDATE) == 6 && sizeof(S_DBTIME) == 6 && sizeof(S_DB_NUMERIC) == 19, "OLE DB layouts");
static_assert(offsetof(S_DB_NUMERIC, val) == 3, "DB_NUMERIC::val @3");

// AFX_IDP_* prompt ids (afxres.h) used by the DDX failure paths, plus the
// ATL string-table id COleDateTime::Format falls back to (atlrc.h).
enum : UINT {
    AFX_IDP_PARSE_REAL_        = 0xF111,
    AFX_IDP_PARSE_STRING_SIZE_ = 0xF114,
    AFX_IDP_PARSE_RADIO_BUTTON_= 0xF115,
    AFX_IDP_PARSE_DATETIME_    = 0xF118,
    AFX_IDP_PARSE_CURRENCY_    = 0xF119,
    AFX_IDP_PARSE_GUID_        = 0xF11A,
    AFX_IDP_PARSE_TIME_        = 0xF11B,
    AFX_IDP_PARSE_DATE_        = 0xF11C,
    AFX_IDS_DELETED_           = 0xF29D,
    ATL_IDS_DATETIME_INVALID_  = 0xD800,
};
const LCID LANG_USER_DEFAULT_ = 0x0400;   // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)

// PrepareEditCtrl followed by CWnd::GetDlgItem(nIDC, &hWnd) -- the prologue
// every DDX_Text overload in this file shares (retail: call 0x209050, store 1
// at +0x14, then call ?GetDlgItem@CWnd@@QEBAXHPEAPEAUHWND__@@@Z on m_pDlgWnd).
// The second lookup is what the bodies actually use for the HWND.
HWND PrepareEditCtrlHwnd(S_Cdataexchange* self, int nIDC)
{
    impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(self, nIDC);
    HWND hWndCtrl = nullptr;
    impl__GetDlgItem_CWnd__QEBAXHPEAPEAUHWND_____Z(self->m_pDlgWnd, nIDC, &hWndCtrl);
    return hWndCtrl;
}

// The window-text read the retail DDX_Text overloads inline (the save arm of
// ?DDX_Text@@...CStringT... at 0x209450 (mfc140u) is the same sequence):
//   nLen = GetWindowTextLength(h);
//   GetWindowText(h, str.GetBufferSetLength(nLen), nLen + 1);
//   str.ReleaseBuffer();
// A zero length is turned into Empty(): the same result, without handing the
// shared empty-string buffer to GetWindowText.
void ReadCtrlText(HWND hWndCtrl, CStr* str)
{
    int nLen = ::GetWindowTextLengthW(hWndCtrl);
    if (nLen <= 0) { str->Empty(); return; }
    wchar_t* buf = str->GetBufferSetLength(nLen);
    if (buf == nullptr) { str->Empty(); return; }
    ::GetWindowTextW(hWndCtrl, buf, nLen + 1);
    str->ReleaseBuffer(-1);
}

// CRecordset::IsDeleted() stand-in for AfxFieldText -- see the deviation
// note on that function.  Always FALSE for now.
bool RecordsetIsDeleted(const void* pRecordset)
{
    (void)pRecordset;
    return false;
}

// ---- ATL COleDateTime members as the retail bodies inline them ------------
// COleDateTime::DoubleFromDate: dates at or above -OLE_DATETIME_HALFSECOND are
// used as they are; otherwise `ceil(dt) - (dt - ceil(dt))` (retail compares
// against the constant -5.787037037037037e-06 and calls ceil).
double OleDTDoubleFromDate(double date)
{
    if (date > -5.787037037037037e-06) return date;
    double temp = std::ceil(date);
    return temp - (date - temp);
}
// COleDateTime::operator>(rhs): false unless both are valid.
bool OleDTGreater(const OleDT& lhs, const OleDT& rhs)
{
    if (lhs.m_status != OLEDT_VALID || rhs.m_status != OLEDT_VALID) return false;
    return OleDTDoubleFromDate(lhs.m_dt) > OleDTDoubleFromDate(rhs.m_dt);
}
// COleDateTime::ParseDateTime(lpsz, dwFlags, lcid): VarDateFromStr; on
// failure m_dt = 0 for DISP_E_TYPEMISMATCH, -1 otherwise, status invalid.
bool OleDTParse(OleDT* dt, const wchar_t* lpsz, DWORD dwFlags, LCID lcid)
{
    HRESULT hr = ::VarDateFromStr(lpsz ? lpsz : L"", lcid, dwFlags, &dt->m_dt);
    if (FAILED(hr)) {
        dt->m_dt = (hr == DISP_E_TYPEMISMATCH) ? 0.0 : -1.0;
        dt->m_status = OLEDT_INVALID;
        return false;
    }
    dt->m_status = OLEDT_VALID;
    return true;
}
// COleDateTime::GetAsSystemTime: valid status && VariantTimeToSystemTime == TRUE.
bool OleDTGetAsSystemTime(const OleDT& dt, SYSTEMTIME* st)
{
    std::memset(st, 0, sizeof(*st));
    return dt.m_status == OLEDT_VALID && ::VariantTimeToSystemTime(dt.m_dt, st) == TRUE;
}
// COleDateTime::GetYear/GetMonth/... : the field, or -1 (error) when the
// conversion fails.
int OleDTYear(const OleDT& dt)   { SYSTEMTIME st; return OleDTGetAsSystemTime(dt, &st) ? st.wYear   : -1; }
int OleDTMonth(const OleDT& dt)  { SYSTEMTIME st; return OleDTGetAsSystemTime(dt, &st) ? st.wMonth  : -1; }
int OleDTDay(const OleDT& dt)    { SYSTEMTIME st; return OleDTGetAsSystemTime(dt, &st) ? st.wDay    : -1; }
int OleDTHour(const OleDT& dt)   { SYSTEMTIME st; return OleDTGetAsSystemTime(dt, &st) ? st.wHour   : -1; }
int OleDTMinute(const OleDT& dt) { SYSTEMTIME st; return OleDTGetAsSystemTime(dt, &st) ? st.wMinute : -1; }
int OleDTSecond(const OleDT& dt) { SYSTEMTIME st; return OleDTGetAsSystemTime(dt, &st) ? st.wSecond : -1; }
// COleDateTime::SetDateTime (retail 0x204254 (mfc140u)): zero a SYSTEMTIME,
// fill year..second, then AtlConvertSystemTimeToVariantTime (0x12c8e4):
// SystemTimeToVariantTime, convert back, and require year..second to round
// trip; m_status = valid when that holds, invalid otherwise.
void OleDTSetDateTime(OleDT* dt, int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    st.wYear = (WORD)nYear; st.wMonth = (WORD)nMonth; st.wDay = (WORD)nDay;
    st.wHour = (WORD)nHour; st.wMinute = (WORD)nMin; st.wSecond = (WORD)nSec;
    BOOL ok = ::SystemTimeToVariantTime(&st, &dt->m_dt);
    SYSTEMTIME back;
    std::memset(&back, 0, sizeof(back));
    ok = ok && ::VariantTimeToSystemTime(dt->m_dt, &back);
    ok = ok && (st.wYear == back.wYear && st.wMonth == back.wMonth && st.wDay == back.wDay &&
                st.wHour == back.wHour && st.wMinute == back.wMinute && st.wSecond == back.wSecond);
    dt->m_status = ok ? OLEDT_VALID : OLEDT_INVALID;
}
// The "Invalid DateTime" text COleDateTime::Format falls back to: the
// ATL_IDS_DATETIME_INVALID string resource, else the literal.
void OleDTInvalidText(CStr* out)
{
    if (!out->LoadString(ATL_IDS_DATETIME_INVALID_)) out->Assign(L"Invalid DateTime");
}
// COleDateTime::Format(0, LANG_USER_DEFAULT) as compiled at retail 0x12c508
// (mfc140u; the flags/LCID are folded into the body): null -> L"",
// invalid -> the invalid text, otherwise VarBstrFromDate(m_dt, 0x400, 0) with
// the same invalid text on failure.
void OleDTFormat(const OleDT& dt, CStr* out)
{
    if (dt.m_status == OLEDT_NULL) { out->Assign(L""); return; }
    if (dt.m_status == OLEDT_INVALID) { OleDTInvalidText(out); return; }
    BSTR bstr = nullptr;
    if (FAILED(::VarBstrFromDate(dt.m_dt, LANG_USER_DEFAULT_, 0, &bstr))) { OleDTInvalidText(out); return; }
    out->Assign(bstr ? bstr : L"");
    ::SysFreeString(bstr);
}
// CDateTimeCtrl::SetTime(const COleDateTime&) (retail 0x29a310 (mfc140u)):
//   wParam = GDT_NONE (1) when the status is not valid or
//            VariantTimeToSystemTime(m_dt, &st) != TRUE, else GDT_VALID (0);
//   SendMessage(m_hWnd, DTM_SETSYSTEMTIME, wParam, &st).
// Spelled out here because the tree's CDateTimeCtrl::SetTime thunk drops the
// message on a conversion failure instead of sending GDT_NONE.
void DateTimeCtrlSetTime(HWND hWndCtrl, const OleDT& dt)
{
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    WPARAM wFlag = GDT_NONE;
    if (dt.m_status == OLEDT_VALID) {
        wFlag = (::VariantTimeToSystemTime(dt.m_dt, &st) != TRUE) ? GDT_NONE : GDT_VALID;
    }
    ::SendMessage(hWndCtrl, DTM_SETSYSTEMTIME, wFlag, reinterpret_cast<LPARAM>(&st));
}

// ---- OLE-control property exchange helpers --------------------------------
// COleControlSite::SetProperty(dispid, vt, ...) is retail vftable slot 0xf0,
// which packages its varargs and calls SetPropertyV (slot 0xd8).  The tree's
// ?SetProperty@COleControlSite@@UEAAXJGZZ thunk discards the varargs, so the
// value is routed through the SetPropertyV thunk directly.
void SiteSetProperty(void* pSite, long dispid, unsigned short vt, ...)
{
    va_list args;
    va_start(args, vt);
    char* pArgList = nullptr;
    std::memcpy(&pArgList, &args, sizeof(pArgList));   // an MS-x64 va_list is a char*
    impl__SetPropertyV_COleControlSite__UEAAXJGPEAD_Z(pSite, dispid, vt, pArgList);
    va_end(args);
}
// ENSURE(pDX); pSite = pDX->PrepareOleCtrl(nIDC); ENSURE(pSite);  -- the
// prologue of every read/write DDX_OC* body (0x241450/0x241500/0x2415b0/
// 0x241660/0x241710/0x2417f0 (mfc140u)).
void* PrepareOleSite(S_Cdataexchange* self, int nIDC)
{
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    void* pSite = impl__PrepareOleCtrl_CDataExchange__QEAAPEAVCOleControlSite__H_Z(self, nIDC);
    if (pSite == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    return pSite;
}
// The shared body of the DDX_OC*RO family (retail helper 0x2413f8 (mfc140u),
// reached from 0x2414d0/0x241580/0x241630/0x2416e0/0x241790/0x241870):
//   ENSURE(pDX);
//   if (pDX->m_bSaveAndValidate) {
//       pSite = pDX->PrepareOleCtrl(nIDC); ENSURE(pSite);
//       pSite->GetProperty(dispid, vt, pv);            // vftable slot 0xe8
//   }
void OleCtrlGetPropertyRO(void* pDX, int nIDC, long dispid, unsigned short vt, void* pv)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    if (!self->m_bSaveAndValidate) return;
    void* pSite = impl__PrepareOleCtrl_CDataExchange__QEAAPEAVCOleControlSite__H_Z(self, nIDC);
    if (pSite == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, vt, pv);
}

} // namespace

// Symbol: ??0CDataExchange@@QEAA@PEAVCWnd@@H@Z
// CDataExchange(CWnd* pDlgWnd, BOOL bSaveAndValidate)
extern "C" void* MS_ABI
impl___0CDataExchange__QEAA_PEAVCWnd__H_Z(void* pThis, void* pDlgWnd, BOOL bSaveAndValidate)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    self->m_bSaveAndValidate = bSaveAndValidate;
    self->m_pDlgWnd = pDlgWnd;
    self->m_idLastControl = 0;
    self->m_bEditLastControl = FALSE;
    return pThis;
}
// Symbol: ?PrepareCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
// HWND PrepareCtrl(int nIDC)
// Resolve the control window from the dialog, record it as the last-touched
// (non-edit) control, and hand back its HWND for the DDX routine.
extern "C" HWND MS_ABI
impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), nIDC);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = FALSE;  // not an edit item unless PrepareEditCtrl
    return hWndCtrl;
}
// Symbol: ?PrepareEditCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
// HWND PrepareEditCtrl(int nIDC)
// Same as PrepareCtrl but flags the control as an edit item so that a later
// Fail() knows to select all its text.
extern "C" HWND MS_ABI
impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), nIDC);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = TRUE;
    return hWndCtrl;
}
// Symbol: ?PrepareOleCtrl@CDataExchange@@QEAAPEAVCOleControlSite@@H@Z
// COleControlSite* PrepareOleCtrl(int nIDC) -- retail 0x2380f0 (mfc140u):
//     COleControlContainer* pCont = m_pDlgWnd->m_pCtrlCont;   // CWnd +0xc8
//     if (pCont == NULL) AfxThrowNotSupportedException();
//     COleControlSite* pSite = pCont->FindItem(nIDC);          // vftable slot 0xb8
//     if (pSite == NULL) AfxThrowNotSupportedException();
//     m_idLastControl = nIDC; m_bEditLastControl = FALSE;      // only on success
//     return pSite;
// OpenMFC's CWnd has no m_pCtrlCont member; the container lives in a side
// table read through the CWnd::GetControlContainer thunk, and FindItem is
// reached through its export thunk.  A NULL m_pDlgWnd is treated as "no
// container" (retail would dereference it).
extern "C" void* MS_ABI
impl__PrepareOleCtrl_CDataExchange__QEAAPEAVCOleControlSite__H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    void* pCont = nullptr;
    if (self->m_pDlgWnd != nullptr) {
        pCont = impl__GetControlContainer_CWnd__QEAAPEAVCOleControlContainer__XZ(self->m_pDlgWnd);
    }
    if (pCont == nullptr) impl__AfxThrowNotSupportedException__YAXXZ();
    void* pSite = impl__FindItem_COleControlContainer__UEBAPEAVCOleControlSite__I_Z(
        pCont, static_cast<unsigned int>(nIDC));
    if (pSite == nullptr) impl__AfxThrowNotSupportedException__YAXXZ();
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = FALSE;
    return pSite;
}
// Symbol: ?Fail@CDataExchange@@QEAAXXZ
// void Fail()
// Restore focus to the last control involved in the exchange (selecting all of
// its text if it was an edit control) and then abort the DDX pass by throwing.
extern "C" void MS_ABI
impl__Fail_CDataExchange__QEAAXXZ(void* pThis)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    if (self->m_idLastControl != 0) {
        HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), self->m_idLastControl);
        if (hWndCtrl != nullptr) {
            ::SetFocus(hWndCtrl);
            if (self->m_bEditLastControl) {
                // Select the entire contents of the offending edit control.
                ::SendMessageW(hWndCtrl, EM_SETSEL, 0, static_cast<LPARAM>(-1));
            }
        }
    }
    // AfxThrowUserException(): never returns normally.
    impl__AfxThrowUserException__YAXXZ();
}

// ===========================================================================
// Validation failure helpers
// ===========================================================================

// Symbol: ?AfxFailMaxChars@@YAXPEAVCDataExchange@@H@Z
// void AfxFailMaxChars(CDataExchange* pDX, int nChars) -- retail 0x20a080 (mfc140u):
//     TCHAR szT[32]; _stprintf_s(szT, 32, _T("%d"), nChars);   // __stdio_common_vswprintf_s
//     CString prompt; AfxFormatString1(prompt, AFX_IDP_PARSE_STRING_SIZE, szT);
//     AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
//     prompt.Empty();
//     pDX->Fail();                                              // throws
extern "C" void MS_ABI impl__AfxFailMaxChars__YAXPEAVCDataExchange__H_Z(void* pDX, int nChars)
{
    wchar_t szT[32];
    std::swprintf(szT, 32, L"%d", nChars);
    CStr prompt;
    impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(
        &prompt, AFX_IDP_PARSE_STRING_SIZE_, szT);
    impl__AfxMessageBox__YAHPEB_WII_Z(prompt.c_str(), MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE_);
    prompt.Empty();
    impl__Fail_CDataExchange__QEAAXXZ(pDX);
}

// Symbol: ?AfxFailRadio@@YAXPEAVCDataExchange@@@Z
// void AfxFailRadio(CDataExchange* pDX) -- retail 0x20a110 (mfc140u):
//     CString prompt; AfxFormatStrings(prompt, AFX_IDP_PARSE_RADIO_BUTTON, NULL, 0);
//     AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_RADIO_BUTTON);
//     prompt.Empty();
//     pDX->Fail();
extern "C" void MS_ABI impl__AfxFailRadio__YAXPEAVCDataExchange___Z(void* pDX)
{
    CStr prompt;
    impl__AfxFormatStrings__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEBQEB_WH_Z(
        &prompt, AFX_IDP_PARSE_RADIO_BUTTON_, nullptr, 0);
    impl__AfxMessageBox__YAHPEB_WII_Z(prompt.c_str(), MB_ICONEXCLAMATION, AFX_IDP_PARSE_RADIO_BUTTON_);
    prompt.Empty();
    impl__Fail_CDataExchange__QEAAXXZ(pDX);
}

// Symbol: ?AfxFieldText@@YAHPEAVCDataExchange@@HPEAXPEAVCRecordset@@@Z
// BOOL AfxFieldText(CDataExchange* pDX, int nIDC, void* pv, CRecordset* pRecordset)
// -- retail 0x2048e0 (mfc140u):
//     HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
//     if (pDX->m_bSaveAndValidate) {
//         TCHAR szBuffer[2]; ::GetWindowText(hWndCtrl, szBuffer, 2);
//         if (szBuffer[0] == 0) {
//             if (pRecordset->IsFieldNullable(pv)) { pRecordset->SetFieldNull(pv, TRUE); return TRUE; }
//             return FALSE;
//         }
//         pRecordset->SetFieldNull(pv, FALSE);
//         return FALSE;
//     }
//     if (!pRecordset->IsOpen() || pRecordset->IsFieldNull(pv)) {
//         AfxSetWindowText(hWndCtrl, _T(""));  return TRUE;
//     }
//     if (pRecordset->m_bDeleted == 0) return FALSE;           // IsDeleted(), inlined: +0x16c
//     CString strDeleted((LPCTSTR)MAKEINTRESOURCE(AFX_IDS_DELETED));  // loads the string resource
//     AfxSetWindowText(hWndCtrl, strDeleted);  return TRUE;
// The recordset calls go through their export thunks (note that
// ?IsFieldNullable@CRecordset@@QEAAHPEAX@Z is still a stub in this tree, so
// an empty edit never nulls the field here).
// DEVIATION -- the deleted-record branch: CRecordset::IsDeleted() is inline
// in MFC (no export), OpenMFC's CRecordset keeps m_bDeleted at its own
// (unpinned) offset, and its declaration is only reachable through
// openmfc/afxdb.h, which this unit cannot include (see CStr).  Until a C
// accessor exists (requested), RecordsetIsDeleted() below answers FALSE, so a
// field of a deleted record is handed on to the normal DDX_Text path instead
// of showing the AFX_IDS_DELETED text.
extern "C" int MS_ABI impl__AfxFieldText__YAHPEAVCDataExchange__HPEAXPEAVCRecordset___Z(
    void* pDX, int nIDC, void* pv, void* pRecordset)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pRs = pRecordset;
    HWND hWndCtrl = impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(self, nIDC);
    if (self->m_bSaveAndValidate) {
        wchar_t szBuffer[2] = { 0, 0 };
        ::GetWindowTextW(hWndCtrl, szBuffer, 2);
        if (szBuffer[0] == L'\0') {
            if (impl__IsFieldNullable_CRecordset__QEAAHPEAX_Z(pRs, pv)) {
                impl__SetFieldNull_CRecordset__QEAAXPEAXH_Z(pRs, pv, TRUE);
                return TRUE;
            }
            return FALSE;
        }
        impl__SetFieldNull_CRecordset__QEAAXPEAXH_Z(pRs, pv, FALSE);
        return FALSE;
    }
    if (!impl__IsOpen_CRecordset__QEBAHXZ(pRs) || impl__IsFieldNull_CRecordset__QEAAHPEAX_Z(pRs, pv)) {
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, L"");
        return TRUE;
    }
    if (!RecordsetIsDeleted(pRs)) return FALSE;
    CStr strDeleted;
    strDeleted.LoadString(AFX_IDS_DELETED_);
    impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, strDeleted.c_str());
    return TRUE;
}

// Symbol: ?AfxTextFloatFormat@@YAXPEAVCDataExchange@@HPEAXNH@Z
// void AfxTextFloatFormat(CDataExchange* pDX, int nIDC, void* pData, double value, int nSizeGcvt)
// -- retail 0x20e720 (mfc140u):
//     pDX->PrepareEditCtrl(nIDC);  pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);
//     TCHAR szBuffer[400];
//     if (pDX->m_bSaveAndValidate) {
//         ::GetWindowText(hWndCtrl, szBuffer, 400);
//         double d;
//         if (_stscanf_s(szBuffer, _T("%lf"), &d) != 1) {       // __stdio_common_vswscanf
//             AfxMessageBox(AFX_IDP_PARSE_REAL, MB_OK, (UINT)-1);  pDX->Fail();
//         }
//         if (nSizeGcvt == FLT_DIG /*6*/) *(float*)pData = (float)d; else *(double*)pData = d;
//     } else {
//         ATL_CRT_ERRORCHECK(_sntprintf_s(szBuffer, 400, 399, _T("%.*g"), nSizeGcvt, value));
//         AfxSetWindowText(hWndCtrl, szBuffer);
//     }
// Deviation: retail's ATL_CRT_ERRORCHECK saves/clears errno around the
// _sntprintf_s and throws (memory exception for ENOMEM, an OLE exception for
// EINVAL/ERANGE/other) when it is set; the CRT here does not report through
// errno for this call and that check is not reproduced.
extern "C" void MS_ABI impl__AfxTextFloatFormat__YAXPEAVCDataExchange__HPEAXNH_Z(
    void* pDX, int nIDC, void* pData, double value, int nSizeGcvt)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    wchar_t szBuffer[400];
    if (self->m_bSaveAndValidate) {
        szBuffer[0] = L'\0';
        ::GetWindowTextW(hWndCtrl, szBuffer, 400);
        double d = 0.0;
        if (std::swscanf(szBuffer, L"%lf", &d) != 1) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_REAL_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
        if (nSizeGcvt == 6)
            *static_cast<float*>(pData) = static_cast<float>(d);
        else
            *static_cast<double*>(pData) = d;
    } else {
        std::swprintf(szBuffer, 400, L"%.*g", nSizeGcvt, value);
        szBuffer[399] = L'\0';
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, szBuffer);
    }
}

// ===========================================================================
// DDV_MinMax* for date/time pickers and month calendars
// ===========================================================================
// All four share one shape (retail 0x29ba10 / 0x29ba80 / 0x29bbb0 / 0x29bc20
// (mfc140u)):
//     CWnd* pWnd = pDX->m_pDlgWnd->GetDlgItem(pDX->m_idLastControl);  // ?GetDlgItem@CWnd@@QEBAPEAV1@H@Z
//     if (pDX->m_bSaveAndValidate) {
//         if (pMin != NULL && *pMin > value) return;
//         if (pMax != NULL && *pMax < value) return;
//     }
//     ENSURE(pWnd);                                    // AfxThrowInvalidArgException
//     pWnd->SetRange(pMin, pMax);
// i.e. the range is pushed to the control when loading, or when saving with
// the value inside [min, max]; a value outside the range returns without
// touching the control.  CTime compares its __time64_t; COleDateTime uses
// operator>/operator< (both must be valid, DoubleFromDate on each side).

// Symbol: ?DDV_MinMaxDateTime@@YAXPEAVCDataExchange@@AEAVCOleDateTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxDateTime__YAXPEAVCDataExchange__AEAVCOleDateTime_ATL__PEBV23_2_Z(
    void* pDX, void* pValue, const void* pMinRange, const void* pMaxRange)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    const OleDT& value = *static_cast<const OleDT*>(pValue);
    const OleDT* pMin = static_cast<const OleDT*>(pMinRange);
    const OleDT* pMax = static_cast<const OleDT*>(pMaxRange);
    void* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(self->m_pDlgWnd, static_cast<int>(self->m_idLastControl));
    if (self->m_bSaveAndValidate) {
        if (pMin != nullptr && OleDTGreater(*pMin, value)) return;
        if (pMax != nullptr && OleDTGreater(value, *pMax)) return;
    }
    if (pWnd == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    impl__SetRange_CDateTimeCtrl__QEAAHPEBVCOleDateTime_ATL__0_Z(pWnd, pMin, pMax);
}

// Symbol: ?DDV_MinMaxDateTime@@YAXPEAVCDataExchange@@AEAVCTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxDateTime__YAXPEAVCDataExchange__AEAVCTime_ATL__PEBV23_2_Z(
    void* pDX, void* pValue, const void* pMinRange, const void* pMaxRange)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    const AtlCTime& value = *static_cast<const AtlCTime*>(pValue);
    const AtlCTime* pMin = static_cast<const AtlCTime*>(pMinRange);
    const AtlCTime* pMax = static_cast<const AtlCTime*>(pMaxRange);
    void* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(self->m_pDlgWnd, static_cast<int>(self->m_idLastControl));
    if (self->m_bSaveAndValidate) {
        if (pMin != nullptr && pMin->m_time > value.m_time) return;
        if (pMax != nullptr && pMax->m_time < value.m_time) return;
    }
    if (pWnd == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    impl__SetRange_CDateTimeCtrl__QEAAHPEBVCTime_ATL__0_Z(pWnd, pMin, pMax);
}

// Symbol: ?DDV_MinMaxMonth@@YAXPEAVCDataExchange@@AEAVCOleDateTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxMonth__YAXPEAVCDataExchange__AEAVCOleDateTime_ATL__PEBV23_2_Z(
    void* pDX, void* pValue, const void* pMinRange, const void* pMaxRange)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    const OleDT& value = *static_cast<const OleDT*>(pValue);
    const OleDT* pMin = static_cast<const OleDT*>(pMinRange);
    const OleDT* pMax = static_cast<const OleDT*>(pMaxRange);
    void* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(self->m_pDlgWnd, static_cast<int>(self->m_idLastControl));
    if (self->m_bSaveAndValidate) {
        if (pMin != nullptr && OleDTGreater(*pMin, value)) return;
        if (pMax != nullptr && OleDTGreater(value, *pMax)) return;
    }
    if (pWnd == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    impl__SetRange_CMonthCalCtrl__QEAAHPEBVCOleDateTime_ATL__0_Z(pWnd, pMin, pMax);
}

// Symbol: ?DDV_MinMaxMonth@@YAXPEAVCDataExchange@@AEAVCTime@ATL@@PEBV23@2@Z
extern "C" void MS_ABI impl__DDV_MinMaxMonth__YAXPEAVCDataExchange__AEAVCTime_ATL__PEBV23_2_Z(
    void* pDX, void* pValue, const void* pMinRange, const void* pMaxRange)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    const AtlCTime& value = *static_cast<const AtlCTime*>(pValue);
    const AtlCTime* pMin = static_cast<const AtlCTime*>(pMinRange);
    const AtlCTime* pMax = static_cast<const AtlCTime*>(pMaxRange);
    void* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(self->m_pDlgWnd, static_cast<int>(self->m_idLastControl));
    if (self->m_bSaveAndValidate) {
        if (pMin != nullptr && pMin->m_time > value.m_time) return;
        if (pMax != nullptr && pMax->m_time < value.m_time) return;
    }
    if (pWnd == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    impl__SetRange_CMonthCalCtrl__QEAAHPEBVCTime_ATL__0_Z(pWnd, pMin, pMax);
}

// ===========================================================================
// DDX_DateTimeCtrl / DDX_MonthCalCtrl for the OLE DB date types
// ===========================================================================
// Shared prologue (retail 0x27f7b0 / 0x27f870 / 0x27f950 (mfc140u)):
//     ENSURE(pDX);
//     CWnd* pWnd = CWnd::FromHandle(pDX->PrepareCtrl(nIDC));  ENSURE(pWnd);
// Only pWnd->m_hWnd is used afterwards.  CWnd::FromHandle (0x28ad70) goes
// through CHandleMap::FromHandle (0x2a6080), which returns NULL only for a
// NULL handle and otherwise hands back a permanent or freshly allocated
// temporary wrapper (then CWnd::AttachControlSite on it); so the HWND from
// PrepareCtrl is checked and used directly here, without materialising the
// temporary CWnd or the AttachControlSite side effect.
// One deviation inherited from this file's PrepareCtrl: retail's 0x209050
// throws CNotSupportedException (0x2276f0) itself when the id names no child
// window (after falling back to the OLE control container), so ENSURE(pWnd)
// only ever fires for an OLE control site (PrepareCtrl then returns a NULL
// HWND).  The PrepareCtrl above returns NULL for a missing control instead,
// so a bad nIDC reaches the ENSURE here and raises CInvalidArgException.

// Symbol: ?DDX_DateTimeCtrl@@YAXPEAVCDataExchange@@HAEAUtagDBDATE@@@Z
// void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, DBDATE& value)
// -- retail 0x27f7b0 (mfc140u):
//     ENSURE(pDX);  pWnd = CWnd::FromHandle(pDX->PrepareCtrl(nIDC));  ENSURE(pWnd);
//     if (pDX->m_bSaveAndValidate) {
//         SYSTEMTIME st; ::SendMessage(pWnd->m_hWnd, DTM_GETSYSTEMTIME, 0, &st);   // result ignored
//         value.year = st.wYear; value.month = st.wMonth; value.day = st.wDay;
//     } else {
//         SYSTEMTIME st = {0}; st.wYear = value.year; st.wMonth = value.month; st.wDay = value.day;
//         ::SendMessage(pWnd->m_hWnd, DTM_SETSYSTEMTIME, GDT_VALID, &st);
//     }
// (DTM_GETSYSTEMTIME = 0x1001, DTM_SETSYSTEMTIME = 0x1002; the SendMessage
// slot resolves to USER32!SendMessageW.)  Retail leaves st uninitialized on
// the get path -- it is zeroed here first, which only matters if the control
// does not fill it.
extern "C" void MS_ABI impl__DDX_DateTimeCtrl__YAXPEAVCDataExchange__HAEAUtagDBDATE___Z(
    void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    S_DBDATE* value = static_cast<S_DBDATE*>(pValue);
    HWND hWndCtrl = impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(self, nIDC);
    if (hWndCtrl == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    if (self->m_bSaveAndValidate) {
        ::SendMessage(hWndCtrl, DTM_GETSYSTEMTIME, 0, reinterpret_cast<LPARAM>(&st));
        value->year = static_cast<SHORT>(st.wYear);
        value->month = st.wMonth;
        value->day = st.wDay;
    } else {
        st.wYear = static_cast<WORD>(value->year);
        st.wMonth = value->month;
        st.wDay = value->day;
        ::SendMessage(hWndCtrl, DTM_SETSYSTEMTIME, GDT_VALID, reinterpret_cast<LPARAM>(&st));
    }
}

// Symbol: ?DDX_DateTimeCtrl@@YAXPEAVCDataExchange@@HAEAUtagDBTIME@@@Z
// void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, DBTIME& value)
// -- retail 0x27f870 (mfc140u):
//     ENSURE(pDX);  pWnd = CWnd::FromHandle(pDX->PrepareCtrl(nIDC));  ENSURE(pWnd);
//     if (pDX->m_bSaveAndValidate) {
//         SYSTEMTIME st; ::SendMessage(pWnd->m_hWnd, DTM_GETSYSTEMTIME, 0, &st);   // result ignored
//         value.hour = st.wHour; value.minute = st.wMinute; value.second = st.wSecond;
//     } else {
//         SYSTEMTIME st; ::GetSystemTime(&st);                 // today's date (KERNEL32!GetSystemTime)
//         st.wHour = value.hour; st.wMinute = value.minute; st.wSecond = value.second; st.wMilliseconds = 0;
//         ::SendMessage(pWnd->m_hWnd, DTM_SETSYSTEMTIME, GDT_VALID, &st);
//     }
// As for the DBDATE overload, st is zeroed here before the get where retail
// leaves it uninitialized.
extern "C" void MS_ABI impl__DDX_DateTimeCtrl__YAXPEAVCDataExchange__HAEAUtagDBTIME___Z(
    void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    S_DBTIME* value = static_cast<S_DBTIME*>(pValue);
    HWND hWndCtrl = impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(self, nIDC);
    if (hWndCtrl == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    if (self->m_bSaveAndValidate) {
        ::SendMessage(hWndCtrl, DTM_GETSYSTEMTIME, 0, reinterpret_cast<LPARAM>(&st));
        value->hour = st.wHour;
        value->minute = st.wMinute;
        value->second = st.wSecond;
    } else {
        ::GetSystemTime(&st);
        st.wHour = value->hour;
        st.wMinute = value->minute;
        st.wSecond = value->second;
        st.wMilliseconds = 0;
        ::SendMessage(hWndCtrl, DTM_SETSYSTEMTIME, GDT_VALID, reinterpret_cast<LPARAM>(&st));
    }
}

// Symbol: ?DDX_DateTimeCtrl@@YAXPEAVCDataExchange@@HAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CString& value)
// -- retail 0x29b810 (mfc140u):
//     if (pDX->m_bSaveAndValidate)
//         DDX_Text(pDX, nIDC, value);                          // 0x209450: the control's window text
//     else {
//         COleDateTime dt;  dt.ParseDateTime(value, 0, LANG_USER_DEFAULT);   // VarDateFromStr inlined
//         DDX_DateTimeCtrl(pDX, nIDC, dt);                     // 0x29b6d0
//     }
// and 0x29b6d0's load arm is: PrepareCtrl(nIDC); FromHandle; ENSURE(pWnd);
// pWnd->SetTime(dt) (0x29a310).  Both callees are spelled out here rather
// than routed through the DdxExchange.cpp thunks, whose CDataExchange view
// puts an HWND at +0x10 and a BOOL at +0x18 (see detail/DdxCore.h) and so
// would clobber m_idLastControl/m_bEditLastControl and write past the object.
extern "C" void MS_ABI impl__DDX_DateTimeCtrl__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    CStr* value = static_cast<CStr*>(pValue);
    if (self->m_bSaveAndValidate) {
        // DDX_Text(pDX, nIDC, CString&), save arm.
        HWND hWndCtrl = impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(self, nIDC);
        ReadCtrlText(hWndCtrl, value);
        return;
    }
    OleDT dt = { 0.0, OLEDT_VALID };
    OleDTParse(&dt, value->c_str(), 0, LANG_USER_DEFAULT_);
    // DDX_DateTimeCtrl(pDX, nIDC, COleDateTime&), load arm.
    HWND hWndCtrl = impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(self, nIDC);
    if (hWndCtrl == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    DateTimeCtrlSetTime(hWndCtrl, dt);
}

// Symbol: ?DDX_MonthCalCtrl@@YAXPEAVCDataExchange@@HAEAUtagDBDATE@@@Z
// -- retail 0x27f950 (mfc140u); byte-for-byte the DBDATE DDX_DateTimeCtrl body
// with the same message numbers, which for a month calendar are
// MCM_GETCURSEL (0x1001) / MCM_SETCURSEL (0x1002):
//     if (pDX->m_bSaveAndValidate) {
//         SYSTEMTIME st; ::SendMessage(hWnd, MCM_GETCURSEL, 0, &st);
//         value.year = st.wYear; value.month = st.wMonth; value.day = st.wDay;
//     } else {
//         SYSTEMTIME st = {0}; st.wYear = value.year; st.wMonth = value.month; st.wDay = value.day;
//         ::SendMessage(hWnd, MCM_SETCURSEL, 0, &st);
//     }
extern "C" void MS_ABI impl__DDX_MonthCalCtrl__YAXPEAVCDataExchange__HAEAUtagDBDATE___Z(
    void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    S_DBDATE* value = static_cast<S_DBDATE*>(pValue);
    HWND hWndCtrl = impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(self, nIDC);
    if (hWndCtrl == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    SYSTEMTIME st;
    std::memset(&st, 0, sizeof(st));
    if (self->m_bSaveAndValidate) {
        ::SendMessage(hWndCtrl, MCM_GETCURSEL, 0, reinterpret_cast<LPARAM>(&st));
        value->year = static_cast<SHORT>(st.wYear);
        value->month = st.wMonth;
        value->day = st.wDay;
    } else {
        st.wYear = static_cast<WORD>(value->year);
        st.wMonth = value->month;
        st.wDay = value->day;
        ::SendMessage(hWndCtrl, MCM_SETCURSEL, 0, reinterpret_cast<LPARAM>(&st));
    }
}

// ===========================================================================
// DDX_OC* -- OLE control property exchange
// ===========================================================================
// Read/write shape (retail 0x241500 (VT_BOOL), 0x2415b0 (VT_I4; the Color and
// both Int exports are folded onto this one body), 0x241660 (VT_I2),
// 0x241710 (VT_R4), 0x2417f0 (VT_R8), 0x241450 (VT_BSTR) (mfc140u)):
//     ENSURE(pDX); pSite = pDX->PrepareOleCtrl(nIDC); ENSURE(pSite);
//     if (pDX->m_bSaveAndValidate) pSite->GetProperty(dispid, vt, &value);   // slot 0xe8
//     else                         pSite->SetProperty(dispid, vt, value);    // slot 0xf0
// The RO variants are OleCtrlGetPropertyRO above.

// Symbol: ?DDX_OCBool@@YAXPEAVCDataExchange@@HJAEAH@Z
extern "C" void MS_ABI impl__DDX_OCBool__YAXPEAVCDataExchange__HJAEAH_Z(void* pDX, int nIDC, long dispid, int* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate)
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_BOOL, pValue);
    else
        SiteSetProperty(pSite, dispid, VT_BOOL, *pValue);
}

// Symbol: ?DDX_OCBoolRO@@YAXPEAVCDataExchange@@HJAEAH@Z
extern "C" void MS_ABI impl__DDX_OCBoolRO__YAXPEAVCDataExchange__HJAEAH_Z(void* pDX, int nIDC, long dispid, int* pValue)
{
    OleCtrlGetPropertyRO(pDX, nIDC, dispid, VT_BOOL, pValue);
}

// Symbol: ?DDX_OCColor@@YAXPEAVCDataExchange@@HJAEAK@Z
// VT_COLOR is VT_I4; the exported body is the one at 0x2415b0.
extern "C" void MS_ABI impl__DDX_OCColor__YAXPEAVCDataExchange__HJAEAK_Z(void* pDX, int nIDC, long dispid, unsigned long* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate)
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_I4, pValue);
    else
        SiteSetProperty(pSite, dispid, VT_I4, static_cast<long>(*pValue));
}

// Symbol: ?DDX_OCColorRO@@YAXPEAVCDataExchange@@HJAEAK@Z
extern "C" void MS_ABI impl__DDX_OCColorRO__YAXPEAVCDataExchange__HJAEAK_Z(void* pDX, int nIDC, long dispid, unsigned long* pValue)
{
    OleCtrlGetPropertyRO(pDX, nIDC, dispid, VT_I4, pValue);
}

// Symbol: ?DDX_OCFloat@@YAXPEAVCDataExchange@@HJAEAM@Z
// The float is widened to double for the vararg put (cvtps2pd in retail).
extern "C" void MS_ABI impl__DDX_OCFloat__YAXPEAVCDataExchange__HJAEAM_Z(void* pDX, int nIDC, long dispid, float* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate)
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_R4, pValue);
    else
        SiteSetProperty(pSite, dispid, VT_R4, static_cast<double>(*pValue));
}

// Symbol: ?DDX_OCFloat@@YAXPEAVCDataExchange@@HJAEAN@Z
extern "C" void MS_ABI impl__DDX_OCFloat__YAXPEAVCDataExchange__HJAEAN_Z(void* pDX, int nIDC, long dispid, double* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate)
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_R8, pValue);
    else
        SiteSetProperty(pSite, dispid, VT_R8, *pValue);
}

// Symbol: ?DDX_OCFloatRO@@YAXPEAVCDataExchange@@HJAEAM@Z
extern "C" void MS_ABI impl__DDX_OCFloatRO__YAXPEAVCDataExchange__HJAEAM_Z(void* pDX, int nIDC, long dispid, float* pValue)
{
    OleCtrlGetPropertyRO(pDX, nIDC, dispid, VT_R4, pValue);
}

// Symbol: ?DDX_OCFloatRO@@YAXPEAVCDataExchange@@HJAEAN@Z
extern "C" void MS_ABI impl__DDX_OCFloatRO__YAXPEAVCDataExchange__HJAEAN_Z(void* pDX, int nIDC, long dispid, double* pValue)
{
    OleCtrlGetPropertyRO(pDX, nIDC, dispid, VT_R8, pValue);
}

// Symbol: ?DDX_OCInt@@YAXPEAVCDataExchange@@HJAEAH@Z
// Folded onto 0x2415b0 in retail (VT_I4).
extern "C" void MS_ABI impl__DDX_OCInt__YAXPEAVCDataExchange__HJAEAH_Z(void* pDX, int nIDC, long dispid, int* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate)
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_I4, pValue);
    else
        SiteSetProperty(pSite, dispid, VT_I4, static_cast<long>(*pValue));
}

// Symbol: ?DDX_OCInt@@YAXPEAVCDataExchange@@HJAEAJ@Z
// Folded onto 0x2415b0 in retail (VT_I4).
extern "C" void MS_ABI impl__DDX_OCInt__YAXPEAVCDataExchange__HJAEAJ_Z(void* pDX, int nIDC, long dispid, long* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate)
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_I4, pValue);
    else
        SiteSetProperty(pSite, dispid, VT_I4, *pValue);
}

// Symbol: ?DDX_OCIntRO@@YAXPEAVCDataExchange@@HJAEAH@Z
extern "C" void MS_ABI impl__DDX_OCIntRO__YAXPEAVCDataExchange__HJAEAH_Z(void* pDX, int nIDC, long dispid, int* pValue)
{
    OleCtrlGetPropertyRO(pDX, nIDC, dispid, VT_I4, pValue);
}

// Symbol: ?DDX_OCIntRO@@YAXPEAVCDataExchange@@HJAEAJ@Z
extern "C" void MS_ABI impl__DDX_OCIntRO__YAXPEAVCDataExchange__HJAEAJ_Z(void* pDX, int nIDC, long dispid, long* pValue)
{
    OleCtrlGetPropertyRO(pDX, nIDC, dispid, VT_I4, pValue);
}

// Symbol: ?DDX_OCShort@@YAXPEAVCDataExchange@@HJAEAF@Z
// The short is sign-extended to int for the vararg put (movswl in retail).
extern "C" void MS_ABI impl__DDX_OCShort__YAXPEAVCDataExchange__HJAEAF_Z(void* pDX, int nIDC, long dispid, short* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate)
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_I2, pValue);
    else
        SiteSetProperty(pSite, dispid, VT_I2, static_cast<int>(*pValue));
}

// Symbol: ?DDX_OCShortRO@@YAXPEAVCDataExchange@@HJAEAF@Z
extern "C" void MS_ABI impl__DDX_OCShortRO__YAXPEAVCDataExchange__HJAEAF_Z(void* pDX, int nIDC, long dispid, short* pValue)
{
    OleCtrlGetPropertyRO(pDX, nIDC, dispid, VT_I2, pValue);
}

// Symbol: ?DDX_OCText@@YAXPEAVCDataExchange@@HJAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Retail 0x241450 (mfc140u): GetProperty(dispid, VT_BSTR, &value) writes the
// CString itself; SetProperty(dispid, VT_BSTR, (LPCTSTR)value).  OpenMFC's
// GetProperty hands a VT_BSTR result back as a raw BSTR (CopyDispatchResult in
// detail/OlecoreSupport.cpp), so the BSTR is adopted into the caller's CString
// here; when the get fails OpenMFC leaves the BSTR NULL and the string becomes
// empty, whereas retail would have thrown out of the get.
extern "C" void MS_ABI impl__DDX_OCText__YAXPEAVCDataExchange__HJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pDX, int nIDC, long dispid, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    CStr* value = static_cast<CStr*>(pValue);
    void* pSite = PrepareOleSite(self, nIDC);
    if (self->m_bSaveAndValidate) {
        BSTR bstr = nullptr;
        impl__GetProperty_COleControlSite__UEBAXJGPEAX_Z(pSite, dispid, VT_BSTR, &bstr);
        value->Assign(bstr ? bstr : L"");
        if (bstr) ::SysFreeString(bstr);
    } else {
        SiteSetProperty(pSite, dispid, VT_BSTR, value->c_str());
    }
}

// Symbol: ?DDX_OCTextRO@@YAXPEAVCDataExchange@@HJAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// Retail 0x2414d0 (mfc140u) -> helper 0x2413f8 with VT_BSTR; the BSTR
// adoption note on DDX_OCText applies.
extern "C" void MS_ABI impl__DDX_OCTextRO__YAXPEAVCDataExchange__HJAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pDX, int nIDC, long dispid, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    if (!self->m_bSaveAndValidate) return;
    CStr* value = static_cast<CStr*>(pValue);
    BSTR bstr = nullptr;
    OleCtrlGetPropertyRO(self, nIDC, dispid, VT_BSTR, &bstr);
    value->Assign(bstr ? bstr : L"");
    if (bstr) ::SysFreeString(bstr);
}

// ===========================================================================
// DDX_Text for the OLE / OLE DB value types
// ===========================================================================
// Common shape (retail 0x2702d0 / 0x2704a0 / 0x270700 / 0x270890 / 0x270a70 /
// 0x27ed10 / 0x27f220 / 0x27f4f0 (mfc140u)):
//     [ENSURE(pDX);]   -- present in the FILETIME/COleDateTime/DB* bodies
//     pDX->PrepareEditCtrl(nIDC);  pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);
//     if (pDX->m_bSaveAndValidate) { CString str = <window text>; parse; on failure
//                                    AfxMessageBox(AFX_IDP_PARSE_xxx, MB_OK, -1); pDX->Fail(); }
//     else                         { AfxSetWindowText(hWndCtrl, <formatted>); }

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAU_FILETIME@@@Z
// -- retail 0x2704a0 (mfc140u):
//     save: dt.ParseDateTime(str, 0, LANG_USER_DEFAULT) else { AFX_IDP_PARSE_DATETIME; Fail }
//           VariantTimeToSystemTime(dt.m_dt, &st);  SystemTimeToFileTime(&st, &value);
//     load: FileTimeToSystemTime(&value, &st);
//           dt.SetDateTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
//           AfxSetWindowText(hWndCtrl, dt.Format());
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAU_FILETIME___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    FILETIME* value = static_cast<FILETIME*>(pValue);
    OleDT dt = { 0.0, OLEDT_VALID };
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        if (!OleDTParse(&dt, str.c_str(), 0, LANG_USER_DEFAULT_)) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_DATETIME_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
        SYSTEMTIME st;
        std::memset(&st, 0, sizeof(st));
        ::VariantTimeToSystemTime(dt.m_dt, &st);          // result ignored, as in retail
        ::SystemTimeToFileTime(&st, value);
    } else {
        SYSTEMTIME st;
        std::memset(&st, 0, sizeof(st));
        ::FileTimeToSystemTime(value, &st);
        OleDTSetDateTime(&dt, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        CStr str;
        OleDTFormat(dt, &str);
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, str.c_str());
    }
}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAU_GUID@@@Z
// -- retail 0x270a70 (mfc140u):
//     save: if (FAILED(CLSIDFromString(str, &value))) { AFX_IDP_PARSE_GUID; Fail }
//     load: OLECHAR sz[64]; StringFromGUID2(value, sz, 64); AfxSetWindowText(hWndCtrl, sz);
//           (retail copies sz through a fixed-buffer CString first; same text)
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAU_GUID___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    GUID* value = static_cast<GUID*>(pValue);
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        if (FAILED(::CLSIDFromString(str.c_str(), value))) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_GUID_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
    } else {
        wchar_t szGUID[64];
        szGUID[0] = L'\0';
        ::StringFromGUID2(*value, szGUID, 64);
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, szGUID);
    }
}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDBDATE@@@Z
// -- retail 0x27f220 (mfc140u):
//     save: dt.ParseDateTime(str, 0, LANG_USER_DEFAULT) else { AFX_IDP_PARSE_DATE; Fail }
//           value.year = dt.GetYear(); value.month = dt.GetMonth(); value.day = dt.GetDay();
//     load: dt.SetDateTime(value.year, value.month, value.day, 0, 0, 0);
//           AfxSetWindowText(hWndCtrl, dt.Format());
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDBDATE___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    S_DBDATE* value = static_cast<S_DBDATE*>(pValue);
    OleDT dt = { 0.0, OLEDT_VALID };
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        if (!OleDTParse(&dt, str.c_str(), 0, LANG_USER_DEFAULT_)) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_DATE_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
        value->year = static_cast<SHORT>(OleDTYear(dt));
        value->month = static_cast<USHORT>(OleDTMonth(dt));
        value->day = static_cast<USHORT>(OleDTDay(dt));
    } else {
        OleDTSetDateTime(&dt, value->year, value->month, value->day, 0, 0, 0);
        CStr str;
        OleDTFormat(dt, &str);
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, str.c_str());
    }
}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDBTIME@@@Z
// -- retail 0x27f4f0 (mfc140u):
//     save: dt.ParseDateTime(str, VAR_TIMEVALUEONLY, LANG_USER_DEFAULT) else { AFX_IDP_PARSE_TIME; Fail }
//           value.hour = dt.GetHour(); value.minute = dt.GetMinute(); value.second = dt.GetSecond();
//     load: dt.SetDateTime(1899, 12, 30, value.hour, value.minute, value.second);   // == SetTime
//           AfxSetWindowText(hWndCtrl, dt.Format());
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDBTIME___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    S_DBTIME* value = static_cast<S_DBTIME*>(pValue);
    OleDT dt = { 0.0, OLEDT_VALID };
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        if (!OleDTParse(&dt, str.c_str(), VAR_TIMEVALUEONLY, LANG_USER_DEFAULT_)) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_TIME_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
        value->hour = static_cast<USHORT>(OleDTHour(dt));
        value->minute = static_cast<USHORT>(OleDTMinute(dt));
        value->second = static_cast<USHORT>(OleDTSecond(dt));
    } else {
        OleDTSetDateTime(&dt, 1899, 12, 30, value->hour, value->minute, value->second);
        CStr str;
        OleDTFormat(dt, &str);
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, str.c_str());
    }
}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDB_NUMERIC@@@Z
// -- retail 0x27ed10 (mfc140u):
//   save:
//     CString str = <window text>;  BYTE digits[len] (CTempBuffer<BYTE,128>: heap above 128);
//     NUMPARSE np = { cDig = len, dwInFlags = NUMPRS_STD & ~NUMPRS_HEX_OCT (0x1fbf), 0, 0, 0, 0 };
//     if (FAILED(VarParseNumFromStr(str, GetThreadLocale(), 0x1fbf, &np, digits))) pDX->Fail();  // no message box
//     if (np.nPwr10 > 0) { if (np.nPwr10 + np.cDig > 38) pDX->Fail();  value.scale = 0; }
//     else               { if (np.nPwr10 < -38)          pDX->Fail();  value.scale = (BYTE)-np.nPwr10; }
//     value.precision = (BYTE)np.cDig + (np.nPwr10 > 0 ? (BYTE)np.nPwr10 : 0);
//     value.sign = ((np.dwOutFlags >> 16) & 1) ^ 1;                    // NUMPRS_NEG -> 0, else 1
//     uint128 acc = 0;  for each digit: acc = acc * 10 + digit;  nPwr10 (if > 0) more times: acc *= 10;
//     store acc little-endian into value.val[0..15]  (low 8 bytes at val+0, high 8 at val+8)
//   load:
//     uint128 v = value.val;  p = end of a wide buffer;  *p = 0;  digits = 0;
//     loop: if (v == 0 && digits > value.scale) break;
//           if (digits == value.scale && digits != 0) *--p = '.';
//           *--p = '0' + v % 10;  v /= 10;  ++digits;             // 128-bit shift-subtract division
//     if (p == end) *--p = '0';
//     if (value.sign == 0) *--p = '-';
//     AfxSetWindowText(hWndCtrl, p);
// The 128-bit multiply/divide loops are written with unsigned __int128, which
// wraps modulo 2^128 exactly like retail's carry chains.  The load buffer is
// 320 wide characters (scale is a BYTE, so at most 256 digits plus '.', '-'
// and the terminator); retail's is smaller.
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDB_NUMERIC___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    S_DB_NUMERIC* value = static_cast<S_DB_NUMERIC*>(pValue);
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        int nLen = str.GetLength();
        BYTE fixedDigits[128];
        BYTE* digits = fixedDigits;
        BYTE* heapDigits = nullptr;
        if (nLen > 128) {
            heapDigits = static_cast<BYTE*>(std::malloc(static_cast<size_t>(nLen)));
            if (heapDigits == nullptr) impl__AfxThrowMemoryException__YAXXZ();
            digits = heapDigits;
        }
        NUMPARSE np;
        std::memset(&np, 0, sizeof(np));
        np.cDig = nLen;
        np.dwInFlags = NUMPRS_STD & ~NUMPRS_HEX_OCT;
        HRESULT hr = ::VarParseNumFromStr(str.c_str(), ::GetThreadLocale(),
                                          NUMPRS_STD & ~NUMPRS_HEX_OCT, &np, digits);
        if (FAILED(hr)) {
            std::free(heapDigits);
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
        int cDig = np.cDig;
        int nPwr10 = np.nPwr10;
        if (nPwr10 > 0) {
            if (nPwr10 + cDig > 38) { std::free(heapDigits); impl__Fail_CDataExchange__QEAAXXZ(self); }
            value->scale = 0;
        } else {
            if (nPwr10 < -38) { std::free(heapDigits); impl__Fail_CDataExchange__QEAAXXZ(self); }
            value->scale = static_cast<BYTE>(-nPwr10);
        }
        value->precision = static_cast<BYTE>(static_cast<BYTE>(cDig) + (nPwr10 > 0 ? static_cast<BYTE>(nPwr10) : 0));
        value->sign = static_cast<BYTE>(((np.dwOutFlags >> 16) & 1u) ^ 1u);
        unsigned __int128 acc = 0;
        for (int i = 0; i < cDig; ++i) acc = acc * 10u + digits[i];
        for (int i = 0; i < nPwr10; ++i) acc = acc * 10u;
        unsigned long long lo = static_cast<unsigned long long>(acc);
        unsigned long long hi = static_cast<unsigned long long>(acc >> 64);
        std::memcpy(value->val, &lo, 8);
        std::memcpy(value->val + 8, &hi, 8);
        std::free(heapDigits);
    } else {
        unsigned long long lo = 0, hi = 0;
        std::memcpy(&lo, value->val, 8);
        std::memcpy(&hi, value->val + 8, 8);
        unsigned __int128 v = (static_cast<unsigned __int128>(hi) << 64) | lo;
        wchar_t buf[320];
        wchar_t* const end = buf + 319;
        wchar_t* p = end;
        *p = L'\0';
        unsigned nDigits = 0;
        const unsigned nScale = value->scale;
        for (;;) {
            if (v == 0 && nDigits > nScale) break;
            if (nDigits == nScale && nDigits != 0) *--p = L'.';
            *--p = static_cast<wchar_t>(L'0' + static_cast<unsigned>(v % 10u));
            v /= 10u;
            ++nDigits;
        }
        if (p == end) *--p = L'0';
        if (value->sign == 0) *--p = L'-';
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, p);
    }
}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAUtagDEC@@@Z
// -- retail 0x270890 (mfc140u):
//     save: BSTR b = SysAllocStringLen(str, str.GetLength());  if (!b) AfxThrowMemoryException();
//           hr = VarDecFromStr(b, GetThreadLocale(), 0, &value);  SysFreeString(b);
//           if (FAILED(hr)) { AFX_IDP_PARSE_REAL; Fail }
//     load: if (SUCCEEDED(VarBstrFromDec(&value, GetThreadLocale(), 0, &b))) {
//               CString str(b); SysFreeString(b); AfxSetWindowText(hWndCtrl, str); }
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAUtagDEC___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    DECIMAL* value = static_cast<DECIMAL*>(pValue);
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        BSTR bstr = ::SysAllocStringLen(str.c_str(), static_cast<UINT>(str.GetLength()));
        if (bstr == nullptr) impl__AfxThrowMemoryException__YAXXZ();
        HRESULT hr = ::VarDecFromStr(bstr, ::GetThreadLocale(), 0, value);
        ::SysFreeString(bstr);
        if (FAILED(hr)) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_REAL_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
    } else {
        BSTR bstr = nullptr;
        if (SUCCEEDED(::VarBstrFromDec(value, ::GetThreadLocale(), 0, &bstr))) {
            CStr str;
            str.Assign(bstr ? bstr : L"");
            ::SysFreeString(bstr);
            impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, str.c_str());
        }
    }
}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAVCOleCurrency@@@Z
// -- retail 0x270700 (mfc140u):
//     save: if (!value.ParseCurrency(str, 0, LANG_USER_DEFAULT)) { AFX_IDP_PARSE_CURRENCY; Fail }
//     load: AfxSetWindowText(hWndCtrl, value.Format(0, LANG_USER_DEFAULT));
// Both COleCurrency members are reached through their export thunks.
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAVCOleCurrency___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        if (!impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(pValue, str.c_str(), 0, LANG_USER_DEFAULT_)) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_CURRENCY_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
    } else {
        CStrSlot str;
        impl__Format_COleCurrency__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__KK_Z(
            &str, pValue, 0, LANG_USER_DEFAULT_);
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, str.c_str());
    }
}

// Symbol: ?DDX_Text@@YAXPEAVCDataExchange@@HAEAVCOleDateTime@ATL@@@Z
// -- retail 0x2702d0 (mfc140u):
//     save: if (!value.ParseDateTime(str, 0, LANG_USER_DEFAULT)) { AFX_IDP_PARSE_DATETIME; Fail }
//           (ParseDateTime writes value.m_dt/m_status directly)
//     load: AfxSetWindowText(hWndCtrl, value.Format());
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAVCOleDateTime_ATL___Z(void* pDX, int nIDC, void* pValue)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pDX);
    if (self == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    OleDT* value = static_cast<OleDT*>(pValue);
    HWND hWndCtrl = PrepareEditCtrlHwnd(self, nIDC);
    if (self->m_bSaveAndValidate) {
        CStr str;
        ReadCtrlText(hWndCtrl, &str);
        if (!OleDTParse(value, str.c_str(), 0, LANG_USER_DEFAULT_)) {
            impl__AfxMessageBox__YAHIII_Z(AFX_IDP_PARSE_DATETIME_, MB_OK, static_cast<UINT>(-1));
            impl__Fail_CDataExchange__QEAAXXZ(self);   // throws
        }
    } else {
        CStr str;
        OleDTFormat(*value, &str);
        impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(hWndCtrl, str.c_str());
    }
}
