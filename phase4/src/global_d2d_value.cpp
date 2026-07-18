// Real constructors for the Direct2D value wrappers CD2DPointU / CD2DSizeU /
// CD2DRectU (afxrendertarget.h). Each derives directly from the underlying D2D
// POD (D2D1_POINT_2U / D2D1_SIZE_U / D2D1_RECT_U), so a constructor just copies
// UINT32 fields -- no vtable. Verified byte-exact vs real mfc140u.dll.
#include <cstdint>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
struct PT { uint32_t x, y; };            // D2D1_POINT_2U
struct SZ { uint32_t width, height; };   // D2D1_SIZE_U
struct RC { uint32_t left, top, right, bottom; }; // D2D1_RECT_U
struct CPt { int32_t x, y; };            // CPoint
struct CSz { int32_t cx, cy; };          // CSize
struct CRc { int32_t left, top, right, bottom; }; // CRect
}

extern "C" {

// ---- CD2DPointU ----
// Symbol: ??0CD2DPointU@@QEAA@AEBUD2D_POINT_2U@@@Z
void* MS_ABI impl___0CD2DPointU__QEAA_AEBUD2D_POINT_2U___Z(void* p, const PT* src) { *(PT*)p = *src; return p; }
// Symbol: ??0CD2DPointU@@QEAA@PEBUD2D_POINT_2U@@@Z
void* MS_ABI impl___0CD2DPointU__QEAA_PEBUD2D_POINT_2U___Z(void* p, const PT* src) { *(PT*)p = *src; return p; }
// Symbol: ??0CD2DPointU@@QEAA@AEBVCPoint@@@Z
void* MS_ABI impl___0CD2DPointU__QEAA_AEBVCPoint___Z(void* p, const CPt* pt) { ((PT*)p)->x = (uint32_t)pt->x; ((PT*)p)->y = (uint32_t)pt->y; return p; }
// Symbol: ??0CD2DPointU@@QEAA@II@Z
void* MS_ABI impl___0CD2DPointU__QEAA_II_Z(void* p, uint32_t x, uint32_t y) { ((PT*)p)->x = x; ((PT*)p)->y = y; return p; }

// ---- CD2DSizeU ----
// Symbol: ??0CD2DSizeU@@QEAA@AEBUD2D_SIZE_U@@@Z
void* MS_ABI impl___0CD2DSizeU__QEAA_AEBUD2D_SIZE_U___Z(void* p, const SZ* src) { *(SZ*)p = *src; return p; }
// Symbol: ??0CD2DSizeU@@QEAA@PEBUD2D_SIZE_U@@@Z
void* MS_ABI impl___0CD2DSizeU__QEAA_PEBUD2D_SIZE_U___Z(void* p, const SZ* src) { *(SZ*)p = *src; return p; }
// Symbol: ??0CD2DSizeU@@QEAA@AEBVCSize@@@Z
void* MS_ABI impl___0CD2DSizeU__QEAA_AEBVCSize___Z(void* p, const CSz* sz) { ((SZ*)p)->width = (uint32_t)sz->cx; ((SZ*)p)->height = (uint32_t)sz->cy; return p; }
// Symbol: ??0CD2DSizeU@@QEAA@II@Z
void* MS_ABI impl___0CD2DSizeU__QEAA_II_Z(void* p, uint32_t cx, uint32_t cy) { ((SZ*)p)->width = cx; ((SZ*)p)->height = cy; return p; }

// ---- CD2DRectU ----
// Symbol: ??0CD2DRectU@@QEAA@AEBUD2D_RECT_U@@@Z
void* MS_ABI impl___0CD2DRectU__QEAA_AEBUD2D_RECT_U___Z(void* p, const RC* src) { *(RC*)p = *src; return p; }
// Symbol: ??0CD2DRectU@@QEAA@PEBUD2D_RECT_U@@@Z
void* MS_ABI impl___0CD2DRectU__QEAA_PEBUD2D_RECT_U___Z(void* p, const RC* src) { *(RC*)p = *src; return p; }
// Symbol: ??0CD2DRectU@@QEAA@AEBVCRect@@@Z
void* MS_ABI impl___0CD2DRectU__QEAA_AEBVCRect___Z(void* p, const CRc* r) {
    ((RC*)p)->left = (uint32_t)r->left; ((RC*)p)->top = (uint32_t)r->top;
    ((RC*)p)->right = (uint32_t)r->right; ((RC*)p)->bottom = (uint32_t)r->bottom; return p;
}
// Symbol: ??0CD2DRectU@@QEAA@IIII@Z
void* MS_ABI impl___0CD2DRectU__QEAA_IIII_Z(void* p, uint32_t l, uint32_t t, uint32_t r, uint32_t b) {
    ((RC*)p)->left = l; ((RC*)p)->top = t; ((RC*)p)->right = r; ((RC*)p)->bottom = b; return p;
}

} // extern "C"
