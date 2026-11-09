// The wire schema and name length are 16-bit values; the name length wraps
// rather than being capped at the reader's separate 63-byte lookup limit.
#include "detail/FilecoreSupport.h"
void CRuntimeClass::Store(CArchive& archive) const {
    WORD length=m_lpszClassName?WORD(strlen(m_lpszClassName)):0;
    CArchiveAccess::StoreScalar(archive,WORD(m_wSchema));
    CArchiveAccess::StoreScalar(archive,length);
    archive.Write(m_lpszClassName,length);
}
// Symbol: ?Store@CRuntimeClass@@QEBAXAEAVCArchive@@@Z
extern "C" void impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(const CRuntimeClass* type,CArchive* archive) {type->Store(*archive);}
