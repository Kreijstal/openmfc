// CDataRecoveryHandler — OpenMFC implementation.
// Sources: collections_strings.cpp, manual_small_stub_implementations.cpp (placeholders),
// rewritten from the retail disassembly.
//
// The Restart-Manager / crash-recovery helper (retail declaration:
// atlmfc/include/afxdatarecovery.h:50, `class CDataRecoveryHandler : public CObject`, no
// DECLARE_DYNAMIC).  OpenMFC has no public declaration of it, so -- like
// core/app/CJumpList.cpp -- the thunks take `void* pThis` and work on the layout view
// S_DRH pinned below.
//
// Every body carrying a retail RVA was transcribed from the retail disassembly (the method
// described in the header of core/ole/COleControl.cpp).  ALL RVAs in this file are
// mfc140u.dll RVAs (read with `disas.py --u` / `vtdump_u.py`); absolute addresses quoted as
// 0x180xxxxxxx are mfc140u.dll VAs.  Where a body departs from retail the comment says so
// and why.
//
// ---- layout (read from the retail constructor ??0CDataRecoveryHandler@@QEAA@KH@Z, 0x37440) --
//   +0x000 vfptr                          ctor: the CDataRecoveryHandler vftable (0x1802e3ba8)
//   +0x008 CMap<CString,LPCTSTR,CString,LPCTSTR>        m_mapDocNameToAutosaveName
//   +0x040 CMap<CString,LPCTSTR,CDocument*,CDocument*>  m_mapDocNameToDocumentPtr
//   +0x078 CMap<CString,LPCTSTR,bool,bool>              m_mapDocNameToRestoreBool
//   +0x0b0 CMap<CDocument*,CDocument*,CString,LPCTSTR>  m_mapDocumentPtrToDocName
//   +0x0e8 CMap<CDocument*,CDocument*,CString,LPCTSTR>  m_mapDocumentPtrToDocTitle
//          each map: +0 vfptr, +8 m_pHashTable (NULL), +0x10 UINT m_nHashTableSize (17),
//          +0x18 m_nCount (0), +0x20 m_pFreeList, +0x28 m_pBlocks, +0x30 m_nBlockSize (10)
//   +0x120 CStringList m_lstAutosavesToDelete           (0x38 bytes; m_nBlockSize 10)
//   +0x158 CString m_strAutosavePath                    ctor: nil string
//   +0x160 CString m_strRestartIdentifier               ctor: nil string
//   +0x168 DWORD   m_dwRestartManagerSupportFlags       ctor: 1st argument
//   +0x16c int     m_nAutosaveInterval                  ctor: 2nd argument
//   +0x170 BOOL    m_bSaveDocumentInfoOnIdle            ctor: the qword store at +0x170
//   +0x174 BOOL    m_bShutdownByRestartManager                zeroes both
//   +0x178 BOOL    m_bRestoringPreviousOpenDocs         ctor: 0
//   +0x180 UINT_PTR m_nTimerID                          ctor: 0
//   sizeof == 0x188 (the scalar deleting destructor, 0x375f0, passes 0x188 as the size).
// That is exactly the member order of afxdatarecovery.h.  The BOOL at +0x170/+0x174 are
// confirmed by the inline accessors in the vftable (slots 12-15: 0x373b0/0x373c0/0x373d0/
// 0x373e0 read/write +0x170 and +0x174).
//
// ---- the vftable (retail 0x1802e3ba8, 32 slots, dumped with vtdump_u.py) --------------
//    0 CObject::GetRuntimeClass (0x37a0)   1 scalar deleting dtor (0x375f0)
//    2-4 Serialize/AssertValid/Dump: the shared empty body 0x27d0
//    5 Initialize (0x37790)                6 GetAutosaveInterval (0x37320)
//    7 SetAutosaveInterval (0x37a50)       8 GetAutosavePath (0x37330)
//    9 SetAutosavePath (0x37360)          10 GetRestartIdentifier (0x37370)
//   11 SetRestartIdentifier (0x373a0)     12/13 Get/SetSaveDocumentInfoOnIdle (0x373b0/0x373c0)
//   14/15 Get/SetShutdownByRestartManager (0x373d0/0x373e0)
//   16 SaveOpenDocumentList (0x37aa0)     17 ReadOpenDocumentList (0x37c80)
//   18 AutosaveDocumentInfo (0x37f50)     19 AutosaveAllDocumentInfo (0x38300)
//   20 CreateDocumentInfo (0x383f0)       21 UpdateDocumentInfo (0x38610)
//   22 RemoveDocumentInfo (0x386e0)       23 DeleteAutosavedFile (0x38850)
//   24 DeleteAllAutosavedFiles (0x388a0)  25 ReopenPreviousDocuments (0x389e0)
//   26 QueryRestoreAutosavedDocuments (0x38c80)  27 RestoreAutosavedDocuments (0x391a0)
//   28 GetDocumentListName (0x398c0)      29 GenerateAutosaveFileName (0x399f0)
//   30 GetRecoveredDocumentTitle (0x39c40) 31 GetNormalDocumentTitle (0x39d20)
// There is no exported ??_7CDataRecoveryHandler@@6B@, so a client that does
// `new CDataRecoveryHandler(...)` relies on the exported constructor to install it; the
// constructor below installs g_DRH_Vtbl, an MSVC-layout table of this file's thunks (the
// mechanism of featurepack/taskspane/CMFCTasksPane.cpp).  A client-derived class installs
// its own vftable after this constructor returns.  Every call the retail bodies make
// through `this`'s vftable is made here through the object's own vfptr by slot number
// (VCall), so client overrides are honoured exactly as in retail.
//
// ---- the CMaps ---------------------------------------------------------------------
// The five maps are CMap<> template instantiations; retail compiles their members as local,
// non-exported helpers.  OpenMFC's own CMap template (include/openmfc/afx.h) has neither
// retail's layout nor its vfptr, so the helpers used are transcribed below (DRM_*) over
// the retail layout.  CAssoc layout, read from those helpers: +0x00 key (CString or
// CDocument*), +0x08 value (CString, CDocument* or bool), +0x10 pNext, +0x18 UINT
// nHashValue; sizeof 0x20 (the 0x20 element size passed to CPlex::Create).
//   CString keys: GetAssocAt 0x314e0 (hash through the exported ??$HashKey@PEB_W@@, then
//     CompareElements 0x31558 = wcscmp), operator[] 0x39dac (CString value) / 0x3a218 (8-byte
//     value), RemoveKey 0x39f00 (CString value) / 0x3a348 (8-byte value), GetNextAssoc
//     0x39fdc, RemoveAll 0x3ac50 (CString value) / 0x3a400 (8-byte value).
//   CDocument* keys: GetAssocAt 0x1bfec, operator[] 0x3a6fc, RemoveKey 0x3a844, RemoveAll
//     0x3ad28; the key hash is afxtempl.h's inline HashKey (ldiv by 127773, see DRM_HashPtr).
// Allocation goes through the same exported entry points retail uses, so that client code
// running afxtempl.h's inline CMap members on these protected maps (a derived class)
// interoperates: the bucket array through ??2@YAPEAX_K@Z (0x27f0), the assoc blocks
// through ?Create@CPlex@@ (0x271300), the string hash through ??$HashKey@PEB_W@@ (0x2731c0).
// (OpenMFC's HashKey body, core/runtime/Globals.cpp, is not retail's algorithm; calling the
// export anyway keeps the bucket choice identical to what a client's inline CMap code, which
// imports the same export, computes.)
// DEVIATION: retail's RemoveAll inlines CPlex::FreeDataChain as a `free` loop; here the
// exported ?FreeDataChain@CPlex@@ thunk is called, because OpenMFC's CPlex::Create does not
// allocate with malloc.  DEVIATION: retail stores each map's (unexported) CMap<...> vftable
// (0x1802e3b78 / 0x1802e3b48 / 0x1802e3b18 / 0x1802e3ae8 twice) in its vfptr; no retail
// CDataRecoveryHandler code dispatches through those (the destructor calls the RemoveAll
// helpers directly) and OpenMFC has no such tables, so nullptr is stored, as
// core/app/CJumpList.cpp does for its map.
//
// ---- m_lstAutosavesToDelete ----------------------------------------------------------
// Retail drives it through the exported CStringList members (AddTail 0x2325f0, RemoveHead
// 0x2326e0, RemoveAll 0x232300 -- which ~CStringList 0x232390 calls after restoring its
// vftable, the two steps the retail destructor inlines).  OpenMFC's CStringList keeps
// its contents in a side table keyed by `this` (core/collections/CStringList.cpp), so the
// list is constructed and driven through OpenMFC's exported CStringList thunks.  The
// destructor's inline `m_nCount != 0` test (+0x138) becomes FindIndex(0) != NULL, since
// OpenMFC does not maintain the in-object count.
//
// ---- the doc/view objects -------------------------------------------------------------
// CDocument and CWinApp objects handed to this class are client objects with MSVC-layout
// vftables, so their virtuals are called by retail slot number (read off the retail call
// sites; CDocument slots cross-checked against the COleDocument vftable 0x18032e7f0,
// CWinApp slots against the CWinApp vftable 0x180320d58).  CDocTemplate objects are
// constructed by OpenMFC's exported constructors, which install no MSVC vftable, so the
// template walk goes through OpenMFC's template thunks instead (see AutosaveAllDocumentInfo).

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <new>
#include <objbase.h>

// ---------------------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition in the file named; the parameter
// lists are derived from the mangled names.
// ---------------------------------------------------------------------------------------
//   core/runtime/Globals.cpp : ??$HashKey@PEB_W@@YAIPEB_W@Z
extern "C" unsigned int MS_ABI impl____HashKey_PEB_W__YAIPEB_W_Z(const wchar_t* key);
//   core/collections/CPlex.cpp : ?Create@CPlex@@SAPEAU1@AEAPEAU1@_K1@Z, ?FreeDataChain@CPlex@@QEAAXXZ
extern "C" void* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(void** ppHead, unsigned long long nMax, unsigned long long cbElement);
extern "C" void MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(void* pThis);
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z (malloc)
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
//   detail/MfcExceptionsSupport.cpp : ?AfxThrowInvalidArgException@@YAXXZ
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
//   core/collections/CStringList.cpp : ctor / dtor / AddTail(const CString&) / RemoveHead / FindIndex
extern "C" void* MS_ABI impl___0CStringList__QEAA__J_Z(CStringList* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CStringList__UEAA_XZ(CStringList* pThis);
extern "C" CStringList::POSITION MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CStringList* pThis, const CString& value);
extern "C" void MS_ABI impl__RemoveHead_CStringList__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CStringList* pThis, void* ret);
extern "C" CStringList::POSITION MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(const CStringList* pThis, long long nIndex);
//   core/runtime/CObject.cpp : ?GetRuntimeClass@CObject@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);
//   detail/RegcoreSupport.cpp : ?AfxGetApp@@ (retail: AfxGetModuleState()->m_pCurrentWinApp, 0x133930 + 8)
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
//   core/app/CWinApp.cpp : GetAppRegistryKey, GetFirstDocTemplatePosition, GetNextDocTemplate
extern "C" HKEY MS_ABI impl__GetAppRegistryKey_CWinApp__QEAAPEAUHKEY____PEAVCAtlTransactionManager_ATL___Z(CWinApp* pThis, void* pTM);
extern "C" void* MS_ABI impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(const CWinApp* pThis);
extern "C" CDocTemplate* MS_ABI impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(const CWinApp* pThis, void** pos);
//   detail/DocviewSupport.cpp : CDocTemplate::GetFirstDocPosition / GetNextDoc (the bodies both
//   ?GetFirstDocPosition@CMultiDocTemplate@@ and @CSingleDocTemplate@@ forward to), and the
//   CDocument title / path-name accessors
extern "C" void* MS_ABI impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(const CDocTemplate* pThis);
extern "C" CDocument* MS_ABI impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(const CDocTemplate* pThis, void*& pos);
extern "C" const wchar_t* MS_ABI impl__GetTitle_CDocument__UEBAPEB_WXZ(const CDocument* pThis);
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(const CDocument* pThis);
//   core/dialog/CTaskDialog.cpp : ?IsSupported@CTaskDialog@@SAHXZ, ?ShowDialog@CTaskDialog@@SA_J...
extern "C" int MS_ABI impl__IsSupported_CTaskDialog__SAHXZ();
extern "C" __int64 MS_ABI impl__ShowDialog_CTaskDialog__SA_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HHHH0_Z(
    const CString* strContent, const CString* strMainInstruction, const CString* strTitle,
    int nIDCommandControlsFirst, int nIDCommandControlsLast, int nCommonButtons, int nTaskDialogOptions,
    const CString* strFooter);
//   core/collections/Globals.cpp : ?AfxMessageBox@@YAHPEB_WII@Z
extern "C" int MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, UINT nType, UINT nIDHelp);
//   featurepack/CMFC_misc_stubs.cpp : ?AfxFindStringResourceHandle@@YAPEAUHINSTANCE__@@I@Z
//   core/collections/CStringT.cpp   : CStringT::LoadStringW(HINSTANCE, UINT)
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID);
//   core/file/CFile.cpp : ?GetStatus@CFile@@SAHPEB_WAEAUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" int MS_ABI impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(
    const wchar_t* lpszFileName, CFileStatus& rStatus, void* pTM);
