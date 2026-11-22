#define OPENMFC_APPCORE_IMPL
#include "detail/DockContextSupport.h"
#include <cstdlib>

extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl___1CDockContext__UEAA_XZ(void*);
extern "C" void impl__StartDrag_CDockContext__UEAAXVCPoint___Z(void*, long long);
extern "C" void impl__StartResize_CDockContext__UEAAXHVCPoint___Z(void*, int, long long);
extern "C" void impl__ToggleDocking_CDockContext__UEAAXXZ(void*);
extern "C" int impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(void*, CControlBar*, int, int);

namespace {
using openmfc::detail::dock::DockContext;
constexpr DWORD kCBRS_ALIGN_TOP = 0x2000;
constexpr DWORD kCBRS_ALIGN_LEFT = 0x1000;
constexpr DWORD kCBRS_ORIENT_HORZ = 0xa000;
constexpr DWORD kCBRS_SIZE_DYNAMIC = 0x04;
constexpr DWORD kCBRS_FLOAT_MULTI = 0x40;
DockContext& Ctx(void* context) { return *static_cast<DockContext*>(context); }

// The scalar deleting destructor (retail vftable slot 3, 0x21e830, mfc140u):
//     ~CDockContext();                               // 0x21e870
//     if (flags & 1) operator delete(this, 0xc8);    // 0x2b77b0 -> jmp 0x27c0 -> IAT free
// The retail sized delete is a jump into the CRT's free; OpenMFC's exported
// operator new (detail/MemcoreSupport.cpp) is std::malloc, so std::free here
// pairs with a client `new CDockContext(...)` that went through the DLL's
// operator new.
void* DockContextDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CDockContext__UEAA_XZ(pThis);
    if (flags & 1u) std::free(pThis);
    return pThis;
}

// MSVC-layout vtable, same slot order as the retail vftable 0x180328f98
// (mfc140u; vtdump_u.py): 0 StartDrag 0x21e8d0, 1 StartResize 0x21ef00,
// 2 ToggleDocking 0x21f2c0, 3 scalar deleting destructor 0x21e830.
void* const g_DockContextVtbl[4] = {
    reinterpret_cast<void*>(&impl__StartDrag_CDockContext__UEAAXVCPoint___Z),
    reinterpret_cast<void*>(&impl__StartResize_CDockContext__UEAAXHVCPoint___Z),
    reinterpret_cast<void*>(&impl__ToggleDocking_CDockContext__UEAAXXZ),
    reinterpret_cast<void*>(&DockContextDeletingDtor),
};

} // namespace

