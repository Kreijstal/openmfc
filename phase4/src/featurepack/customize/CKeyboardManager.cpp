// CKeyboardManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstdint>

// ===========================================================================
// The accelerator-table half of CKeyboardManager (the bodies at the end of
// this file) is transcribed from the retail mfc140u.dll disassembly, the method
// described in the header of core/ole/COleControl.cpp.  Entry RVAs (mfc140u),
// resolved from each export's ordinal through mfc140u.dll's export address
// table (mfc140u_rva_symbols.json has only some of them):
//   UpdateAccelTable(CMultiDocTemplate*, LPACCEL, int, CFrameWnd*) 0x733a0
//   UpdateAccelTable(CMultiDocTemplate*, HACCEL, CFrameWnd*)       0x73520
//   SaveAcceleratorState 0x73670   LoadAcceleratorState 0x73860
//   FindDefaultAccelerator 0x74020 SetAccelTable 0x741f0
//   IsKeyHandled 0x744c0           ShowAllAccelerators 0x74580
//
// Retail's class is CObject plus static members only (afxkeyboardmanager.h:27;
// sizeof is CObject's), and every body below is either static or never reads
// `this`.  The statics are the exported data objects (mfc140u .data):
//   m_hAccelLast 0x3be220   m_hAccelDefaultLast 0x3be228   m_nAccelSize 0x3be230
//   m_nAccelDefaultSize 0x3be234   m_lpAccelDefault 0x3be238   m_lpAccel 0x3be240
//   m_bAllAccelerators 0x3be248
// defined in core/runtime/StaticData.cpp (the pointers and handles) and
// featurepack/customize/StaticData.cpp (the ints), and read here the way retail
// reads them.
//
// Structural deviations, named again where they bite:
//  (1) CFrameWnd::m_hAccelTable.  Retail reads and writes it at +0xf8.  It is
//      accessed here through OpenMFC's own C++ member (at +0xf0 in OpenMFC's
//      include/openmfc/afxwin.h layout), as CMFCToolBarsKeyboardPropertyPage.cpp
//      and CMFCKeyMapDialog.cpp do, so this file agrees with every other
//      OpenMFC reader and writer of the frame's table (see headerRequests).
//  (2) CMultiDocTemplate::m_hAccelTable.  Retail reads it at +0xf8; OpenMFC's
//      CMultiDocTemplate declares no accelerator table at all (the same
//      deviation (3) of CMFCToolBarsKeyboardPropertyPage.cpp).
//  (3) CFrameWnd::GetDefaultAccelerator is retail vslot 109 (+0x368).  It is
//      dispatched through that slot only when the frame's vftable lies outside
//      this image (an MSVC client class, whose vftable has retail layout);
//      an object OpenMFC built itself carries a g++ vftable and goes through
//      the exported thunk, which makes the C++ virtual call.
//  (4) The CSettingsStore calls (retail vslots 0x28 CreateKey, 0x30 Open,
//      0x60 Write(LPCTSTR,LPBYTE,UINT), 0x98 Read(LPCTSTR,LPBYTE*,UINT*) of the
//      CSettingsStore vftable at mfc140u 0x30e610 -- slot contents read from the
//      table and named by ordinal) go through the exported thunks, as
//      CMouseManager.cpp and CMFCToolBar.cpp do, so a store subclass installed
//      with CSettingsStoreSP::SetRuntimeClass is not dispatched to.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the definitions in the tree (file
// named on each line).
// ---------------------------------------------------------------------------
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                    // detail/MfcExceptionsSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                         // detail/MemcoreSupport.cpp
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);                                                 // detail/MemcoreSupport.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();             // core/frame/CFrameWnd.cpp
extern "C" HACCEL__* MS_ABI impl__GetDefaultAccelerator_CFrameWnd__UEAAPEAUHACCEL____XZ(CFrameWnd* pThis);   // core/frame/Thunks.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                               // core/app/Globals.cpp
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);              // core/app/CWinThread.cpp
extern "C" void* MS_ABI impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(void* pThis, LPACCEL lpAccel);   // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void  MS_ABI impl___1CMFCAcceleratorKey__UEAA_XZ(void* pThis);                              // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void  MS_ABI impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const void* pThis, CString* str);                                                                  // featurepack/customize/CMFCAcceleratorKey.cpp
extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(void* pThis, int bAdmin, int bReadOnly);   // core/app/CSettingsStoreSP.cpp
extern "C" int   MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* pStore, const wchar_t* lpszPath);   // core/app/CSettingsStore.cpp
extern "C" int   MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(void* pStore, const wchar_t* lpszPath);   // core/app/CSettingsStore.cpp
extern "C" int   MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WPEAPEAEPEAI_Z(void* pStore, const wchar_t* lpszValueName, unsigned char** ppData, unsigned int* pBytes);   // core/app/CSettingsStore.cpp
extern "C" int   MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(void* pStore, const wchar_t* lpszValueName, const unsigned char* pData, unsigned int nBytes);   // core/app/CSettingsStore.cpp