//   featurepack/CMFC_misc_stubs.cpp : ?AfxAutosaveTimerProc@@YAXPEAUHWND__@@I_KK@Z.  Its
//   definition there still carries a generated parameter list (`void* /*struct*/* p0, unsigned
//   int p1, unsigned __int64 p2, unsigned long p3`), which is ABI-identical to TIMERPROC's
//   (HWND, UINT, UINT_PTR, DWORD); only its address is taken here.
extern "C" void MS_ABI impl__AfxAutosaveTimerProc__YAXPEAUHWND____I_KK_Z(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

// ---------------------------------------------------------------------------------------
// This file's own thunks, forward-declared (the constructor, the vftable and several bodies
// reference siblings defined further down).  Signatures follow the mangled names; a member
// function returning a CString by value takes (this, hidden return slot, args...) and
// returns the return slot, as the retail bodies do (e.g. 0x39d20: RCX this, RDX result,
// R8 pDocument, `mov %rsi,%rax` on exit).
// ---------------------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CDataRecoveryHandler__QEAA_KH_Z(void* pThis, DWORD dwRestartManagerSupportFlags, int nAutosaveInterval);
extern "C" void  MS_ABI impl___1CDataRecoveryHandler__UEAA_XZ(void* pThis);
extern "C" int   MS_ABI impl__Initialize_CDataRecoveryHandler__UEAAHXZ(void* pThis);
extern "C" void  MS_ABI impl__SetAutosaveInterval_CDataRecoveryHandler__UEAAXH_Z(void* pThis, int nAutosaveInterval);
extern "C" int   MS_ABI impl__SaveOpenDocumentList_CDataRecoveryHandler__UEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__ReadOpenDocumentList_CDataRecoveryHandler__UEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__AutosaveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument__H_Z(void* pThis, CDocument* pDocument, int bResetModifiedFlag);
extern "C" int   MS_ABI impl__AutosaveAllDocumentInfo_CDataRecoveryHandler__UEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__CreateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* pThis, CDocument* pDocument);
extern "C" int   MS_ABI impl__UpdateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* pThis, CDocument* pDocument);
extern "C" int   MS_ABI impl__RemoveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* pThis, CDocument* pDocument);
extern "C" int   MS_ABI impl__DeleteAutosavedFile_CDataRecoveryHandler__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, const CString* strAutosavedFile);
extern "C" int   MS_ABI impl__DeleteAllAutosavedFiles_CDataRecoveryHandler__UEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__ReopenPreviousDocuments_CDataRecoveryHandler__UEAAHXZ(void* pThis);
extern "C" void  MS_ABI impl__QueryRestoreAutosavedDocuments_CDataRecoveryHandler__UEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__RestoreAutosavedDocuments_CDataRecoveryHandler__UEAAHXZ(void* pThis);
extern "C" CString* MS_ABI impl__GetDocumentListName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    const void* pThis, CString* pRet, CDocument* pDocument);
extern "C" CString* MS_ABI impl__GenerateAutosaveFileName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    const void* pThis, CString* pRet, const CString* strDocumentName);
extern "C" CString* MS_ABI impl__GetRecoveredDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    const void* pThis, CString* pRet, const CString* strDocumentTitle);
extern "C" CString* MS_ABI impl__GetNormalDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    const void* pThis, CString* pRet, CDocument* pDocument);

namespace {

// ---- pinned layout -----------------------------------------------------------------
struct DRM_Assoc {
    void*      key;          // +0x00 CString (m_pszData) or CDocument*
    void*      value;        // +0x08 CString (m_pszData), CDocument* or bool (low byte)
    DRM_Assoc* pNext;        // +0x10
    UINT       nHashValue;   // +0x18
};
static_assert(sizeof(DRM_Assoc) == 0x20, "CAssoc is 0x20 bytes (the CPlex::Create element size in operator[] 0x39dac)");
static_assert(offsetof(DRM_Assoc, value) == 0x08, "assoc value @+0x08 (callers of GetAssocAt 0x314e0 read +8)");
static_assert(offsetof(DRM_Assoc, pNext) == 0x10, "assoc pNext @+0x10 (GetAssocAt 0x314e0)");
static_assert(offsetof(DRM_Assoc, nHashValue) == 0x18, "assoc nHashValue @+0x18 (GetAssocAt 0x314e0)");

struct DRM_Map {
    void*       vfptr;             // +0x00 (see DEVIATION in the file header)
    DRM_Assoc** m_pHashTable;      // +0x08
    UINT        m_nHashTableSize;  // +0x10
    INT_PTR     m_nCount;          // +0x18
    DRM_Assoc*  m_pFreeList;       // +0x20
    void*       m_pBlocks;         // +0x28 CPlex*
    INT_PTR     m_nBlockSize;      // +0x30
};
static_assert(sizeof(DRM_Map) == 0x38, "CMap is 0x38 bytes (maps at 0x08/0x40/0x78/0xb0/0xe8 in the ctor 0x37440)");
static_assert(offsetof(DRM_Map, m_pHashTable) == 0x08, "m_pHashTable");
static_assert(offsetof(DRM_Map, m_nHashTableSize) == 0x10, "m_nHashTableSize");
static_assert(offsetof(DRM_Map, m_nCount) == 0x18, "m_nCount");
static_assert(offsetof(DRM_Map, m_pFreeList) == 0x20, "m_pFreeList");
static_assert(offsetof(DRM_Map, m_pBlocks) == 0x28, "m_pBlocks");
static_assert(offsetof(DRM_Map, m_nBlockSize) == 0x30, "m_nBlockSize");

struct S_DRH {
    void*         vfptr;                              // +0x000
    DRM_Map       m_mapDocNameToAutosaveName;         // +0x008 CString -> CString
    DRM_Map       m_mapDocNameToDocumentPtr;          // +0x040 CString -> CDocument*
    DRM_Map       m_mapDocNameToRestoreBool;          // +0x078 CString -> bool
    DRM_Map       m_mapDocumentPtrToDocName;          // +0x0b0 CDocument* -> CString
    DRM_Map       m_mapDocumentPtrToDocTitle;         // +0x0e8 CDocument* -> CString
    unsigned char m_lstAutosavesToDelete[0x38];       // +0x120 CStringList
    wchar_t*      m_strAutosavePath;                  // +0x158 CString
    wchar_t*      m_strRestartIdentifier;             // +0x160 CString
    DWORD         m_dwRestartManagerSupportFlags;     // +0x168
    int           m_nAutosaveInterval;                // +0x16c
    BOOL          m_bSaveDocumentInfoOnIdle;          // +0x170
    BOOL          m_bShutdownByRestartManager;        // +0x174
    BOOL          m_bRestoringPreviousOpenDocs;       // +0x178
    UINT_PTR      m_nTimerID;                         // +0x180
};
static_assert(offsetof(S_DRH, m_mapDocNameToAutosaveName) == 0x008, "ctor 0x37440 stores map 1's vftable at +0x08");
static_assert(offsetof(S_DRH, m_mapDocNameToDocumentPtr) == 0x040, "ctor 0x37440: map 2 @+0x40");
static_assert(offsetof(S_DRH, m_mapDocNameToRestoreBool) == 0x078, "ctor 0x37440: map 3 @+0x78");
static_assert(offsetof(S_DRH, m_mapDocumentPtrToDocName) == 0x0b0, "ctor 0x37440: map 4 @+0xb0");
static_assert(offsetof(S_DRH, m_mapDocumentPtrToDocTitle) == 0x0e8, "ctor 0x37440: map 5 @+0xe8");
static_assert(offsetof(S_DRH, m_lstAutosavesToDelete) == 0x120, "ctor 0x37440: CStringList vftable @+0x120");
static_assert(offsetof(S_DRH, m_strAutosavePath) == 0x158, "GetAutosavePath 0x37330 reads +0x158");
static_assert(offsetof(S_DRH, m_strRestartIdentifier) == 0x160, "GetRestartIdentifier 0x37370 reads +0x160");
static_assert(offsetof(S_DRH, m_dwRestartManagerSupportFlags) == 0x168, "ctor stores edx @+0x168");
static_assert(offsetof(S_DRH, m_nAutosaveInterval) == 0x16c, "GetAutosaveInterval 0x37320 reads +0x16c");
static_assert(offsetof(S_DRH, m_bSaveDocumentInfoOnIdle) == 0x170, "0x373b0/0x373c0 use +0x170");
static_assert(offsetof(S_DRH, m_bShutdownByRestartManager) == 0x174, "0x373d0/0x373e0 use +0x174");
static_assert(offsetof(S_DRH, m_bRestoringPreviousOpenDocs) == 0x178, "CreateDocumentInfo 0x383f0 tests +0x178");
static_assert(offsetof(S_DRH, m_nTimerID) == 0x180, "SetAutosaveInterval 0x37a50 passes/stores +0x180");
static_assert(sizeof(S_DRH) == 0x188, "sizeof(CDataRecoveryHandler) == 0x188 (scalar deleting dtor 0x375f0)");
static_assert(sizeof(CStringList) <= 0x38, "OpenMFC's CStringList must fit the retail 0x38-byte slot at +0x120");
static_assert(sizeof(CString) == sizeof(wchar_t*), "CString is one m_pszData pointer");

inline S_DRH* Self(void* p) { return static_cast<S_DRH*>(p); }
inline const S_DRH* Self(const void* p) { return static_cast<const S_DRH*>(p); }
inline CString& Str(wchar_t*& p) { return *reinterpret_cast<CString*>(&p); }
inline CString& AStr(void*& p) { return *reinterpret_cast<CString*>(&p); }
inline CStringList* List(S_DRH* s) { return reinterpret_cast<CStringList*>(s->m_lstAutosavesToDelete); }

// AFX_RESTART_MANAGER_* (atlmfc/include/afxwin.h:5133)
constexpr DWORD kRM_AutosaveAtRestart = 0x04;
constexpr DWORD kRM_AutosaveAtInterval = 0x08;
constexpr DWORD kRM_ReopenPreviousFiles = 0x10;
constexpr DWORD kRM_RestoreAutosavedFiles = 0x20;

// ---- virtual dispatch by retail slot number ---------------------------------------------
template <typename R, typename... A>
inline R VCall(const void* obj, unsigned slot, A... args) {
    using Fn = R (MS_ABI*)(const void*, A...);
    return reinterpret_cast<Fn>((*static_cast<void* const* const*>(obj))[slot])(obj, args...);
}

// CDataRecoveryHandler vftable slots (see the file header).
enum : unsigned {
    kDR_GetAutosaveInterval = 6, kDR_GetAutosavePath = 8, kDR_SetAutosavePath = 9,
    kDR_GetRestartIdentifier = 10, kDR_AutosaveDocumentInfo = 18, kDR_RemoveDocumentInfo = 22,
    kDR_DeleteAutosavedFile = 23, kDR_QueryRestoreAutosavedDocuments = 26,
    kDR_GetDocumentListName = 28, kDR_GenerateAutosaveFileName = 29, kDR_GetRecoveredDocumentTitle = 30,
};
// CDocument vftable slots (COleDocument vftable 0x18032e7f0: 23 = CDocument::SetTitle 0x21ad60,
// 24 = SetPathName, 25 = ?ClearPathName@CDocument@@ 0x21b100, 26/27 = the inline
// IsModified/SetModifiedFlag bodies 0x96780/0x96770 on +0x90, 35 = OnCloseDocument,
// 58 = DoSave).
enum : unsigned {
    kDoc_SetTitle = 23, kDoc_SetPathName = 24, kDoc_ClearPathName = 25, kDoc_IsModified = 26,
    kDoc_SetModifiedFlag = 27, kDoc_OnCloseDocument = 35, kDoc_DoSave = 58,
};
// CWinApp vftable slots (0x180320d58: 31 = ?GetMainWnd@CWinThread@@ 0x274960; 42 = the
// OpenDocumentFile(LPCTSTR, BOOL) overload 0x1ce860, which forwards R8 to the doc manager).
enum : unsigned { kApp_GetMainWnd = 31, kApp_OpenDocumentFile = 42 };

// Storage for a CString returned by value through a vftable slot (the callee constructs it).
struct RetStr {
    alignas(CString) unsigned char buf[sizeof(CString)];
    CString* slot() { return reinterpret_cast<CString*>(buf); }
    CString& get() { return *slot(); }
    ~RetStr() { get().~CString(); }
};

inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

// ---- CMap helpers over the retail layout ----------------------------------------------
enum class KeyKind { String, Ptr };
enum class ValKind { String, Pod };

void* const kBeforeStartPosition = reinterpret_cast<void*>(static_cast<INT_PTR>(-1));

// afxtempl.h's HashKey<ARG_KEY> for a CDocument* key, as inlined in GetAssocAt 0x1bfec and
// RemoveKey 0x3a844: ldiv of the low 32 bits of the pointer by 127773 (0x1f31d), then
// 16807 * rem - 2836 * quot (0x41a7 / 0xb14), plus 0x7fffffff when negative.
UINT DRM_HashPtr(const void* key) {
    const long v = static_cast<long>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(key)));
    const ldiv_t h = std::ldiv(v, 127773L);
    std::int32_t rem = static_cast<std::int32_t>(16807 * h.rem - 2836 * h.quot);
    if (rem < 0) rem += 2147483647;
    return static_cast<UINT>(rem);
}

