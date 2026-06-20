#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdao.h"

#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CLongBinary is faithfully defined in include/openmfc/afxdao.h as a simple
// CObject-derived data holder (HGLOBAL m_hData; DWORD m_dwDataLength;). Its
// default constructor just initializes the CObject vtable and zeroes the two
// members, so a placement-new is a safe, exact reproduction.

// Symbol: ??0CLongBinary@@QEAA@XZ
extern "C" void* MS_ABI impl___0CLongBinary__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    return new (pThis) CLongBinary();
}
