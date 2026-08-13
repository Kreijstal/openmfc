// CMonthCalCtrl — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; Win32 month-calendar wrappers.

#define OPENMFC_APPCORE_IMPL

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
HWND MonthCalHwnd(const CWnd* pThis) {
    return pThis ? pThis->GetSafeHwnd() : nullptr;
}

// ATL's CTime is a single __time64_t member; OpenMFC headers do not declare it.
struct CTime_atl {
    __time64_t m_time;
};

bool StToDate(const SYSTEMTIME* st, DATE* out) {
    return ::SystemTimeToVariantTime(st, out) != 0;
}
bool DateToSt(DATE d, SYSTEMTIME* out) {
    return ::VariantTimeToSystemTime(d, out) != 0;
}
__time64_t StToTimeT(const SYSTEMTIME* st) {
    FILETIME ft;
    ::SystemTimeToFileTime(st, &ft);
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return (__time64_t)((u.QuadPart - 116444736000000000ULL) / 10000000ULL);
}
void TimeTToSt(__time64_t t, SYSTEMTIME* st) {
    ULARGE_INTEGER u;
    u.QuadPart = (unsigned __int64)t * 10000000ULL + 116444736000000000ULL;
    FILETIME ft;
    ft.dwLowDateTime = u.LowPart;
    ft.dwHighDateTime = u.HighPart;
    ::FileTimeToSystemTime(&ft, st);
}
}

// Symbol: ??1CMonthCalCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMonthCalCtrl__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CWnd*>(pThis)->~CWnd();
}

// Symbol: ?Create@CMonthCalCtrl@@UEAAHKAEBUtagPOINT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CMonthCalCtrl__UEAAHKAEBUtagPOINT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const POINT* pPt, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pPt) return FALSE;
    return pThis->CWnd::CreateEx(0, MONTHCAL_CLASSW, L"", dwStyle,
        pPt->x, pPt->y, 0, 0,
        pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr, (HMENU)(UINT_PTR)nID, nullptr);
}

// Symbol: ?Create@CMonthCalCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CMonthCalCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::Create(MONTHCAL_CLASSW, L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?GetCurSel@CMonthCalCtrl@@QEBAHAEAVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__GetCurSel_CMonthCalCtrl__QEBAHAEAVCOleDateTime_ATL___Z(
    const CWnd* pThis, COleDateTime* pDateTime) {
    if (!pDateTime) return FALSE;
    SYSTEMTIME st = {};
    if (!::SendMessageW(MonthCalHwnd(pThis), MCM_GETCURSEL, 0, (LPARAM)&st)) return FALSE;
    DATE d = 0;
    if (!StToDate(&st, &d)) return FALSE;
    pDateTime->m_dt = d;
    pDateTime->m_status = COleDateTime::DT_VALID;
    return TRUE;
}

// Symbol: ?GetCurSel@CMonthCalCtrl@@QEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetCurSel_CMonthCalCtrl__QEBAHAEAVCTime_ATL___Z(
    const CWnd* pThis, void* pDateTime) {
    if (!pDateTime) return FALSE;
    SYSTEMTIME st = {};
    if (!::SendMessageW(MonthCalHwnd(pThis), MCM_GETCURSEL, 0, (LPARAM)&st)) return FALSE;
    reinterpret_cast<CTime_atl*>(pDateTime)->m_time = StToTimeT(&st);
    return TRUE;
}

// Symbol: ?GetFirstDayOfWeek@CMonthCalCtrl@@QEBAHPEAH@Z
extern "C" int MS_ABI impl__GetFirstDayOfWeek_CMonthCalCtrl__QEBAHPEAH_Z(
    const CWnd* pThis, int* pFirstDayOfWeek) {
    DWORD dw = 0;
    ::SendMessageW(MonthCalHwnd(pThis), MCM_GETFIRSTDAYOFWEEK, 0,
        pFirstDayOfWeek ? (LPARAM)&dw : 0);
    if (pFirstDayOfWeek) *pFirstDayOfWeek = (int)(short)LOWORD(dw);
    return TRUE;
}