template <KeyKind K>
UINT DRM_Hash(const void* key) {
    if (K == KeyKind::String) return impl____HashKey_PEB_W__YAIPEB_W_Z(static_cast<const wchar_t*>(key));
    return DRM_HashPtr(key);
}

// CompareElements (0x31558) for CString keys, then wcscmp == 0.  Retail throws
// AfxThrowInvalidArgException (0x227720) for a NULL element pointer and AtlThrow(E_FAIL)
// (0x333c) for a NULL key string; it does not test the stored key's buffer.  DEVIATION: here a
// NULL key or stored key both go to AfxThrowInvalidArgException (no caller passes either).
template <KeyKind K>
bool DRM_KeyEq(const DRM_Assoc* p, const void* key) {
    if (K == KeyKind::Ptr) return p->key == key;
    if (key == nullptr || p->key == nullptr) { ThrowInvalidArg(); return false; }
    return std::wcscmp(static_cast<const wchar_t*>(p->key), static_cast<const wchar_t*>(key)) == 0;
}

// GetAssocAt (0x314e0 / 0x1bfec): hash, bucket = hash % m_nHashTableSize, NULL table -> NULL,
// else walk the bucket comparing nHashValue then the key.
template <KeyKind K>
DRM_Assoc* DRM_GetAssocAt(const DRM_Map* m, const void* key, UINT& nHashBucket, UINT& nHashValue) {
    nHashValue = DRM_Hash<K>(key);
    nHashBucket = nHashValue % m->m_nHashTableSize;
    if (m->m_pHashTable == nullptr) return nullptr;
    for (DRM_Assoc* p = m->m_pHashTable[nHashBucket]; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHashValue && DRM_KeyEq<K>(p, key)) return p;
    }
    return nullptr;
}
template <KeyKind K>
DRM_Assoc* DRM_Find(const DRM_Map* m, const void* key) {
    UINT b = 0, h = 0;
    return DRM_GetAssocAt<K>(m, key, b, h);
}

template <KeyKind K, ValKind V>
void DRM_DestroyAssoc(DRM_Assoc* p) {
    if (V == ValKind::String) AStr(p->value).~CString();   // value first (RemoveAll 0x3ac50)
    if (K == KeyKind::String) AStr(p->key).~CString();
}

// RemoveAll (0x3ac50 / 0x3a400 / 0x3ad28): destroy every CString element, free the bucket
// array, reset m_nCount / m_pFreeList, free the CPlex chain (see the DEVIATION in the file
// header).  m_nHashTableSize is kept.
template <KeyKind K, ValKind V>
void DRM_RemoveAll(DRM_Map* m) {
    if (m->m_pHashTable != nullptr) {
        for (UINT nHash = 0; nHash < m->m_nHashTableSize; ++nHash) {
            for (DRM_Assoc* p = m->m_pHashTable[nHash]; p != nullptr; p = p->pNext) DRM_DestroyAssoc<K, V>(p);
        }
        std::free(m->m_pHashTable);
        m->m_pHashTable = nullptr;
    }
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    if (m->m_pBlocks != nullptr) impl__FreeDataChain_CPlex__QEAAXXZ(m->m_pBlocks);
    m->m_pBlocks = nullptr;
}

// operator[] (0x39dac / 0x3a218 / 0x3a6fc): GetAssocAt; if absent, InitHashTable on first use
// (a zeroed ??2 allocation of m_nHashTableSize pointers, ENSURE'd), then the inlined NewAssoc
// -- refill the free list from a CPlex::Create block of m_nBlockSize 0x20-byte assocs chained
// last-to-first, ENSURE a free assoc, zero it (keeping pNext), pop it, ++m_nCount, construct
// the key (CString(LPCTSTR) or the pointer) and the value (a nil CString, or the zero from the
// memset) -- then store nHashValue and link it at the head of its bucket.  Returns the value
// slot.  NULL only if the invalid-argument throw returns.
// Retail's CString(LPCTSTR) key constructor (0xdcb0) also accepts a MAKEINTRESOURCE id; every
// key here is a real string.
template <KeyKind K, ValKind V>
void** DRM_Index(DRM_Map* m, const void* key) {
    UINT nHashBucket = 0, nHashValue = 0;
    DRM_Assoc* p = DRM_GetAssocAt<K>(m, key, nHashBucket, nHashValue);
    if (p == nullptr) {
        if (m->m_pHashTable == nullptr) {
            const UINT n = m->m_nHashTableSize;
            m->m_pHashTable = static_cast<DRM_Assoc**>(impl___2_YAPEAX_K_Z(sizeof(DRM_Assoc*) * static_cast<std::size_t>(n)));
            if (m->m_pHashTable == nullptr) { ThrowInvalidArg(); return nullptr; }
            std::memset(m->m_pHashTable, 0, sizeof(DRM_Assoc*) * static_cast<std::size_t>(n));
            m->m_nHashTableSize = n;
        }
        if (m->m_pFreeList == nullptr) {
            const INT_PTR nBlock = m->m_nBlockSize;
            void* pBlock = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&m->m_pBlocks, static_cast<unsigned long long>(nBlock), sizeof(DRM_Assoc));
            if (pBlock != nullptr) {
                DRM_Assoc* pAssoc = reinterpret_cast<DRM_Assoc*>(static_cast<unsigned char*>(pBlock) + sizeof(void*)) + (nBlock - 1);
                for (INT_PTR i = nBlock - 1; i >= 0; --i, --pAssoc) {
                    pAssoc->pNext = m->m_pFreeList;
                    m->m_pFreeList = pAssoc;
                }
            }
        }
        p = m->m_pFreeList;
        if (p == nullptr) { ThrowInvalidArg(); return nullptr; }
        DRM_Assoc* pNextFree = p->pNext;
        std::memset(p, 0, sizeof(DRM_Assoc));
        p->pNext = pNextFree;
        m->m_pFreeList = m->m_pFreeList->pNext;
        ++m->m_nCount;
        if (K == KeyKind::String) new (&p->key) CString(static_cast<const wchar_t*>(key));
        else p->key = const_cast<void*>(key);
        if (V == ValKind::String) new (&p->value) CString();
        p->nHashValue = nHashValue;
        p->pNext = m->m_pHashTable[nHashBucket];
        m->m_pHashTable[nHashBucket] = p;
    }
    return &p->value;
}

// RemoveKey (0x39f00 / 0x3a348 / 0x3a844): NULL table -> FALSE; walk the bucket keeping the
// link that points at the current assoc; on a match unlink it and FreeAssoc it (destroy, push
// on the free list, and RemoveAll when m_nCount drops to 0).
template <KeyKind K, ValKind V>
BOOL DRM_RemoveKey(DRM_Map* m, const void* key) {
    if (m->m_pHashTable == nullptr) return FALSE;
    const UINT nHashValue = DRM_Hash<K>(key);
    DRM_Assoc** ppAssocPrev = &m->m_pHashTable[nHashValue % m->m_nHashTableSize];
    for (DRM_Assoc* p = *ppAssocPrev; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHashValue && DRM_KeyEq<K>(p, key)) {
            *ppAssocPrev = p->pNext;
            DRM_DestroyAssoc<K, V>(p);
            p->pNext = m->m_pFreeList;
            m->m_pFreeList = p;
            if (--m->m_nCount == 0) DRM_RemoveAll<K, V>(m);
            return TRUE;
        }
        ppAssocPrev = &p->pNext;
    }
    return FALSE;
}

// GetStartPosition (inline in afxtempl.h; retail inlines it as `neg; sbb` on m_nCount).
inline void* DRM_GetStartPosition(const DRM_Map* m) {
    return m->m_nCount != 0 ? kBeforeStartPosition : nullptr;
}

// GetNextAssoc (0x39fdc): throws on a NULL table or position; BEFORE_START_POSITION picks the
// first non-empty bucket (throwing if there is none); the next position is pNext or the head
// of the next non-empty bucket after this assoc's.  Retail copies key and value into the
// caller's CStrings; the callers here do that with the returned assoc.
DRM_Assoc* DRM_GetNextAssoc(const DRM_Map* m, void*& rNextPosition) {
    DRM_Assoc* pAssocRet = static_cast<DRM_Assoc*>(rNextPosition);
    if (m->m_pHashTable == nullptr || pAssocRet == nullptr) { ThrowInvalidArg(); rNextPosition = nullptr; return nullptr; }
    if (pAssocRet == kBeforeStartPosition) {
        pAssocRet = nullptr;
        for (UINT nBucket = 0; nBucket < m->m_nHashTableSize; ++nBucket) {
            if ((pAssocRet = m->m_pHashTable[nBucket]) != nullptr) break;
        }
        if (pAssocRet == nullptr) { ThrowInvalidArg(); rNextPosition = nullptr; return nullptr; }
    }
    DRM_Assoc* pAssocNext = pAssocRet->pNext;
    if (pAssocNext == nullptr) {
        for (UINT nBucket = (pAssocRet->nHashValue % m->m_nHashTableSize) + 1; nBucket < m->m_nHashTableSize; ++nBucket) {
            if ((pAssocNext = m->m_pHashTable[nBucket]) != nullptr) break;
        }
    }
    rNextPosition = pAssocNext;
    return pAssocRet;
}
// The loop body every caller writes: `m_map.GetNextAssoc(pos, strKey, strValue)` on the
// CString -> CString map.
void DRM_NextStrStr(const DRM_Map* m, void*& pos, CString& strKey, CString& strValue) {
    DRM_Assoc* p = DRM_GetNextAssoc(m, pos);
    if (p == nullptr) return;
    strKey = AStr(p->key);
    strValue = AStr(p->value);
}

// The five maps, typed.
inline DRM_Map* MapNameToAutosave(S_DRH* s) { return &s->m_mapDocNameToAutosaveName; }
inline DRM_Map* MapNameToDoc(S_DRH* s) { return &s->m_mapDocNameToDocumentPtr; }
inline DRM_Map* MapNameToRestore(S_DRH* s) { return &s->m_mapDocNameToRestoreBool; }
inline DRM_Map* MapDocToName(S_DRH* s) { return &s->m_mapDocumentPtrToDocName; }
inline DRM_Map* MapDocToTitle(S_DRH* s) { return &s->m_mapDocumentPtrToDocTitle; }

// m_mapDocNameToAutosaveName[strDocName] = <psz>  (operator[] 0x39dac, then SetString).
void SetAutosaveName(S_DRH* s, const wchar_t* strDocName, const wchar_t* psz) {
    if (void** v = DRM_Index<KeyKind::String, ValKind::String>(MapNameToAutosave(s), strDocName)) AStr(*v) = psz;
}
// m_mapDocumentPtrToDocName / ToDocTitle [pDoc] = str  (operator[] 0x3a6fc, then operator=).
void SetDocString(DRM_Map* m, CDocument* pDoc, const CString& str) {
    if (void** v = DRM_Index<KeyKind::Ptr, ValKind::String>(m, pDoc)) AStr(*v) = str;
}
// CMap::Lookup(key, CString&): copies the value only when the key is present.
template <KeyKind K>
bool LookupString(const DRM_Map* m, const void* key, CString& rValue) {
    DRM_Assoc* p = DRM_Find<K>(m, key);
    if (p == nullptr) return false;
    rValue = AStr(p->value);
    return true;
}

// CStringT::LoadString(UINT) as retail inlines it: AfxFindStringResourceHandle (0x2aee00),
// then the exported LoadString(HINSTANCE, UINT) (0xdb70); the result is not ENSURE'd.
// DEVIATION: OpenMFC ships none of MFC's string resources, so when the lookup fails the
// retail en-US text is used -- `fallback`, copied from mfc140u.dll's RT_STRING table
// (language 1033).  A resource of the same id in the application still wins, exactly as the
// retail lookup order (resource handle first) gives it.
void LoadStringOr(CString& str, UINT nID, const wchar_t* fallback) {
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst != nullptr &&
        impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, nID)) {
        return;
    }
    str = fallback;
}
// AFX_IDS_AUTOSAVE_RECOVERY_ASK_1..6 (atlmfc/include/afxres.h:606) -- retail en-US text.
constexpr UINT kIDS_Ask1 = 0xF2E4, kIDS_Ask2 = 0xF2E5, kIDS_Ask3 = 0xF2E6, kIDS_Ask4 = 0xF2E7,
               kIDS_Ask5 = 0xF2E8, kIDS_Ask6 = 0xF2E9;
const wchar_t kTextAsk1[] = L"One or more auto-saved documents were found.";
const wchar_t kTextAsk2[] = L"These are more recently saved than the currently open documents and contain changes that were made before the application closed.";
const wchar_t kTextAsk3[] = L"Do you want to recover these auto-saved documents?";
const wchar_t kTextAsk4[] = L"Note that if you choose to recover the auto-saved documents, you must explicitly save them to overwrite the original documents.  If you choose to not recover the auto-saved versions, they will be deleted.";