// Transcribed from retail RVA 0x21e780 (mfc140u):
//     vfptr = CDockContext vftable;
//     m_pBar = pBar;
//     m_ptLast = m_rectLast = m_sizeLast = 0;                         // qwords 0x08..0x27
//     m_rectDragHorz = m_rectDragVert = m_rectFrameDragHorz/Vert = 0; // qwords 0x2c..0x6b
//     m_rectMRUDockPos = 0;  m_ptMRUFloatPos = 0;                     // 0xac, 0xb4, 0xc0
//     m_pDockSite = pBar->m_pDockSite;                                // +0x130
//     m_uMRUDockID = 0;
//     if (pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
//         m_dwMRUFloatStyle = pBar->m_dwStyle & (CBRS_ALIGN_TOP | CBRS_SIZE_DYNAMIC);
//     else if (pBar->m_dwStyle & CBRS_ORIENT_HORZ)
//         m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (pBar->m_dwStyle & CBRS_FLOAT_MULTI);
//     else
//         m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (pBar->m_dwStyle & CBRS_FLOAT_MULTI);
//     m_ptMRUFloatPos.x = CW_USEDEFAULT;                              // 0x80000000
//     m_pDC = NULL;
// Retail leaves m_bDitherLast, m_dwDockStyle, m_dwOverDockStyle, m_dwStyle,
// m_bFlip, m_bForceFrame, m_bDragging and m_nHitTest unwritten (InitLoop /
// StartDrag / StartResize set them before use); so does this body.  The
// vfptr is this file's MSVC-layout vtable (see the file header).  pBar is
// dereferenced without a NULL test, as in retail.
// Symbol: ??0CDockContext@@QEAA@PEAVCControlBar@@@Z
extern "C" void* impl___0CDockContext__QEAA_PEAVCControlBar___Z(void* pThis, CControlBar* pBar) {
    DockContext& c = Ctx(pThis);
    c.vfptr = g_DockContextVtbl;
    c.m_pBar = pBar;
    c.m_ptLast = POINT{ 0, 0 };
    c.m_rectLast = RECT{ 0, 0, 0, 0 };
    c.m_sizeLast = SIZE{ 0, 0 };
    c.m_rectDragHorz = RECT{ 0, 0, 0, 0 };
    c.m_rectDragVert = RECT{ 0, 0, 0, 0 };
    c.m_rectFrameDragHorz = RECT{ 0, 0, 0, 0 };
    c.m_rectFrameDragVert = RECT{ 0, 0, 0, 0 };
    c.m_rectMRUDockPos = RECT{ 0, 0, 0, 0 };
    c.m_ptMRUFloatPos = POINT{ 0, 0 };
    c.m_pDockSite = static_cast<CFrameWnd*>(pBar->m_pDockSite);
    c.m_uMRUDockID = 0;
    const DWORD dwStyle = pBar->m_dwStyle;
    if (dwStyle & kCBRS_SIZE_DYNAMIC) {
        c.m_dwMRUFloatStyle = dwStyle & (kCBRS_ALIGN_TOP | kCBRS_SIZE_DYNAMIC);
    } else if (dwStyle & kCBRS_ORIENT_HORZ) {
        c.m_dwMRUFloatStyle = (dwStyle & kCBRS_FLOAT_MULTI) | kCBRS_ALIGN_TOP;
    } else {
        c.m_dwMRUFloatStyle = (dwStyle & kCBRS_FLOAT_MULTI) | kCBRS_ALIGN_LEFT;
    }
    c.m_ptMRUFloatPos.x = static_cast<LONG>(CW_USEDEFAULT);
    c.m_pDC = nullptr;
    return pThis;
}

// Transcribed from retail RVA 0x21e870 (mfc140u):
//     vfptr = CDockContext vftable;
//     if (m_pBar->m_pDockBar != NULL)                                 // +0x70 -> +0x138
//         m_pBar->m_pDockBar->RemoveControlBar(m_pBar, -1, FALSE);   // 0x1d8a10
// m_pBar is dereferenced without a NULL test, as in retail.
// Symbol: ??1CDockContext@@UEAA@XZ
extern "C" void impl___1CDockContext__UEAA_XZ(void* pThis) {
    DockContext& c = Ctx(pThis);
    c.vfptr = g_DockContextVtbl;
    if (c.m_pBar->m_pDockBar != nullptr) {
        impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(c.m_pBar->m_pDockBar, c.m_pBar, -1, FALSE);
    }
}

// Symbol: ?EnableDocking@CControlBar@@QEAAXK@Z
extern "C" void impl__EnableDocking_CControlBar__QEAAXK_Z(CControlBar* bar, DWORD style) {
    bar->m_dwDockStyle = style;
    if (!bar->m_pDockContext) {
        void* memory = impl___2_YAPEAX_K_Z(sizeof(DockContext));
        if (memory) {
            try {
                bar->m_pDockContext = impl___0CDockContext__QEAA_PEAVCControlBar___Z(memory, bar);
            } catch (...) {
                std::free(memory);
                throw;
            }
        }
    }
    if (!bar->m_hWndOwner) bar->m_hWndOwner = ::GetParent(bar->m_hWnd);
}
