// Regression test: embedded C++ subobjects must survive constructor memsets.
//
// Several OpenMFC constructors zero their retail member block wholesale so the
// object matches what a real client would inspect. That is fine for scalars,
// but parts of those spans hold genuine C++ objects, and memset destroys their
// representation -- including the vfptr of any polymorphic member. When that
// happens the object looks correct to a byte-level layout test yet faults on
// the first virtual call, destruction included.
//
// Two constructors had exactly this defect (found in review of PR #334):
//   CMFCToolBarImages  -- embeds CDC m_dcMem and CBitmap m_bmpMem
//   CMFCToolBar        -- m_bLocked sits at 4280, but the three CObLists
//                         (4488/4544/4600) and CPen m_penDrag (4800) all
//                         follow it, so they were inside the zeroed span
// Measured against the pre-fix build this left 8 null vfptrs per object pair.
// Both constructors now re-establish the affected members with placement new.
//
// The test deliberately checks the vfptr slot rather than calling a virtual
// method: a null-dispatch crash under Wine is far less diagnosable than a named
// failing assertion, and the vfptr is the thing the fix actually restores.
//
// Note this test loads the DLL with LoadLibrary/GetProcAddress instead of
// linking it. MinGW cannot link OpenMFC's MSVC-mangled exports, but it can
// resolve them dynamically by their exact mangled name, which is what makes a
// MinGW/Wine functionality test possible at all.

#include <windows.h>
#include <cstdio>
#include <cstring>

static int pass = 0, fail = 0;
#define CHECK(cond, msg) do { \
        if (cond) { pass++; printf("  PASS %s\n", (msg)); } \
        else      { fail++; printf("  FAIL %s\n", (msg)); } \
    } while (0)

// Retail offsets. These are the same values asserted by the static_asserts in
// phase4/src -- if a layout change breaks them, this test fails loudly rather
// than silently probing the wrong bytes.
#define I_DCMEM         72      // CMFCToolBarImages::m_dcMem   (CDC)
#define I_BMPMEM        192     // CMFCToolBarImages::m_bmpMem  (CBitmap)
#define I_SIZE          408     // sizeof(CMFCToolBarImages)

#define T_IMAGESLOCKED  1016    // CMFCToolBar::m_ImagesLocked
#define T_BUTTONS       4488    // CMFCToolBar::m_Buttons            (CObList)
#define T_ORIGBUTTONS   4544    // CMFCToolBar::m_OrigButtons        (CObList)
#define T_ORIGRESET     4600    // CMFCToolBar::m_OrigResetButtons   (CObList)
#define T_PENDRAG       4800    // CMFCToolBar::m_penDrag            (CPen)
#define T_SIZE          4944    // sizeof(CMFCToolBar)

#define VPTR(p, off) (*(void**)((unsigned char*)(p) + (off)))

int main(int argc, char** argv)
{
    const char* dll = (argc > 1) ? argv[1] : "openmfc.dll";
    HMODULE h = LoadLibraryA(dll);
    if (!h) {
        printf("cannot load %s (error %lu)\n", dll, GetLastError());
        return 1;
    }

    typedef void* (__stdcall *CtorFn)(void*);
    CtorFn ImgCtor = (CtorFn)GetProcAddress(h, "??0CMFCToolBarImages@@QEAA@XZ");
    CtorFn TbCtor  = (CtorFn)GetProcAddress(h, "??0CMFCToolBar@@QEAA@XZ");
    if (!ImgCtor || !TbCtor) {
        printf("missing constructor export(s)\n");
        return 1;
    }

    // --- CMFCToolBarImages -------------------------------------------------
    // Storage is pre-zeroed so a constructor that fails to establish a vfptr
    // leaves a null there rather than stale stack data that might look valid.
    // alignas: unsigned char[] guarantees only 1-byte alignment, and these
    // buffers hold objects full of pointers. Current compilers happen to align
    // large statics generously, but constructing into under-aligned storage is
    // undefined behaviour and not something to leave to chance.
    alignas(16) static unsigned char img[I_SIZE];
    memset(img, 0, sizeof img);
    ImgCtor(img);
    CHECK(VPTR(img, 0)        != NULL, "CMFCToolBarImages vfptr set");
    CHECK(VPTR(img, I_DCMEM)  != NULL, "CMFCToolBarImages::m_dcMem (CDC) vfptr survives memset");
    CHECK(VPTR(img, I_BMPMEM) != NULL, "CMFCToolBarImages::m_bmpMem (CBitmap) vfptr survives memset");

    // --- CMFCToolBar -------------------------------------------------------
    alignas(16) static unsigned char tb[T_SIZE];
    memset(tb, 0, sizeof tb);
    TbCtor(tb);
    CHECK(VPTR(tb, 0)             != NULL, "CMFCToolBar vfptr set");
    CHECK(VPTR(tb, T_BUTTONS)     != NULL, "CMFCToolBar::m_Buttons (CObList) vfptr survives memset");
    CHECK(VPTR(tb, T_ORIGBUTTONS) != NULL, "CMFCToolBar::m_OrigButtons (CObList) vfptr survives memset");
    CHECK(VPTR(tb, T_ORIGRESET)   != NULL, "CMFCToolBar::m_OrigResetButtons (CObList) vfptr survives memset");
    CHECK(VPTR(tb, T_PENDRAG)     != NULL, "CMFCToolBar::m_penDrag (CPen) vfptr survives memset");

    // The eight embedded CMFCToolBarImages members precede m_bLocked, so they
    // are outside CMFCToolBar's memset span -- but they are constructed by the
    // very constructor tested above, so a regression there surfaces here too.
    CHECK(VPTR(tb, T_IMAGESLOCKED)            != NULL, "CMFCToolBar::m_ImagesLocked vfptr set");
    CHECK(VPTR(tb, T_IMAGESLOCKED + I_DCMEM)  != NULL, "m_ImagesLocked.m_dcMem vfptr set");
    CHECK(VPTR(tb, T_IMAGESLOCKED + I_BMPMEM) != NULL, "m_ImagesLocked.m_bmpMem vfptr set");

    printf("%d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}