// CStringT::Format (retail: the exported ?Format@CStringT@@ 0xda00) with Microsoft printf
// semantics, through msvcrt's _vscwprintf / _vsnwprintf.  OpenMFC's inline CString::FormatV
// goes through mingw's C99 vswprintf (whose %s is narrow in a wide format under this build's
// __USE_MINGW_ANSI_STDIO=1); only integer conversions are formatted here, so either would do,
// but this keeps the conversion rules the retail format strings were written for.
void FormatMs(CString& str, const wchar_t* pszFormat, ...) {
    va_list args;
    va_start(args, pszFormat);
    va_list argsLen;
    va_copy(argsLen, args);
    const int nLen = _vscwprintf(pszFormat, argsLen);
    va_end(argsLen);
    if (nLen < 0) { str.Empty(); va_end(args); return; }
    wchar_t* pBuf = str.GetBuffer(nLen);
    _vsnwprintf(pBuf, static_cast<std::size_t>(nLen) + 1, pszFormat, args);
    str.ReleaseBuffer(nLen);
    va_end(args);
}

// CRegKey::QueryStringValue as retail instantiates it (0x3b1a8): RegQueryValueExW with
// *pnChars * 2 bytes and *pnChars zeroed first; failure is returned as is; a type other than
// REG_SZ / REG_EXPAND_SZ, an odd byte count or a value not NUL-terminated inside the
// returned bytes gives ERROR_INVALID_DATA (13); an empty value stores a NUL; on success
// *pnChars = bytes / 2.
LONG RegQueryString(HKEY hKey, const wchar_t* pszValueName, wchar_t* pszValue, ULONG* pnChars) {
    DWORD dwType = 0;
    DWORD nBytes = *pnChars * 2;
    *pnChars = 0;
    LONG lRes = ::RegQueryValueExW(hKey, pszValueName, nullptr, &dwType, reinterpret_cast<LPBYTE>(pszValue), &nBytes);
    if (lRes != ERROR_SUCCESS) return lRes;
    if (dwType != REG_SZ && dwType != REG_EXPAND_SZ) return ERROR_INVALID_DATA;
    if (pszValue != nullptr) {
        if (nBytes != 0) {
            if ((nBytes & 1) != 0 || pszValue[nBytes / 2 - 1] != L'\0') return ERROR_INVALID_DATA;
        } else {
            pszValue[0] = L'\0';
        }
    }
    *pnChars = nBytes / 2;
    return ERROR_SUCCESS;
}

// The main window's HWND for the WM_SETREDRAW / InvalidateRect / UpdateWindow calls in
// RestoreAutosavedDocuments: retail evaluates AfxGetApp()->GetMainWnd()->m_hWnd (vslot 31,
// then +0x40) at every call.  DEVIATION: a NULL app or main window yields NULL here and the
// caller skips the call (retail would fault; ::InvalidateRect(NULL, ...) would repaint every
// window).
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads CWnd::m_hWnd at +0x40");
HWND MainWndHandle() {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr) return nullptr;
    CWnd* pMain = VCall<CWnd*>(pApp, kApp_GetMainWnd);
    return pMain != nullptr ? pMain->m_hWnd : nullptr;
}

// FOLDERID_LocalAppData {F1B32785-6FBA-4FCF-9D55-7B8E7F157091}, read from mfc140u 0x1802d9838,
// the GUID Initialize passes to SHGetKnownFolderPath.
const GUID kFOLDERID_LocalAppData = {0xF1B32785, 0x6FBA, 0x4FCF, {0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91}};

// ---- the vftable (retail 0x1802e3ba8) ------------------------------------------------------
// Slots 2-4 are the shared empty body 0x27d0 in retail; slots 6 and 8-15 are the inline
// accessors of afxdatarecovery.h, transcribed from their retail bodies.
void MS_ABI DR_EmptyVirtual(void*) {}
void MS_ABI DR_EmptyVirtual1(void*, void*) {}
int MS_ABI DR_GetAutosaveInterval(const void* p) { return Self(p)->m_nAutosaveInterval; }              // 0x37320
CString* MS_ABI DR_GetAutosavePath(const void* p, CString* pRet) {                                      // 0x37330
    return new (pRet) CString(Str(const_cast<S_DRH*>(Self(p))->m_strAutosavePath));
}
void MS_ABI DR_SetAutosavePath(void* p, const CString* str) { Str(Self(p)->m_strAutosavePath) = *str; } // 0x37360
CString* MS_ABI DR_GetRestartIdentifier(const void* p, CString* pRet) {                                 // 0x37370
    return new (pRet) CString(Str(const_cast<S_DRH*>(Self(p))->m_strRestartIdentifier));
}
void MS_ABI DR_SetRestartIdentifier(void* p, const CString* str) { Str(Self(p)->m_strRestartIdentifier) = *str; } // 0x373a0
int MS_ABI DR_GetSaveDocumentInfoOnIdle(const void* p) { return Self(p)->m_bSaveDocumentInfoOnIdle; }  // 0x373b0
void MS_ABI DR_SetSaveDocumentInfoOnIdle(void* p, int b) { Self(p)->m_bSaveDocumentInfoOnIdle = b; }  // 0x373c0
int MS_ABI DR_GetShutdownByRestartManager(const void* p) { return Self(p)->m_bShutdownByRestartManager; } // 0x373d0
void MS_ABI DR_SetShutdownByRestartManager(void* p, int b) { Self(p)->m_bShutdownByRestartManager = b; } // 0x373e0

// Scalar deleting destructor, retail 0x375f0: ~CDataRecoveryHandler(); then, when bit 0 of
// the flags is set, free(this) -- or, when bit 2 is also set, a call to 0x27d0 (an empty body)
// with the size 0x188.
void* MS_ABI DR_ScalarDeletingDtor(void* p, unsigned int flags) {
    impl___1CDataRecoveryHandler__UEAA_XZ(p);
    if ((flags & 1) != 0 && (flags & 4) == 0) std::free(p);
    return p;
}

void* const g_DRH_Vtbl[32] = {
    reinterpret_cast<void*>(&impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),   //  0
    reinterpret_cast<void*>(&DR_ScalarDeletingDtor),                                     //  1
    reinterpret_cast<void*>(&DR_EmptyVirtual1),                                          //  2 Serialize(CArchive&)
    reinterpret_cast<void*>(&DR_EmptyVirtual),                                           //  3 AssertValid()
    reinterpret_cast<void*>(&DR_EmptyVirtual1),                                          //  4 Dump(CDumpContext&)
    reinterpret_cast<void*>(&impl__Initialize_CDataRecoveryHandler__UEAAHXZ),            //  5
    reinterpret_cast<void*>(&DR_GetAutosaveInterval),                                    //  6
    reinterpret_cast<void*>(&impl__SetAutosaveInterval_CDataRecoveryHandler__UEAAXH_Z),  //  7
    reinterpret_cast<void*>(&DR_GetAutosavePath),                                        //  8
    reinterpret_cast<void*>(&DR_SetAutosavePath),                                        //  9
    reinterpret_cast<void*>(&DR_GetRestartIdentifier),                                   // 10
    reinterpret_cast<void*>(&DR_SetRestartIdentifier),                                   // 11
    reinterpret_cast<void*>(&DR_GetSaveDocumentInfoOnIdle),                              // 12
    reinterpret_cast<void*>(&DR_SetSaveDocumentInfoOnIdle),                              // 13
    reinterpret_cast<void*>(&DR_GetShutdownByRestartManager),                            // 14
    reinterpret_cast<void*>(&DR_SetShutdownByRestartManager),                            // 15
    reinterpret_cast<void*>(&impl__SaveOpenDocumentList_CDataRecoveryHandler__UEAAHXZ),  // 16
    reinterpret_cast<void*>(&impl__ReadOpenDocumentList_CDataRecoveryHandler__UEAAHXZ),  // 17
    reinterpret_cast<void*>(&impl__AutosaveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument__H_Z), // 18
    reinterpret_cast<void*>(&impl__AutosaveAllDocumentInfo_CDataRecoveryHandler__UEAAHXZ), // 19
    reinterpret_cast<void*>(&impl__CreateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z), // 20
    reinterpret_cast<void*>(&impl__UpdateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z), // 21
    reinterpret_cast<void*>(&impl__RemoveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z), // 22
    reinterpret_cast<void*>(&impl__DeleteAutosavedFile_CDataRecoveryHandler__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z), // 23
    reinterpret_cast<void*>(&impl__DeleteAllAutosavedFiles_CDataRecoveryHandler__UEAAHXZ), // 24
    reinterpret_cast<void*>(&impl__ReopenPreviousDocuments_CDataRecoveryHandler__UEAAHXZ), // 25
    reinterpret_cast<void*>(&impl__QueryRestoreAutosavedDocuments_CDataRecoveryHandler__UEAAXXZ), // 26
    reinterpret_cast<void*>(&impl__RestoreAutosavedDocuments_CDataRecoveryHandler__UEAAHXZ), // 27
    reinterpret_cast<void*>(&impl__GetDocumentListName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z), // 28
    reinterpret_cast<void*>(&impl__GenerateAutosaveFileName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z), // 29
    reinterpret_cast<void*>(&impl__GetRecoveredDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z), // 30
    reinterpret_cast<void*>(&impl__GetNormalDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z), // 31
};

}  // namespace

// ---------------------------------------------------------------------------------------
// Exported thunks
// ---------------------------------------------------------------------------------------

// Retail 0x37440.  Not on the stub list this file was assigned from (its placeholder body
// `return pThis;` was not counted as a stub), but it initialised nothing -- not even the
// vfptr -- and every method below reads the members, so it is transcribed as well.
// DEVIATIONS: the five map vfptrs are stored as nullptr (see the file header), and
// m_lstAutosavesToDelete is constructed through OpenMFC's CStringList constructor thunk
// (retail inlines the CStringList constructor: its vftable, zeroed node fields, block size 10).
// Symbol: ??0CDataRecoveryHandler@@QEAA@KH@Z
extern "C" void* MS_ABI impl___0CDataRecoveryHandler__QEAA_KH_Z(void* pThis, DWORD dwRestartManagerSupportFlags, int nAutosaveInterval) {
    S_DRH* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_DRH_Vtbl);
    DRM_Map* maps[5] = { &s->m_mapDocNameToAutosaveName, &s->m_mapDocNameToDocumentPtr, &s->m_mapDocNameToRestoreBool,
                         &s->m_mapDocumentPtrToDocName, &s->m_mapDocumentPtrToDocTitle };
    for (DRM_Map* m : maps) {
        m->vfptr = nullptr;
        m->m_pHashTable = nullptr;
        m->m_nHashTableSize = 17;
        m->m_nCount = 0;
        m->m_pFreeList = nullptr;
        m->m_pBlocks = nullptr;
        m->m_nBlockSize = 10;
    }
    std::memset(s->m_lstAutosavesToDelete, 0, sizeof(s->m_lstAutosavesToDelete));
    impl___0CStringList__QEAA__J_Z(List(s), 10);
    new (&s->m_strAutosavePath) CString();
    new (&s->m_strRestartIdentifier) CString();
    s->m_dwRestartManagerSupportFlags = dwRestartManagerSupportFlags;
    s->m_nAutosaveInterval = nAutosaveInterval;
    s->m_bSaveDocumentInfoOnIdle = FALSE;
    s->m_bShutdownByRestartManager = FALSE;
    s->m_bRestoringPreviousOpenDocs = FALSE;
    s->m_nTimerID = 0;
    return pThis;
}

// Retail 0x37640:
//   vfptr = CDataRecoveryHandler vftable;
//   while (m_lstAutosavesToDelete.GetCount() != 0) {           // inline, +0x138
//       CString strFile = m_lstAutosavesToDelete.RemoveHead(); // 0x2326e0
//       ::DeleteFile(strFile);
//   }
//   ::KillTimer(NULL, m_nTimerID);
//   member destructors: m_strRestartIdentifier, m_strAutosavePath, m_lstAutosavesToDelete
//   (vftable + RemoveAll 0x232300), then the maps in reverse order -- +0xe8 and +0xb0
//   (RemoveAll 0x3ad28), +0x78 and +0x40 (RemoveAll 0x3a400), +0x08 (RemoveAll 0x3ac50).
// The count test is FindIndex(0) != NULL here (see the file header).
// Symbol: ??1CDataRecoveryHandler@@UEAA@XZ
extern "C" void MS_ABI impl___1CDataRecoveryHandler__UEAA_XZ(void* pThis) {
    S_DRH* s = Self(pThis);
    s->vfptr = const_cast<void**>(g_DRH_Vtbl);
    while (impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(List(s), 0) != CStringList::POSITION(nullptr)) {
        RetStr strFile;
        impl__RemoveHead_CStringList__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(List(s), strFile.slot());
        ::DeleteFileW(strFile.get());
    }
    ::KillTimer(nullptr, s->m_nTimerID);
    Str(s->m_strRestartIdentifier).~CString();
    Str(s->m_strAutosavePath).~CString();
    impl___1CStringList__UEAA_XZ(List(s));
    DRM_RemoveAll<KeyKind::Ptr, ValKind::String>(&s->m_mapDocumentPtrToDocTitle);
    DRM_RemoveAll<KeyKind::Ptr, ValKind::String>(&s->m_mapDocumentPtrToDocName);
    DRM_RemoveAll<KeyKind::String, ValKind::Pod>(&s->m_mapDocNameToRestoreBool);
    DRM_RemoveAll<KeyKind::String, ValKind::Pod>(&s->m_mapDocNameToDocumentPtr);
    DRM_RemoveAll<KeyKind::String, ValKind::String>(&s->m_mapDocNameToAutosaveName);
}

