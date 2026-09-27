// CMFCZoomKernel — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CMFCZoomKernel is the resampling-weight table CMFCToolBarImages uses for
// smooth image scaling.  It is not declared in any public MFC header (nor in
// include/openmfc), so every thunk here takes `void* pThis` and the layout is
// pinned in-file below.  All bodies are transcribed from the retail exports;
// RVAs are mfc140u.dll unless stated otherwise (bodies were read from
// mfc140.dll, whose function bytes are identical).
//
// Layout (sizeof 0x18 -- the scalar deleting destructor, vftable slot 0 at
// RVA 0x16aaf0 (mfc140u), frees with `operator delete(this, 0x18)`):
//   +0x00 vfptr      ctor `mov %rax,(%rcx)` (vftable 0x1803184d8, mfc140u)
//   +0x08 m_nCount   UINT; ctor `movl $0,0x8(%rcx)`; Create stores nDst here
//                    and every loop over it compares unsigned (jb/jbe)
//   +0x10 m_pList    array of m_nCount ZoomContribList; ctor `movq $0,0x10(%rcx)`
// ZoomContribList (16 bytes, indexed `(%rax,%r15,8)` with r15 = 2*i):
//   +0x00 count      UINT, number of valid entries in p (cmpl $0 / jbe in Empty)
//   +0x08 p          array of ZoomContributor
// ZoomContributor (16 bytes, indexed `(%r12,%rax,8)` with rax = 2*d):
//   +0x00 pixel      int source pixel index
//   +0x08 weight     double
//
// The filter table (retail 0x1803b2170, mfc140u) is 7 x {proc, width}:
//   0 box 0.5 (0x16a790)    1 triangle 1.0 (0x16a7c0)  2 hermite 1.0 (0x16a7f0)
//   3 bell 1.5 (0x16a830)   4 B-spline 2.0 (0x16a880)  5 Lanczos3 3.0 (0x16a8f0)
//   6 Mitchell 2.0 (0x16a9d0)
// The names are descriptive of the math only; the XFilterType enumerator
// names are not available (no public header, no source on this host).
// Entry 7 of the retail table is unrelated data (its "proc" 0x3298f0 lies in
// .rdata, not .text; width 0.0),
// i.e. the table has exactly 7 filters.  Retail indexes it WITHOUT a bounds
// check; this file deviates by range-checking the type (see FilterEntryFor).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cmath>
#include <cstdlib>
#include <cstdint>

// Forward declarations of this file's own thunks (called before their
// definitions below).
extern "C" void MS_ABI impl__Empty_CMFCZoomKernel__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCZoomKernel__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Create_CMFCZoomKernel__QEAAXJJJJW4XFilterType_1__Z(
    void* pThis, long nSrcSize, long nDstSize, long nSrcOffset, long nSrcLength, int filterType);