// Exported static data members, declared with the types of their definitions.
extern "C" void* impl__m_hAccelLast_CKeyboardManager__1PEAUHACCEL____EA;          // core/runtime/StaticData.cpp
extern "C" void* impl__m_hAccelDefaultLast_CKeyboardManager__1PEAUHACCEL____EA;   // core/runtime/StaticData.cpp
extern "C" void* impl__m_lpAccel_CKeyboardManager__1PEAUtagACCEL__EA;             // core/runtime/StaticData.cpp
extern "C" void* impl__m_lpAccelDefault_CKeyboardManager__1PEAUtagACCEL__EA;      // core/runtime/StaticData.cpp
extern "C" std::int32_t impl__m_nAccelSize_CKeyboardManager__1HA;                 // featurepack/customize/StaticData.cpp
extern "C" std::int32_t impl__m_nAccelDefaultSize_CKeyboardManager__1HA;          // featurepack/customize/StaticData.cpp
extern "C" std::int32_t impl__m_bAllAccelerators_CKeyboardManager__1HA;           // featurepack/customize/StaticData.cpp

// ?m_lstUnpermittedCommands@CMFCToolBar@@1V?$CList@II@@A (mfc140u 0x3b2058) is
// defined with the retail CList<UINT,UINT> layout in
// featurepack/toolbar/CMFCToolBar.cpp; this mirror (the one
// CMFCToolBarsCustomizeDialog.cpp also uses) lets LoadAcceleratorState walk it.
struct KbdUIntListNode {
    KbdUIntListNode* pNext;   // +0x00
    KbdUIntListNode* pPrev;   // +0x08
    UINT             data;    // +0x10
};
struct KbdUIntList {
    void*            vfptr;         // +0x00
    KbdUIntListNode* m_pNodeHead;   // +0x08
    KbdUIntListNode* m_pNodeTail;   // +0x10
    INT_PTR          m_nCount;      // +0x18
    KbdUIntListNode* m_pNodeFree;   // +0x20
    void*            m_pBlocks;     // +0x28
    INT_PTR          m_nBlockSize;  // +0x30
};
static_assert(sizeof(KbdUIntList) == 56 && sizeof(KbdUIntListNode) == 24, "retail CList<UINT,UINT> shape");
static_assert(offsetof(KbdUIntList, m_pNodeHead) == 0x08 && offsetof(KbdUIntListNode, data) == 0x10, "retail CList<UINT,UINT> offsets");
extern "C" KbdUIntList impl__m_lstUnpermittedCommands_CMFCToolBar__1V__CList_II__A;

// The mingw linker's image base symbol (deviation (3)).
extern "C" IMAGE_DOS_HEADER __ImageBase;

// This file's own thunks that are called before their definitions.
extern "C" void MS_ABI impl__SetAccelTable_CKeyboardManager__KAXAEAPEAUtagACCEL__AEAPEAUHACCEL____AEAHQEAU3__Z(
    LPACCEL* lpAccel, HACCEL* hAccelLast, int* nSize, HACCEL hAccelCur);
extern "C" int MS_ABI impl__UpdateAccelTable_CKeyboardManager__QEAAHPEAVCMultiDocTemplate__PEAUHACCEL____PEAVCFrameWnd___Z(
    void* pThis, CMultiDocTemplate* pTemplate, HACCEL hAccelNew, CFrameWnd* pDefaultFrame);