// Retail 0x37790:
//   BOOL bRet = TRUE;
//   if (m_dwRestartManagerSupportFlags & (AUTOSAVE_AT_RESTART | AUTOSAVE_AT_INTERVAL)) {   // 0x0c
//       PWSTR pszPath = NULL;
//       pfn = SHGetKnownFolderPath, resolved once through GetModuleHandleW(L"shell32.dll") +
//             GetProcAddress and cached EncodePointer'd (static at 0x1803c3f30); a failed
//             GetModuleHandleW returns FALSE without caching;
//       if (pfn == NULL) return FALSE;
//       if (pfn(FOLDERID_LocalAppData, 0, NULL, &pszPath) != S_OK || pszPath == NULL) return FALSE;
//       SetAutosavePath(CString(pszPath));                    // vslot 9
//       ::CoTaskMemFree(pszPath);
//   }
//   if (m_dwRestartManagerSupportFlags & AUTOSAVE_AT_INTERVAL)                              // 0x08
//       m_nTimerID = ::SetTimer(NULL, m_nTimerID, GetAutosaveInterval() /* vslot 6 */,
//                               AfxAutosaveTimerProc);        // export 0x373f0
//   return bRet;
// The early FALSE returns skip the timer, as in retail.
// Symbol: ?Initialize@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__Initialize_CDataRecoveryHandler__UEAAHXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    if ((s->m_dwRestartManagerSupportFlags & (kRM_AutosaveAtRestart | kRM_AutosaveAtInterval)) != 0) {
        using PfnSHGetKnownFolderPath = HRESULT (WINAPI*)(const GUID&, DWORD, HANDLE, PWSTR*);
        static void* s_pfnEncoded = nullptr;
        PfnSHGetKnownFolderPath pfn = nullptr;
        if (s_pfnEncoded == nullptr) {
            HMODULE hShell = ::GetModuleHandleW(L"shell32.dll");
            if (hShell == nullptr) return FALSE;
            pfn = reinterpret_cast<PfnSHGetKnownFolderPath>(reinterpret_cast<void*>(::GetProcAddress(hShell, "SHGetKnownFolderPath")));
            s_pfnEncoded = ::EncodePointer(reinterpret_cast<void*>(pfn));
        } else {
            pfn = reinterpret_cast<PfnSHGetKnownFolderPath>(::DecodePointer(s_pfnEncoded));
        }
        if (pfn == nullptr) return FALSE;
        PWSTR pszPath = nullptr;
        if (pfn(kFOLDERID_LocalAppData, 0, nullptr, &pszPath) != S_OK) return FALSE;
        if (pszPath == nullptr) return FALSE;
        {
            CString strPath(pszPath);
            VCall<void>(s, kDR_SetAutosavePath, static_cast<const CString*>(&strPath));
        }
        ::CoTaskMemFree(pszPath);
    }
    if ((s->m_dwRestartManagerSupportFlags & kRM_AutosaveAtInterval) != 0) {
        const int nInterval = VCall<int>(s, kDR_GetAutosaveInterval);
        s->m_nTimerID = ::SetTimer(nullptr, s->m_nTimerID, static_cast<UINT>(nInterval),
                                   reinterpret_cast<TIMERPROC>(&impl__AfxAutosaveTimerProc__YAXPEAUHWND____I_KK_Z));
    }
    return TRUE;
}

// Retail 0x37a50 (unconditional):
//   m_nAutosaveInterval = nAutosaveInterval;
//   m_nTimerID = ::SetTimer(NULL, m_nTimerID, GetAutosaveInterval() /* vslot 6 */, AfxAutosaveTimerProc);
// Symbol: ?SetAutosaveInterval@CDataRecoveryHandler@@UEAAXH@Z
extern "C" void MS_ABI impl__SetAutosaveInterval_CDataRecoveryHandler__UEAAXH_Z(void* pThis, int nAutosaveInterval) {
    S_DRH* s = Self(pThis);
    s->m_nAutosaveInterval = nAutosaveInterval;
    const int nInterval = VCall<int>(s, kDR_GetAutosaveInterval);
    s->m_nTimerID = ::SetTimer(nullptr, s->m_nTimerID, static_cast<UINT>(nInterval),
                               reinterpret_cast<TIMERPROC>(&impl__AfxAutosaveTimerProc__YAXPEAUHWND____I_KK_Z));
}

// Retail 0x37aa0:
//   BOOL bRet = TRUE;
//   POSITION pos = m_mapDocNameToAutosaveName.GetStartPosition();
//   if (pos != NULL) {
//       CRegKey rkDocs;
//       HKEY hAppKey = AfxGetApp()->GetAppRegistryKey(NULL);            // 0x1cf8f0
//       // rkDocs.Create(hAppKey, GetRestartIdentifier()), inline:     // vslot 10
//       LONG lResult = ::RegCreateKeyExW(hAppKey, strRestartIdentifier, 0, NULL, 0,
//                                        KEY_READ | KEY_WRITE /* 0x2001f */, NULL, &hKey, &dwDisp);
//       if (lResult == ERROR_SUCCESS) {
//           do {
//               CString strDocName, strAutosave;
//               m_mapDocNameToAutosaveName.GetNextAssoc(pos, strDocName, strAutosave);
//               rkDocs.SetStringValue(strDocName, strAutosave);  // inline: RegSetValueExW(REG_SZ,
//                                                                 //   (wcslen + 1) * 2 bytes), result ignored
//           } while (pos != NULL);
//       } else bRet = FALSE;
//       // ~CRegKey: RegCloseKey(hKey) when it was opened
//   }
//   return bRet;
// hAppKey is never closed in retail (it is neither wrapped nor passed to RegCloseKey); that
// leak is reproduced.
// Symbol: ?SaveOpenDocumentList@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__SaveOpenDocumentList_CDataRecoveryHandler__UEAAHXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    BOOL bRet = TRUE;
    void* pos = DRM_GetStartPosition(MapNameToAutosave(s));
    if (pos != nullptr) {
        HKEY hKey = nullptr;
        LONG lResult;
        {
            RetStr strRestartId;
            VCall<CString*>(s, kDR_GetRestartIdentifier, strRestartId.slot());
            HKEY hAppKey = impl__GetAppRegistryKey_CWinApp__QEAAPEAUHKEY____PEAVCAtlTransactionManager_ATL___Z(
                impl__AfxGetApp__YAPEAVCWinApp__XZ(), nullptr);
            HKEY hNew = nullptr;
            DWORD dwDisposition = 0;
            lResult = ::RegCreateKeyExW(hAppKey, strRestartId.get(), 0, nullptr, 0, KEY_READ | KEY_WRITE,
                                        nullptr, &hNew, &dwDisposition);
            if (lResult == ERROR_SUCCESS) hKey = hNew;
        }
        if (lResult == ERROR_SUCCESS) {
            do {
                CString strDocName, strAutosave;
                DRM_NextStrStr(MapNameToAutosave(s), pos, strDocName, strAutosave);
                const wchar_t* pszValue = strAutosave;
                if (pszValue != nullptr) {
                    ::RegSetValueExW(hKey, strDocName, 0, REG_SZ, reinterpret_cast<const BYTE*>(pszValue),
                                     static_cast<DWORD>((std::wcslen(pszValue) + 1) * sizeof(wchar_t)));
                }
            } while (pos != nullptr);
        } else {
            bRet = FALSE;
        }
        if (hKey != nullptr) ::RegCloseKey(hKey);
    }
    return bRet;
}

// Retail 0x37c80:
//   BOOL bRet = FALSE;
//   CRegKey rkApp(AfxGetApp()->GetAppRegistryKey(NULL));                 // 0x1cf8f0
//   CRegKey rkDocs;
//   // rkDocs.Open(hAppKey, GetRestartIdentifier()), inline:           // vslot 10
//   if (::RegOpenKeyExW(hAppKey, strRestartIdentifier, 0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS) {
//       DWORD dwIndex = 0;  TCHAR szName[MAX_PATH] = {0};  DWORD cchName = MAX_PATH;
//       while (::RegEnumValueW(hKey, dwIndex, szName, &cchName, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
//           ++dwIndex;  cchName = MAX_PATH;
//           TCHAR szValue[MAX_PATH] = {0};  ULONG cchValue = MAX_PATH;
//           if (rkDocs.QueryStringValue(szName, szValue, &cchValue) == ERROR_SUCCESS) {   // 0x3b1a8
//               m_mapDocNameToAutosaveName[szName] = szValue;
//               bRet = TRUE;
//           }
//       }
//       rkDocs.Close();
//       // rkApp.DeleteSubKey(GetRestartIdentifier()), inline:            // vslot 10
//       //   RegDeleteKeyExW(hAppKey, strId, 0, 0) through GetProcAddress(GetModuleHandleW(
//       //   L"Advapi32.dll"), "RegDeleteKeyExW"), resolved once (statics at 0x1803c3da0 /
//       //   0x1803c3da8), else RegDeleteKeyW(hAppKey, strId); result ignored
//   }
//   ~rkDocs, ~rkApp: RegCloseKey on each open key
//   return bRet;
// Symbol: ?ReadOpenDocumentList@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__ReadOpenDocumentList_CDataRecoveryHandler__UEAAHXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    BOOL bRet = FALSE;
    HKEY hAppKey = impl__GetAppRegistryKey_CWinApp__QEAAPEAUHKEY____PEAVCAtlTransactionManager_ATL___Z(
        impl__AfxGetApp__YAPEAVCWinApp__XZ(), nullptr);
    HKEY hDocsKey = nullptr;
    LONG lResult;
    {
        RetStr strRestartId;
        VCall<CString*>(s, kDR_GetRestartIdentifier, strRestartId.slot());
        HKEY hOpened = nullptr;
        lResult = ::RegOpenKeyExW(hAppKey, strRestartId.get(), 0, KEY_READ | KEY_WRITE, &hOpened);
        if (lResult == ERROR_SUCCESS) hDocsKey = hOpened;
    }
    if (lResult == ERROR_SUCCESS) {
        DWORD dwIndex = 0;
        wchar_t szName[MAX_PATH] = {};
        DWORD cchName = MAX_PATH;
        while (::RegEnumValueW(hDocsKey, dwIndex, szName, &cchName, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
            ++dwIndex;
            cchName = MAX_PATH;
            wchar_t szValue[MAX_PATH] = {};
            ULONG cchValue = MAX_PATH;
            if (RegQueryString(hDocsKey, szName, szValue, &cchValue) == ERROR_SUCCESS) {
                SetAutosaveName(s, szName, szValue);
                bRet = TRUE;
            }
        }
        ::RegCloseKey(hDocsKey);
        hDocsKey = nullptr;

        RetStr strRestartId;
        VCall<CString*>(s, kDR_GetRestartIdentifier, strRestartId.slot());
        using PfnRegDeleteKeyExW = LONG (WINAPI*)(HKEY, LPCWSTR, REGSAM, DWORD);
        static PfnRegDeleteKeyExW s_pfnRegDeleteKeyExW = nullptr;
        static bool s_bResolved = false;
        if (!s_bResolved) {
            if (HMODULE hAdvapi = ::GetModuleHandleW(L"Advapi32.dll")) {
                s_pfnRegDeleteKeyExW = reinterpret_cast<PfnRegDeleteKeyExW>(reinterpret_cast<void*>(::GetProcAddress(hAdvapi, "RegDeleteKeyExW")));
            }
            s_bResolved = true;
        }
        if (s_pfnRegDeleteKeyExW != nullptr) s_pfnRegDeleteKeyExW(hAppKey, strRestartId.get(), 0, 0);
        else ::RegDeleteKeyW(hAppKey, strRestartId.get());
    }
    if (hDocsKey != nullptr) ::RegCloseKey(hDocsKey);
    if (hAppKey != nullptr) ::RegCloseKey(hAppKey);
    return bRet;
}