namespace {

struct ZoomContributor {
    int pixel;
    double weight;
};
static_assert(sizeof(ZoomContributor) == 0x10, "CMFCZoomKernel contributor is 16 bytes (Create stride)");
static_assert(offsetof(ZoomContributor, pixel) == 0x0, "pixel at +0x0 (`mov %ecx,(%r12,%rax,8)`)");
static_assert(offsetof(ZoomContributor, weight) == 0x8, "weight at +0x8 (`movsd %xmm1,0x8(%r12,%rax,8)`)");

struct ZoomContribList {
    unsigned int count;
    ZoomContributor* p;
};
static_assert(sizeof(ZoomContribList) == 0x10, "CMFCZoomKernel list entry is 16 bytes (Create stride)");
static_assert(offsetof(ZoomContribList, count) == 0x0, "count at +0x0 (`mov %r14d,(%rax,%r15,8)`)");
static_assert(offsetof(ZoomContribList, p) == 0x8, "p at +0x8 (`mov %rax,0x8(%rcx,%r15,8)`)");

struct ZoomKernel {
    const void* vfptr;
    unsigned int m_nCount;
    ZoomContribList* m_pList;
};
static_assert(sizeof(ZoomKernel) == 0x18, "sizeof(CMFCZoomKernel) == 0x18 (deleting dtor frees 0x18)");
static_assert(offsetof(ZoomKernel, m_nCount) == 0x8, "m_nCount at +0x8");
static_assert(offsetof(ZoomKernel, m_pList) == 0x10, "m_pList at +0x10");

inline ZoomKernel& Kernel(void* pThis) { return *static_cast<ZoomKernel*>(pThis); }

// Retail allocates both arrays with MFC's own `operator new` (??2@YAPEAX_K@Z),
// size = 16 * n with an overflow check that saturates the request to SIZE_MAX
// (`mul; cmovb -1`), and frees them with the CRT's `free` (IAT slot
// api-ms-win-crt-heap free).  OpenMFC's exported operator new is std::malloc
// (detail/MemcoreSupport.cpp), so malloc/free is the matching pair.  Retail
// does not NULL-test the result; neither does this file.
inline void* AllocArray16(std::size_t n) {
    const std::size_t bytes = (n > SIZE_MAX / 16u) ? SIZE_MAX : n * 16u;
    return std::malloc(bytes);
}

typedef double (MS_ABI* ZoomFilterFn)(double);

// Each filter transcribed from its retail body (RVAs mfc140u).  |x| is done in
// retail by `if (0 > x) x ^= signbit`; comparisons are written so that a NaN
// argument takes the same branch retail's comisd/jbe sequence takes.

// 0x16a790: return (x > -0.5 && x <= 0.5) ? 1.0 : 0.0.
double MS_ABI BoxFilter(double x) {
    if (x > -0.5 && x <= 0.5) return 1.0;
    return 0.0;
}

// 0x16a7c0: x = |x|; return x < 1 ? 1 - x : 0.
double MS_ABI TriangleFilter(double x) {
    if (x < 0.0) x = -x;
    if (x < 1.0) return 1.0 - x;
    return 0.0;
}

// 0x16a7f0: x = |x|; return x < 1 ? ((2x - 3) * x) * x + 1 : 0.
double MS_ABI HermiteFilter(double x) {
    if (x < 0.0) x = -x;
    if (x < 1.0) return (((x + x) - 3.0) * x) * x + 1.0;
    return 0.0;
}

// 0x16a830: x = |x|; x < 0.5 -> 0.75 - x*x; x < 1.5 -> t = x - 1.5, (t*0.5)*t; else 0.
double MS_ABI BellFilter(double x) {
    if (x < 0.0) x = -x;
    if (x < 0.5) return 0.75 - x * x;
    if (x < 1.5) {
        x = x - 1.5;
        return (x * 0.5) * x;
    }
    return 0.0;
}

// 0x16a880: x = |x|; x < 1 -> ((x*x*0.5)*x - x*x) + 2/3; x < 2 -> t = 2 - x, (t*t*t)/6; else 0.
// 0.6666666666666666 is the retail constant's exact value (bits 0x3fe5555555555555).
double MS_ABI BSplineFilter(double x) {
    if (x < 0.0) x = -x;
    if (x < 1.0) {
        const double tt = x * x;
        return ((tt * 0.5) * x - tt) + 0.6666666666666666;
    }
    if (x < 2.0) {
        x = 2.0 - x;
        return ((x * x) * x) / 6.0;
    }
    return 0.0;
}

// 0x16a8f0: x = |x|; if (x < 3) return sinc(x/3) * sinc(x), with
// sinc(v) = v == 0 ? 1 : sin(v*pi)/(v*pi) (IAT sin); else 0.
double MS_ABI Lanczos3Filter(double x) {
    if (x < 0.0) x = -x;
    if (x < 3.0) {
        const double kPi = 3.141592653589793;   // bits 0x400921fb54442d18
        double a = 1.0;
        if (x != 0.0) {
            const double px = x * kPi;
            a = std::sin(px) / px;
        }
        double t = x / 3.0;
        double b = 1.0;
        if (t != 0.0) {
            t = t * kPi;
            b = std::sin(t) / t;
        }
        return b * a;
    }
    return 0.0;
}

// 0x16a9d0 (Mitchell-Netravali, B = C = 1/3): x = |x|; tt = x*x;
//   x < 1 -> (((tt*x)*7) - tt*12 + 16/3) / 6
//   x < 2 -> ((tt*12) - (tt*x)*(7/3) - x*20 + 32/3) / 6
//   else 0.
// Constants are the retail doubles' exact values: 7/3 = 2.3333333333333335
// (0x4002aaaaaaaaaaab), 16/3 = 5.333333333333333 (0x4015555555555555),
// 32/3 = 10.666666666666666 (0x4025555555555555).
double MS_ABI MitchellFilter(double x) {
    if (x < 0.0) x = -x;
    const double tt = x * x;
    if (x < 1.0) {
        return (((tt * x) * 7.0 - tt * 12.0) + 5.333333333333333) / 6.0;
    }
    if (x < 2.0) {
        return (((tt * 12.0 - (tt * x) * 2.3333333333333335) - x * 20.0) + 10.666666666666666) / 6.0;
    }
    return 0.0;
}

struct ZoomFilterEntry {
    ZoomFilterFn proc;
    double width;
};

const ZoomFilterEntry kZoomFilters[7] = {
    { &BoxFilter,      0.5 },
    { &TriangleFilter, 1.0 },
    { &HermiteFilter,  1.0 },
    { &BellFilter,     1.5 },
    { &BSplineFilter,  2.0 },
    { &Lanczos3Filter, 3.0 },
    { &MitchellFilter, 2.0 },
};

// DEVIATION: retail does `table[(int)type]` with no range check (movslq; no
// cmp), so an out-of-range type reads past the table.  Here it yields NULL
// and callers fall back as documented at each use.
inline const ZoomFilterEntry* FilterEntryFor(int type) {
    if (type < 0 || type >= 7) return nullptr;
    return &kZoomFilters[type];
}

// Scalar deleting destructor, retail vftable slot 0 at RVA 0x16aaf0 (mfc140u):
//     vfptr = CMFCZoomKernel vftable;
//     Empty();                                   // 0x16ae80
//     if (flags & 1) operator delete(this, 0x18);  // sized delete -> CRT free
// (retail inlines ~CMFCZoomKernel, which is exactly the first two lines.)
void* MS_ABI ZoomKernelDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CMFCZoomKernel__UEAA_XZ(pThis);
    if (flags & 1u) std::free(pThis);
    return pThis;
}