namespace {

static_assert(sizeof(ACCEL) == 6 && offsetof(ACCEL, key) == 2 && offsetof(ACCEL, cmd) == 4,
              "ACCEL: fVirt +0, key +2, cmd +4 (IsKeyHandled 0x744c0 reads +2 / +0, FindDefaultAccelerator 0x74020 reads +4)");

// ENSURE failure: ?AfxThrowInvalidArgException@@YAXXZ (0x227720, mfc140u).
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

// new ACCEL[n]: retail computes 6 * n (n sign-extended) with `mul` and
// saturates to SIZE_MAX on overflow before calling ??2@ (0x27f0, mfc140u).
LPACCEL NewAccelArray(int n) {
    const unsigned long long count = static_cast<unsigned long long>(static_cast<long long>(n));
    const unsigned long long bytes = count > (~0ULL) / sizeof(ACCEL) ? ~0ULL : count * sizeof(ACCEL);
    return static_cast<LPACCEL>(impl___2_YAPEAX_K_Z(static_cast<std::size_t>(bytes)));
}

// A pointer that lies inside this DLL's own image (a g++ vftable) as opposed
// to an MSVC client's image (the helper of CMFCToolBarsKeyboardPropertyPage.cpp).
bool PointsIntoThisImage(const void* p) {
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* q = static_cast<const unsigned char*>(p);
    return q >= base && q < base + nt->OptionalHeader.SizeOfImage;
}

// pWndFrame->GetDefaultAccelerator() -- retail `call *0x368(%rax)`, vslot 109
// (the same slot core/ole/COleIPFrameWnd.cpp reads out of the COleIPFrameWnd
// vftable).  Deviation (3).
constexpr int kSlotGetDefaultAccelerator = 0x368 / 8;   // 109
HACCEL FrameDefaultAccelerator(CFrameWnd* pFrame) {
    const void* vptr = *reinterpret_cast<void* const*>(pFrame);
    if (vptr != nullptr && !PointsIntoThisImage(vptr)) {
        typedef HACCEL (MS_ABI *Fn)(CFrameWnd*);
        return reinterpret_cast<Fn const*>(vptr)[kSlotGetDefaultAccelerator](pFrame);
    }
    return impl__GetDefaultAccelerator_CFrameWnd__UEAAPEAUHACCEL____XZ(pFrame);
}

// The bIsDefaultFrame selection IsKeyHandled and FindDefaultAccelerator both
// make with three `cmovne`s: the ...Default statics for the main frame, the
// plain ones otherwise.
struct AccelStatics {
    LPACCEL* ppAccel;
    HACCEL*  phAccelLast;
    int*     pnSize;
};
AccelStatics SelectAccelStatics(BOOL bIsDefaultFrame) {
    if (bIsDefaultFrame) {
        return { reinterpret_cast<LPACCEL*>(&impl__m_lpAccelDefault_CKeyboardManager__1PEAUtagACCEL__EA),
                 reinterpret_cast<HACCEL*>(&impl__m_hAccelDefaultLast_CKeyboardManager__1PEAUHACCEL____EA),
                 reinterpret_cast<int*>(&impl__m_nAccelDefaultSize_CKeyboardManager__1HA) };
    }
    return { reinterpret_cast<LPACCEL*>(&impl__m_lpAccel_CKeyboardManager__1PEAUtagACCEL__EA),
             reinterpret_cast<HACCEL*>(&impl__m_hAccelLast_CKeyboardManager__1PEAUHACCEL____EA),
             reinterpret_cast<int*>(&impl__m_nAccelSize_CKeyboardManager__1HA) };
}

// CMFCToolBar::IsCommandPermitted, inlined in LoadAcceleratorState as
// m_lstUnpermittedCommands.Find(cmd) (the CList<UINT,UINT>::Find instantiation
// called at 0x7399a inside 0x73860, mfc140u) tested against NULL.
bool IsCommandPermitted(UINT uiCmd) {
    for (const KbdUIntListNode* n = impl__m_lstUnpermittedCommands_CMFCToolBar__1V__CList_II__A.m_pNodeHead;
         n != nullptr; n = n->pNext) {
        if (n->data == uiCmd) return false;
    }
    return true;
}

// The 16-byte CSettingsStoreSP retail builds on the stack ({m_pRegistry,
// m_dwUserData}, both zeroed), fills with ?Create@CSettingsStoreSP@@ (0x12a550)
// and on scope exit deletes m_pRegistry through its deleting destructor
// (vslot 1, flag 1: the `mov 0x8(%rax),%rax` / `mov $0x1,%edx` pairs in 0x73670
// and 0x73860).  Hand-rolled as in CMouseManager.cpp / CMFCToolBar.cpp, with one
// difference: the store is not always an OpenMFC object.  OpenMFC's Create
// (core/app/CSettingsStoreSP.cpp) instantiates whatever class a client
// installed with CSettingsStoreSP::SetRuntimeClass, and an MSVC client class
// carries an MSVC vftable whose slot 1 is the scalar deleting destructor (a
// g++ `delete` would call slot 2 of it instead).  So a store whose vftable lies
// outside this image is deleted through retail's slot-1 call; an OpenMFC store
// (g++ vftable, in this image) through the C++ virtual destructor.
struct KbdSettingsStoreSP {
    void* slots[2] = { nullptr, nullptr };
    void* Create(int bAdmin, int bReadOnly) {
        return impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(slots, bAdmin, bReadOnly);
    }
    ~KbdSettingsStoreSP() {
        if (slots[0] == nullptr) return;
        const void* vptr = *static_cast<void* const*>(slots[0]);
        if (vptr != nullptr && !PointsIntoThisImage(vptr)) {
            typedef void* (MS_ABI *DeletingDtor)(void*, unsigned int);
            reinterpret_cast<DeletingDtor const*>(vptr)[1](slots[0], 1);
        } else {
            delete static_cast<CObject*>(slots[0]);
        }
    }
};

// Retail literals (mfc140u .rdata): the section format at 0x33f6c0 is
// L"%TsKeyboard-%d" and the value name at 0x33f6e0 is L"Accelerators".
// OpenMFC's CString::Format runs vswprintf, where a wide %s is what the MSVC
// %Ts means (the same substitution CMFCToolBar.cpp makes).
constexpr const wchar_t* kSectionFmt = L"%sKeyboard-%d";
constexpr const wchar_t* kEntryData  = L"Accelerators";

}  // namespace

