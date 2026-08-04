// CDumpContext — OpenMFC implementation.
// Sources: global_cdumpcontext.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CDumpContextSupport.h"

// Symbol: ??0CDumpContext@@QEAA@PEAVCFile@@@Z
extern "C" void MS_ABI impl___0CDumpContext__QEAA_PEAVCFile___Z(CDumpContext* pThis, CFile* pFile) {
    if (!pThis) return;
    pThis->m_nDepth = 0;
    pThis->m_pFile = pFile;
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@_J@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0__J_Z(CDumpContext* pThis, __int64 n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@_K@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0__K_Z(CDumpContext* pThis, unsigned __int64 n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@AEBVCObject@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_AEBVCObject___Z(CDumpContext* pThis, const CObject* pOb) {
    if (!pThis) return pThis;
    if (!pOb) return pThis;
    return &(*pThis << *pOb);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@E@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_E_Z(CDumpContext* pThis, unsigned char ch) {
    if (!pThis) return pThis;
    return &(*pThis << ch);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@G@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_G_Z(CDumpContext* pThis, unsigned short w) {
    if (!pThis) return pThis;
    return &(*pThis << w);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@H@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_H_Z(CDumpContext* pThis, int n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@I@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_I_Z(CDumpContext* pThis, unsigned int n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@J@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_J_Z(CDumpContext* pThis, long n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@K@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_K_Z(CDumpContext* pThis, unsigned long n) {
    if (!pThis) return pThis;
    return &(*pThis << n);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHACCEL__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHACCEL_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHDC__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHDC_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHFONT__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHFONT_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHMENU__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHMENU_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEAUHWND__@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEAUHWND_____Z(CDumpContext* pThis, void* h) {
    if (!pThis) return pThis;
    return &(*pThis << static_cast<const void*>(h));
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEB_W@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEB_W_Z(CDumpContext* pThis, const wchar_t* lpsz) {
    if (!pThis) return pThis;
    return &(*pThis << lpsz);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBD@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBD_Z(CDumpContext* pThis, const char* lpsz) {
    if (!pThis) return pThis;
    return &(*pThis << lpsz);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBVCObject@@@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBVCObject___Z(CDumpContext* pThis, const CObject* pOb) {
    if (!pThis) return pThis;
    return &(*pThis << pOb);
}
// Symbol: ??6CDumpContext@@QEAAAEAV0@PEBX@Z
extern "C" CDumpContext* MS_ABI impl___6CDumpContext__QEAAAEAV0_PEBX_Z(CDumpContext* pThis, const void* lp) {
    if (!pThis) return pThis;
    return &(*pThis << lp);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@_J@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1__J_Z(CDumpContext* pThis, __int64 n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@_K@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1__K_Z(CDumpContext* pThis, unsigned __int64 n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@E@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_E_Z(CDumpContext* pThis, unsigned char ch) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(ch);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@G@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_G_Z(CDumpContext* pThis, unsigned short w) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(w);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@H@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_H_Z(CDumpContext* pThis, int n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@I@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_I_Z(CDumpContext* pThis, unsigned int n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@J@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_J_Z(CDumpContext* pThis, long n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}
// Symbol: ?DumpAsHex@CDumpContext@@QEAAAEAV1@K@Z
extern "C" CDumpContext* MS_ABI impl__DumpAsHex_CDumpContext__QEAAAEAV1_K_Z(CDumpContext* pThis, unsigned long n) {
    if (!pThis) return pThis;
    return &pThis->DumpAsHex(n);
}
// Symbol: ?Flush@CDumpContext@@QEAAXXZ
extern "C" void MS_ABI impl__Flush_CDumpContext__QEAAXXZ(CDumpContext* pThis) {
    if (!pThis) return;
    pThis->Flush();
}
// Symbol: ?HexDump@CDumpContext@@QEAAXPEB_WPEAEHH@Z
extern "C" void MS_ABI impl__HexDump_CDumpContext__QEAAXPEB_WPEAEHH_Z(CDumpContext* pThis, const wchar_t* lpszLine, unsigned char* pby, int nBytes, int nWidth) {
    if (!pThis) return;
    pThis->HexDump(lpszLine, pby, nBytes, nWidth);
}
// Symbol: ?OutputString@CDumpContext@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__OutputString_CDumpContext__IEAAXPEB_W_Z(CDumpContext* pThis, const wchar_t* lpsz) {
    if (!pThis) return;
    // OutputString is protected; invoke via operator<< which delegates to it.
    if (lpsz) {
        *pThis << lpsz;
    }
}