// MSVC-layout vtable, same slot order as the retail vftable 0x1803184d8
// (mfc140u), which has a single slot: 0 scalar deleting destructor 0x16aaf0.
// KNOWN GAP: the retail vftable is preceded by an RTTI Complete Object Locator
// (vftable[-1] = 0x18035ffe8, mfc140u); this table has none.
void* const g_ZoomKernelVtbl[1] = {
    reinterpret_cast<void*>(&ZoomKernelDeletingDtor),
};

} // namespace

// Transcribed from retail RVA 0x16aad0 (mfc140u):
//     vfptr = CMFCZoomKernel vftable; m_nCount = 0; m_pList = NULL;
// The vfptr is this file's MSVC-layout vtable (see g_ZoomKernelVtbl).
// Symbol: ??0CMFCZoomKernel@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCZoomKernel__QEAA_XZ(void* pThis) {
    ZoomKernel& k = Kernel(pThis);
    k.m_nCount = 0;
    k.vfptr = g_ZoomKernelVtbl;
    k.m_pList = nullptr;
    return pThis;
}

// Transcribed from retail RVA 0x16ab30 (mfc140u):
//     vfptr = CMFCZoomKernel vftable;
//     Empty();                                   // tail jump to 0x16ae80
// Symbol: ??1CMFCZoomKernel@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCZoomKernel__UEAA_XZ(void* pThis) {
    Kernel(pThis).vfptr = g_ZoomKernelVtbl;
    impl__Empty_CMFCZoomKernel__QEAAXXZ(pThis);
}