// Symbol: ??0CKeyboardManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CKeyboardManager__QEAA_XZ(void* pThis) { return new (pThis) CKeyboardManager(); }
// Symbol: ??1CKeyboardManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CKeyboardManager__UEAA_XZ(CKeyboardManager* pThis) { if (pThis) pThis->~CKeyboardManager(); }
// Symbol: ?IsKeyPrintable@CKeyboardManager@@SAHI@Z
extern "C" int MS_ABI impl__IsKeyPrintable_CKeyboardManager__SAHI_Z(unsigned int ch) { return CKeyboardManager::IsKeyPrintable(ch); }
// Symbol: ?TranslateCharToUpper@CKeyboardManager@@SAII@Z
extern "C" unsigned int MS_ABI impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(unsigned int ch) { return CKeyboardManager::TranslateCharToUpper(ch); }
// Symbol: ?ShowAllAccelerators@CKeyboardManager@@SAXH@Z
extern "C" void MS_ABI impl__ShowAllAccelerators_CKeyboardManager__SAXH_Z(int show) { CKeyboardManager::ShowAllAccelerators(show); }
// Symbol: ?CleanUp@CKeyboardManager@@SAXXZ
extern "C" void MS_ABI impl__CleanUp_CKeyboardManager__SAXXZ() { CKeyboardManager::CleanUp(); }
// Symbol: ?ResetAll@CKeyboardManager@@QEAAXXZ
extern "C" void MS_ABI impl__ResetAll_CKeyboardManager__QEAAXXZ(CKeyboardManager* pThis) { if (pThis) pThis->ResetAll(); }
// Symbol: ?LoadState@CKeyboardManager@@QEAAHPEB_WPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__LoadState_CKeyboardManager__QEAAHPEB_WPEAVCFrameWnd___Z(CKeyboardManager* pThis, const wchar_t* profile, CFrameWnd* frame) { return pThis ? pThis->LoadState(profile, frame) : FALSE; }
// Symbol: ?SaveState@CKeyboardManager@@QEAAHPEB_WPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__SaveState_CKeyboardManager__QEAAHPEB_WPEAVCFrameWnd___Z(CKeyboardManager* pThis, const wchar_t* profile, CFrameWnd* frame) { return pThis ? pThis->SaveState(profile, frame) : FALSE; }
CKeyboardManager::CKeyboardManager() { memset(_keyboardmanager_padding, 0, sizeof(_keyboardmanager_padding)); }
CKeyboardManager::~CKeyboardManager() {}
BOOL CKeyboardManager::IsKeyPrintable(UINT nChar) { return nChar >= 0x20 && nChar < 0x7f; }
UINT CKeyboardManager::TranslateCharToUpper(UINT nChar) { return static_cast<UINT>(std::towupper(static_cast<wint_t>(nChar))); }
// Retail ShowAllAccelerators (RVA 0x74580, mfc140u) is `mov %ecx,m_bAllAccelerators ; ret`:
// it stores into the exported static, which FindDefaultAccelerator below reads
// and retail's header-inline IsShowAllAccelerators() returns.  The OpenMFC-internal
// g_showAllAccelerators (detail/MfccoreSupport.cpp) is still written as before.
void CKeyboardManager::ShowAllAccelerators(BOOL bShowAll) {
    impl__m_bAllAccelerators_CKeyboardManager__1HA = bShowAll;
    g_showAllAccelerators = bShowAll;
}
void CKeyboardManager::CleanUp() { g_showAllAccelerators = FALSE; }
void CKeyboardManager::ResetAll() { g_showAllAccelerators = FALSE; }
BOOL CKeyboardManager::LoadState(const wchar_t*, CFrameWnd*) { return TRUE; }
BOOL CKeyboardManager::SaveState(const wchar_t*, CFrameWnd*) { return TRUE; }
// Retail (RVA 0x741f0, mfc140u), fully transcribed:
//     ENSURE(hAccelCur != NULL);
//     if (hAccelCur == hAccelLast) {             // cached copy is current
//         ENSURE(lpAccel != NULL);
//         return;
//     }
//     if (lpAccel != NULL) {
//         delete[] lpAccel;                      // import slot 0x1802c74e8 = ucrt free
//         lpAccel = NULL;
//     }
//     nSize = ::CopyAcceleratorTable(hAccelCur, NULL, 0);   // import slot 0x1802c6d68
//     lpAccel = new ACCEL[nSize];                // ??2@ (0x27f0), 6 * n saturated
//     ENSURE(lpAccel != NULL);
//     ::CopyAcceleratorTable(hAccelCur, lpAccel, nSize);
//     hAccelLast = hAccelCur;
// The array is allocated with ??2@ and released with ??3@ (OpenMFC's pair,
// detail/MemcoreSupport.cpp: malloc / free), matching retail's new / free.
// Symbol: ?SetAccelTable@CKeyboardManager@@KAXAEAPEAUtagACCEL@@AEAPEAUHACCEL__@@AEAHQEAU3@@Z
extern "C" void MS_ABI impl__SetAccelTable_CKeyboardManager__KAXAEAPEAUtagACCEL__AEAPEAUHACCEL____AEAHQEAU3__Z(
    LPACCEL* lpAccel, HACCEL* hAccelLast, int* nSize, HACCEL hAccelCur) {
    if (hAccelCur == nullptr) { ThrowInvalidArg(); return; }
    if (hAccelCur == *hAccelLast) {
        if (*lpAccel == nullptr) ThrowInvalidArg();
        return;
    }
    if (*lpAccel != nullptr) {
        impl___3_YAXPEAX_Z(*lpAccel);
        *lpAccel = nullptr;
    }
    *nSize = ::CopyAcceleratorTableW(hAccelCur, nullptr, 0);
    *lpAccel = NewAccelArray(*nSize);
    if (*lpAccel == nullptr) { ThrowInvalidArg(); return; }
    ::CopyAcceleratorTableW(hAccelCur, *lpAccel, *nSize);
    *hAccelLast = hAccelCur;
}