// Retail 0x37f50:
//   if (!(m_dwRestartManagerSupportFlags & (AUTOSAVE_AT_RESTART | AUTOSAVE_AT_INTERVAL)) ||
//       pDocument == NULL) return FALSE;
//   CString strDocName;
//   m_mapDocumentPtrToDocName.Lookup(pDocument, strDocName);          // GetAssocAt 0x1bfec
//   if (strDocName.IsEmpty()) {
//       strDocName = GetDocumentListName(pDocument);                  // vslot 28
//       m_mapDocumentPtrToDocName[pDocument] = strDocName;            // 0x3a6fc
//   }
//   if (!strDocName.IsEmpty()) {
//       CString strAutosaveName, strNewAutosaveName;
//       if (pDocument->IsModified()) {                                // doc vslot 26
//           if (!m_mapDocNameToAutosaveName.Lookup(strDocName, strAutosaveName))   // 0x314e0
//               m_mapDocNameToAutosaveName[strDocName] = _T("");       // 0x39dac
//           if (m_dwRestartManagerSupportFlags & (AUTOSAVE_AT_RESTART | AUTOSAVE_AT_INTERVAL)) {
//               strNewAutosaveName = GenerateAutosaveFileName(strDocName);          // vslot 29
//               if (pDocument->DoSave(strNewAutosaveName, FALSE)) {               // doc vslot 58
//                   m_mapDocNameToAutosaveName[strDocName] = strNewAutosaveName;
//                   if (strAutosaveName.CompareNoCase(strNewAutosaveName) != 0)   // _wcsicmp
//                       DeleteAutosavedFile(strAutosaveName);                     // vslot 23
//                   pDocument->SetModifiedFlag(bResetModifiedFlag ? TRUE : FALSE); // doc vslot 27
//               }
//           }
//       } else {
//           BOOL bHad = m_mapDocNameToAutosaveName.Lookup(strDocName, strAutosaveName);
//           m_mapDocNameToAutosaveName[strDocName] = _T("");
//           if (bHad) DeleteAutosavedFile(strAutosaveName);                       // vslot 23
//       }
//   }
//   return TRUE;
// (CompareNoCase's ATLENSURE on a NULL buffer -- the AtlThrow(E_FAIL) block at instruction
// 0x382f0 inside this function, entry 0x37f50 -- cannot fire on a constructed CString and is
// not reproduced.)
// Symbol: ?AutosaveDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@H@Z
extern "C" int MS_ABI impl__AutosaveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument__H_Z(void* pThis, CDocument* pDocument, int bResetModifiedFlag) {
    S_DRH* s = Self(pThis);
    if ((s->m_dwRestartManagerSupportFlags & (kRM_AutosaveAtRestart | kRM_AutosaveAtInterval)) == 0 || pDocument == nullptr) {
        return FALSE;
    }
    CString strDocName;
    LookupString<KeyKind::Ptr>(MapDocToName(s), pDocument, strDocName);
    if (strDocName.IsEmpty()) {
        {
            RetStr strListName;
            VCall<CString*>(s, kDR_GetDocumentListName, strListName.slot(), pDocument);
            strDocName = strListName.get();
        }
        SetDocString(MapDocToName(s), pDocument, strDocName);
    }
    if (!strDocName.IsEmpty()) {
        CString strAutosaveName;
        CString strNewAutosaveName;
        if (VCall<int>(pDocument, kDoc_IsModified)) {
            if (!LookupString<KeyKind::String>(MapNameToAutosave(s), static_cast<const wchar_t*>(strDocName), strAutosaveName)) {
                SetAutosaveName(s, strDocName, L"");
            }
            if ((s->m_dwRestartManagerSupportFlags & (kRM_AutosaveAtRestart | kRM_AutosaveAtInterval)) != 0) {
                {
                    RetStr strGenerated;
                    VCall<CString*>(s, kDR_GenerateAutosaveFileName, strGenerated.slot(), static_cast<const CString*>(&strDocName));
                    strNewAutosaveName = strGenerated.get();
                }
                if (VCall<int>(pDocument, kDoc_DoSave, static_cast<const wchar_t*>(strNewAutosaveName), FALSE)) {
                    SetAutosaveName(s, strDocName, strNewAutosaveName);
                    if (::_wcsicmp(strAutosaveName, strNewAutosaveName) != 0) {
                        VCall<int>(s, kDR_DeleteAutosavedFile, static_cast<const CString*>(&strAutosaveName));
                    }
                    VCall<void>(pDocument, kDoc_SetModifiedFlag, bResetModifiedFlag ? TRUE : FALSE);
                }
            }
        } else {
            const bool bHad = LookupString<KeyKind::String>(MapNameToAutosave(s), static_cast<const wchar_t*>(strDocName), strAutosaveName);
            SetAutosaveName(s, strDocName, L"");
            if (bHad) VCall<int>(s, kDR_DeleteAutosavedFile, static_cast<const CString*>(&strAutosaveName));
        }
    }
    return TRUE;
}

// Retail 0x38300:
//   BOOL bRet = TRUE;
//   if (m_dwRestartManagerSupportFlags & (AUTOSAVE_AT_RESTART | AUTOSAVE_AT_INTERVAL)) {
//       CWinApp* pApp = AfxGetApp();
//       if (pApp != NULL && pApp->m_pDocManager != NULL) {             // +0xb8
//           POSITION posTemplate = m_pDocManager->GetFirstDocTemplatePosition();   // vslot 6
//           while (posTemplate != NULL) {
//               CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(posTemplate);  // vslot 7
//               POSITION posDoc = pTemplate->GetFirstDocPosition();                // vslot 23
//               while (posDoc != NULL) {
//                   CDocument* pDoc = pTemplate->GetNextDoc(posDoc);               // vslot 24
//                   bRet &= AutosaveDocumentInfo(pDoc, FALSE);                     // vslot 18
//               }
//           }
//       }
//   }
//   return bRet;
// DEVIATIONS: OpenMFC's CWinApp keeps its templates in core/app/CWinApp.cpp's side table, not
// in an m_pDocManager at +0xb8, so the template walk goes through the exported
// CWinApp::GetFirstDocTemplatePosition / GetNextDocTemplate (which in retail forward to those
// same two doc-manager slots).  The per-template document walk calls OpenMFC's
// CDocTemplate::GetFirstDocPosition / GetNextDoc thunks directly instead of vslots 23/24:
// OpenMFC's template constructors install no MSVC vftable, and those two bodies are what both
// OpenMFC template classes' own GetFirstDocPosition / GetNextDoc forward to.  A NULL template
// is skipped (retail would fault).
// Symbol: ?AutosaveAllDocumentInfo@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__AutosaveAllDocumentInfo_CDataRecoveryHandler__UEAAHXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    BOOL bRet = TRUE;
    if ((s->m_dwRestartManagerSupportFlags & (kRM_AutosaveAtRestart | kRM_AutosaveAtInterval)) != 0) {
        CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
        if (pApp != nullptr) {
            void* posTemplate = impl__GetFirstDocTemplatePosition_CWinApp__QEBAPEAU__POSITION__XZ(pApp);
            while (posTemplate != nullptr) {
                CDocTemplate* pTemplate = impl__GetNextDocTemplate_CWinApp__QEBAPEAVCDocTemplate__AEAPEAU__POSITION___Z(pApp, &posTemplate);
                if (pTemplate == nullptr) continue;
                void* posDoc = impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(pTemplate);
                while (posDoc != nullptr) {
                    CDocument* pDoc = impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(pTemplate, posDoc);
                    bRet &= VCall<int>(s, kDR_AutosaveDocumentInfo, pDoc, FALSE);
                }
            }
        }
    }
    return bRet;
}

// Retail 0x383f0:
//   if ((m_dwRestartManagerSupportFlags & REOPEN_PREVIOUS_FILES) && !m_bRestoringPreviousOpenDocs) {
//       CString strDocName;
//       if (m_mapDocumentPtrToDocName.Lookup(pDocument, strDocName)) {  // GetAssocAt 0x1bfec
//           m_mapDocumentPtrToDocName.RemoveKey(pDocument);              // 0x3a844
//           m_mapDocumentPtrToDocTitle.RemoveKey(pDocument);             // 0x3a844
//           m_mapDocNameToDocumentPtr.RemoveKey(strDocName);             // 0x3a348
//           CString strAutosaveName;
//           m_mapDocNameToAutosaveName.Lookup(strDocName, strAutosaveName);   // 0x314e0
//           DeleteAutosavedFile(strAutosaveName);                        // vslot 23
//           m_mapDocNameToAutosaveName.RemoveKey(strDocName);            // 0x39f00
//       }
//       strDocName = GetDocumentListName(pDocument);                     // vslot 28
//       m_mapDocumentPtrToDocName[pDocument] = strDocName;               // 0x3a6fc
//       m_mapDocNameToAutosaveName[strDocName] = _T("");                 // 0x39dac
//   }
//   return TRUE;
// Symbol: ?CreateDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@@Z
extern "C" int MS_ABI impl__CreateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* pThis, CDocument* pDocument) {
    S_DRH* s = Self(pThis);
    if ((s->m_dwRestartManagerSupportFlags & kRM_ReopenPreviousFiles) != 0 && !s->m_bRestoringPreviousOpenDocs) {
        CString strDocName;
        if (LookupString<KeyKind::Ptr>(MapDocToName(s), pDocument, strDocName)) {
            DRM_RemoveKey<KeyKind::Ptr, ValKind::String>(MapDocToName(s), pDocument);
            DRM_RemoveKey<KeyKind::Ptr, ValKind::String>(MapDocToTitle(s), pDocument);
            DRM_RemoveKey<KeyKind::String, ValKind::Pod>(MapNameToDoc(s), static_cast<const wchar_t*>(strDocName));
            CString strAutosaveName;
            LookupString<KeyKind::String>(MapNameToAutosave(s), static_cast<const wchar_t*>(strDocName), strAutosaveName);
            VCall<int>(s, kDR_DeleteAutosavedFile, static_cast<const CString*>(&strAutosaveName));
            DRM_RemoveKey<KeyKind::String, ValKind::String>(MapNameToAutosave(s), static_cast<const wchar_t*>(strDocName));
        }
        {
            RetStr strListName;
            VCall<CString*>(s, kDR_GetDocumentListName, strListName.slot(), pDocument);
            strDocName = strListName.get();
        }
        SetDocString(MapDocToName(s), pDocument, strDocName);
        SetAutosaveName(s, strDocName, L"");
    }
    return TRUE;
}

// Retail 0x38610:
//   if (m_dwRestartManagerSupportFlags & REOPEN_PREVIOUS_FILES) {
//       RemoveDocumentInfo(pDocument);                                   // vslot 22, result ignored
//       CString strDocName = GetDocumentListName(pDocument);             // vslot 28
//       m_mapDocumentPtrToDocName[pDocument] = strDocName;               // 0x3a6fc
//       m_mapDocNameToAutosaveName[strDocName] = _T("");                 // 0x39dac
//   }
//   return TRUE;
// Symbol: ?UpdateDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@@Z
extern "C" int MS_ABI impl__UpdateDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* pThis, CDocument* pDocument) {
    S_DRH* s = Self(pThis);
    if ((s->m_dwRestartManagerSupportFlags & kRM_ReopenPreviousFiles) != 0) {
        VCall<int>(s, kDR_RemoveDocumentInfo, pDocument);
        RetStr strDocName;
        VCall<CString*>(s, kDR_GetDocumentListName, strDocName.slot(), pDocument);
        SetDocString(MapDocToName(s), pDocument, strDocName.get());
        SetAutosaveName(s, strDocName.get(), L"");
    }
    return TRUE;
}

// Retail 0x386e0:
//   if (m_dwRestartManagerSupportFlags & REOPEN_PREVIOUS_FILES) {
//       CString strDocName;
//       if (m_mapDocumentPtrToDocName.Lookup(pDocument, strDocName)) {  // GetAssocAt 0x1bfec
//           CString strAutosaveName;
//           m_mapDocNameToAutosaveName.Lookup(strDocName, strAutosaveName);   // 0x314e0
//           DeleteAutosavedFile(strAutosaveName);                        // vslot 23
//           m_mapDocNameToAutosaveName.RemoveKey(strDocName);            // 0x39f00
//           m_mapDocNameToDocumentPtr.RemoveKey(strDocName);             // 0x3a348
//       }
//       m_mapDocumentPtrToDocName.RemoveKey(pDocument);                  // 0x3a844
//       m_mapDocumentPtrToDocTitle.RemoveKey(pDocument);                 // 0x3a844
//   }
//   return TRUE;
// Symbol: ?RemoveDocumentInfo@CDataRecoveryHandler@@UEAAHPEAVCDocument@@@Z
extern "C" int MS_ABI impl__RemoveDocumentInfo_CDataRecoveryHandler__UEAAHPEAVCDocument___Z(void* pThis, CDocument* pDocument) {
    S_DRH* s = Self(pThis);
    if ((s->m_dwRestartManagerSupportFlags & kRM_ReopenPreviousFiles) != 0) {
        CString strDocName;
        if (LookupString<KeyKind::Ptr>(MapDocToName(s), pDocument, strDocName)) {
            CString strAutosaveName;
            LookupString<KeyKind::String>(MapNameToAutosave(s), static_cast<const wchar_t*>(strDocName), strAutosaveName);
            VCall<int>(s, kDR_DeleteAutosavedFile, static_cast<const CString*>(&strAutosaveName));
            DRM_RemoveKey<KeyKind::String, ValKind::String>(MapNameToAutosave(s), static_cast<const wchar_t*>(strDocName));
            DRM_RemoveKey<KeyKind::String, ValKind::Pod>(MapNameToDoc(s), static_cast<const wchar_t*>(strDocName));
        }
        DRM_RemoveKey<KeyKind::Ptr, ValKind::String>(MapDocToName(s), pDocument);
        DRM_RemoveKey<KeyKind::Ptr, ValKind::String>(MapDocToTitle(s), pDocument);
    }
    return TRUE;
}

// Retail 0x38850:
//   if (!strAutosavedFile.IsEmpty() && !::DeleteFile(strAutosavedFile))
//       m_lstAutosavesToDelete.AddTail(strAutosavedFile);             // 0x2325f0
//   return TRUE;
// Not on this file's stub list, but its placeholder body returned FALSE for an empty name and
// never queued a failed deletion for the destructor, so it is transcribed as well.
// Symbol: ?DeleteAutosavedFile@CDataRecoveryHandler@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__DeleteAutosavedFile_CDataRecoveryHandler__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, const CString* strAutosavedFile) {
    S_DRH* s = Self(pThis);
    if (!strAutosavedFile->IsEmpty() && !::DeleteFileW(*strAutosavedFile)) {
        impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(List(s), *strAutosavedFile);
    }
    return TRUE;
}

