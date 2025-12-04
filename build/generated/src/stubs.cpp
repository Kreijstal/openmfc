#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

// Forward declarations for exception helpers
extern "C" void MS_ABI AfxThrowMemoryException();
extern "C" void MS_ABI AfxThrowFileException(int cause, int lOsError, const char* pFileName);

extern "C" void MS_ABI stub_0() { AfxThrowMemoryException(); }
extern "C" void MS_ABI stub_1(int cause, int lOsError, const char* pFileName) { AfxThrowFileException(cause, lOsError, pFileName); }
