// CArchive — OpenMFC implementation.
// Sources: filecore.cpp, global_ctordtor_param.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/FilecoreSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// BSTR + SysReAllocStringLen/SysFreeString, used by the CComBSTR loader below.
#include <oleauto.h>

// AfxThrowArchiveException lives in another translation unit and exists only as
// its extern "C" thunk (definition: phase4/src/detail/MfcExceptionsSupport.cpp).
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(
    int cause, const wchar_t* lpszArchiveName);
// Definition: phase4/src/detail/MfcExceptionsSupport.cpp.
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

namespace {

// Retail (client-header) layouts of the value types the free operators at the
// bottom of this file serialize.  They are local on purpose: OpenMFC's own
// COleCurrency/COleDateTime in include/openmfc/afxole.h carry extra padding
// fields, and CTime/CTimeSpan/COleDateTimeSpan are not declared in this tree
// at all.  The offsets are the ones the retail operator bodies address
// (+0x0/+0x4/+0x8 for COleCurrency, +0x0/+0x8 for COleDateTime and
// COleDateTimeSpan), which are also those of the real ATL/MFC headers:
// COleCurrency = { CURRENCY m_cur; CurrencyStatus m_status; } (CURRENCY being
// { ULONG Lo; LONG Hi; } on little-endian), COleDateTime = { DATE m_dt;
// DateTimeStatus m_status; }, COleDateTimeSpan = { double m_span;
// DateTimeSpanStatus m_status; }.
struct S_ArchOleCurrency {
    unsigned long Lo;       // +0x0  m_cur.Lo
    long          Hi;       // +0x4  m_cur.Hi
    long          m_status; // +0x8
    long          _pad;     // +0xc  (alignment of the 8-byte CURRENCY)
};
static_assert(offsetof(S_ArchOleCurrency, Lo) == 0x0, "COleCurrency m_cur.Lo");
static_assert(offsetof(S_ArchOleCurrency, Hi) == 0x4, "COleCurrency m_cur.Hi");
static_assert(offsetof(S_ArchOleCurrency, m_status) == 0x8, "COleCurrency m_status");
static_assert(sizeof(S_ArchOleCurrency) == 0x10, "COleCurrency size");

struct S_ArchOleDate {      // COleDateTime and COleDateTimeSpan
    double m_value;         // +0x0  m_dt / m_span
    long   m_status;        // +0x8
};
static_assert(offsetof(S_ArchOleDate, m_value) == 0x0, "COleDateTime m_dt");
static_assert(offsetof(S_ArchOleDate, m_status) == 0x8, "COleDateTime m_status");
static_assert(sizeof(S_ArchOleDate) == 0x10, "COleDateTime size");

// Stand-ins for retail's inline CArchive primitive operators (see the block
// comment above the value-type operators at the bottom of this file).  A load
// from a storing archive throws CArchiveException::writeOnly, a store into a
// loading archive throws ::readOnly -- exactly the causes retail passes.
// Archive mode errors carry the captured file name.
void ArRequireLoading(CArchive* ar) {
    if (!ar->IsLoading())
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::writeOnly, ar->m_strFileName.GetString());
}
void ArRequireStoring(CArchive* ar) {
    if (ar->IsLoading())
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::readOnly, ar->m_strFileName.GetString());
}
template <class T> void ArPut(CArchive* ar, T value) {
    ArRequireStoring(ar);
    ar->Write(&value, sizeof(value));
}
template <class T> T ArGet(CArchive* ar) {
    ArRequireLoading(ar);
    T value{};
    ar->Read(&value, sizeof(value));
    return value;
}

} // namespace


