// static void CorrectZoomSize(const CSize& sizeSrc, CSize& sizeDst, XZoomType type)
// Transcribed from retail RVA 0x16a720 (mfc140u):
//     if (type == 0) return;
//     double sx = (double)sizeDst.cx / sizeSrc.cx;
//     double sy = (double)sizeDst.cy / sizeSrc.cy;
//     switch (type) {
//     case 1: sx = min(sx, sy);   // minsd; then falls into case 2
//     case 2: sy = sx; break;
//     case 3: sx = sy; break;
//     default: break;              // any other value: independent x/y scale
//     }
//     sizeDst.cx = (int)(sizeSrc.cx * sx);      // cvttsd2si (truncation)
//     sizeDst.cy = (int)(sizeSrc.cy * sy);      // sizeSrc.cy re-read after the cx store
// The XZoomType enumerator names are not available; the values are as above.
// Symbol: ?CorrectZoomSize@CMFCZoomKernel@@SAXAEBVCSize@@AEAV2@W4XZoomType@1@@Z
extern "C" void MS_ABI impl__CorrectZoomSize_CMFCZoomKernel__SAXAEBVCSize__AEAV2_W4XZoomType_1__Z(
    const SIZE* sizeSrc, SIZE* sizeDst, int zoomType) {
    if (zoomType == 0) return;
    const double srcCx = static_cast<double>(sizeSrc->cx);
    double sx = static_cast<double>(sizeDst->cx) / srcCx;
    double sy = static_cast<double>(sizeDst->cy) / static_cast<double>(sizeSrc->cy);
    switch (zoomType) {
    case 1:
        sx = (sx < sy) ? sx : sy;   // minsd %xmm2,%xmm1
        sy = sx;
        break;
    case 2:
        sy = sx;
        break;
    case 3:
        sx = sy;
        break;
    default:
        break;
    }
    sizeDst->cx = static_cast<int>(srcCx * sx);
    sizeDst->cy = static_cast<int>(static_cast<double>(sizeSrc->cy) * sy);
}