// Retail (RVA 0x744c0, mfc140u), fully transcribed:
//     if (pWndFrame == NULL) return FALSE;
//     HACCEL hAccelTable = pWndFrame->GetDefaultAccelerator();   // vslot 109 (+0x368)
//     if (hAccelTable == NULL) return FALSE;
//     LPACCEL& lpAccel = bIsDefaultFrame ? m_lpAccelDefault   : m_lpAccel;
//     HACCEL&  hLast   = bIsDefaultFrame ? m_hAccelDefaultLast : m_hAccelLast;
//     int&     nSize   = bIsDefaultFrame ? m_nAccelDefaultSize : m_nAccelSize;
//     SetAccelTable(lpAccel, hLast, nSize, hAccelTable);        // 0x741f0
//     ENSURE(lpAccel != NULL);
//     for (int i = 0; i < nSize; i++)
//         if (lpAccel[i].key == nKey && lpAccel[i].fVirt == fVirt)   // +2 (word), +0 (byte)
//             return TRUE;
//     return FALSE;
// The GetDefaultAccelerator dispatch is deviation (3).
// Symbol: ?IsKeyHandled@CKeyboardManager@@SAHGEPEAVCFrameWnd@@H@Z
extern "C" int MS_ABI impl__IsKeyHandled_CKeyboardManager__SAHGEPEAVCFrameWnd__H_Z(
    unsigned short nKey, unsigned char fVirt, CFrameWnd* pWndFrame, int bIsDefaultFrame) {
    if (pWndFrame == nullptr) return FALSE;
    HACCEL hAccelTable = FrameDefaultAccelerator(pWndFrame);
    if (hAccelTable == nullptr) return FALSE;

    const AccelStatics st = SelectAccelStatics(bIsDefaultFrame);
    impl__SetAccelTable_CKeyboardManager__KAXAEAPEAUtagACCEL__AEAPEAUHACCEL____AEAHQEAU3__Z(
        st.ppAccel, st.phAccelLast, st.pnSize, hAccelTable);
    LPACCEL lpAccel = *st.ppAccel;
    if (lpAccel == nullptr) { ThrowInvalidArg(); return FALSE; }

    for (int i = 0; i < *st.pnSize; i++) {
        if (lpAccel[i].key == nKey && lpAccel[i].fVirt == fVirt) return TRUE;
    }
    return FALSE;
}

// Retail (RVA 0x74020, mfc140u), fully transcribed:
//     str.Empty();                                   // 0x33b0, before any test
//     if (pWndFrame == NULL) return FALSE;
//     HACCEL hAccelTable = pWndFrame->GetDefaultAccelerator();   // vslot 109 (+0x368)
//     if (hAccelTable == NULL) return FALSE;
//     (lpAccel, hLast, nSize selected on bIsDefaultFrame exactly as in IsKeyHandled)
//     SetAccelTable(lpAccel, hLast, nSize, hAccelTable);        // 0x741f0
//     ENSURE(lpAccel != NULL);
//     BOOL bFound = FALSE;
//     for (int i = 0; i < nSize; i++) {
//         if (lpAccel[i].cmd == uiCmd) {             // +4 (word), zero-extended, vs the full UINT
//             bFound = TRUE;
//             CMFCAcceleratorKey helper(&lpAccel[i]);   // vftable 0x1802da2c8 stored inline
//             CString strKey;                        // nil string from the string manager
//             helper.Format(strKey);                 // 0x28f0
//             if (!str.IsEmpty()) str += _T("; ");   // Append(0x33f720, wcslen) -- 0x2ba0
//             str += strKey;                         // Append (0x2ba0)
//             if (!m_bAllAccelerators) break;        // 0x3be248
//         }
//     }
//     return bFound;
// The GetDefaultAccelerator dispatch is deviation (3).  The helper is built and
// torn down with CMFCAcceleratorKey's exported ctor / dtor thunks where retail
// inlines them (the dtor is a vftable store in retail and a no-op in OpenMFC).
// Symbol: ?FindDefaultAccelerator@CKeyboardManager@@SAHIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCFrameWnd@@H@Z
extern "C" int MS_ABI impl__FindDefaultAccelerator_CKeyboardManager__SAHIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCFrameWnd__H_Z(
    unsigned int uiCmd, CString* str, CFrameWnd* pWndFrame, int bIsDefaultFrame) {
    str->Empty();
    if (pWndFrame == nullptr) return FALSE;
    HACCEL hAccelTable = FrameDefaultAccelerator(pWndFrame);
    if (hAccelTable == nullptr) return FALSE;

    const AccelStatics st = SelectAccelStatics(bIsDefaultFrame);
    impl__SetAccelTable_CKeyboardManager__KAXAEAPEAUtagACCEL__AEAPEAUHACCEL____AEAHQEAU3__Z(
        st.ppAccel, st.phAccelLast, st.pnSize, hAccelTable);
    if (*st.ppAccel == nullptr) { ThrowInvalidArg(); return FALSE; }

    BOOL bFound = FALSE;
    for (int i = 0; i < *st.pnSize; i++) {
        LPACCEL pEntry = &(*st.ppAccel)[i];
        if (static_cast<unsigned int>(pEntry->cmd) != uiCmd) continue;

        bFound = TRUE;
        struct { void* vfptr; LPACCEL m_lpAccel; } helper;   // CMFCAcceleratorKey (0x10 bytes)
        impl___0CMFCAcceleratorKey__QEAA_PEAUtagACCEL___Z(&helper, pEntry);
        CString strKey;
        impl__Format_CMFCAcceleratorKey__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(&helper, &strKey);
        if (!str->IsEmpty()) *str += L"; ";
        *str += strKey;
        impl___1CMFCAcceleratorKey__UEAA_XZ(&helper);

        if (!impl__m_bAllAccelerators_CKeyboardManager__1HA) break;
    }
    return bFound;
}