// Symbol: ?WriteClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__WriteClass_CArchive__QEAAXPEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass* pClass) {
    if (pThis && pClass) impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(pClass, pThis);
}
// Symbol: ?SerializeClass@CArchive@@QEAAXPEBUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SerializeClass_CArchive__QEAAXPEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass* pClass) {
    if (pThis && pThis->IsStoring()) impl__WriteClass_CArchive__QEAAXPEBUCRuntimeClass___Z(pThis, pClass);
    else if (pThis) {
        unsigned int schema = 0;
        (void)impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(pThis, &schema);
        pThis->SetObjectSchema(schema);
    }
}
// Symbol: ?ReadClass@CArchive@@QEAAPEAUCRuntimeClass@@PEBU2@PEAIPEAK@Z
extern "C" CRuntimeClass* MS_ABI impl__ReadClass_CArchive__QEAAPEAUCRuntimeClass__PEBU2_PEAIPEAK_Z(
    CArchive* pThis, const CRuntimeClass*, unsigned int* schema, unsigned long*) {
    return pThis ? impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(pThis, schema) : nullptr;
}
// Symbol: ?WriteObject@CArchive@@QEAAXPEBVCObject@@@Z
extern "C" void MS_ABI impl__WriteObject_CArchive__QEAAXPEBVCObject___Z(CArchive* pThis, const CObject* object) {
    if (pThis) (*pThis) << object;
}
// Symbol: ?ReadObject@CArchive@@QEAAPEAVCObject@@PEBUCRuntimeClass@@@Z
extern "C" CObject* MS_ABI impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass*) {
    CObject* object = nullptr;
    if (pThis) (*pThis) >> object;
    return object;
}
// Symbol: ?ReadString@CArchive@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__ReadString_CArchive__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CArchive* pThis, CString* stringOut) {
    return (pThis && stringOut) ? pThis->ReadString(*stringOut) : 0;
}
// Symbol: ?MapObject@CArchive@@QEAAXPEBVCObject@@@Z
// Remaining stub: register class/object identities and maintain the native load
// array, store map and schema-type array. The fields now have their native
// layout; object-graph serialization still needs the corresponding protocol.
extern "C" void MS_ABI impl__MapObject_CArchive__QEAAXPEBVCObject___Z(CArchive*, const CObject*) {
}
// Symbol: ?EnsureSchemaMapExists@CArchive@@QEAAXPEAPEAV?$CArray@W4LoadArrayObjType@CArchive@@AEBW412@@@@Z
// Remaining stub: create the native schema map and its reserved type-array
// entry, including allocation-failure ownership and caller-visible array state.
extern "C" void MS_ABI impl__EnsureSchemaMapExists_CArchive__QEAAXPEAPEAV__CArray_W4LoadArrayObjType_CArchive__AEBW412____Z(CArchive*, void**) {
}
CArchive& CArchive::operator>>(bool& b) {
    char ch;
    Read(&ch, sizeof(ch));
    b = (ch != 0);
    return *this;
}
CArchive& CArchive::operator>>(char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator>>(unsigned char& ch) {
    Read(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator>>(short& w) {
    Read(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator>>(unsigned short& w) {
    Read(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator>>(int& i) {
    Read(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator>>(unsigned int& i) {
    Read(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator>>(long& l) {
    Read(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator>>(unsigned long& l) {
    Read(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator>>(float& f) {
    Read(&f, sizeof(f));
    return *this;
}
CArchive& CArchive::operator>>(double& d) {
    Read(&d, sizeof(d));
    return *this;
}
CArchive& CArchive::operator>>(CObject*& pOb) {
    unsigned char hasObject = 0;
    if (Read(&hasObject, sizeof(hasObject)) != sizeof(hasObject) || hasObject == 0) {
        pOb = nullptr;
        return *this;
    }

    unsigned int nSchema = 0xFFFF;
    CRuntimeClass* pClass = impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(this, &nSchema);
    SetObjectSchema(nSchema);

    if (!pClass || !pClass->m_pfnCreateObject) {
        pOb = nullptr;
        return *this;
    }

    pOb = pClass->CreateObject();
    if (pOb) {
        pOb->Serialize(*this);
    }
    return *this;
}
CArchive& CArchive::operator>>(void*& p) {
    uintptr_t value = 0;
    Read(&value, sizeof(value));
    p = reinterpret_cast<void*>(value);
    return *this;
}
CArchive& CArchive::operator>>(CString& str) {
    // Read string length
    UINT nLen = 0;
    Read(&nLen, sizeof(nLen));

    if (nLen > 0 && nLen < 0x10000000) {
        wchar_t* pBuf = str.GetBuffer(nLen + 1);
        Read(pBuf, nLen * sizeof(wchar_t));
        pBuf[nLen] = L'\0';
        str.ReleaseBuffer(nLen);
    } else {
        str.Empty();
    }
    return *this;
}
CArchive& CArchive::operator<<(bool b) {
    char ch = b ? 1 : 0;
    Write(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator<<(char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator<<(unsigned char ch) {
    Write(&ch, sizeof(ch));
    return *this;
}
CArchive& CArchive::operator<<(short w) {
    Write(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator<<(unsigned short w) {
    Write(&w, sizeof(w));
    return *this;
}
CArchive& CArchive::operator<<(int i) {
    Write(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator<<(unsigned int i) {
    Write(&i, sizeof(i));
    return *this;
}
CArchive& CArchive::operator<<(long l) {
    Write(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator<<(unsigned long l) {
    Write(&l, sizeof(l));
    return *this;
}
CArchive& CArchive::operator<<(float f) {
    Write(&f, sizeof(f));
    return *this;
}
CArchive& CArchive::operator<<(double d) {
    Write(&d, sizeof(d));
    return *this;
}
CArchive& CArchive::operator<<(const CObject* pOb) {
    unsigned char hasObject = (pOb != nullptr) ? 1 : 0;
    Write(&hasObject, sizeof(hasObject));

    if (!hasObject) {
        return *this;
    }

    const CRuntimeClass* pClass = pOb->GetRuntimeClass();
    if (pClass == nullptr) {
        pClass = &CObject::classCObject;
    }
    impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(pClass, this);

    // Conservative format: only write payload if load side can create the object.
    if (pClass->m_pfnCreateObject) {
        const_cast<CObject*>(pOb)->Serialize(*this);
    }
    return *this;
}
CArchive& CArchive::operator<<(const void* p) {
    uintptr_t value = reinterpret_cast<uintptr_t>(p);
    Write(&value, sizeof(value));
    return *this;
}
CArchive& CArchive::operator<<(const CString& str) {
    // Write string length and data
    UINT nLen = str.GetLength();
    Write(&nLen, sizeof(nLen));

    if (nLen > 0) {
        Write((const wchar_t*)str, nLen * sizeof(wchar_t));
    }
    return *this;
}
wchar_t* CArchive::ReadString(wchar_t* lpsz, UINT nMax) {
    if (!lpsz || nMax == 0) return nullptr;

    UINT nRead = 0;
    while (nRead < nMax - 1) {
        wchar_t ch;
        if (Read(&ch, sizeof(ch)) != sizeof(ch)) break;
        if (ch == L'\n') {
            lpsz[nRead++] = ch;
            break;
        }
        if (ch == L'\r') continue;  // Skip CR
        lpsz[nRead++] = ch;
    }
    lpsz[nRead] = L'\0';
    return nRead != 0 ? lpsz : nullptr;
}
int CArchive::ReadString(CString& rString) {
    rString.Empty();
    wchar_t buf[256];
    int nTotal = 0;

    while (true) {
        if (!ReadString(buf, 256)) break;
        int nRead = static_cast<int>(wcslen(buf));
        rString += buf;
        nTotal += nRead;
        if (buf[nRead - 1] == L'\n') break;
    }
    return nTotal;
}
void CArchive::WriteString(const wchar_t* lpsz) {
    if (!lpsz) return;
    UINT nLen = (UINT)wcslen(lpsz);
    Write(lpsz, nLen * sizeof(wchar_t));
}
// ---------------------------------------------------------------------------
// IMPLEMENT_SERIAL extraction operators:  CArchive& operator>>(CArchive&, T*&)
//
// All 42 retail bodies below were disassembled and are instruction-identical
// apart from the one data address they load.  Taking CDocItem (RVA 0x254730)
// as the worked example:
//
//     mov  %rdx,%rbx                    ; rbx = &pOb   (2nd arg)
//     mov  %rcx,%rdi                    ; rdi = &ar    (1st arg)
//     lea  0x1562d1(%rip),%rdx          ; 2nd arg to the call below
//     call 0x1801d06e0                  ; ?ReadObject@CArchive@@QEAAPEAVCObject@@PEBUCRuntimeClass@@@Z
//     mov  %rax,(%rbx)                  ; pOb = returned CObject*
//     mov  %rdi,%rax                    ; return ar
//
// The %rdx operand is the callee's `const CRuntimeClass*` parameter and is a
// distinct data address per class, i.e. the class's own runtime-class
// descriptor -- the IMPLEMENT_SERIAL expansion
//     pOb = (T*)ar.ReadObject(RUNTIME_CLASS(T));  return ar;
//
// No CRuntimeClass descriptor carries a name in this host's RVA map, so the
// per-class identity was established from the data rather than from a symbol
// lookup: the first qword of a descriptor is CRuntimeClass::m_lpszClassName,
// and the ASCII string it points at was read out of the retail image for each
// of the 42 lea targets.  Every one spells its own class ("CDocItem" via
// 0x1803aaa18 for the worked example above), so no operator below hands
// ReadObject a sibling's descriptor.  The 42 cited RVAs were likewise checked
// against the export map, and once addresses and rip displacements are
// blanked all 42 bodies collapse to the single 13-instruction shape shown,
// whose only call target is ?ReadObject@CArchive@@QEAAPEAVCObject@@... .
//
// The descriptor is reached here through each class's
// impl__GetThisClass_*__SAPEAUCRuntimeClass__XZ thunk rather than through
// RUNTIME_CLASS(T).  The reason was checked per class, not assumed: 27 of the
// 42 (CDockState, CMouseManager, CMFCColorBar, CPaneDialog, ...) are only
// forward-declared in include/openmfc, so RUNTIME_CLASS(T) would not compile
// here at all; and for several of those the descriptor the DLL really defines
// is a file-scope object in a RuntimeClasses.cpp (e.g. `CRuntimeClass
// classCMouseManager` in featurepack/customize/RuntimeClasses.cpp), not the
// T::classT static that RUNTIME_CLASS names.  The thunk is the one route valid
// for all 42.  It is NOT true that these statics are never linkable from
// inside the DLL -- CDocItem's, for one, is defined by IMPLEMENT_DYNAMIC at
// phase4/src/detail/OlecoreSupport.cpp:4 -- so do not generalise that from
// BRIEFING2 s1, which is about methods.
// Every thunk named below was checked to have a definition under phase4/src.
//
// Caveat worth knowing: OpenMFC's own ReadObject
// (impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z, defined
// above in this file) currently ignores its CRuntimeClass* argument and just
// runs `ar >> pObject`.  The class is passed anyway so these stay correct if
// ReadObject starts validating it.
// ---------------------------------------------------------------------------
#define OPENMFC_ARCHIVE_EXTRACT_OBJ(fn_name, getthisclass_fn)                  \
    extern "C" CRuntimeClass* MS_ABI getthisclass_fn();                        \
    extern "C" CArchive* MS_ABI fn_name(CArchive* ar, CObject** ppOb) {        \
        if (ar == nullptr || ppOb == nullptr) return ar;                       \
        *ppOb = impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z( \
            ar, getthisclass_fn());                                            \
        return ar;                                                             \
    }

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCDocItem@@@Z
// retail RVA 0x254730
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCDocItem___Z,
                            impl__GetThisClass_CDocItem__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCDockState@@@Z
// retail RVA 0x220540
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCDockState___Z,
                            impl__GetThisClass_CDockState__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCDockablePane@@@Z
// retail RVA 0x3fc80
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCDockablePane___Z,
                            impl__GetThisClass_CDockablePane__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCDockablePaneAdapter@@@Z
// retail RVA 0x466c0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCDockablePaneAdapter___Z,
                            impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCHelpComboBoxButton@@@Z
// retail RVA 0x8b290
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCHelpComboBoxButton___Z,
                            impl__GetThisClass_CHelpComboBoxButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCColorBar@@@Z
// retail RVA 0x245e0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCColorBar___Z,
                            impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCColorMenuButton@@@Z
// retail RVA 0x2a5f0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCColorMenuButton___Z,
                            impl__GetThisClass_CMFCColorMenuButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCCustomizeButton@@@Z
// retail RVA 0x34eb0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCCustomizeButton___Z,
                            impl__GetThisClass_CMFCCustomizeButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCDropDownFrame@@@Z
// retail RVA 0x5d0c0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCDropDownFrame___Z,
                            impl__GetThisClass_CMFCDropDownFrame__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCDropDownToolBar@@@Z
// retail RVA 0x5cb90
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCDropDownToolBar___Z,
                            impl__GetThisClass_CMFCDropDownToolBar__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCDropDownToolbarButton@@@Z
// retail RVA 0x5dd40
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCDropDownToolbarButton___Z,
                            impl__GetThisClass_CMFCDropDownToolbarButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCMenuBar@@@Z
// retail RVA 0x8b300
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCMenuBar___Z,
                            impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCOutlookBar@@@Z
// retail RVA 0x97120
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCOutlookBar___Z,
                            impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCOutlookBarPane@@@Z
// retail RVA 0x98670
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCOutlookBarPane___Z,
                            impl__GetThisClass_CMFCOutlookBarPane__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCOutlookBarPaneAdapter@@@Z
// retail RVA 0x9ab70
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCOutlookBarPaneAdapter___Z,
                            impl__GetThisClass_CMFCOutlookBarPaneAdapter__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCOutlookBarPaneButton@@@Z
// retail RVA 0x9ac50
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCOutlookBarPaneButton___Z,
                            impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCPopupMenu@@@Z
// retail RVA 0xb5240
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCPopupMenu___Z,
                            impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCPopupMenuBar@@@Z
// retail RVA 0xbc4f0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCPopupMenuBar___Z,
                            impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCTasksPane@@@Z
// retail RVA 0x142680
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCTasksPane___Z,
                            impl__GetThisClass_CMFCTasksPane__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCTasksPaneFrameWnd@@@Z
// retail RVA 0x14a6a0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCTasksPaneFrameWnd___Z,
                            impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCTasksPaneToolBar@@@Z
// retail RVA 0x142160
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCTasksPaneToolBar___Z,
                            impl__GetThisClass_CMFCTasksPaneToolBar__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBar@@@Z
// retail RVA 0x14b8f0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBar___Z,
                            impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarButton@@@Z
// retail RVA 0x15a4f0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarButton___Z,
                            impl__GetThisClass_CMFCToolBarButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarColorButton@@@Z
// retail RVA 0x23e90
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarColorButton___Z,
                            impl__GetThisClass_CMFCToolBarColorButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarComboBoxButton@@@Z
// retail RVA 0x15faf0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarComboBoxButton___Z,
                            impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarDateTimeCtrl@@@Z
// retail RVA 0x164010
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarDateTimeCtrl___Z,
                            impl__GetThisClass_CMFCToolBarDateTimeCtrl__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarEditBoxButton@@@Z
// retail RVA 0x165cb0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarEditBoxButton___Z,
                            impl__GetThisClass_CMFCToolBarEditBoxButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarFontComboBox@@@Z
// retail RVA 0x167640
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarFontComboBox___Z,
                            impl__GetThisClass_CMFCToolBarFontComboBox__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarFontSizeComboBox@@@Z
// retail RVA 0x168520
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarFontSizeComboBox___Z,
                            impl__GetThisClass_CMFCToolBarFontSizeComboBox__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarMenuButton@@@Z
// retail RVA 0x170dd0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarMenuButton___Z,
                            impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarSpinEditBoxButton@@@Z
// retail RVA 0x17d5a0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarSpinEditBoxButton___Z,
                            impl__GetThisClass_CMFCToolBarSpinEditBoxButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMFCToolBarSystemMenuButton@@@Z
// retail RVA 0x17dc90
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMFCToolBarSystemMenuButton___Z,
                            impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMouseManager@@@Z
// retail RVA 0x90870
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMouseManager___Z,
                            impl__GetThisClass_CMouseManager__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCMultiPaneFrameWnd@@@Z
// retail RVA 0x92420
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCMultiPaneFrameWnd___Z,
                            impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCPaneDialog@@@Z
// retail RVA 0xaaf60
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCPaneDialog___Z,
                            impl__GetThisClass_CPaneDialog__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCPaneFrameWnd@@@Z
// retail RVA 0xad6a0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCPaneFrameWnd___Z,
                            impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCRichEditCntrItem@@@Z
// retail RVA 0x285c10
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCRichEditCntrItem___Z,
                            impl__GetThisClass_CRichEditCntrItem__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCTabbedPane@@@Z
// retail RVA 0x1371b0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCTabbedPane___Z,
                            impl__GetThisClass_CTabbedPane__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCTasksPaneHistoryButton@@@Z
// retail RVA 0x141f60
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCTasksPaneHistoryButton___Z,
                            impl__GetThisClass_CTasksPaneHistoryButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCTasksPaneMenuButton@@@Z
// retail RVA 0x1420c0
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCTasksPaneMenuButton___Z,
                            impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCTasksPaneNavigateButton@@@Z
// retail RVA 0x141c80
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCTasksPaneNavigateButton___Z,
                            impl__GetThisClass_CTasksPaneNavigateButton__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAPEAVCUserTool@@@Z
// retail RVA 0x181140
OPENMFC_ARCHIVE_EXTRACT_OBJ(impl___5_YAAEAVCArchive__AEAV0_AEAPEAVCUserTool___Z,
                            impl__GetThisClass_CUserTool__SAPEAUCRuntimeClass__XZ)

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCComBSTR@ATL@@@Z
// CArchive& operator>>(CArchive& ar, ATL::CComBSTR& bstr)
// Transcribed from the retail body at RVA 0x26de30 (mfc140, the ANSI twin;
// the same bytes are the mfc140u export entry at RVA 0x26f020).  Every other
// address in this comment (FillBuffer, Read, the IAT slots, the je target)
// is likewise an mfc140 (ANSI) address:
//     testb $0x1,0x20(%rcx)          ; m_nMode & CArchive::load
//     je    0x26def9                 ;   -> AfxThrowArchiveException(4, m_strFileName)
//     <inline 4-byte read of the length out of the archive buffer, with
//      FillBuffer @0x1cfc70 when m_lpBufCur+4 > m_lpBufMax>
//     mov   (%rsi),%rcx              ; rcx = bstr.m_str
//     if (nLen != 0) {
//         movq $0,(%rsi)             ; bstr.m_str = NULL
//         tmp = old m_str;
//         SysReAllocStringLen(&tmp, NULL, nLen)   ; oleaut32 ord#5
//         if (that succeeded)
//             if (CArchive::Read(ar, tmp, nLen*2) != nLen*2)   ; Read @0x1cf7f0
//                 AfxThrowArchiveException(3 /*endOfFile*/, NULL);
//         if (bstr.m_str != tmp) { SysFreeString(bstr.m_str);  ; oleaut32 ord#6
//                                  bstr.m_str = tmp; }
//     } else {
//         SysFreeString(bstr.m_str); bstr.m_str = NULL;
//     }
//     return ar;
// The two oleaut32 entries were resolved through the import table: IAT slot
// 0x1802c4ac8 -> OLEAUT32 ord#5 (SysReAllocStringLen), 0x1802c4a08 -> ord#6
// (SysFreeString).  Exception causes 4 and 3 match CArchiveException::writeOnly
// and ::endOfFile in include/openmfc/afxwin.h.
// ATL::CComBSTR is not declared anywhere in this tree; retail dereferences the
// reference straight at offset 0 as a BSTR, so the parameter is taken as BSTR*.
// One deliberate difference: retail passes its m_strFileName to
// AfxThrowArchiveException, and OpenMFC's CArchive has no such member, so NULL
// is passed instead.
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCComBSTR_ATL___Z(
        CArchive* ar, BSTR* pbstr) {
    if (ar == nullptr || pbstr == nullptr) return ar;
    if (!ar->IsLoading()) {
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::writeOnly, ar->m_strFileName.GetString());
        return ar;
    }

    unsigned int nLen = 0;
    ar->Read(&nLen, sizeof(nLen));

    BSTR bstrTmp = *pbstr;
    *pbstr = nullptr;
    if (nLen != 0) {
        if (::SysReAllocStringLen(&bstrTmp, nullptr, nLen)) {
            const unsigned int nBytes = nLen * 2u;
            if (ar->Read(bstrTmp, nBytes) != nBytes) {
                impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::endOfFile, nullptr);
            }
        }
        if (*pbstr != bstrTmp) {
            if (*pbstr != nullptr) ::SysFreeString(*pbstr);
            *pbstr = bstrTmp;
        }
    } else if (bstrTmp != nullptr) {
        ::SysFreeString(bstrTmp);
    }
    return ar;
}

// ---------------------------------------------------------------------------
// Value-type insertion/extraction operators and the string-length helpers.
//
// Every body below was transcribed from the retail mfc140u.dll export (RVA of
// each function's entry cited per symbol).  Retail inlines CArchive's primitive
// operator<</operator>> into all of them, so each primitive step there is
//     test  $0x1,0x20(%rcx)              ; m_nMode & CArchive::load
//     <throw on wrong mode>
//     if (m_lpBufCur + n > m_lpBufMax) Flush() / FillBuffer(...)
//     copy n bytes at m_lpBufCur;  m_lpBufCur += n
// with Flush = ?Flush@CArchive@@QEAAXXZ (0x1d1be0, mfc140u), FillBuffer =
// ?FillBuffer@CArchive@@QEAAXI@Z (0x1d1cc0, mfc140u) and the throw being
// ?AfxThrowArchiveException@@YAXHPEB_W@Z (0x1d3610, mfc140u) called with
// m_strFileName (the CString at this+0x18, read through GetString at 0x2e10).
// The throw cause is CArchiveException::writeOnly (4) for a load from a storing
// archive and ::readOnly (2) for a store into a loading archive.
//
// OpenMFC's CArchive (include/openmfc/afx.h) has a different member layout, so
// those inline sequences are re-expressed with the ArRequire*/ArPut/ArGet
// helpers in the anonymous namespace at the top of this file, which go through
// CArchive::Read/Write (defined in this TU).  Deviations, all deliberate:
//   * m_strFileName does not exist in OpenMFC's CArchive, so NULL is passed as
//     the archive name to AfxThrowArchiveException (same as the CComBSTR
//     loader above).
//   * Retail's underflow path is FillBuffer; OpenMFC's Read returns short on
//     EOF instead of throwing (retail FillBuffer throws ::endOfFile (3)), so
//     ArGet zero-initialises its value first and a truncated archive yields
//     zero or partially filled fields rather than an exception.
//   * Retail tests the mode before EVERY primitive; the helpers do the same
//     per primitive, so the observable order (which step throws) is unchanged.
// ---------------------------------------------------------------------------

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCOleCurrency@@@Z
// CArchive& operator>>(CArchive& ar, COleCurrency& curSrc)
// Transcribed from retail RVA 0x26f9d0 (mfc140u): three 4-byte loads, stored
// in this order to curSrc+0x8 (m_status), curSrc+0x4 (m_cur.Hi) and
// curSrc+0x0 (m_cur.Lo).  Returns ar.
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCOleCurrency___Z(
        CArchive* ar, S_ArchOleCurrency* cur) {
    if (ar == nullptr || cur == nullptr) return ar;
    cur->m_status = ArGet<long>(ar);
    cur->Hi       = ArGet<long>(ar);
    cur->Lo       = ArGet<unsigned long>(ar);
    return ar;
}

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCOleDateTime@ATL@@@Z
// CArchive& operator>>(CArchive& ar, ATL::COleDateTime& dtSrc)
// Transcribed from retail RVA 0x26fb90 (mfc140u): a 4-byte load into
// dtSrc+0x8 (m_status), then an 8-byte load into dtSrc+0x0 (m_dt).
// The COleDateTimeSpan overload below resolves (by export ordinal, through
// mfc140u's export address table) to this same entry, 0x26fb90: the two
// classes share the {double at +0; status at +8} shape and the linker folded
// the identical bodies.
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCOleDateTime_ATL___Z(
        CArchive* ar, S_ArchOleDate* dt) {
    if (ar == nullptr || dt == nullptr) return ar;
    dt->m_status = ArGet<long>(ar);
    dt->m_value  = ArGet<double>(ar);
    return ar;
}

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCOleDateTimeSpan@ATL@@@Z
// CArchive& operator>>(CArchive& ar, ATL::COleDateTimeSpan& dtSpanSrc)
// Same retail entry as the COleDateTime extractor: RVA 0x26fb90 (mfc140u).
// Status (+0x8, 4 bytes) first, then m_span (+0x0, 8 bytes).
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCOleDateTimeSpan_ATL___Z(
        CArchive* ar, S_ArchOleDate* span) {
    return impl___5_YAAEAVCArchive__AEAV0_AEAVCOleDateTime_ATL___Z(ar, span);
}

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCTime@ATL@@@Z
// CArchive& operator>>(CArchive& ar, ATL::CTime& rtime)
// Transcribed from retail RVA 0x274ac0 (mfc140u):
//     n = <4-byte load, sign-extended (movslq)>;
//     if ((DWORD)n == 0x8000000A) n = <8-byte load>;
//     rtime.m_time = n;   return ar;
// 0x8000000A is the marker the CTime inserter below writes ahead of the
// 64-bit value; a bare 32-bit value is the older format and is sign-extended.
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCTime_ATL___Z(
        CArchive* ar, long long* ptime) {
    if (ar == nullptr || ptime == nullptr) return ar;
    long long t = ArGet<int>(ar);
    if (static_cast<unsigned int>(t) == 0x8000000Au)
        t = ArGet<long long>(ar);
    *ptime = t;
    return ar;
}

// Symbol: ??5@YAAEAVCArchive@@AEAV0@AEAVCTimeSpan@ATL@@@Z
// CArchive& operator>>(CArchive& ar, ATL::CTimeSpan& rtimeSpan)
// Transcribed from retail RVA 0x274bf0 (mfc140u): one 4-byte load,
// sign-extended (movslq) into the 8-byte m_timeSpan.  Returns ar.
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCTimeSpan_ATL___Z(
        CArchive* ar, long long* pspan) {
    if (ar == nullptr || pspan == nullptr) return ar;
    *pspan = ArGet<int>(ar);
    return ar;
}

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCComBSTR@ATL@@@Z
// CArchive& operator<<(CArchive& ar, ATL::CComBSTR string)
// Transcribed from retail RVA 0x26ef70 (mfc140u):
//     len = SysStringLen(string.m_str);            ; before the mode test
//     <mode test: loading -> throw readOnly (2)>
//     <4-byte store of len>
//     if (SysStringLen(string.m_str) != 0)
//         CArchive::Write(ar, string.m_str, SysStringLen(string.m_str)*2);
//                                                  ; Write = 0x1d1a70 (mfc140u)
//     SysFreeString(string.m_str);                 ; destroys the by-value arg
//     return ar;
// mfc140u IAT slots resolved with iatu.py: 0x1802c69d0 -> OLEAUT32 ord#7
// (SysStringLen), 0x1802c69d8 -> OLEAUT32 ord#6 (SysFreeString).
// The CComBSTR is passed by value; under the MSVC x64 ABI a non-trivially-
// copyable argument arrives as a pointer to the caller's temporary and the
// CALLEE destroys it, hence the SysFreeString at the end and BSTR* here.
// Deviation: on the loading-archive throw path this frees the BSTR before
// throwing.  Retail leaves that to unwinding (its unwind tables were not
// inspected); this code has no MSVC unwind funclet to do it, so freeing first
// keeps ownership identical without leaking.
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCComBSTR_ATL___Z(
        CArchive* ar, BSTR* pbstr) {
    if (ar == nullptr || pbstr == nullptr) return ar;
    const unsigned int nLen = ::SysStringLen(*pbstr);
    if (ar->IsLoading()) {
        ::SysFreeString(*pbstr);
        ArRequireStoring(ar);
        return ar;
    }
    ArPut<unsigned int>(ar, nLen);
    if (::SysStringLen(*pbstr) != 0)
        ar->Write(*pbstr, ::SysStringLen(*pbstr) * 2u);
    ::SysFreeString(*pbstr);
    return ar;
}

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCOleCurrency@@@Z
// CArchive& operator<<(CArchive& ar, COleCurrency curSrc)
// Transcribed from retail RVA 0x26f8e0 (mfc140u).  The 16-byte COleCurrency
// arrives by pointer (%rdx) to the caller's copy; retail stores, 4 bytes each,
// curSrc+0x8 (m_status), curSrc+0x4 (m_cur.Hi), curSrc+0x0 (m_cur.Lo).
// No destructor call follows (COleCurrency's is trivial).
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCOleCurrency___Z(
        CArchive* ar, const S_ArchOleCurrency* cur) {
    if (ar == nullptr || cur == nullptr) return ar;
    ArPut<long>(ar, cur->m_status);
    ArPut<long>(ar, cur->Hi);
    ArPut<unsigned long>(ar, cur->Lo);
    return ar;
}

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCOleDateTime@ATL@@@Z
// CArchive& operator<<(CArchive& ar, ATL::COleDateTime dateSrc)
// Transcribed from retail RVA 0x26fad0 (mfc140u): by-pointer argument; stores
// dateSrc+0x8 (m_status, 4 bytes) then dateSrc+0x0 (m_dt, 8 bytes, movsd).
// The COleDateTimeSpan inserter resolves by ordinal to this same entry.
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCOleDateTime_ATL___Z(
        CArchive* ar, const S_ArchOleDate* dt) {
    if (ar == nullptr || dt == nullptr) return ar;
    ArPut<long>(ar, dt->m_status);
    ArPut<double>(ar, dt->m_value);
    return ar;
}

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCOleDateTimeSpan@ATL@@@Z
// CArchive& operator<<(CArchive& ar, ATL::COleDateTimeSpan dateSpanSrc)
// Same retail entry as the COleDateTime inserter: RVA 0x26fad0 (mfc140u).
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCOleDateTimeSpan_ATL___Z(
        CArchive* ar, const S_ArchOleDate* span) {
    return impl___6_YAAEAVCArchive__AEAV0_VCOleDateTime_ATL___Z(ar, span);
}

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCTime@ATL@@@Z
// CArchive& operator<<(CArchive& ar, ATL::CTime time)
// Transcribed from retail RVA 0x274a20 (mfc140u).  CTime is 8 bytes and is
// passed by value IN %rdx (the __time64_t itself, not a pointer).  Retail
// stores the 4-byte marker 0x8000000A, then the 8-byte time.
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCTime_ATL___Z(
        CArchive* ar, long long t) {
    if (ar == nullptr) return ar;
    ArPut<unsigned int>(ar, 0x8000000Au);
    ArPut<long long>(ar, t);
    return ar;
}

// Symbol: ??6@YAAEAVCArchive@@AEAV0@VCTimeSpan@ATL@@@Z
// CArchive& operator<<(CArchive& ar, ATL::CTimeSpan timeSpan)
// Transcribed from retail RVA 0x274b80 (mfc140u).  The 8-byte span arrives in
// %rdx.  FIRST, before any mode test: if span > 0xFFFFFFFF (signed 64-bit
// compare, `cmp %rax,%rdx; jg`) retail calls ?AfxThrowInvalidArgException@@YAXXZ
// (0x227720, mfc140u).  Negative spans pass that test.  Then the low 32 bits
// are stored as one 4-byte value.
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCTimeSpan_ATL___Z(
        CArchive* ar, long long span) {
    if (ar == nullptr) return ar;
    if (span > 0xFFFFFFFFll) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return ar;
    }
    ArPut<unsigned int>(ar, static_cast<unsigned int>(span));
    return ar;
}

// Symbol: ?AfxReadStringLength@@YA_KAEAVCArchive@@AEAH@Z
// ULONGLONG AfxReadStringLength(CArchive& ar, int& nCharSize)
// Transcribed from retail RVA 0x1d0c10 (mfc140u):
//     nCharSize = 1;                         ; written before the first mode test
//     b = <BYTE>;  if (b < 0xFF) return b;
//     w = <WORD>;
//     if (w == 0xFFFE) {                     ; Unicode marker
//         nCharSize = 2;
//         b = <BYTE>;  if (b < 0xFF) return b;
//         w = <WORD>;
//     }
//     if (w < 0xFFFF) return w;
//     d = <DWORD>;  if (d < 0xFFFFFFFF) return d;
//     return <QWORD>;
// Every load is the inline primitive (mode test -> writeOnly (4) throw).
extern "C" unsigned long long MS_ABI impl__AfxReadStringLength__YA_KAEAVCArchive__AEAH_Z(
        CArchive* ar, int* pnCharSize) {
    if (ar == nullptr || pnCharSize == nullptr) return 0;
    *pnCharSize = 1;
    unsigned char b = ArGet<unsigned char>(ar);
    if (b < 0xFF) return b;
    unsigned short w = ArGet<unsigned short>(ar);
    if (w == 0xFFFE) {
        *pnCharSize = 2;
        b = ArGet<unsigned char>(ar);
        if (b < 0xFF) return b;
        w = ArGet<unsigned short>(ar);
    }
    if (w < 0xFFFF) return w;
    const unsigned int d = ArGet<unsigned int>(ar);
    if (d < 0xFFFFFFFFu) return d;
    return ArGet<unsigned long long>(ar);
}

// Symbol: ?AfxWriteStringLength@@YAXAEAVCArchive@@_KH@Z
// void AfxWriteStringLength(CArchive& ar, ULONGLONG nLength, BOOL bUnicode)
// Transcribed from retail RVA 0x1d0e40 (mfc140u):
//     if (bUnicode) { <BYTE 0xFF>; <WORD 0xFFFE>; }
//     if      (nLength < 0xFF)       <BYTE nLength>;
//     else if (nLength < 0xFFFE)     <BYTE 0xFF>; <WORD nLength>;
//     else if (nLength < 0xFFFFFFFF) <BYTE 0xFF>; <WORD 0xFFFF>; <DWORD nLength>;
//     else    <BYTE 0xFF>; <WORD 0xFFFF>; <DWORD 0xFFFFFFFF>; <QWORD nLength>;
// Every store is the inline primitive (mode test -> readOnly (2) throw).
// Note the WORD tier's bound is 0xFFFE, not 0xFFFF: 0xFFFE itself is the
// Unicode marker, so a length of 0xFFFE must take the DWORD tier.
// This is the exact inverse of AfxReadStringLength above.
extern "C" void MS_ABI impl__AfxWriteStringLength__YAXAEAVCArchive___KH_Z(
        CArchive* ar, unsigned long long nLength, int bUnicode) {
    if (ar == nullptr) return;
    if (bUnicode != 0) {
        ArPut<unsigned char>(ar, 0xFF);
        ArPut<unsigned short>(ar, 0xFFFE);
    }
    if (nLength < 0xFFull) {
        ArPut<unsigned char>(ar, static_cast<unsigned char>(nLength));
    } else if (nLength < 0xFFFEull) {
        ArPut<unsigned char>(ar, 0xFF);
        ArPut<unsigned short>(ar, static_cast<unsigned short>(nLength));
    } else if (nLength < 0xFFFFFFFFull) {
        ArPut<unsigned char>(ar, 0xFF);
        ArPut<unsigned short>(ar, 0xFFFF);
        ArPut<unsigned int>(ar, static_cast<unsigned int>(nLength));
    } else {
        ArPut<unsigned char>(ar, 0xFF);
        ArPut<unsigned short>(ar, 0xFFFF);
        ArPut<unsigned int>(ar, 0xFFFFFFFFu);
        ArPut<unsigned long long>(ar, nLength);
    }
}
