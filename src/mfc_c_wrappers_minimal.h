// Minimal C Wrappers for MFC AfxThrow Functions
// Provides C linkage for MSVC/MinGW interoperability

#ifndef OPENMFC_C_WRAPPERS_MINIMAL_H
#define OPENMFC_C_WRAPPERS_MINIMAL_H

#ifdef __cplusplus
extern "C" {
#endif

void openmfc_afxthrowarchiveexception_stub(void);  // TODO: Implement AfxThrowArchiveException
void openmfc_afxthrowdbexception_stub(void);  // TODO: Implement AfxThrowDBException
void openmfc_afxthrowfileexception_stub(void);  // TODO: Implement AfxThrowFileException
void openmfc_afxthrowinternetexception_stub(void);  // TODO: Implement AfxThrowInternetException
void openmfc_afxthrowinvalidargexception(void);
void openmfc_afxthrowlastcleanup(void);
void openmfc_afxthrowmemoryexception(void);
void openmfc_afxthrownotsupportedexception(void);
void openmfc_afxthrowoledispatchexception_stub(void);  // TODO: Implement AfxThrowOleDispatchException
void openmfc_afxthrowoledispatchexception_stub(void);  // TODO: Implement AfxThrowOleDispatchException
void openmfc_afxthrowoleexception_stub(void);  // TODO: Implement AfxThrowOleException
void openmfc_afxthrowresourceexception(void);
void openmfc_afxthrowuserexception(void);

#ifdef __cplusplus
}
#endif

#endif // OPENMFC_C_WRAPPERS_MINIMAL_H
