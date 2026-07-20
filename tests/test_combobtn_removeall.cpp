// RemoveAllItems completes the CMFCToolBarComboBoxButton item API. Verified
// through the real GetCount/AddItem/GetItem exports on the shared side table.
#include <windows.h>
#include <cstdio>
#include <cstring>

static int pass = 0, fail = 0;
#define CHECK(c, msg) do { if (c) { pass++; printf("  PASS %s\n", msg); } \
                           else { fail++; printf("  FAIL %s\n", msg); } } while (0)

int main()
{
    HMODULE h = LoadLibraryA("openmfc.dll");
    if (!h) { printf("cannot load openmfc.dll\n"); return 1; }

    typedef void* (__stdcall *Ctor)(void*);
    typedef long long (__stdcall *AddItem)(void*, const wchar_t*, unsigned long long);
    typedef long long (__stdcall *GetCount)(const void*);
    typedef const wchar_t* (__stdcall *GetItem)(const void*, int);
    typedef void (__stdcall *RemoveAll)(void*);
    typedef void (__stdcall *SetDDH)(void*, int);
    typedef void (__stdcall *SetStyle)(void*, unsigned int);

    auto CtorF = (Ctor)   GetProcAddress(h, "??0CMFCToolBarComboBoxButton@@QEAA@XZ");
    auto Add   = (AddItem)GetProcAddress(h, "?AddItem@CMFCToolBarComboBoxButton@@UEAA_JPEB_W_K@Z");
    auto Count = (GetCount)GetProcAddress(h, "?GetCount@CMFCToolBarComboBoxButton@@QEBA_JXZ");
    auto Item  = (GetItem)GetProcAddress(h, "?GetItem@CMFCToolBarComboBoxButton@@QEBAPEB_WH@Z");
    auto RmAll = (RemoveAll)GetProcAddress(h, "?RemoveAllItems@CMFCToolBarComboBoxButton@@QEAAXXZ");
    auto DDH   = (SetDDH) GetProcAddress(h, "?SetDropDownHeight@CMFCToolBarComboBoxButton@@QEAAXH@Z");
    auto Style = (SetStyle)GetProcAddress(h, "?SetStyle@CMFCToolBarComboBoxButton@@UEAAXI@Z");
    if (!CtorF||!Add||!Count||!Item||!RmAll||!DDH||!Style) { printf("missing export(s)\n"); return 1; }

    // CMFCToolBarComboBoxButton real sizeof 336; give generous storage.
    alignas(16) unsigned char btn[512];
    memset(btn, 0, sizeof btn);
    CtorF(btn);

    Add(btn, L"one",   0);
    Add(btn, L"two",   0);
    Add(btn, L"three", 0);
    CHECK(Count(btn) == 3, "three items added");

    // Setters must run without disturbing the item list.
    DDH(btn, 120);
    Style(btn, 3);
    CHECK(Count(btn) == 3, "SetDropDownHeight/SetStyle left the item list intact");

    RmAll(btn);
    CHECK(Count(btn) == 0, "RemoveAllItems cleared the item list");

    // The list is usable again afterwards.
    Add(btn, L"fresh", 0);
    CHECK(Count(btn) == 1, "item list works again after RemoveAllItems");
    const wchar_t* it = Item(btn, 0);
    CHECK(it && wcscmp(it, L"fresh") == 0, "re-added item reads back correctly");

    printf("%d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}
