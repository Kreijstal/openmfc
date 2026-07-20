// CMFCTabCtrl overrides SetActiveTab/MoveTab/SwapTabs, re-exported under its own
// mangling and sharing the base CMFCBaseTabCtrl side table. Verified through the
// inherited getters, which route to that same table.
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
    typedef int   (__stdcall *SetAct)(void*, int);
    typedef void* (__stdcall *GetWnd0)(const void*);
    typedef int   (__stdcall *GetID)(const void*, int);
    typedef void  (__stdcall *Void2)(void*, int, int);

    // No GetActiveTab is exported, so SetActiveTab is observed through
    // GetActiveWnd (which returns the active tab's window) by giving each tab a
    // distinct window handle.
    auto TCtor = (Ctor)  GetProcAddress(h, "??0CMFCTabCtrl@@QEAA@XZ");
    auto Add   = (AddLbl)GetProcAddress(h, "?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@PEB_WIH@Z");
    auto SetAc = (SetAct)GetProcAddress(h, "?SetActiveTab@CMFCTabCtrl@@UEAAHH@Z");
    auto ActWn = (GetWnd0)GetProcAddress(h, "?GetActiveWnd@CMFCBaseTabCtrl@@UEBAPEAVCWnd@@XZ");
    auto GetId = (GetID) GetProcAddress(h, "?GetTabID@CMFCBaseTabCtrl@@QEBAHH@Z");
    auto Move  = (Void2) GetProcAddress(h, "?MoveTab@CMFCTabCtrl@@UEAAXHH@Z");
    auto Swap  = (Void2) GetProcAddress(h, "?SwapTabs@CMFCTabCtrl@@UEAAXHH@Z");
    if (!TCtor||!Add||!SetAc||!ActWn||!GetId||!Move||!Swap) { printf("missing export(s)\n"); return 1; }

    alignas(16) unsigned char tab[512];
    memset(tab, 0, sizeof tab);
    TCtor(tab);
    void* w0 = (void*)(uintptr_t)0x1000;
    void* w1 = (void*)(uintptr_t)0x2000;
    void* w2 = (void*)(uintptr_t)0x3000;
    Add(tab, w0, L"a", 100, TRUE);   // idx 0
    Add(tab, w1, L"b", 200, TRUE);   // idx 1
    Add(tab, w2, L"c", 300, TRUE);   // idx 2

    // SetActiveTab returns BOOL (CMFCTabCtrl override); observed via GetActiveWnd.
    CHECK(SetAc(tab, 2) == TRUE, "CMFCTabCtrl::SetActiveTab(2) returns TRUE");
    CHECK(ActWn(tab) == w2,      "GetActiveWnd reflects the override (tab 2's window)");
    CHECK(SetAc(tab, 99) == FALSE, "SetActiveTab(out-of-range) returns FALSE");
    CHECK(ActWn(tab) == w2,      "out-of-range SetActiveTab left the active tab unchanged");

    // SwapTabs override exchanges the two tabs on the shared table.
    Swap(tab, 0, 2);
    CHECK(GetId(tab, 0) == 300 && GetId(tab, 2) == 100, "CMFCTabCtrl::SwapTabs(0,2) exchanged tabs");

    // MoveTab override relocates a tab.
    Move(tab, 0, 1);
    CHECK(GetId(tab, 1) == 300, "CMFCTabCtrl::MoveTab(0,1) relocated the tab");

    printf("%d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}