// Retail (RVA 0x73860, mfc140u), fully transcribed:
//     ENSURE(hAccelTable == NULL);
//     CString strSection;
//     strSection.Format(L"%TsKeyboard-%d", lpszProfileName, uiResId);   // 0xda00, literal 0x33f6c0
//     CSettingsStoreSP regSP;
//     CSettingsStore& reg = regSP.Create(FALSE, FALSE);                  // 0x12a550
//     if (!reg.Open(strSection)) return FALSE;                           // vslot 0x30
//     UINT uiSize;  LPACCEL lpAccel;
//     if (reg.Read(L"Accelerators", (LPBYTE*)&lpAccel, &uiSize)) {      // vslot 0x98, literal 0x33f6e0
//         ENSURE(lpAccel != NULL);
//         int nAccelSize = uiSize / sizeof(ACCEL);                       // unsigned divide by 6
//         for (int i = 0; i < nAccelSize; i++)
//             if (!CMFCToolBar::IsCommandPermitted(lpAccel[i].cmd))      // inlined list Find
//                 lpAccel[i].cmd = 0;
//         hAccelTable = ::CreateAcceleratorTable(lpAccel, nAccelSize);  // import slot 0x1802c6d60
//     }
//     delete[] lpAccel;                              // ucrt free, on both paths
//     return hAccelTable != NULL;
// DEVIATIONS: the store calls are deviation (4); a NULL store from OpenMFC's
// Create (retail's returns a reference) reads as a failed Open; lpAccel starts
// NULL so the failed-Read path frees nothing; the buffer is released with
// ::LocalFree, not free, because OpenMFC's CSettingsStore::Read
// (core/app/CSettingsStore.cpp) allocates it with LocalAlloc (the same
// deviation CMouseManager.cpp makes).
// Symbol: ?LoadAcceleratorState@CKeyboardManager@@IEAAHPEB_WIAEAPEAUHACCEL__@@@Z
extern "C" int MS_ABI impl__LoadAcceleratorState_CKeyboardManager__IEAAHPEB_WIAEAPEAUHACCEL_____Z(
    void* pThis, const wchar_t* lpszProfileName, unsigned int uiResId, HACCEL* hAccelTable) {
    (void)pThis;   // retail never reads `this`
    if (*hAccelTable != nullptr) { ThrowInvalidArg(); return FALSE; }

    CString strSection;
    strSection.Format(kSectionFmt, lpszProfileName, uiResId);

    KbdSettingsStoreSP regSP;
    void* pReg = regSP.Create(FALSE, FALSE);
    if (pReg == nullptr || !impl__Open_CSettingsStore__UEAAHPEB_W_Z(pReg, strSection.GetString())) return FALSE;

    unsigned int uiSize = 0;
    LPACCEL lpAccel = nullptr;
    if (impl__Read_CSettingsStore__UEAAHPEB_WPEAPEAEPEAI_Z(pReg, kEntryData, reinterpret_cast<unsigned char**>(&lpAccel), &uiSize)) {
        const int nAccelSize = static_cast<int>(uiSize / sizeof(ACCEL));
        if (lpAccel == nullptr) { ThrowInvalidArg(); return FALSE; }
        for (int i = 0; i < nAccelSize; i++) {
            if (!IsCommandPermitted(lpAccel[i].cmd)) lpAccel[i].cmd = 0;
        }
        *hAccelTable = ::CreateAcceleratorTableW(lpAccel, nAccelSize);
    }

    if (lpAccel != nullptr) ::LocalFree(lpAccel);
    return *hAccelTable != nullptr;
}