// void Create(long nSrcSize, long nDstSize, long nSrcOffset, long nSrcLength, XFilterType type)
// (parameter names are descriptive; roles read from the body.)
// Transcribed from retail RVA 0x16ab40 (mfc140u):
//     if (nSrcSize <= 0) return;  if (nDstSize <= 0) return;
//     Empty();
//     proc = table[type].proc;  width = table[type].width;
//     m_nCount = nDstSize;
//     scale = (double)nDstSize / (double)nSrcSize;
//     m_pList = new[16 * nDstSize];
//     if (scale < 1.0) { bias = 0.25;  fscale = scale; width /= scale; }
//     else             { bias = -0.25; fscale = 1.0; }
//     for (UINT i = 0; i < m_nCount; i++) {
//         center = (double)i / scale;
//         left  = (int)floor(center - width);     // IAT floor
//         right = (int)ceil(center + width);      // IAT ceil
//         n = right - left;
//         m_pList[i].count = 0;
//         if (n + 1 == 0) continue;
//         m_pList[i].p = new[16 * (n + 1)];
//         first = TRUE; wrapped = FALSE; cnt = 0; sum = 0.0;
//         if (left > right) continue;
//         for (j = left; j <= right; j++) {
//             w = proc(((center - j) + bias) * fscale) * fscale;
//             if (w == 0.0) { if (first) continue; else break; }
//             first = FALSE;
//             pix = nSrcOffset + j;
//             if (pix < 0)                { pix = -(pix % nSrcLength); wrapped = TRUE; }
//             else if (pix >= nSrcLength) { pix = nSrcLength - pix % nSrcLength - 1; wrapped = TRUE; }
//             if (wrapped) {                       // merge a mirrored duplicate
//                 for (d = 0; d < cnt; d++)
//                     if (p[d].pixel == pix) { p[d].weight += w; goto add; }
//             }
//             p[cnt].weight = w; p[cnt].pixel = pix; cnt++; m_pList[i].count = cnt;
//         add:
//             sum += w;
//         }
//         if (sum != 0.0)                          // ucomisd: NaN also normalizes
//             for (d = 0; d <= min(m_pList[i].count, n); d++) p[d].weight /= sum;
//     }
// DEVIATIONS:
//  (1) The normalisation loop divides entries [0, count) only.  Retail's bound
//      is inclusive of min(count, n), which (since count <= n + 1) always covers
//      [0, count) and at most one further, never-written slot of the n + 1
//      allocated; skipping that indeterminate slot changes no valid entry.
//  (2) An out-of-range type (retail: unchecked table read) leaves the kernel
//      Empty()'d and returns, instead of reading past the table.
// A zero nSrcLength with a wrapped pixel divides by zero, as in retail (idiv).
// A row whose weights are all zero keeps count 0 and its p allocated; Empty()
// then skips it (retail frees p only when count > 0), a leak also present in
// retail and kept here because a count of 0 is the only marker Empty() has.
// Symbol: ?Create@CMFCZoomKernel@@QEAAXJJJJW4XFilterType@1@@Z
extern "C" void MS_ABI impl__Create_CMFCZoomKernel__QEAAXJJJJW4XFilterType_1__Z(
    void* pThis, long nSrcSize, long nDstSize, long nSrcOffset, long nSrcLength, int filterType) {
    if (nSrcSize <= 0) return;
    if (nDstSize <= 0) return;
    ZoomKernel& k = Kernel(pThis);
    impl__Empty_CMFCZoomKernel__QEAAXXZ(pThis);

    const ZoomFilterEntry* entry = FilterEntryFor(filterType);
    if (entry == nullptr) return;   // DEVIATION (2)
    const ZoomFilterFn proc = entry->proc;
    double width = entry->width;

    const unsigned int nDst = static_cast<unsigned int>(nDstSize);
    k.m_nCount = nDst;
    const double scale = static_cast<double>(nDst) / static_cast<double>(static_cast<int>(nSrcSize));
    k.m_pList = static_cast<ZoomContribList*>(AllocArray16(nDst));

    double fscale = 1.0;
    double bias;
    if (scale < 1.0) {
        bias = 0.25;
        fscale = scale;
        width = width / scale;
    } else {
        bias = -0.25;
    }

    for (unsigned int i = 0; i < k.m_nCount; ++i) {
        const double center = static_cast<double>(i) / scale;
        int j = static_cast<int>(std::floor(center - width));
        const int right = static_cast<int>(std::ceil(center + width));
        const int n = right - j;
        ZoomContribList& row = k.m_pList[i];
        row.count = 0;
        if (n + 1 == 0) continue;
        row.p = static_cast<ZoomContributor*>(
            AllocArray16(static_cast<std::size_t>(static_cast<long long>(n + 1))));
        ZoomContributor* p = row.p;

        bool first = true;
        bool wrapped = false;
        unsigned int cnt = 0;
        double sum = 0.0;
        if (j > right) continue;

        for (; j <= right; ++j) {
            const double w = proc(((center - static_cast<double>(j)) + bias) * fscale) * fscale;
            if (w == 0.0) {
                if (first) continue;
                break;
            }
            first = false;

            int pix = static_cast<int>(nSrcOffset) + j;
            if (pix < 0) {
                pix = -(pix % static_cast<int>(nSrcLength));
                wrapped = true;
            } else if (pix >= static_cast<int>(nSrcLength)) {
                pix = static_cast<int>(nSrcLength) - pix % static_cast<int>(nSrcLength) - 1;
                wrapped = true;
            }

            bool merged = false;
            if (wrapped) {
                for (unsigned int d = 0; d < cnt; ++d) {
                    if (p[d].pixel == pix) {
                        p[d].weight = w + p[d].weight;
                        merged = true;
                        break;
                    }
                }
            }
            if (!merged) {
                p[cnt].weight = w;
                p[cnt].pixel = pix;
                ++cnt;
                row.count = cnt;
            }
            sum += w;
        }

        if (!(sum == 0.0)) {
            for (unsigned int d = 0; d < row.count; ++d)   // DEVIATION (1)
                p[d].weight = p[d].weight / sum;
        }
    }
}