// Symbol: ?GetMonthRange@CMonthCalCtrl@@QEBAHAEAVCOleDateTime@ATL@@0K@Z
extern "C" int MS_ABI impl__GetMonthRange_CMonthCalCtrl__QEBAHAEAVCOleDateTime_ATL__0K_Z(
    const CWnd* pThis, COleDateTime* pMin, COleDateTime* pMax, unsigned long dwFlags) {
    SYSTEMTIME st[2] = {};
    int count = (int)::SendMessageW(MonthCalHwnd(pThis), MCM_GETMONTHRANGE, (WPARAM)dwFlags, (LPARAM)st);
    if (pMin) {
        DATE d = 0;
        if (StToDate(&st[0], &d)) { pMin->m_dt = d; pMin->m_status = COleDateTime::DT_VALID; }
    }
    if (pMax) {
        DATE d = 0;
        if (StToDate(&st[1], &d)) { pMax->m_dt = d; pMax->m_status = COleDateTime::DT_VALID; }
    }
    return count;
}

// Symbol: ?GetMonthRange@CMonthCalCtrl@@QEBAHAEAVCTime@ATL@@0K@Z
extern "C" int MS_ABI impl__GetMonthRange_CMonthCalCtrl__QEBAHAEAVCTime_ATL__0K_Z(
    const CWnd* pThis, void* pMin, void* pMax, unsigned long dwFlags) {
    SYSTEMTIME st[2] = {};
    int count = (int)::SendMessageW(MonthCalHwnd(pThis), MCM_GETMONTHRANGE, (WPARAM)dwFlags, (LPARAM)st);
    if (pMin) reinterpret_cast<CTime_atl*>(pMin)->m_time = StToTimeT(&st[0]);
    if (pMax) reinterpret_cast<CTime_atl*>(pMax)->m_time = StToTimeT(&st[1]);
    return count;
}

// Symbol: ?GetMonthRange@CMonthCalCtrl@@QEBAHPEAU_SYSTEMTIME@@0K@Z
extern "C" int MS_ABI impl__GetMonthRange_CMonthCalCtrl__QEBAHPEAU_SYSTEMTIME__0K_Z(
    const CWnd* pThis, SYSTEMTIME* pMin, SYSTEMTIME* pMax, unsigned long dwFlags) {
    SYSTEMTIME st[2] = {};
    int count = (int)::SendMessageW(MonthCalHwnd(pThis), MCM_GETMONTHRANGE, (WPARAM)dwFlags, (LPARAM)st);
    if (pMin) *pMin = st[0];
    if (pMax) *pMax = st[1];
    return count;
}

// Symbol: ?GetRange@CMonthCalCtrl@@QEBAKPEAU_SYSTEMTIME@@0@Z
extern "C" unsigned long MS_ABI impl__GetRange_CMonthCalCtrl__QEBAKPEAU_SYSTEMTIME__0_Z(
    const CWnd* pThis, SYSTEMTIME* pMin, SYSTEMTIME* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = (DWORD)::SendMessageW(MonthCalHwnd(pThis), MCM_GETRANGE, 0, (LPARAM)st);
    if ((flags & GDTR_MIN) && pMin) *pMin = st[0];
    if ((flags & GDTR_MAX) && pMax) *pMax = st[1];
    return flags;
}

// Symbol: ?GetRange@CMonthCalCtrl@@QEBAKPEAVCOleDateTime@ATL@@0@Z
extern "C" unsigned long MS_ABI impl__GetRange_CMonthCalCtrl__QEBAKPEAVCOleDateTime_ATL__0_Z(
    const CWnd* pThis, COleDateTime* pMin, COleDateTime* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = (DWORD)::SendMessageW(MonthCalHwnd(pThis), MCM_GETRANGE, 0, (LPARAM)st);
    if ((flags & GDTR_MIN) && pMin) {
        DATE d = 0;
        if (StToDate(&st[0], &d)) { pMin->m_dt = d; pMin->m_status = COleDateTime::DT_VALID; }
    }
    if ((flags & GDTR_MAX) && pMax) {
        DATE d = 0;
        if (StToDate(&st[1], &d)) { pMax->m_dt = d; pMax->m_status = COleDateTime::DT_VALID; }
    }
    return flags;
}

