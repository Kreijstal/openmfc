// Version information for OpenMFC
// Returns MFC-compatible version numbers

#include <cstdint>

// MS ABI calling convention for x64
#if defined(__GNUC__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// AfxGetDllVersion - Returns MFC DLL version
// Original symbol: ?AfxGetDllVersion@@YAKXZ
// Returns: DWORD (high word = major, low word = minor)
//   0x0E00 = MFC 14.0 (Visual Studio 2015+)
//   0x0D00 = MFC 13.0
//   0x0C00 = MFC 12.0
extern "C" uint32_t MS_ABI stub__AfxGetDllVersion__YAKXZ() {
    return 0x0E00;  // MFC 14.0 (mfc140u.dll)
}