// void Create(long nSrcSize, long nDstSize, XFilterType type)
// Transcribed from retail RVA 0x16ae60 (mfc140u):
//     Create(nSrcSize, nDstSize, 0, nSrcSize, type);
// (r9d <- 0 is the 4th argument nSrcOffset; the incoming edx goes to the
// 5th argument, the first stack slot 0x20(%rsp); the incoming r9d (type)
// goes to the 6th argument, 0x28(%rsp).)
// Symbol: ?Create@CMFCZoomKernel@@QEAAXJJW4XFilterType@1@@Z
extern "C" void MS_ABI impl__Create_CMFCZoomKernel__QEAAXJJW4XFilterType_1__Z(
    void* pThis, long nSrcSize, long nDstSize, int filterType) {
    impl__Create_CMFCZoomKernel__QEAAXJJJJW4XFilterType_1__Z(pThis, nSrcSize, nDstSize, 0, nSrcSize, filterType);
}

// Transcribed from retail RVA 0x16ae80 (mfc140u):
//     if (m_pList == NULL) return;
//     for (UINT i = 0; i < m_nCount; i++)
//         if (m_pList[i].count > 0) free(m_pList[i].p);   // IAT free
//     free(m_pList);
//     m_pList = NULL; m_nCount = 0;
// Symbol: ?Empty@CMFCZoomKernel@@QEAAXXZ
extern "C" void MS_ABI impl__Empty_CMFCZoomKernel__QEAAXXZ(void* pThis) {
    ZoomKernel& k = Kernel(pThis);
    if (k.m_pList == nullptr) return;
    for (unsigned int i = 0; i < k.m_nCount; ++i) {
        if (k.m_pList[i].count > 0) std::free(k.m_pList[i].p);
    }
    std::free(k.m_pList);
    k.m_pList = nullptr;
    k.m_nCount = 0;
}

// static double Filter(XFilterType type, double x)
// Transcribed from retail RVA 0x16aab0 (mfc140u): tail call (CFG dispatch)
//     return table[type].proc(x);
// DEVIATION: out-of-range type returns 0.0 (retail: unchecked table read).
// Symbol: ?Filter@CMFCZoomKernel@@SANW4XFilterType@1@N@Z
extern "C" double MS_ABI impl__Filter_CMFCZoomKernel__SANW4XFilterType_1_N_Z(int filterType, double x) {
    const ZoomFilterEntry* entry = FilterEntryFor(filterType);
    if (entry == nullptr) return 0.0;
    return entry->proc(x);
}

// static double (*FilterProc(XFilterType type))(double)
// Transcribed from retail RVA 0x16aa70 (mfc140u): return table[type].proc;
// DEVIATION: out-of-range type returns NULL (retail: unchecked table read).
// Symbol: ?FilterProc@CMFCZoomKernel@@SAP6ANN@ZW4XFilterType@1@@Z
extern "C" ZoomFilterFn MS_ABI impl__FilterProc_CMFCZoomKernel__SAP6ANN_ZW4XFilterType_1__Z(int filterType) {
    const ZoomFilterEntry* entry = FilterEntryFor(filterType);
    return entry != nullptr ? entry->proc : nullptr;
}

// static double FilterWidth(XFilterType type)
// Transcribed from retail RVA 0x16aa90 (mfc140u): return table[type].width;
// DEVIATION: out-of-range type returns 0.0 (retail: unchecked table read).
// Symbol: ?FilterWidth@CMFCZoomKernel@@SANW4XFilterType@1@@Z
extern "C" double MS_ABI impl__FilterWidth_CMFCZoomKernel__SANW4XFilterType_1__Z(int filterType) {
    const ZoomFilterEntry* entry = FilterEntryFor(filterType);
    return entry != nullptr ? entry->width : 0.0;
}