// Retail (RVA 0x73670, mfc140u), fully transcribed:
//     ENSURE(hAccelTable != NULL);
//     CString strSection;
//     strSection.Format(L"%TsKeyboard-%d", lpszProfileName, uiResId);   // 0xda00, literal 0x33f6c0
//     CSettingsStoreSP regSP;
//     CSettingsStore& reg = regSP.Create(FALSE, FALSE);                  // 0x12a550
//     int nAccelSize = ::CopyAcceleratorTable(hAccelTable, NULL, 0);    // import slot 0x1802c6d68
//     if (nAccelSize == 0) return FALSE;
//     if (!reg.CreateKey(strSection)) return FALSE;                      // vslot 0x28
//     LPACCEL lpAccel = new ACCEL[nAccelSize];       // ??2@ (0x27f0), 6 * n saturated
//     ENSURE(lpAccel != NULL);
//     ::CopyAcceleratorTable(hAccelTable, lpAccel, nAccelSize);
//     reg.Write(L"Accelerators", (LPBYTE)lpAccel, nAccelSize * sizeof(ACCEL));   // vslot 0x60, result ignored
//     delete[] lpAccel;                              // ucrt free
//     return TRUE;
// DEVIATIONS: the store calls are deviation (4); a NULL store from OpenMFC's
// Create reads as a failed CreateKey; the array goes through ??2@ / ??3@
// (OpenMFC's malloc / free pair).
// Symbol: ?SaveAcceleratorState@CKeyboardManager@@IEAAHPEB_WIPEAUHACCEL__@@@Z
extern "C" int MS_ABI impl__SaveAcceleratorState_CKeyboardManager__IEAAHPEB_WIPEAUHACCEL_____Z(
    void* pThis, const wchar_t* lpszProfileName, unsigned int uiResId, HACCEL hAccelTable) {
    (void)pThis;   // retail never reads `this`
    if (hAccelTable == nullptr) { ThrowInvalidArg(); return FALSE; }

    CString strSection;
    strSection.Format(kSectionFmt, lpszProfileName, uiResId);

    KbdSettingsStoreSP regSP;
    void* pReg = regSP.Create(FALSE, FALSE);

    const int nAccelSize = ::CopyAcceleratorTableW(hAccelTable, nullptr, 0);
    if (nAccelSize == 0) return FALSE;
    if (pReg == nullptr || !impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(pReg, strSection.GetString())) return FALSE;

    LPACCEL lpAccel = NewAccelArray(nAccelSize);
    if (lpAccel == nullptr) { ThrowInvalidArg(); return FALSE; }
    ::CopyAcceleratorTableW(hAccelTable, lpAccel, nAccelSize);
    (void)impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(pReg, kEntryData, reinterpret_cast<const unsigned char*>(lpAccel),
                                                        static_cast<unsigned int>(nAccelSize) * static_cast<unsigned int>(sizeof(ACCEL)));
    impl___3_YAXPEAX_Z(lpAccel);
    return TRUE;
}

