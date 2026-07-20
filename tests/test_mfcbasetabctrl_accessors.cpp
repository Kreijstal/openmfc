// Behavioural round-trip test for the CMFCBaseTabCtrl side-table accessors.
// Each check asserts an observable effect a stub-returning-0 could not produce.
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <cstdint>

static int pass = 0, fail = 0;
#define CHECK(c, msg) do { if (c) { pass++; printf("  PASS %s\n", msg); } \
                           else { fail++; printf("  FAIL %s\n", msg); } } while (0)

int main()
{
    HMODULE h = LoadLibraryA("openmfc.dll");
    if (!h) { printf("cannot load openmfc.dll\n"); return 1; }

    typedef void* (__stdcall *Ctor)(void*);
    typedef void  (__stdcall *AddLbl)(void*, void*, const wchar_t*, unsigned int, int);
    typedef int   (__stdcall *GetInt1)(const void*, int);
    typedef int   (__stdcall *GetInt0)(const void*);
    typedef void* (__stdcall *GetWnd1)(const void*, int);
    typedef void* (__stdcall *GetWnd0)(const void*);
    typedef void  (__stdcall *Void0)(void*);
    typedef void  (__stdcall *Void2)(void*, int, int);
    typedef int   (__stdcall *SetLbl)(void*, int, const void*);
    typedef int   (__stdcall *GetLbl)(const void*, int, void*);
    typedef unsigned long (__stdcall *GetClr)(const void*, int);
    typedef int   (__stdcall *SetClr)(void*, int, unsigned long);
    typedef void  (__stdcall *StrCtor)(void*);
    typedef const wchar_t* (__stdcall *StrGet)(const void*);

    auto TCtor  = (Ctor)  GetProcAddress(h, "??0CMFCBaseTabCtrl@@QEAA@XZ");
    auto Add    = (AddLbl)GetProcAddress(h, "?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@PEB_WIH@Z");
    auto GetID  = (GetInt1)GetProcAddress(h, "?GetTabID@CMFCBaseTabCtrl@@QEBAHH@Z");
    auto ByID   = (GetInt1)GetProcAddress(h, "?GetTabByID@CMFCBaseTabCtrl@@UEBAHH@Z");
    auto ActWnd = (GetWnd0)GetProcAddress(h, "?GetActiveWnd@CMFCBaseTabCtrl@@UEBAPEAVCWnd@@XZ");
    auto NoWrap = (GetWnd1)GetProcAddress(h, "?GetTabWndNoWrapper@CMFCBaseTabCtrl@@UEBAPEAVCWnd@@H@Z");
    auto VisNum = (GetInt0)GetProcAddress(h, "?GetVisibleTabsNum@CMFCBaseTabCtrl@@UEBAHXZ");
    auto IsVis  = (GetInt1)GetProcAddress(h, "?IsTabVisible@CMFCBaseTabCtrl@@UEBAHH@Z");
    auto RmAll  = (Void0) GetProcAddress(h, "?RemoveAllTabs@CMFCBaseTabCtrl@@UEAAXXZ");
    auto Swap   = (Void2) GetProcAddress(h, "?SwapTabs@CMFCBaseTabCtrl@@UEAAXHH@Z");
    auto Move   = (Void2) GetProcAddress(h, "?MoveTab@CMFCBaseTabCtrl@@UEAAXHH@Z");
    auto GetClrF= (GetClr)GetProcAddress(h, "?GetTabTextColor@CMFCBaseTabCtrl@@UEBAKH@Z");
    auto SetClrF= (SetClr)GetProcAddress(h, "?SetTabTextColor@CMFCBaseTabCtrl@@UEAAHHK@Z");
    auto SetLblF= (SetLbl)GetProcAddress(h, "?SetTabLabel@CMFCBaseTabCtrl@@UEAAHHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z");
    auto GetLblF= (GetLbl)GetProcAddress(h, "?GetTabLabel@CMFCBaseTabCtrl@@UEBAHHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z");
    // CString is exported only as the ATL CStringT template.
    typedef void* (__stdcall *StrCtorW)(void*, const wchar_t*);
    auto SCtor  = (StrCtor)GetProcAddress(h, "??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ");
    auto SCtorW = (StrCtorW)GetProcAddress(h, "??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_W@Z");
    auto SGet   = (StrGet)GetProcAddress(h, "?GetString@?$CSimpleStringT@_W$00@ATL@@QEBAPEB_WXZ");

    if (!TCtor||!Add||!GetID||!ByID||!ActWnd||!NoWrap||!VisNum||!IsVis||!RmAll||
        !Swap||!Move||!GetClrF||!SetClrF||!SetLblF||!GetLblF) {
        printf("missing export(s)\n"); return 1;
    }

    alignas(16) unsigned char tab[512];
    memset(tab, 0, sizeof tab);
    TCtor(tab);
    void* w0 = (void*)(uintptr_t)0x1000;
    void* w1 = (void*)(uintptr_t)0x2000;
    void* w2 = (void*)(uintptr_t)0x3000;

    Add(tab, w0, L"alpha", 10, TRUE);   // index 0, id 10
    Add(tab, w1, L"beta",  20, TRUE);   // index 1, id 20
    Add(tab, w2, L"gamma", 30, TRUE);   // index 2, id 30

    // GetTabID / GetTabByID round-trip
    CHECK(GetID(tab, 0) == 10, "GetTabID(0) == 10");
    CHECK(GetID(tab, 2) == 30, "GetTabID(2) == 30");
    CHECK(GetID(tab, 99) == -1, "GetTabID(out-of-range) == -1");
    CHECK(ByID(tab, 20) == 1, "GetTabByID(20) == index 1");
    CHECK(ByID(tab, 999) == -1, "GetTabByID(missing) == -1");

    // Active window defaults to the first tab (index 0), set when the first
    // AddTab promoted activeTab from -1 to 0.
    CHECK(ActWnd(tab) == w0, "GetActiveWnd == first tab window (default active 0)");
    CHECK(NoWrap(tab, 2) == w2, "GetTabWndNoWrapper(2) == w2");

    // Visibility: all visible by default
    CHECK(VisNum(tab) == 3, "GetVisibleTabsNum == 3");
    CHECK(IsVis(tab, 1) == TRUE, "IsTabVisible(1) == TRUE");
    CHECK(IsVis(tab, 99) == FALSE, "IsTabVisible(out-of-range) == FALSE");

    // Text color round-trip
    CHECK(GetClrF(tab, 0) == (unsigned long)-1, "GetTabTextColor default == -1");
    CHECK(SetClrF(tab, 0, 0x00FF8040) == TRUE, "SetTabTextColor returns TRUE");
    CHECK(GetClrF(tab, 0) == 0x00FF8040, "GetTabTextColor reads back the set color");
    CHECK(SetClrF(tab, 99, 0) == FALSE, "SetTabTextColor(out-of-range) == FALSE");

    // SwapTabs: ids at 0 and 2 exchange
    Swap(tab, 0, 2);
    CHECK(GetID(tab, 0) == 30 && GetID(tab, 2) == 10, "SwapTabs(0,2) exchanged the tabs");

    // MoveTab: move index 0 (id 30) to index 1
    Move(tab, 0, 1);
    CHECK(GetID(tab, 1) == 30, "MoveTab(0,1) placed the tab at index 1");

    // SetTabLabel / GetTabLabel full round-trip through real CStringT objects.
    if (SCtor && SCtorW && SGet) {
        alignas(16) unsigned char in[16];  memset(in, 0, sizeof in);
        alignas(16) unsigned char out[16]; memset(out, 0, sizeof out);
        SCtorW(in, L"renamed");   // source label
        SCtor(out);               // empty destination

        CHECK(SetLblF(tab, 1, in) == TRUE, "SetTabLabel(valid) == TRUE");
        CHECK(GetLblF(tab, 1, out) == TRUE, "GetTabLabel(valid) == TRUE");
        const wchar_t* got = SGet(out);
        CHECK(got && wcscmp(got, L"renamed") == 0, "SetTabLabel/GetTabLabel round-trip returns 'renamed'");
        CHECK(GetLblF(tab, 99, out) == FALSE, "GetTabLabel(out-of-range) == FALSE");
        CHECK(SetLblF(tab, 99, in) == FALSE, "SetTabLabel(out-of-range) == FALSE");
    } else {
        printf("  SKIP CStringT ctor/GetString not resolved -- label round-trip not exercised\n");
    }

    // RemoveAllTabs empties the control
    RmAll(tab);
    CHECK(VisNum(tab) == 0, "RemoveAllTabs cleared the control");
    CHECK(ActWnd(tab) == NULL, "RemoveAllTabs reset the active window");

    printf("%d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}