// Symbol: ?GetRange@CMonthCalCtrl@@QEBAKPEAVCTime@ATL@@0@Z
extern "C" unsigned long MS_ABI impl__GetRange_CMonthCalCtrl__QEBAKPEAVCTime_ATL__0_Z(
    const CWnd* pThis, void* pMin, void* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = (DWORD)::SendMessageW(MonthCalHwnd(pThis), MCM_GETRANGE, 0, (LPARAM)st);
    if ((flags & GDTR_MIN) && pMin) reinterpret_cast<CTime_atl*>(pMin)->m_time = StToTimeT(&st[0]);
    if ((flags & GDTR_MAX) && pMax) reinterpret_cast<CTime_atl*>(pMax)->m_time = StToTimeT(&st[1]);
    return flags;
}

// Symbol: ?GetSelRange@CMonthCalCtrl@@QEBAHAEAVCOleDateTime@ATL@@0@Z
extern "C" int MS_ABI impl__GetSelRange_CMonthCalCtrl__QEBAHAEAVCOleDateTime_ATL__0_Z(
    const CWnd* pThis, COleDateTime* pMin, COleDateTime* pMax) {
    SYSTEMTIME st[2] = {};
    if (!::SendMessageW(MonthCalHwnd(pThis), MCM_GETSELRANGE, 0, (LPARAM)st)) return FALSE;
    if (pMin) {
        DATE d = 0;
        if (StToDate(&st[0], &d)) { pMin->m_dt = d; pMin->m_status = COleDateTime::DT_VALID; }
    }
    if (pMax) {
        DATE d = 0;
        if (StToDate(&st[1], &d)) { pMax->m_dt = d; pMax->m_status = COleDateTime::DT_VALID; }
    }
    return TRUE;
}

// Symbol: ?GetSelRange@CMonthCalCtrl@@QEBAHAEAVCTime@ATL@@0@Z
extern "C" int MS_ABI impl__GetSelRange_CMonthCalCtrl__QEBAHAEAVCTime_ATL__0_Z(
    const CWnd* pThis, void* pMin, void* pMax) {
    SYSTEMTIME st[2] = {};
    if (!::SendMessageW(MonthCalHwnd(pThis), MCM_GETSELRANGE, 0, (LPARAM)st)) return FALSE;
    if (pMin) reinterpret_cast<CTime_atl*>(pMin)->m_time = StToTimeT(&st[0]);
    if (pMax) reinterpret_cast<CTime_atl*>(pMax)->m_time = StToTimeT(&st[1]);
    return TRUE;
}

// Symbol: ?GetSelRange@CMonthCalCtrl@@QEBAHPEAU_SYSTEMTIME@@0@Z
extern "C" int MS_ABI impl__GetSelRange_CMonthCalCtrl__QEBAHPEAU_SYSTEMTIME__0_Z(
    const CWnd* pThis, SYSTEMTIME* pMin, SYSTEMTIME* pMax) {
    SYSTEMTIME st[2] = {};
    if (!::SendMessageW(MonthCalHwnd(pThis), MCM_GETSELRANGE, 0, (LPARAM)st)) return FALSE;
    if (pMin) *pMin = st[0];
    if (pMax) *pMax = st[1];
    return TRUE;
}

// Symbol: ?GetToday@CMonthCalCtrl@@QEBAHAEAVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__GetToday_CMonthCalCtrl__QEBAHAEAVCOleDateTime_ATL___Z(
    const CWnd* pThis, COleDateTime* pDateTime) {
    if (!pDateTime) return FALSE;
    SYSTEMTIME st = {};
    if (!::SendMessageW(MonthCalHwnd(pThis), MCM_GETTODAY, 0, (LPARAM)&st)) return FALSE;
    DATE d = 0;
    if (!StToDate(&st, &d)) return FALSE;
    pDateTime->m_dt = d;
    pDateTime->m_status = COleDateTime::DT_VALID;
    return TRUE;
}

// Symbol: ?GetToday@CMonthCalCtrl@@QEBAHAEAVCTime@ATL@@@Z
extern "C" int MS_ABI impl__GetToday_CMonthCalCtrl__QEBAHAEAVCTime_ATL___Z(
    const CWnd* pThis, void* pDateTime) {
    if (!pDateTime) return FALSE;
    SYSTEMTIME st = {};
    if (!::SendMessageW(MonthCalHwnd(pThis), MCM_GETTODAY, 0, (LPARAM)&st)) return FALSE;
    reinterpret_cast<CTime_atl*>(pDateTime)->m_time = StToTimeT(&st);
    return TRUE;
}

