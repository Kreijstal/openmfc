#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int,long,const wchar_t*);
namespace openmfc { namespace detail { namespace filecore {
// Native MFC retains several historical DOS/Win32 error classifications.
int FileExceptionFromOsError(long error) {
    switch (error) {
    case 0:
        return CFileException::none;
    case 2: case 6: case 18: case 107:
        return CFileException::fileNotFound;
    case 3: case 15: case 17: case 34: case 52: case 53:
    case 55: case 67: case 70: case 85: case 111: case 123:
    case 124: case 125: case 144: case 154: case 161: case 206:
    case 208: case 267:
        return CFileException::badPath;
    case 4: case 36: case 68: case 113:
        return CFileException::tooManyOpenFiles;
    case 5: case 12: case 19: case 29: case 54: case 58:
    case 64: case 65: case 71: case 80: case 82: case 86:
    case 170: case 183: case 999:
        return CFileException::accessDenied;
    case 11: case 26: case 60: case 66: case 114: case 182:
    case 191: case 193:
        return CFileException::invalidFile;
    case 16: case 145:
        return CFileException::removeCurrentDir;
    case 62:
        return CFileException::directoryFull;
    case 24: case 25: case 27: case 30: case 131: case 132:
        return CFileException::badSeek;
    case 20: case 21: case 22: case 23: case 57: case 59:
    case 88: case 117: case 995: case 996: case 997:
        return CFileException::hardIO;
    case 32:
        return CFileException::sharingViolation;
    case 33: case 108: case 167:
        return CFileException::lockViolation;
    case 39: case 112:
        return CFileException::diskFull;
    case 38:
        return CFileException::endOfFile;
    default: return CFileException::genericException;
    }
}
} } }
// Symbol: ?OsErrorToException@CFileException@@SAHJ@Z
extern "C" int impl__OsErrorToException_CFileException__SAHJ_Z(long error) {
    return CFileException::OsErrorToException(error);
}
// Symbol: ?ThrowOsError@CFileException@@SAXJPEB_W@Z
extern "C" void impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long error,const wchar_t* name) {
    CFileException::ThrowOsError(error,name);
}

int PASCAL CFileException::OsErrorToException(LONG error) {
    return openmfc::detail::filecore::FileExceptionFromOsError(error);
}
void PASCAL CFileException::ThrowOsError(LONG error,const wchar_t* name) {
    if(error)impl__AfxThrowFileException__YAXHJPEB_W_Z(OsErrorToException(error),error,name);
}
