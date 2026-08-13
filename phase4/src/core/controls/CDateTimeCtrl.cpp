// CDateTimeCtrl — OpenMFC implementation.
// Sources: typed_stubs-generated baseline; Win32 date-time-picker wrappers.

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
HWND DateTimeHwnd(const CWnd* pThis) {
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

// Symbol: ?Create@CDateTimeCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CDateTimeCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CWnd* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID) {
    if (!pThis || !pRect) return FALSE;
    return pThis->CWnd::Create(DATETIMEPICK_CLASSW, L"", dwStyle, *pRect, pParentWnd, nID, nullptr);
}

// Symbol: ?GetRange@CDateTimeCtrl@@QEBAKPEAVCOleDateTime@ATL@@0@Z
extern "C" unsigned long MS_ABI impl__GetRange_CDateTimeCtrl__QEBAKPEAVCOleDateTime_ATL__0_Z(
    const CWnd* pThis, COleDateTime* pMin, COleDateTime* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = (DWORD)::SendMessageW(DateTimeHwnd(pThis), DTM_GETRANGE, 0, (LPARAM)st);
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

// Symbol: ?GetRange@CDateTimeCtrl@@QEBAKPEAVCTime@ATL@@0@Z
extern "C" unsigned long MS_ABI impl__GetRange_CDateTimeCtrl__QEBAKPEAVCTime_ATL__0_Z(
    const CWnd* pThis, void* pMin, void* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = (DWORD)::SendMessageW(DateTimeHwnd(pThis), DTM_GETRANGE, 0, (LPARAM)st);
    if ((flags & GDTR_MIN) && pMin) reinterpret_cast<CTime_atl*>(pMin)->m_time = StToTimeT(&st[0]);
    if ((flags & GDTR_MAX) && pMax) reinterpret_cast<CTime_atl*>(pMax)->m_time = StToTimeT(&st[1]);
    return flags;
}

// Symbol: ?GetTime@CDateTimeCtrl@@QEBAHAEAVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__GetTime_CDateTimeCtrl__QEBAHAEAVCOleDateTime_ATL___Z(
    const CWnd* pThis, COleDateTime* pTime) {
    if (!pTime) return FALSE;
    SYSTEMTIME st = {};
    DWORD result = (DWORD)::SendMessageW(DateTimeHwnd(pThis), DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
    if (result == GDT_NONE) return FALSE;
    DATE d = 0;
    if (!StToDate(&st, &d)) return FALSE;
    pTime->m_dt = d;
    pTime->m_status = COleDateTime::DT_VALID;
    return TRUE;
}

// Symbol: ?GetTime@CDateTimeCtrl@@QEBAKAEAVCTime@ATL@@@Z
extern "C" unsigned long MS_ABI impl__GetTime_CDateTimeCtrl__QEBAKAEAVCTime_ATL___Z(
    const CWnd* pThis, void* pTime) {
    if (!pTime) return 0;
    SYSTEMTIME st = {};
    DWORD result = (DWORD)::SendMessageW(DateTimeHwnd(pThis), DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
    if (result == GDT_NONE) return 0;
    reinterpret_cast<CTime_atl*>(pTime)->m_time = StToTimeT(&st);
    return 1;
}

// Symbol: ?SetRange@CDateTimeCtrl@@QEAAHPEBVCOleDateTime@ATL@@0@Z
extern "C" int MS_ABI impl__SetRange_CDateTimeCtrl__QEAAHPEBVCOleDateTime_ATL__0_Z(
    CWnd* pThis, const COleDateTime* pMin, const COleDateTime* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = 0;
    if (pMin) { flags |= GDTR_MIN; DateToSt(pMin->m_dt, &st[0]); }
    if (pMax) { flags |= GDTR_MAX; DateToSt(pMax->m_dt, &st[1]); }
    return (int)::SendMessageW(DateTimeHwnd(pThis), DTM_SETRANGE, (WPARAM)flags, (LPARAM)st);
}

// Symbol: ?SetRange@CDateTimeCtrl@@QEAAHPEBVCTime@ATL@@0@Z
extern "C" int MS_ABI impl__SetRange_CDateTimeCtrl__QEAAHPEBVCTime_ATL__0_Z(
    CWnd* pThis, const void* pMin, const void* pMax) {
    SYSTEMTIME st[2] = {};
    DWORD flags = 0;
    if (pMin) { flags |= GDTR_MIN; TimeTToSt(reinterpret_cast<const CTime_atl*>(pMin)->m_time, &st[0]); }
    if (pMax) { flags |= GDTR_MAX; TimeTToSt(reinterpret_cast<const CTime_atl*>(pMax)->m_time, &st[1]); }
    return (int)::SendMessageW(DateTimeHwnd(pThis), DTM_SETRANGE, (WPARAM)flags, (LPARAM)st);
}

// Symbol: ?SetTime@CDateTimeCtrl@@QEAAHAEBVCOleDateTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTime_CDateTimeCtrl__QEAAHAEBVCOleDateTime_ATL___Z(
    CWnd* pThis, const COleDateTime* pTime) {
    if (!pTime) return FALSE;
    SYSTEMTIME st = {};
    if (!DateToSt(pTime->m_dt, &st)) return FALSE;
    return (int)::SendMessageW(DateTimeHwnd(pThis), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&st);
}

// Symbol: ?SetTime@CDateTimeCtrl@@QEAAHPEAU_SYSTEMTIME@@@Z
extern "C" int MS_ABI impl__SetTime_CDateTimeCtrl__QEAAHPEAU_SYSTEMTIME___Z(
    CWnd* pThis, const SYSTEMTIME* pTime) {
    if (!pTime) return FALSE;
    return (int)::SendMessageW(DateTimeHwnd(pThis), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)pTime);
}

// Symbol: ?SetTime@CDateTimeCtrl@@QEAAHPEBVCTime@ATL@@@Z
extern "C" int MS_ABI impl__SetTime_CDateTimeCtrl__QEAAHPEBVCTime_ATL___Z(
    CWnd* pThis, const void* pTime) {
    if (!pTime) return FALSE;
    SYSTEMTIME st = {};
    TimeTToSt(reinterpret_cast<const CTime_atl*>(pTime)->m_time, &st);
    return (int)::SendMessageW(DateTimeHwnd(pThis), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&st);
}