// Symbol: ?SetCurSel@CMonthCalCtrl@@QEAAHAEBVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__SetCurSel_CMonthCalCtrl__QEAAHAEBVCOleDateTime_ATL___Z(
    CWnd* pThis, const COleDateTime* pDateTime) {
    if (!pDateTime) return FALSE;
    SYSTEMTIME st = {};
    if (!DateToSt(pDateTime->m_dt, &st)) return FALSE;
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETCURSEL, 0, (LPARAM)&st);
}

// Symbol: ?SetCurSel@CMonthCalCtrl@@QEAAHAEBVCTime@ATL@@@Z
extern "C" int MS_ABI impl__SetCurSel_CMonthCalCtrl__QEAAHAEBVCTime_ATL___Z(
    CWnd* pThis, const void* pDateTime) {
    if (!pDateTime) return FALSE;
    SYSTEMTIME st = {};
    TimeTToSt(reinterpret_cast<const CTime_atl*>(pDateTime)->m_time, &st);
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETCURSEL, 0, (LPARAM)&st);
}

// Symbol: ?SetDayState@CMonthCalCtrl@@QEAAHHPEAK@Z
extern "C" int MS_ABI impl__SetDayState_CMonthCalCtrl__QEAAHHPEAK_Z(
    CWnd* pThis, int iMonths, unsigned long* pArray) {
    if (!pArray || iMonths <= 0) return FALSE;
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETDAYSTATE, (WPARAM)iMonths, (LPARAM)pArray);
}

// Symbol: ?SetFirstDayOfWeek@CMonthCalCtrl@@QEAAHHPEAH@Z
extern "C" int MS_ABI impl__SetFirstDayOfWeek_CMonthCalCtrl__QEAAHHPEAH_Z(
    CWnd* pThis, int iDay, int* pOldFirstDayOfWeek) {
    BOOL ok = (BOOL)::SendMessageW(MonthCalHwnd(pThis), MCM_SETFIRSTDAYOFWEEK, 0, MAKELONG(iDay, 0));
    if (pOldFirstDayOfWeek) {
        DWORD dw = 0;
        ::SendMessageW(MonthCalHwnd(pThis), MCM_GETFIRSTDAYOFWEEK, 0, (LPARAM)&dw);
        *pOldFirstDayOfWeek = (int)(short)LOWORD(dw);
    }
    return ok;
}

// Symbol: ?SetRange@CMonthCalCtrl@@QEAAHPEBVCOleDateTime@ATL@@0@Z
extern "C" int MS_ABI impl__SetRange_CMonthCalCtrl__QEAAHPEBVCOleDateTime_ATL__0_Z(
    CWnd* pThis, const COleDateTime* pMin, const COleDateTime* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = 0;
    if (pMin) { flags |= GDTR_MIN; DateToSt(pMin->m_dt, &st[0]); }
    if (pMax) { flags |= GDTR_MAX; DateToSt(pMax->m_dt, &st[1]); }
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETRANGE, (WPARAM)flags, (LPARAM)st);
}

// Symbol: ?SetRange@CMonthCalCtrl@@QEAAHPEBVCTime@ATL@@0@Z
extern "C" int MS_ABI impl__SetRange_CMonthCalCtrl__QEAAHPEBVCTime_ATL__0_Z(
    CWnd* pThis, const void* pMin, const void* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = 0;
    if (pMin) { flags |= GDTR_MIN; TimeTToSt(reinterpret_cast<const CTime_atl*>(pMin)->m_time, &st[0]); }
    if (pMax) { flags |= GDTR_MAX; TimeTToSt(reinterpret_cast<const CTime_atl*>(pMax)->m_time, &st[1]); }
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETRANGE, (WPARAM)flags, (LPARAM)st);
}

// Symbol: ?SetRange@CMonthCalCtrl@@QEAAHQEAU_SYSTEMTIME@@0@Z
extern "C" int MS_ABI impl__SetRange_CMonthCalCtrl__QEAAHQEAU_SYSTEMTIME__0_Z(
    CWnd* pThis, const SYSTEMTIME* pMin, const SYSTEMTIME* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = 0;
    if (pMin) { flags |= GDTR_MIN; st[0] = *pMin; }
    if (pMax) { flags |= GDTR_MAX; st[1] = *pMax; }
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETRANGE, (WPARAM)flags, (LPARAM)st);
}

