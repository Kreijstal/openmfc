// Shared entry points for native-ABI CFile virtual dispatch.
#include "openmfc/afx.h"
extern "C" {
UINT OpenMFC_File_Read(CFile* p,void* b,UINT n) { return p->Read(b,n); }
void OpenMFC_File_Write(CFile* p,const void* b,UINT n) { p->Write(b,n); }
ULONGLONG OpenMFC_File_Seek(CFile* p,LONGLONG o,UINT from) { return p->Seek(o,from); }
void OpenMFC_File_SetLength(CFile* p,ULONGLONG n) { p->SetLength(n); }
ULONGLONG OpenMFC_File_GetLength(CFile* p) { return p->GetLength(); }
ULONGLONG OpenMFC_File_GetPosition(CFile* p) { return p->GetPosition(); }
void OpenMFC_File_Flush(CFile* p) { p->Flush(); }
void OpenMFC_File_Close(CFile* p) { p->Close(); }
}