// Retail 0x388a0:
//   POSITION pos = m_mapDocNameToAutosaveName.GetStartPosition();
//   while (pos != NULL) {
//       CString strDocName, strAutosaveName;
//       m_mapDocNameToAutosaveName.GetNextAssoc(pos, strDocName, strAutosaveName);   // 0x39fdc
//       if (!strAutosaveName.IsEmpty()) {
//           m_mapDocNameToAutosaveName[strDocName] = _T("");              // 0x39dac
//           DeleteAutosavedFile(strAutosaveName);                         // vslot 23
//       }
//   }
//   return TRUE;
// Symbol: ?DeleteAllAutosavedFiles@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__DeleteAllAutosavedFiles_CDataRecoveryHandler__UEAAHXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    void* pos = DRM_GetStartPosition(MapNameToAutosave(s));
    while (pos != nullptr) {
        CString strDocName, strAutosaveName;
        DRM_NextStrStr(MapNameToAutosave(s), pos, strDocName, strAutosaveName);
        if (!strAutosaveName.IsEmpty()) {
            SetAutosaveName(s, strDocName, L"");
            VCall<int>(s, kDR_DeleteAutosavedFile, static_cast<const CString*>(&strAutosaveName));
        }
    }
    return TRUE;
}

// Retail 0x389e0:
//   BOOL bRet = FALSE;
//   m_mapDocNameToDocumentPtr.RemoveAll();                               // 0x3a400
//   m_bRestoringPreviousOpenDocs = TRUE;
//   if (m_dwRestartManagerSupportFlags & REOPEN_PREVIOUS_FILES) {
//       POSITION pos = m_mapDocNameToAutosaveName.GetStartPosition();
//       while (pos != NULL) {
//           CString strDocName, strAutosaveName;
//           m_mapDocNameToAutosaveName.GetNextAssoc(pos, strDocName, strAutosaveName);
//           CDocument* pDocument = NULL;
//           if (strDocName.FindOneOf(_T(":/\\")) != -1) {                 // wcspbrk
//               pDocument = AfxGetApp()->OpenDocumentFile(strDocName, TRUE);    // app vslot 42
//           } else {
//               CWinApp* pApp = AfxGetApp();
//               if (pApp != NULL && pApp->m_pDocManager != NULL) {         // +0xb8
//                   CDocTemplate* pTemplate = m_pDocManager->GetBestTemplate(strDocName);   // vslot 14
//                   if (pTemplate != NULL &&
//                       (pDocument = pTemplate->OpenDocumentFile(NULL, FALSE, TRUE)) != NULL) {   // vslot 34
//                       CString strTitle(strDocName);
//                       int iDot = strDocName.ReverseFind(_T('.'));
//                       if (iDot > 0) strTitle = strDocName.Left(iDot);
//                       pDocument->SetTitle(strTitle);                     // doc vslot 23
//                   }
//               }
//           }
//           m_mapDocNameToDocumentPtr[strDocName] = pDocument;           // 0x3a218
//           bRet |= (pDocument != NULL);
//       }
//   }
//   m_bRestoringPreviousOpenDocs = FALSE;
//   return bRet;
// DEVIATION: the untitled-document branch (no ':', '/' or '\\' in the name) takes retail's
// `m_pDocManager == NULL` path and reopens nothing: OpenMFC's CWinApp has no m_pDocManager at
// +0xb8 (its templates live in core/app/CWinApp.cpp's side table) and there is no exported
// entry point for GetBestTemplate that does not need a CDocManager object.  DEVIATION: a NULL
// AfxGetApp() gives a NULL document instead of faulting.
// Symbol: ?ReopenPreviousDocuments@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__ReopenPreviousDocuments_CDataRecoveryHandler__UEAAHXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    BOOL bRet = FALSE;
    DRM_RemoveAll<KeyKind::String, ValKind::Pod>(MapNameToDoc(s));
    s->m_bRestoringPreviousOpenDocs = TRUE;
    if ((s->m_dwRestartManagerSupportFlags & kRM_ReopenPreviousFiles) != 0) {
        void* pos = DRM_GetStartPosition(MapNameToAutosave(s));
        while (pos != nullptr) {
            CString strDocName, strAutosaveName;
            DRM_NextStrStr(MapNameToAutosave(s), pos, strDocName, strAutosaveName);
            CDocument* pDocument = nullptr;
            if (strDocName.FindOneOf(L":/\\") != -1) {
                if (CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
                    pDocument = VCall<CDocument*>(pApp, kApp_OpenDocumentFile, static_cast<const wchar_t*>(strDocName), TRUE);
                }
            }
            if (void** v = DRM_Index<KeyKind::String, ValKind::Pod>(MapNameToDoc(s), static_cast<const wchar_t*>(strDocName))) {
                *v = pDocument;
            }
            bRet |= (pDocument != nullptr) ? TRUE : FALSE;
        }
    }
    s->m_bRestoringPreviousOpenDocs = FALSE;
    return bRet;
}

// Retail 0x38c80:
//   int nResult;
//   if (CTaskDialog::IsSupported()) {                                   // 0x142380
//       CString strMain;   strMain.LoadString(ASK_1);  strMain += _T("\r\n");
//       CString strLine2;  strLine2.LoadString(ASK_2); strMain += strLine2;
//       CString strContent; strContent.LoadString(ASK_3);
//       CString strFooter;  strFooter.LoadString(ASK_4);
//       CString strTitle(AfxGetApp()->m_pszAppName);                     // +0xa0
//       INT_PTR nChoice = CTaskDialog::ShowDialog(strContent, strMain, strTitle,
//                             ASK_5, ASK_6, 0 /*buttons*/, TDF_USE_COMMAND_LINKS /*0x10*/,
//                             strFooter);
//       // 0x1423f0 has no name in the symbol map; its body builds a CTaskDialog from its eight
//       // arguments, runs DoModal and destroys it -- the static CTaskDialog::ShowDialog.
//       nResult = (nChoice == ASK_5) ? IDYES : IDNO;
//   } else {
//       CString strMsg;  strMsg.LoadString(ASK_1);  strMsg += _T("\r\n");
//       CString str;     str.LoadString(ASK_2);  strMsg += str;  strMsg += _T("\r\n\r\n");
//                        str.LoadString(ASK_3);  strMsg += str;  strMsg += _T("\r\n\r\n");
//                        str.LoadString(ASK_4);  strMsg += str;
//       nResult = AfxMessageBox(strMsg, MB_YESNO, 0);                     // 0x1cec10
//   }
//   POSITION pos = m_mapDocNameToAutosaveName.GetStartPosition();
//   while (pos != NULL) {
//       CString strDocName, strAutosaveName;
//       m_mapDocNameToAutosaveName.GetNextAssoc(pos, strDocName, strAutosaveName);
//       if (!strAutosaveName.IsEmpty())
//           m_mapDocNameToRestoreBool[strDocName] = (nResult == IDYES);   // 0x3a218
//   }
// (ASK_n = AFX_IDS_AUTOSAVE_RECOVERY_ASK_n, 0xF2E4..0xF2E9.)
// DEVIATION: the task-dialog branch is transcribed but not taken (kUseTaskDialog).  OpenMFC's
// ?ShowDialog@CTaskDialog@@ thunk (core/dialog/CTaskDialog.cpp) hands its 4th-7th arguments to
// a CTaskDialog constructor whose parameters are (nCommonButtons, nTaskDialogOptions, int, int),
// so ASK_5 becomes the common-button mask, ASK_6 the option flags, and no command link is
// created: the dialog can never return ASK_5, every answer would read as IDNO, and RestoreAutosavedDocuments would then
// delete every auto-saved file.  Until that thunk is fixed the AfxMessageBox branch -- retail's
// own path when task dialogs are unsupported -- is used unconditionally.
// DEVIATION: the strings fall back to the retail en-US text when OpenMFC finds no resource
// (LoadStringOr), and the task-dialog title reads OpenMFC's CWinApp::m_pszAppName member
// (+0x88 in OpenMFC's layout; retail +0xa0), NULL-checked.
// Symbol: ?QueryRestoreAutosavedDocuments@CDataRecoveryHandler@@UEAAXXZ
extern "C" void MS_ABI impl__QueryRestoreAutosavedDocuments_CDataRecoveryHandler__UEAAXXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    constexpr bool kUseTaskDialog = false;   // see DEVIATION above
    int nResult;
    if (kUseTaskDialog && impl__IsSupported_CTaskDialog__SAHXZ()) {
        CString strMain;
        LoadStringOr(strMain, kIDS_Ask1, kTextAsk1);
        CString strLine2;
        LoadStringOr(strLine2, kIDS_Ask2, kTextAsk2);
        strMain += L"\r\n";
        strMain += strLine2;
        CString strContent;
        LoadStringOr(strContent, kIDS_Ask3, kTextAsk3);
        CString strFooter;
        LoadStringOr(strFooter, kIDS_Ask4, kTextAsk4);
        CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
        CString strTitle((pApp != nullptr) ? pApp->m_pszAppName : nullptr);
        const __int64 nChoice = impl__ShowDialog_CTaskDialog__SA_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HHHH0_Z(
            &strContent, &strMain, &strTitle, static_cast<int>(kIDS_Ask5), static_cast<int>(kIDS_Ask6), 0, TDF_USE_COMMAND_LINKS, &strFooter);
        nResult = (nChoice == static_cast<__int64>(kIDS_Ask5)) ? IDYES : IDNO;
    } else {
        CString strMsg;
        LoadStringOr(strMsg, kIDS_Ask1, kTextAsk1);
        strMsg += L"\r\n";
        CString str;
        LoadStringOr(str, kIDS_Ask2, kTextAsk2);
        strMsg += str;
        strMsg += L"\r\n\r\n";
        LoadStringOr(str, kIDS_Ask3, kTextAsk3);
        strMsg += str;
        strMsg += L"\r\n\r\n";
        LoadStringOr(str, kIDS_Ask4, kTextAsk4);
        strMsg += str;
        nResult = impl__AfxMessageBox__YAHPEB_WII_Z(strMsg, MB_YESNO, 0);
    }
    void* pos = DRM_GetStartPosition(MapNameToAutosave(s));
    while (pos != nullptr) {
        CString strDocName, strAutosaveName;
        DRM_NextStrStr(MapNameToAutosave(s), pos, strDocName, strAutosaveName);
        if (!strAutosaveName.IsEmpty()) {
            if (void** v = DRM_Index<KeyKind::String, ValKind::Pod>(MapNameToRestore(s), static_cast<const wchar_t*>(strDocName))) {
                *reinterpret_cast<unsigned char*>(v) = (nResult == IDYES) ? 1 : 0;   // retail: sete %cl; mov %cl,(%rax)
            }
        }
    }
}