// Retail (RVA 0x73520, mfc140u), fully transcribed:
//     ENSURE(hAccelNew != NULL);
//     HACCEL hAccelTable = NULL;
//     if (pTemplate != NULL) {
//         ENSURE(pDefaultFrame == NULL);
//         hAccelTable = pTemplate->m_hAccelTable;    // +0xf8
//         ENSURE(hAccelTable != NULL);
//         pTemplate->m_hAccelTable = hAccelNew;
//         for (POSITION pos = pTemplate->GetFirstDocPosition(); pos != NULL;) {    // vslot 23 (+0xb8)
//             CDocument* pDoc = pTemplate->GetNextDoc(pos);                         // vslot 24 (+0xc0)
//             for (POSITION posView = pDoc->GetFirstViewPosition(); posView != NULL;) {   // doc vslot 28 (+0xe0)
//                 CView* pView = pDoc->GetNextView(posView);                        // doc vslot 29 (+0xe8)
//                 CFrameWnd* pFrame = pView->GetParentFrame();                      // 0x28e200
//                 if (pFrame->m_hAccelTable == hAccelTable)                         // +0xf8
//                     pFrame->m_hAccelTable = hAccelNew;
//             }
//         }
//     } else {
//         if (pDefaultFrame == NULL) {
//             // AfxGetMainWnd(), inlined: AfxGetModuleThreadState() (0x133a20)
//             // ->m_pCurrentWinThread (+0x8); NULL -> return FALSE; else its
//             // GetMainWnd() (vslot 31, +0xf8); NULL -> return FALSE.
//             pDefaultFrame = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());   // IsKindOf 0x234cf0, 0x18033aef0
//             if (pDefaultFrame == NULL) return FALSE;
//         }
//         hAccelTable = pDefaultFrame->m_hAccelTable;   // +0xf8
//         pDefaultFrame->m_hAccelTable = hAccelNew;     // stored even when the old table is NULL
//         if (hAccelTable == NULL) return FALSE;
//     }
//     ::DestroyAcceleratorTable(hAccelTable);       // import slot 0x1802c6d18
//     return TRUE;
// (The descriptor 0x18033aef0 is CFrameWnd's: in the mfc140.dll twin the same
// call site loads the address ?GetThisClass@CFrameWnd@@ returns.  Template
// vslots 23 / 24 were read out of the CMultiDocTemplate vftable at mfc140u
// 0x329458 (?GetFirstDocPosition@ / ?GetNextDoc@CMultiDocTemplate@@); the
// document slot names are MFC's CDocument declaration, not read from a vftable.)
// DEVIATIONS:
//  * the template branch: OpenMFC's CMultiDocTemplate has no m_hAccelTable
//    (deviation (2)), so its table reads as NULL and retail's
//    ENSURE(hAccelTable != NULL) fires -- that throw is all this branch does
//    here (after the ENSURE(pDefaultFrame == NULL) retail makes first); the
//    document / view walk that follows it in retail is not reached;
//  * the frame's table is OpenMFC's CFrameWnd::m_hAccelTable (deviation (1));
//  * AfxGetMainWnd goes through the AfxGetThread and CWinThread::GetMainWnd
//    thunks (as CMFCKeyMapDialog.cpp does).  Both differ from what retail
//    inlines here.  OpenMFC's AfxGetThread (detail/CWinAppSupport.cpp)
//    falls back to the app object when no thread pointer is set, where retail
//    reads m_pCurrentWinThread alone.  OpenMFC's GetMainWnd thunk returns
//    m_pMainWnd and nothing else, where retail makes the vslot-31 call: a
//    GetMainWnd override is not consulted, and the body of the base
//    CWinThread::GetMainWnd (RVA 0x274960, mfc140u) -- m_pActiveWnd (+0x48)
//    if set, else m_pMainWnd (+0x40), else
//    CWnd::FromHandle(::GetActiveWindow()) -- is not reproduced either, so
//    with no m_pMainWnd this returns FALSE where retail may fall back to the
//    active window.
// Symbol: ?UpdateAccelTable@CKeyboardManager@@QEAAHPEAVCMultiDocTemplate@@PEAUHACCEL__@@PEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__UpdateAccelTable_CKeyboardManager__QEAAHPEAVCMultiDocTemplate__PEAUHACCEL____PEAVCFrameWnd___Z(
    void* pThis, CMultiDocTemplate* pTemplate, HACCEL hAccelNew, CFrameWnd* pDefaultFrame) {
    (void)pThis;   // retail never reads `this`
    if (hAccelNew == nullptr) { ThrowInvalidArg(); return FALSE; }

    if (pTemplate != nullptr) {
        if (pDefaultFrame != nullptr) { ThrowInvalidArg(); return FALSE; }
        // hAccelTable = pTemplate->m_hAccelTable reads as NULL (see DEVIATIONS).
        ThrowInvalidArg();
        return FALSE;
    }

    if (pDefaultFrame == nullptr) {
        CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
        if (pThread == nullptr) return FALSE;
        CWnd* pMainWnd = impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread);
        if (pMainWnd == nullptr ||
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pMainWnd, impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            return FALSE;
        }
        pDefaultFrame = static_cast<CFrameWnd*>(pMainWnd);
    }

    HACCEL hAccelTable = pDefaultFrame->m_hAccelTable;
    pDefaultFrame->m_hAccelTable = hAccelNew;
    if (hAccelTable == nullptr) return FALSE;

    ::DestroyAcceleratorTable(hAccelTable);
    return TRUE;
}

// Retail (RVA 0x733a0, mfc140u) is the HACCEL overload (0x73520) inlined
// around a table built from the array:
//     ENSURE(lpAccel != NULL);
//     HACCEL hAccelNew = ::CreateAcceleratorTable(lpAccel, nSize);   // import slot 0x1802c6d60
//     if (hAccelNew == NULL) return FALSE;
//     if (!UpdateAccelTable(pTemplate, hAccelNew, pDefaultFrame)) {
//         ::DestroyAcceleratorTable(hAccelNew);      // import slot 0x1802c6d18
//         return FALSE;
//     }
//     return TRUE;
// Every FALSE exit of the inlined copy (no current thread, no main window, not a
// CFrameWnd, or a frame whose old table was NULL -- which still keeps
// hAccelNew stored in the frame) reaches the one DestroyAcceleratorTable(hAccelNew)
// call at 0x73509 inside 0x733a0; the ENSUREs throw without destroying it.
// Calling the HACCEL thunk reproduces exactly that, with its deviations.
// Symbol: ?UpdateAccelTable@CKeyboardManager@@QEAAHPEAVCMultiDocTemplate@@PEAUtagACCEL@@HPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__UpdateAccelTable_CKeyboardManager__QEAAHPEAVCMultiDocTemplate__PEAUtagACCEL__HPEAVCFrameWnd___Z(
    void* pThis, CMultiDocTemplate* pTemplate, LPACCEL lpAccel, int nSize, CFrameWnd* pDefaultFrame) {
    if (lpAccel == nullptr) { ThrowInvalidArg(); return FALSE; }
    HACCEL hAccelNew = ::CreateAcceleratorTableW(lpAccel, nSize);
    if (hAccelNew == nullptr) return FALSE;
    if (!impl__UpdateAccelTable_CKeyboardManager__QEAAHPEAVCMultiDocTemplate__PEAUHACCEL____PEAVCFrameWnd___Z(
            pThis, pTemplate, hAccelNew, pDefaultFrame)) {
        ::DestroyAcceleratorTable(hAccelNew);
        return FALSE;
    }
    return TRUE;
}
