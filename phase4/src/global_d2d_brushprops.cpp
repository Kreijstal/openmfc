// Real CD2DBrushProperties (afxrendertarget.h) : public D2D1_BRUSH_PROPERTIES
// { FLOAT opacity; D2D1_MATRIX_3X2_F transform; } = 28 bytes. CommonInit sets
// opacity=1 and an identity transform (verified byte-exact vs real mfc140u.dll).
#include <cstdint>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
struct BP { float opacity; float m[6]; }; // opacity + 3x2 matrix (_11,_12,_21,_22,_31,_32)
static void CommonInit(BP* p) {
    p->opacity = 1.0f;
    p->m[0] = 1.0f; p->m[1] = 0.0f;   // _11 _12
    p->m[2] = 0.0f; p->m[3] = 1.0f;   // _21 _22
    p->m[4] = 0.0f; p->m[5] = 0.0f;   // _31 _32
}
}

extern "C" {
// Symbol: ?CommonInit@CD2DBrushProperties@@IEAAXXZ
void MS_ABI impl__CommonInit_CD2DBrushProperties__IEAAXXZ(void* p) { CommonInit((BP*)p); }

// Symbol: ??0CD2DBrushProperties@@QEAA@XZ
void* MS_ABI impl___0CD2DBrushProperties__QEAA_XZ(void* p) { CommonInit((BP*)p); return p; }

// Symbol: ??0CD2DBrushProperties@@QEAA@M@Z
void* MS_ABI impl___0CD2DBrushProperties__QEAA_M_Z(void* p, float opacity) {
    CommonInit((BP*)p); ((BP*)p)->opacity = opacity; return p;
}

// Symbol: ??0CD2DBrushProperties@@QEAA@UD2D_MATRIX_3X2_F@@M@Z
void* MS_ABI impl___0CD2DBrushProperties__QEAA_UD2D_MATRIX_3X2_F__M_Z(void* p, const float* transform, float opacity) {
    CommonInit((BP*)p);
    for (int i = 0; i < 6; ++i) ((BP*)p)->m[i] = transform[i];
    ((BP*)p)->opacity = opacity;
    return p;
}
} // extern "C"