// Retail 0x391a0:
//   m_mapDocNameToRestoreBool.RemoveAll();                               // 0x3a400
//   m_bRestoringPreviousOpenDocs = TRUE;
//   if ((m_dwRestartManagerSupportFlags & (REOPEN_PREVIOUS_FILES | RESTORE_AUTOSAVED_FILES)) == 0x30) {
//       CFileStatus status;
//       // 1. is there any auto-saved file on disk?
//       for (pos = m_mapDocNameToAutosaveName.GetStartPosition(); pos != NULL; ) {
//           GetNextAssoc(pos, strDocName, strAutosaveName);
//           if (!strAutosaveName.IsEmpty() && CFile::GetStatus(strAutosaveName, status, NULL)) break-found;  // 0x22bac0
//       }
//       if (found) {
//           QueryRestoreAutosavedDocuments();                            // vslot 26
//           bool bRestore;                                               // not initialised
//           for (pos = GetStartPosition(); pos != NULL; ) {
//               CString strDocName, strAutosaveName;  GetNextAssoc(pos, strDocName, strAutosaveName);
//               if (strAutosaveName.IsEmpty() || !CFile::GetStatus(strAutosaveName, status, NULL)) continue;
//               m_mapDocNameToRestoreBool.Lookup(strDocName, bRestore);  // GetAssocAt 0x314e0
//               if (bRestore) {
//                   ::SendMessage(AfxGetApp()->GetMainWnd()->m_hWnd, WM_SETREDRAW, FALSE, 0);   // app vslot 31
//                   CDocument* pDocument = AfxGetApp()->OpenDocumentFile(strAutosaveName, FALSE); // app vslot 42
//                   if (pDocument != NULL) {
//                       CString strTitle;
//                       if (strDocName.FindOneOf(_T(":/\\")) != -1) {
//                           pDocument->SetPathName(strDocName, FALSE);   // doc vslot 24
//                           strTitle = pDocument->GetTitle();            // inline, doc +0x40
//                       } else {
//                           pDocument->ClearPathName();                  // doc vslot 25
//                           strTitle = strDocName;
//                           int iDot = strDocName.ReverseFind(_T('.'));
//                           if (iDot > 0) strTitle = strDocName.Left(iDot);
//                           pDocument->SetTitle(strTitle);               // doc vslot 23
//                       }
//                       m_mapDocumentPtrToDocName[pDocument] = GetDocumentListName(pDocument);   // vslot 28
//                       CDocument* pOld;
//                       if (m_mapDocNameToDocumentPtr.Lookup(strDocName, pOld) && pOld != NULL && pOld != pDocument)
//                           pOld->OnCloseDocument();                     // doc vslot 35
//                       pDocument->SetModifiedFlag(TRUE);                // doc vslot 27
//                       SendMessage(hMain, WM_SETREDRAW, TRUE, 0); InvalidateRect(hMain, NULL, TRUE); UpdateWindow(hMain);
//                       m_mapDocumentPtrToDocTitle[pDocument] = strTitle;
//                       pDocument->SetTitle(GetRecoveredDocumentTitle(strTitle));   // vslot 30, doc vslot 23
//                   } else {
//                       SendMessage(hMain, WM_SETREDRAW, TRUE, 0); InvalidateRect(hMain, NULL, TRUE); UpdateWindow(hMain);
//                   }
//               } else {
//                   m_mapDocNameToAutosaveName[strDocName] = _T("");     // 0x39dac
//                   DeleteAutosavedFile(strAutosaveName);                // vslot 23
//               }
//           }
//       }
//   }
//   m_mapDocNameToRestoreBool.RemoveAll();                               // 0x3a400
//   m_mapDocNameToDocumentPtr.RemoveAll();                               // 0x3a400
//   m_bRestoringPreviousOpenDocs = FALSE;
//   return TRUE;
// (hMain = AfxGetApp()->GetMainWnd()->m_hWnd, re-read before every call.)
// DEVIATIONS: retail's `bRestore` (r15b) is loaded once, uninitialised, before the loop and is
// only overwritten when the lookup hits, so a miss reuses the previous iteration's value (or
// stack garbage on the first).  Here it starts at false; like retail, a miss after a hit still
// reuses the earlier entry's value (the variable lives outside the loop).
// pDocument->GetTitle() reads OpenMFC's CDocument::m_strTitle through
// impl__GetTitle_CDocument__UEBAPEB_WXZ (OpenMFC's CDocument does not keep the title at +0x40,
// and that is the member OpenMFC's SetPathName, reached through doc vslot 24, updates).  The
// window calls are skipped when there is no app or main window (MainWndHandle), and a NULL
// AfxGetApp() gives a NULL document.
// Symbol: ?RestoreAutosavedDocuments@CDataRecoveryHandler@@UEAAHXZ
extern "C" int MS_ABI impl__RestoreAutosavedDocuments_CDataRecoveryHandler__UEAAHXZ(void* pThis) {
    S_DRH* s = Self(pThis);
    DRM_RemoveAll<KeyKind::String, ValKind::Pod>(MapNameToRestore(s));
    s->m_bRestoringPreviousOpenDocs = TRUE;
    if ((s->m_dwRestartManagerSupportFlags & (kRM_ReopenPreviousFiles | kRM_RestoreAutosavedFiles)) ==
        (kRM_ReopenPreviousFiles | kRM_RestoreAutosavedFiles)) {
        CFileStatus status;
        std::memset(&status, 0, sizeof(status));
        bool bFound = false;
        void* pos = DRM_GetStartPosition(MapNameToAutosave(s));
        while (pos != nullptr && !bFound) {
            CString strDocName, strAutosaveName;
            DRM_NextStrStr(MapNameToAutosave(s), pos, strDocName, strAutosaveName);
            if (!strAutosaveName.IsEmpty() &&
                impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(strAutosaveName, status, nullptr)) {
                bFound = true;
            }
        }
        if (bFound) {
            VCall<void>(s, kDR_QueryRestoreAutosavedDocuments);
            bool bRestore = false;
            pos = DRM_GetStartPosition(MapNameToAutosave(s));
            while (pos != nullptr) {
                CString strDocName, strAutosaveName;
                DRM_NextStrStr(MapNameToAutosave(s), pos, strDocName, strAutosaveName);
                if (strAutosaveName.IsEmpty() ||
                    !impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z(strAutosaveName, status, nullptr)) {
                    continue;
                }
                if (DRM_Assoc* a = DRM_Find<KeyKind::String>(MapNameToRestore(s), static_cast<const wchar_t*>(strDocName))) {
                    bRestore = *reinterpret_cast<const unsigned char*>(&a->value) != 0;   // retail: mov 0x8(%rax),%r15b
                }
                if (bRestore) {
                    if (HWND hMain = MainWndHandle()) ::SendMessageW(hMain, WM_SETREDRAW, FALSE, 0);
                    CDocument* pDocument = nullptr;
                    if (CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ()) {
                        pDocument = VCall<CDocument*>(pApp, kApp_OpenDocumentFile, static_cast<const wchar_t*>(strAutosaveName), FALSE);
                    }
                    if (pDocument != nullptr) {
                        CString strTitle;
                        if (strDocName.FindOneOf(L":/\\") != -1) {
                            VCall<void>(pDocument, kDoc_SetPathName, static_cast<const wchar_t*>(strDocName), FALSE);
                            strTitle = impl__GetTitle_CDocument__UEBAPEB_WXZ(pDocument);
                        } else {
                            VCall<void>(pDocument, kDoc_ClearPathName);
                            strTitle = strDocName;
                            const int iDot = strDocName.ReverseFind(L'.');
                            if (iDot > 0) strTitle = strDocName.Left(iDot);
                            VCall<void>(pDocument, kDoc_SetTitle, static_cast<const wchar_t*>(strTitle));
                        }
                        {
                            RetStr strListName;
                            VCall<CString*>(s, kDR_GetDocumentListName, strListName.slot(), pDocument);
                            SetDocString(MapDocToName(s), pDocument, strListName.get());
                        }
                        if (DRM_Assoc* a = DRM_Find<KeyKind::String>(MapNameToDoc(s), static_cast<const wchar_t*>(strDocName))) {
                            CDocument* pOld = static_cast<CDocument*>(a->value);
                            if (pOld != nullptr && pOld != pDocument) VCall<void>(pOld, kDoc_OnCloseDocument);
                        }
                        VCall<void>(pDocument, kDoc_SetModifiedFlag, TRUE);
                        if (HWND hMain = MainWndHandle()) ::SendMessageW(hMain, WM_SETREDRAW, TRUE, 0);
                        if (HWND hMain = MainWndHandle()) ::InvalidateRect(hMain, nullptr, TRUE);
                        if (HWND hMain = MainWndHandle()) ::UpdateWindow(hMain);
                        SetDocString(MapDocToTitle(s), pDocument, strTitle);
                        RetStr strRecovered;
                        VCall<CString*>(s, kDR_GetRecoveredDocumentTitle, strRecovered.slot(), static_cast<const CString*>(&strTitle));
                        VCall<void>(pDocument, kDoc_SetTitle, static_cast<const wchar_t*>(strRecovered.get()));
                    } else {
                        if (HWND hMain = MainWndHandle()) ::SendMessageW(hMain, WM_SETREDRAW, TRUE, 0);
                        if (HWND hMain = MainWndHandle()) ::InvalidateRect(hMain, nullptr, TRUE);
                        if (HWND hMain = MainWndHandle()) ::UpdateWindow(hMain);
                    }
                } else {
                    SetAutosaveName(s, strDocName, L"");
                    VCall<int>(s, kDR_DeleteAutosavedFile, static_cast<const CString*>(&strAutosaveName));
                }
            }
        }
    }
    DRM_RemoveAll<KeyKind::String, ValKind::Pod>(MapNameToRestore(s));
    DRM_RemoveAll<KeyKind::String, ValKind::Pod>(MapNameToDoc(s));
    s->m_bRestoringPreviousOpenDocs = FALSE;
    return TRUE;
}

// Retail 0x398c0:
//   CString strDocumentName = pDocument->GetPathName();                  // inline, doc +0x48
//   if (strDocumentName.IsEmpty()) {
//       strDocumentName = pDocument->GetTitle();                         // inline, doc +0x40
//       CString strExt;
//       CDocTemplate* pTemplate = pDocument->GetDocTemplate();           // inline, doc +0x50
//       if (pTemplate != NULL && pTemplate->GetDocString(strExt, CDocTemplate::filterExt)   // vslot 27, 4
//           && !strExt.IsEmpty()) {
//           int iStart = 0;
//           strDocumentName += strExt.Tokenize(_T(";"), iStart);         // 0x3ae80
//       }
//   }
//   return strDocumentName;
// Not on this file's stub list.  Its placeholder took (result, this, ...) -- the wrong order for
// a member function returning a CString (retail: RCX this, RDX result, R8 pDocument) -- and
// read the title through a virtual call on OpenMFC's CDocument declaration, so it is rewritten.
// DEVIATIONS: path and title come from OpenMFC's CDocument accessors (impl__GetPathName_ /
// impl__GetTitle_CDocument), because OpenMFC's CDocument keeps them elsewhere than +0x48/+0x40;
// the template-extension step takes retail's `GetDocTemplate() == NULL` path, because
// OpenMFC's CDocument has no m_pDocTemplate member.
// Symbol: ?GetDocumentListName@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCDocument@@@Z
extern "C" CString* MS_ABI impl__GetDocumentListName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    const void* pThis, CString* pRet, CDocument* pDocument) {
    (void)pThis;
    CString* pName = new (pRet) CString(impl__GetPathName_CDocument__UEBAPEB_WXZ(pDocument));
    if (pName->IsEmpty()) *pName = impl__GetTitle_CDocument__UEBAPEB_WXZ(pDocument);
    return pRet;
}

// Retail 0x399f0:
//   int iSlash = strDocumentName.ReverseFind(_T('\\'));                   // wcsrchr, -1 if none
//   CString strFileName = strDocumentName.Right(strDocumentName.GetLength() - iSlash - 1);   // 0x3ade0
//   GUID guid = GUID_NULL;  ::CoCreateGuid(&guid);                         // result ignored
//   CString strGuid;
//   strGuid.Format(_T("%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X"),   // 0x18033ef40
//                  guid.Data1, guid.Data2, guid.Data3, guid.Data4[0] ... guid.Data4[7]);
//   CString strAutosaveName = GetAutosavePath();                           // vslot 8
//   strAutosaveName += _T("\\");  strAutosaveName += strGuid;
//   strAutosaveName += _T(".");   strAutosaveName += strFileName;
//   return strAutosaveName;
// (Data3 is formatted with a lower-case %04x in the retail format string.)  Not on this file's
// stub list; its placeholder took (result, this, ...) and built the name from GetTempPathW,
// ignoring the autosave path Initialize stores, so it is rewritten.
// Symbol: ?GenerateAutosaveFileName@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
extern "C" CString* MS_ABI impl__GenerateAutosaveFileName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    const void* pThis, CString* pRet, const CString* strDocumentName) {
    const int iSlash = strDocumentName->ReverseFind(L'\\');
    CString strFileName = strDocumentName->Right(strDocumentName->GetLength() - iSlash - 1);
    GUID guid;
    std::memset(&guid, 0, sizeof(guid));
    ::CoCreateGuid(&guid);
    CString strGuid;
    FormatMs(strGuid, L"%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X",
             static_cast<unsigned long>(guid.Data1), static_cast<unsigned int>(guid.Data2), static_cast<unsigned int>(guid.Data3),
             static_cast<unsigned int>(guid.Data4[0]), static_cast<unsigned int>(guid.Data4[1]),
             static_cast<unsigned int>(guid.Data4[2]), static_cast<unsigned int>(guid.Data4[3]),
             static_cast<unsigned int>(guid.Data4[4]), static_cast<unsigned int>(guid.Data4[5]),
             static_cast<unsigned int>(guid.Data4[6]), static_cast<unsigned int>(guid.Data4[7]));
    new (pRet) CString();
    {
        RetStr strPath;
        VCall<CString*>(pThis, kDR_GetAutosavePath, strPath.slot());
        *pRet = strPath.get();
    }
    *pRet += L"\\";
    *pRet += strGuid;
    *pRet += L".";
    *pRet += strFileName;
    return pRet;
}

// Retail 0x39c40: `CString strFormat; strFormat.LoadString(0xF2EA); CString strRet;
// strRet.Format(strFormat, (LPCTSTR)strDocumentTitle); return strRet;` -- the retail en-US
// resource 0xF2EA is L"%Ts [Recovered]".
// Not on this file's stub list.  Only its placeholder's parameter order was wrong ((result,
// this, ...) instead of (this, result, ...)); the body, which appends L" [Recovered]" to the
// title, produces the retail en-US result without depending on a string resource OpenMFC does
// not ship, and is kept.  DEVIATION: an application that overrides resource 0xF2EA does not see
// its format applied.
// Symbol: ?GetRecoveredDocumentTitle@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
extern "C" CString* MS_ABI impl__GetRecoveredDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    const void* pThis, CString* pRet, const CString* strDocumentTitle) {
    (void)pThis;
    CString* pTitle = new (pRet) CString(CStringText(strDocumentTitle));
    *pTitle += L" [Recovered]";
    return pRet;
}

// Retail 0x39d20: `CString strTitle; m_mapDocumentPtrToDocTitle.Lookup(pDocument, strTitle);
// return strTitle;` (GetAssocAt 0x1bfec on the +0xe8 map).  Not on this file's stub list; its
// placeholder took (result, this, ...) and returned the document's own title, so it is rewritten.
// Symbol: ?GetNormalDocumentTitle@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCDocument@@@Z
extern "C" CString* MS_ABI impl__GetNormalDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    const void* pThis, CString* pRet, CDocument* pDocument) {
    new (pRet) CString();
    LookupString<KeyKind::Ptr>(&const_cast<S_DRH*>(Self(pThis))->m_mapDocumentPtrToDocTitle, pDocument, *pRet);
    return pRet;
}