// Symbol: ?SetSelRange@CMonthCalCtrl@@QEAAHAEBVCOleDateTime@ATL@@0@Z
extern "C" int MS_ABI impl__SetSelRange_CMonthCalCtrl__QEAAHAEBVCOleDateTime_ATL__0_Z(
    CWnd* pThis, const COleDateTime* pMin, const COleDateTime* pMax) {
    SYSTEMTIME st[2] = {};
    if (pMin) DateToSt(pMin->m_dt, &st[0]);
    if (pMax) DateToSt(pMax->m_dt, &st[1]);
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETSELRANGE, 0, (LPARAM)st);
}

// Symbol: ?SetSelRange@CMonthCalCtrl@@QEAAHAEBVCTime@ATL@@0@Z
extern "C" int MS_ABI impl__SetSelRange_CMonthCalCtrl__QEAAHAEBVCTime_ATL__0_Z(
    CWnd* pThis, const void* pMin, const void* pMax) {
    SYSTEMTIME st[2] = {};
    if (pMin) TimeTToSt(reinterpret_cast<const CTime_atl*>(pMin)->m_time, &st[0]);
    if (pMax) TimeTToSt(reinterpret_cast<const CTime_atl*>(pMax)->m_time, &st[1]);
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETSELRANGE, 0, (LPARAM)st);
}

// Symbol: ?SetSelRange@CMonthCalCtrl@@QEAAHQEAU_SYSTEMTIME@@0@Z
extern "C" int MS_ABI impl__SetSelRange_CMonthCalCtrl__QEAAHQEAU_SYSTEMTIME__0_Z(
    CWnd* pThis, const SYSTEMTIME* pMin, const SYSTEMTIME* pMax) {
    SYSTEMTIME st[2] = {};
    if (pMin) st[0] = *pMin;
    if (pMax) st[1] = *pMax;
    return (int)::SendMessageW(MonthCalHwnd(pThis), MCM_SETSELRANGE, 0, (LPARAM)st);
}

// Symbol: ?SetToday@CMonthCalCtrl@@QEAAXAEBVCOleDateTime@ATL@@@Z
extern "C" void MS_ABI impl__SetToday_CMonthCalCtrl__QEAAXAEBVCOleDateTime_ATL___Z(
    CWnd* pThis, const COleDateTime* pDateTime) {
    if (!pDateTime) return;
    SYSTEMTIME st = {};
    if (!DateToSt(pDateTime->m_dt, &st)) return;
    ::SendMessageW(MonthCalHwnd(pThis), MCM_SETTODAY, 0, (LPARAM)&st);
}

// Symbol: ?SetToday@CMonthCalCtrl@@QEAAXPEBVCTime@ATL@@@Z
extern "C" void MS_ABI impl__SetToday_CMonthCalCtrl__QEAAXPEBVCTime_ATL___Z(
    CWnd* pThis, const void* pDateTime) {
    if (!pDateTime) return;
    SYSTEMTIME st = {};
    TimeTToSt(reinterpret_cast<const CTime_atl*>(pDateTime)->m_time, &st);
    ::SendMessageW(MonthCalHwnd(pThis), MCM_SETTODAY, 0, (LPARAM)&st);
}

// Symbol: ?SizeMinReq@CMonthCalCtrl@@QEAAHH@Z
extern "C" int MS_ABI impl__SizeMinReq_CMonthCalCtrl__QEAAHH_Z(
    CWnd* pThis, int bRect) {
    RECT rcMin = {};
    ::SendMessageW(MonthCalHwnd(pThis), MCM_GETMINREQRECT, 0, (LPARAM)&rcMin);
    if (bRect) {
        RECT rcWnd = {};
        ::GetWindowRect(MonthCalHwnd(pThis), &rcWnd);
        int minW = rcMin.right - rcMin.left;
        int minH = rcMin.bottom - rcMin.top;
        int w = rcWnd.right - rcWnd.left;
        int h = rcWnd.bottom - rcWnd.top;
        return (w >= minW && h >= minH);
    }
    return (rcMin.right - rcMin.left) != 0;
}
