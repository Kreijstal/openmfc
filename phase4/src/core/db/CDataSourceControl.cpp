// CDataSourceControl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CDataSourceControl is the OLE-control-container data-source binding helper.
// Retail declares it only in the private occimpl.h (not shipped; afxocc.h has
// just a forward declaration), so the layout below is derived from the retail
// disassembly.  Bodies are transcribed from mfc140u.dll; the disassembler also
// reads the ANSI twin mfc140.dll, whose bodies are byte-identical but sit at
// different RVAs, so every RVA below names its image.
//
// Object layout (retail constructor, RVA 0x23d290 (mfc140u); ANSI twin
// 0x23b890 (mfc140)) -- every member the constructor initialises:
//   +0x00  vfptr                     (mfc140u vftable 0x18032b4d0)
//   +0x08  COleControlSite* m_pClientSite       (the ctor argument)
//   +0x10  ICursorMove*     m_pCursorMove
//   +0x18  ICursorUpdateARow* m_pCursorUpdateARow
//   +0x20  INT_PTR          m_nColumns          (compared as a qword)
//   +0x28  METAROWTYPE*     m_pMetaRowData      (stride 0x40; +0x38 in each
//                                                element is a CPtrList* of
//                                                bound client sites)
//   +0x30  CPtrList         m_CursorBoundProps  (0x38 bytes; the ctor inlines
//                                                CPtrList(10): vfptr = mfc140u
//                                                0x18032a758, block size 10 at
//                                                +0x60, everything else 0)
//   +0x68  void*            m_pVarData
//   +0x70  INT_PTR          m_nBindings
//   +0x78  DBCOLUMNBINDING* m_pColumnBindings   (stride 0x38)
//   +0x80  VARIANT*         m_pValues           (stride 0x18)
//   +0x88  BOOL             m_bUpdateInProgress (4-byte store)
//   +0x90  IUnknown*        m_pDataSource       (OLE DB DataSource)
//   +0x98  IRowPosition*    m_pRowPosition
//   +0xa0  CRowset*         m_pRowset           (ATL consumer; deleted with
//                                                size 0x28 by the dtor)
//   +0xa8  CDynamicAccessor* m_pDynamicAccessor (deleted with size 0x48)
//   +0xb0  DWORD            m_dwRowsetNotify    (4-byte store)
//   sizeof == 0xb8 (the scalar deleting dtor frees 0xb8 bytes)
// The member names are MFC's (occimpl.h); the offsets are the retail ones.
//
// Vtable (mfc140u vftable 0x18032b4d0, read slot by slot):
//   0 scalar deleting dtor (0x239490)   1 GetCursor (0x23d920)
//   2 BindProp(CDataBoundProperty*,BOOL) (0x23e250)
//   3 BindProp(COleControlSite*,BOOL)    (0x23e010)
//   4 BindColumns (0x23e2c0)  5 UpdateControls (0x23ee50)
//   6 UpdateCursor (0x23f070)
// (Slots 5/6 are not in the mfc140u symbol map; in the ANSI twin the same
// slots are 0x23d450 ?UpdateControls@ and 0x23d670 ?UpdateCursor@.)
//
// The COleControlSite the object points at is NOT laid out like retail in
// OpenMFC (see the mapping table at the top of core/ole/COleControlSite.cpp).
// That table maps retail site+0x80 (IOleObject*) onto
// COleControlSite::m_lpObject and records +0x50 (m_pWndCtrl) as having no
// OpenMFC equivalent.  The data-binding members the bodies below also read --
// retail +0xd0, +0xe8, +0xf0, +0xf4, +0xf8, +0x100, +0x108 -- are not in that
// table; OpenMFC's COleControlSite has no members for them (the
// BindDefaultProperty comment in that file names +0xe8/+0xf0/+0xf4/+0xf8),
// which is why the bodies that need them are still stubs below.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>
#include <cwchar>

namespace {

// Retail CPtrList node / object; OpenMFC_CPtrList (detail/CPtrListSupport.h)
// keeps this exact layout, and the CPtrList thunks below operate on it.
struct S_PtrNode {
    S_PtrNode* pNext;
    S_PtrNode* pPrev;
    void*      data;
};
static_assert(offsetof(S_PtrNode, pNext) == 0x00, "CPtrList node pNext");
static_assert(offsetof(S_PtrNode, data) == 0x10, "CPtrList node data");

struct S_PtrList {
    void*      vfptr;
    S_PtrNode* m_pNodeHead;
    S_PtrNode* m_pNodeTail;
    INT_PTR    m_nCount;
    void*      m_pNodeFree;
    void*      m_pBlocks;
    INT_PTR    m_nBlockSize;
};
static_assert(sizeof(S_PtrList) == 0x38, "CPtrList is 0x38 bytes");
static_assert(offsetof(S_PtrList, m_pNodeHead) == 0x08, "CPtrList head");

// ATL OLE DB consumer pieces reached through m_pRowset.  Only the members the
// inlined CRowset::GetData (see RowsetGetData below) touches are named.
struct S_AccessorInfo {        // ATL _ATL_ACCESSOR_INFO, stride 0x10
    ULONG_PTR hAccessor;       // +0x00 HACCESSOR
    bool      bAutoAccessor;   // +0x08
};
static_assert(sizeof(S_AccessorInfo) == 0x10, "accessor info stride 0x10");
static_assert(offsetof(S_AccessorInfo, bAutoAccessor) == 0x08, "bAutoAccessor at +8");

struct S_AccessorBase {        // ATL CAccessorBase
    S_AccessorInfo* m_pAccessorInfo;  // +0x00
    ULONG           m_nAccessors;     // +0x08
    BYTE*           m_pBuffer;        // +0x10
};
static_assert(offsetof(S_AccessorBase, m_nAccessors) == 0x08, "m_nAccessors at +8");
static_assert(offsetof(S_AccessorBase, m_pBuffer) == 0x10, "m_pBuffer at +0x10");

struct S_Rowset {              // ATL CRowset<CAccessorBase>
    IUnknown*       m_spRowset;        // +0x00 IRowset*
    IUnknown*       m_spRowsetChange;  // +0x08
    S_AccessorBase* m_pAccessor;       // +0x10
    ULONG_PTR       m_hRow;            // +0x18 HROW
    void*           m_reserved20;      // +0x20 (not touched here)
};
static_assert(sizeof(S_Rowset) == 0x28, "retail deletes m_pRowset with size 0x28");
static_assert(offsetof(S_Rowset, m_pAccessor) == 0x10, "m_pAccessor at +0x10");
static_assert(offsetof(S_Rowset, m_hRow) == 0x18, "m_hRow at +0x18");

struct S_DataSourceControl {
    void* const*     vfptr;               // +0x00
    COleControlSite* m_pClientSite;       // +0x08
    IUnknown*        m_pCursorMove;       // +0x10 ICursorMove*
    IUnknown*        m_pCursorUpdateARow; // +0x18
    INT_PTR          m_nColumns;          // +0x20
    BYTE*            m_pMetaRowData;      // +0x28 METAROWTYPE[]
    S_PtrList        m_CursorBoundProps;  // +0x30
    void*            m_pVarData;          // +0x68
    INT_PTR          m_nBindings;         // +0x70
    void*            m_pColumnBindings;   // +0x78
    VARIANT*         m_pValues;           // +0x80
    DWORD            m_bUpdateInProgress; // +0x88
    IUnknown*        m_pDataSource;       // +0x90
    IUnknown*        m_pRowPosition;      // +0x98
    S_Rowset*        m_pRowset;           // +0xa0
    void*            m_pDynamicAccessor;  // +0xa8
    DWORD            m_dwRowsetNotify;    // +0xb0
};
static_assert(offsetof(S_DataSourceControl, m_pClientSite) == 0x08, "m_pClientSite");
static_assert(offsetof(S_DataSourceControl, m_pCursorMove) == 0x10, "m_pCursorMove");
static_assert(offsetof(S_DataSourceControl, m_pCursorUpdateARow) == 0x18, "m_pCursorUpdateARow");
static_assert(offsetof(S_DataSourceControl, m_nColumns) == 0x20, "m_nColumns");
static_assert(offsetof(S_DataSourceControl, m_pMetaRowData) == 0x28, "m_pMetaRowData");
static_assert(offsetof(S_DataSourceControl, m_CursorBoundProps) == 0x30, "m_CursorBoundProps");
static_assert(offsetof(S_DataSourceControl, m_pVarData) == 0x68, "m_pVarData");
static_assert(offsetof(S_DataSourceControl, m_nBindings) == 0x70, "m_nBindings");
static_assert(offsetof(S_DataSourceControl, m_pColumnBindings) == 0x78, "m_pColumnBindings");
static_assert(offsetof(S_DataSourceControl, m_pValues) == 0x80, "m_pValues");
static_assert(offsetof(S_DataSourceControl, m_bUpdateInProgress) == 0x88, "m_bUpdateInProgress");
static_assert(offsetof(S_DataSourceControl, m_pDataSource) == 0x90, "m_pDataSource");
static_assert(offsetof(S_DataSourceControl, m_pRowPosition) == 0x98, "m_pRowPosition");
static_assert(offsetof(S_DataSourceControl, m_pRowset) == 0xa0, "m_pRowset");
static_assert(offsetof(S_DataSourceControl, m_pDynamicAccessor) == 0xa8, "m_pDynamicAccessor");
static_assert(offsetof(S_DataSourceControl, m_dwRowsetNotify) == 0xb0, "m_dwRowsetNotify");
static_assert(sizeof(S_DataSourceControl) == 0xb8, "retail sizeof(CDataSourceControl) == 0xb8");

inline S_DataSourceControl* DSC(void* p) { return static_cast<S_DataSourceControl*>(p); }

// OCDB DBCOLUMNID (atlmfc/include/ocdb.h, tagDBCOLUMNID): guid, dwKind, then a
// union of LONG lNumber / LPDBSTR lpdbsz (LPWSTR on Win32/64).
struct S_DBColumnID {
    GUID  guid;       // +0x00
    DWORD dwKind;     // +0x10 DBCOLKIND
    union {
        LONG     lNumber;
        wchar_t* lpdbsz;
    };                // +0x18
};
static_assert(offsetof(S_DBColumnID, dwKind) == 0x10, "DBCOLUMNID dwKind at +0x10");
static_assert(sizeof(S_DBColumnID) == 0x20, "DBCOLUMNID is 0x20 bytes");

// One m_pMetaRowData entry (MFC's METAROWTYPE), stride 0x40 (BindColumns and
// BindProp index it with `shl $0x6`).  Only what this file touches is named:
// BindColumns hands &entry (+0x00) to CopyColumnID as the source column id,
// and +0x38 is the CPtrList* of client sites bound to the column.  +0x28 is
// the column name the (unimplemented) bind path of BindProp(COleControlSite*)
// compares against; the rest is not decoded.
struct S_MetaRow {
    S_DBColumnID idColumn;           // +0x00
    BYTE         m_undecoded20[0x18];// +0x20..+0x37
    S_PtrList*   m_pClientList;      // +0x38
};
static_assert(offsetof(S_MetaRow, m_pClientList) == 0x38, "METAROWTYPE client list at +0x38");
static_assert(sizeof(S_MetaRow) == 0x40, "METAROWTYPE stride 0x40");

// OCDB DBFETCHROWS (ocdb.h tagDBFETCHROWS).  The retail call site fills it at
// rsp+0x30..0x50 with exactly these offsets.
struct S_DBFetchRows {
    ULONG_PTR cRowsRequested;  // +0x00
    DWORD     dwFlags;         // +0x08
    void*     pData;           // +0x10
    void*     pVarData;        // +0x18
    ULONG_PTR cbVarData;       // +0x20
    ULONG_PTR cRowsReturned;   // +0x28
};
static_assert(offsetof(S_DBFetchRows, pData) == 0x10, "DBFETCHROWS pData at +0x10");
static_assert(offsetof(S_DBFetchRows, cbVarData) == 0x20, "DBFETCHROWS cbVarData at +0x20");

// IIDs, read from the 16 bytes each retail call site loads (mfc140u image).
// {9f6aa700-d188-11cd-ad48-00aa003c9cb6} = IID_ICursor (mfc140u 0x1802d9878).
const GUID kIID_ICursor =
    {0x9f6aa700, 0xd188, 0x11cd, {0xad, 0x48, 0x00, 0xaa, 0x00, 0x3c, 0x9c, 0xb6}};
// {1ab42240-8c70-11ce-9421-00aa0062be57} (mfc140u 0x18033e3d8): the VB
// data-source-control interface (IVBDSC in MFC's private sources -- the name is
// not in any header on this host; only the GUID and the slot used are verified).
const GUID kIID_IVBDSC =
    {0x1ab42240, 0x8c70, 0x11ce, {0x94, 0x21, 0x00, 0xaa, 0x00, 0x62, 0xbe, 0x57}};

// ocdb.h: `EXTERNAL_DEFN BYTE NEAR DBBMK_CURRENT = 0x1;` -- the retail call site
// passes the address of a byte holding 1 (mfc140u 0x1802d9f68).
BYTE g_dbbmkCurrent = 0x1;

typedef HRESULT (MS_ABI* PFN_QueryInterface)(void*, const GUID*, void**);
typedef ULONG   (MS_ABI* PFN_Release)(void*);
typedef HRESULT (MS_ABI* PFN_CreateCursor)(void*, void**);                    // IVBDSC slot 5
typedef HRESULT (MS_ABI* PFN_CursorMove)(void*, ULONG, void*, long long, S_DBFetchRows*); // ICursorMove slot 8
typedef HRESULT (MS_ABI* PFN_RowsetGetData)(void*, ULONG_PTR, ULONG_PTR, void*); // IRowset slot 4
typedef void    (MS_ABI* PFN_BindPropProp)(void*, void*, int);                // this slot 2
typedef void    (MS_ABI* PFN_BindPropSite)(void*, void*, int);                // this slot 3
typedef long    (MS_ABI* PFN_UpdateCursor)(void*);                            // this slot 6 (and slot 5, UpdateControls)

inline void* const* Vtbl(void* pInterface) { return *static_cast<void* const* const*>(pInterface); }

// ATL CRowset::GetData(), inlined as the non-exported helper at RVA 0x240dac
// (mfc140u) / 0x23f3ac (mfc140) that GetBoundClientRow calls.  Transcription:
// the accessor count is read once up front; for each index it re-reads
// m_pAccessor, skips the entry when the index is out of range, the info array
// is null, or bAutoAccessor is false, and otherwise calls
// m_spRowset->GetData(m_hRow, hAccessor, m_pBuffer) (IRowset vtable slot 4,
// +0x20), returning at once on a failed HRESULT.  The result is the last
// HRESULT seen (S_OK when no accessor was called).
HRESULT RowsetGetData(S_Rowset* pRowset) {
    HRESULT hr = S_OK;
    const ULONG nAccessors = pRowset->m_pAccessor->m_nAccessors;
    for (ULONG i = 0; i < nAccessors; ++i) {
        S_AccessorBase* pAcc = pRowset->m_pAccessor;
        if (i >= pAcc->m_nAccessors) continue;
        S_AccessorInfo* pInfo = pAcc->m_pAccessorInfo;
        if (pInfo == nullptr || !pInfo[i].bAutoAccessor) continue;
        hr = reinterpret_cast<PFN_RowsetGetData>(Vtbl(pRowset->m_spRowset)[4])(
            pRowset->m_spRowset, pRowset->m_hRow, pInfo[i].hAccessor, pAcc->m_pBuffer);
        if (FAILED(hr)) return hr;
    }
    return hr;
}

} // namespace

// --- thunks used here --------------------------------------------------------
extern "C" void* MS_ABI impl___0CPtrList__QEAA__J_Z(void* pThis, INT_PTR nBlockSize);            // core/collections/CPtrList.cpp
extern "C" void* MS_ABI impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(void* pThis, void* newElement); // core/collections/CPtrList.cpp
extern "C" void  MS_ABI impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(void* pThis, void* position);     // core/collections/CPtrList.cpp
extern "C" void* MS_ABI impl___1CDataSourceControl__UEAA_XZ(void* pThis);                         // core/db/Thunks.cpp
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);                                            // detail/MemcoreSupport.cpp (??3@YAXPEAX@Z)

// --- this class's own entry points, needed for the vtable below --------------
extern "C" void* MS_ABI impl__GetCursor_CDataSourceControl__UEAAPEAUIUnknown__XZ(void* pThis);
extern "C" void  MS_ABI impl__BindProp_CDataSourceControl__UEAAXPEAVCDataBoundProperty__H_Z(void* pThis, void* pProperty, int bBind);
extern "C" void  MS_ABI impl__BindProp_CDataSourceControl__UEAAXPEAVCOleControlSite__H_Z(void* pThis, void* pClientSite, int bBind);
extern "C" void  MS_ABI impl__BindColumns_CDataSourceControl__UEAAXXZ(void* pThis);
extern "C" long  MS_ABI impl__UpdateControls_CDataSourceControl__UEAAJXZ(void* pThis);
extern "C" long  MS_ABI impl__UpdateCursor_CDataSourceControl__UEAAJXZ(void* pThis);
extern "C" long  MS_ABI impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(void* pThis);

namespace {

// Slot 0: the scalar deleting destructor (retail 0x239490 (mfc140u); ANSI
// twin 0x237a90): call ~CDataSourceControl, then free the object when bit 0 of
// the flags is set.  Deviation: retail frees through the sized operator delete
// (0xb8 bytes); here the exported ::operator delete thunk is used, which is
// the counterpart of the ::operator new OpenMFC allocates with.
void* MS_ABI DataSourceControl_ScalarDeletingDtor(void* pThis, unsigned int flags) {
    impl___1CDataSourceControl__UEAA_XZ(pThis);
    if (flags & 1u) impl___3_YAXPEAX_Z(pThis);
    return pThis;
}

// OpenMFC's stand-in for the retail vftable (mfc140u 0x18032b4d0), same slot
// order.  It carries no RTTI complete-object-locator in front of it (retail
// does); the class is private to MFC, so no client runs typeid on it.
void* const g_dataSourceControlVtbl[7] = {
    reinterpret_cast<void*>(&DataSourceControl_ScalarDeletingDtor),
    reinterpret_cast<void*>(&impl__GetCursor_CDataSourceControl__UEAAPEAUIUnknown__XZ),
    reinterpret_cast<void*>(&impl__BindProp_CDataSourceControl__UEAAXPEAVCDataBoundProperty__H_Z),
    reinterpret_cast<void*>(&impl__BindProp_CDataSourceControl__UEAAXPEAVCOleControlSite__H_Z),
    reinterpret_cast<void*>(&impl__BindColumns_CDataSourceControl__UEAAXXZ),
    reinterpret_cast<void*>(&impl__UpdateControls_CDataSourceControl__UEAAJXZ),
    reinterpret_cast<void*>(&impl__UpdateCursor_CDataSourceControl__UEAAJXZ),
};

} // namespace

// Symbol: ??0CDataSourceControl@@QEAA@PEAVCOleControlSite@@@Z
// Transcribed from RVA 0x23d290 (mfc140u): store the vfptr, m_pClientSite =
// pClientSite, zero every other member listed in the layout table above,
// construct m_CursorBoundProps as CPtrList(10) (retail inlines that ctor: list
// vfptr, zeroed fields, block size 10 at +0x60), and return this.  The two
// DWORD members (+0x88, +0xb0) are 4-byte stores; retail leaves their padding
// untouched, and so does this.
extern "C" void* MS_ABI impl___0CDataSourceControl__QEAA_PEAVCOleControlSite___Z(
    void* pThis, void* pSite) {
    S_DataSourceControl* p = DSC(pThis);
    p->vfptr = g_dataSourceControlVtbl;
    p->m_pClientSite = static_cast<COleControlSite*>(pSite);
    p->m_pCursorMove = nullptr;
    p->m_pCursorUpdateARow = nullptr;
    p->m_nColumns = 0;
    p->m_pMetaRowData = nullptr;
    impl___0CPtrList__QEAA__J_Z(&p->m_CursorBoundProps, 10);
    p->m_pVarData = nullptr;
    p->m_nBindings = 0;
    p->m_pColumnBindings = nullptr;
    p->m_pValues = nullptr;
    p->m_bUpdateInProgress = 0;
    p->m_pDataSource = nullptr;
    p->m_pRowPosition = nullptr;
    p->m_pRowset = nullptr;
    p->m_pDynamicAccessor = nullptr;
    p->m_dwRowsetNotify = 0;
    return pThis;
}

// Symbol: ?BindColumns@CDataSourceControl@@UEAAXXZ
// Partly implemented.  Retail RVA 0x23e2c0 (mfc140u):
//   if (m_pDataSource == NULL) {
//       free the old m_pValues (VariantClear each) / m_pColumnBindings, size a
//       new DBCOLUMNBINDING array from the bound-site lists hanging off
//       m_pMetaRowData, fill each entry (CopyColumnID; obData = k*0x18,
//       cbMaxLen 0, obVarDataLen/obInfo -1, dwBinding 1, dwDataType from the
//       bound COleControlSite's member at retail site+0xf4), call
//       m_pCursorMove->SetBindings (ICursor slot 4) and allocate a zeroed
//       VARIANT per binding into m_pValues;
//   }
//   GetBoundClientRow();          // direct call, result ignored
//   this->UpdateControls();       // vtable slot 5 (+0x28), tail call
// Only the unconditional tail is implemented.  The m_pDataSource == NULL
// block is omitted: the bound VARTYPE at retail site+0xf4 is one of the four
// binding members the BindDefaultProperty comment in
// core/ole/COleControlSite.cpp records OpenMFC's COleControlSite as not
// having, so the bindings cannot be built.  (Retail also calls
// m_pCursorMove->SetBindings on that path without a null check; in OpenMFC
// m_pCursorMove is only ever set by Initialize, which is a stub.)
extern "C" void MS_ABI impl__BindColumns_CDataSourceControl__UEAAXXZ(void* pThis) {
    S_DataSourceControl* p = DSC(pThis);
    // (m_pDataSource == NULL block omitted -- see above)
    impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(pThis);
    reinterpret_cast<PFN_UpdateCursor>(p->vfptr[5])(pThis);   // UpdateControls, same shape
}

// Symbol: ?BindProp@CDataSourceControl@@UEAAXPEAVCDataBoundProperty@@H@Z
// Transcribed from RVA 0x23e250 (mfc140u):
//   if (bBind) {
//       this->BindProp(pProperty, FALSE);            // vtable slot 2 (+0x10)
//       m_CursorBoundProps.AddTail(pProperty);
//   } else {
//       this->UpdateCursor();                        // vtable slot 6 (+0x30),
//                                                    // result ignored
//       walk m_CursorBoundProps from the head for the node whose data ==
//       pProperty; if found, m_CursorBoundProps.RemoveAt(node).
//   }
// (The two CPtrList calls are to 0x231e70 / 0x2320d0 (mfc140u), the ICF-folded
// ?AddTail@CPtrList@@ and ?RemoveAt@CPtrList@@ bodies.)
extern "C" void MS_ABI impl__BindProp_CDataSourceControl__UEAAXPEAVCDataBoundProperty__H_Z(
    void* pThis, void* pProperty, int bBind) {
    S_DataSourceControl* p = DSC(pThis);
    if (bBind) {
        reinterpret_cast<PFN_BindPropProp>(p->vfptr[2])(pThis, pProperty, FALSE);
        impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&p->m_CursorBoundProps, pProperty);
        return;
    }
    reinterpret_cast<PFN_UpdateCursor>(p->vfptr[6])(pThis);
    for (S_PtrNode* pNode = p->m_CursorBoundProps.m_pNodeHead; pNode != nullptr; pNode = pNode->pNext) {
        if (pNode->data == pProperty) {
            impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(&p->m_CursorBoundProps, pNode);
            return;
        }
    }
}

// Symbol: ?BindProp@CDataSourceControl@@UEAAXPEAVCOleControlSite@@H@Z
// Retail RVA 0x23e010 (mfc140u).  The unbind path is transcribed in full; the
// bind path only up to its leading self-unbind.
//   Unbind (bBind == FALSE), 0x23e1e0..0x23e22d (mfc140u), no site member used:
//       this->UpdateCursor();                     // vtable slot 6 (+0x30),
//                                                 // result ignored
//       for (int i = 0; i < m_nColumns; ++i)      // m_nColumns read once
//           walk m_pMetaRowData[i].(+0x38 list) from its head (no null check
//           on the list pointer); at the first node whose data == pSite,
//           list->RemoveAt(node) (0x2320d0 (mfc140u), the CPtrList::RemoveAt
//           body) and return.
//   Bind (bBind != FALSE):
//       this->BindProp(pSite, FALSE);             // vtable slot 3 (+0x18)
//       then find the column whose name equals (wcscmp) the site's bound
//       field-name CString at retail site+0xf8 (names come from the
//       CDynamicAccessor when m_pDataSource is set, else from the metadata
//       entry's +0x28), AddTail the site to that column's +0x38 list, and on
//       no match store NULL into retail site+0xe8.
// The bind path stops after the self-unbind here: the field name (+0xf8) and
// the DSC back-pointer (+0xe8) are two of the four binding members that
// core/ole/COleControlSite.cpp (BindDefaultProperty) records OpenMFC's
// COleControlSite as not having.  (With GetMetaData still a stub, m_nColumns
// is always 0 in OpenMFC, where retail's bind path would reduce to the
// self-unbind plus the +0xe8 clear.)
extern "C" void MS_ABI impl__BindProp_CDataSourceControl__UEAAXPEAVCOleControlSite__H_Z(
    void* pThis, void* pClientSite, int bBind) {
    S_DataSourceControl* p = DSC(pThis);
    if (bBind) {
        reinterpret_cast<PFN_BindPropSite>(p->vfptr[3])(pThis, pClientSite, FALSE);
        // (column search / AddTail / site+0xe8 clear omitted -- see above)
        return;
    }
    reinterpret_cast<PFN_UpdateCursor>(p->vfptr[6])(pThis);
    const INT_PTR nColumns = p->m_nColumns;
    S_MetaRow* pMeta = reinterpret_cast<S_MetaRow*>(p->m_pMetaRowData);
    for (int i = 0; i < nColumns; ++i) {
        S_PtrList* pList = pMeta[i].m_pClientList;
        for (S_PtrNode* pNode = pList->m_pNodeHead; pNode != nullptr; pNode = pNode->pNext) {
            if (pNode->data == pClientSite) {
                impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(pList, pNode);
                return;
            }
        }
    }
}

// Symbol: ?CopyColumnID@CDataSourceControl@@QEAAHPEAUtagDBCOLUMNID@@PEBU2@@Z
// Transcribed from RVA 0x23df50 (mfc140u).  `this` is not used.
//   dst->dwKind = src->dwKind;
//   DBCOLKIND_GUID_NAME (0):   copy guid, then the name copy below
//   DBCOLKIND_GUID_NUMBER (1): copy guid and lNumber; TRUE
//   DBCOLKIND_NAME (2):        the name copy below
//   any other kind:            nothing else; TRUE
// Name copy: n = (src->lpdbsz ? (int)wcslen(src->lpdbsz) : 0) + 1; FALSE if n
// is negative as an int; allocate n*2 bytes with CoTaskMemAlloc (FALSE, with
// no allocation, if that exceeds 0xffffffff), store it in dst->lpdbsz (FALSE
// if null), wcscpy_s(dst->lpdbsz, n, src->lpdbsz); TRUE.  As in retail, a null
// source name reaches wcscpy_s with a null source.
extern "C" int MS_ABI impl__CopyColumnID_CDataSourceControl__QEAAHPEAUtagDBCOLUMNID__PEBU2__Z(
    void* pThis, void* pcidDst, const void* pcidSrc) {
    (void)pThis;
    S_DBColumnID* dst = static_cast<S_DBColumnID*>(pcidDst);
    const S_DBColumnID* src = static_cast<const S_DBColumnID*>(pcidSrc);
    dst->dwKind = src->dwKind;
    switch (src->dwKind) {
    case 0:  // DBCOLKIND_GUID_NAME
        dst->guid = src->guid;
        break;
    case 1:  // DBCOLKIND_GUID_NUMBER
        dst->guid = src->guid;
        dst->lNumber = src->lNumber;
        return TRUE;
    case 2:  // DBCOLKIND_NAME
        break;
    default:
        return TRUE;
    }
    unsigned int n = src->lpdbsz ? static_cast<unsigned int>(std::wcslen(src->lpdbsz)) : 0u;
    ++n;
    const long long cch = static_cast<int>(n);           // movslq
    if (static_cast<unsigned long long>(cch) > 0xffffffffULL) return FALSE;
    const unsigned long long cb = static_cast<unsigned long long>(n) * 2u;
    void* pNew = (cb > 0xffffffffULL) ? nullptr : ::CoTaskMemAlloc(static_cast<ULONG>(cb));
    dst->lpdbsz = static_cast<wchar_t*>(pNew);
    if (pNew == nullptr) return FALSE;
    wcscpy_s(dst->lpdbsz, static_cast<size_t>(cch), src->lpdbsz);
    return TRUE;
}

// Symbol: ?GetBoundClientRow@CDataSourceControl@@QEAAJXZ
// Transcribed from RVA 0x23e500 (mfc140u):
//   OLE DB path (m_pDataSource != NULL):
//       return (m_pRowset && m_pRowset->m_hRow) ? m_pRowset->GetData() : S_OK;
//       (GetData is the inlined ATL CRowset::GetData, see RowsetGetData)
//   cursor path: if m_nBindings == 0 return S_OK; otherwise fetch the current
//       row into m_pValues with
//       m_pCursorMove->Move(1, &DBBMK_CURRENT, 0, &fetch)  // slot 8 (+0x40)
//       where fetch = { cRowsRequested 1, dwFlags 0, pData m_pValues,
//       pVarData NULL, cbVarData 0 }, and return its HRESULT.
// Deviation: retail leaves fetch.cRowsReturned uninitialised; it is zeroed here.
extern "C" long MS_ABI impl__GetBoundClientRow_CDataSourceControl__QEAAJXZ(void* pThis) {
    S_DataSourceControl* p = DSC(pThis);
    if (p->m_pDataSource != nullptr) {
        if (p->m_pRowset != nullptr && p->m_pRowset->m_hRow != 0)
            return RowsetGetData(p->m_pRowset);
        return S_OK;
    }
    if (p->m_nBindings == 0) return S_OK;
    S_DBFetchRows fetch;
    fetch.cRowsRequested = 1;
    fetch.dwFlags = 0;
    fetch.pData = p->m_pValues;
    fetch.pVarData = nullptr;
    fetch.cbVarData = 0;
    fetch.cRowsReturned = 0;
    return reinterpret_cast<PFN_CursorMove>(Vtbl(p->m_pCursorMove)[8])(
        p->m_pCursorMove, 1, &g_dbbmkCurrent, 0, &fetch);
}

// Symbol: ?GetCursor@CDataSourceControl@@UEAAPEAUIUnknown@@XZ
// Transcribed from RVA 0x23d920 (mfc140u):
//   if (m_pDataSource) return m_pRowset ? m_pDataSource : NULL;  // no AddRef
//   if (m_pCursorMove) {
//       ICursor* p; if FAILED(m_pCursorMove->QueryInterface(IID_ICursor, &p))
//           return NULL;
//       return p;
//   }
//   pObj = m_pClientSite->(retail +0x80 IOleObject*);
//   if FAILED(pObj->QueryInterface({1ab42240-...}, &pDSC)) return NULL;
//   pDSC->(slot 5, +0x28)(&pCursor);   // CreateCursor; HRESULT ignored
//   pDSC->Release();
//   return pCursor;
// Retail +0x80 on the site is mapped onto COleControlSite::m_lpObject (the
// mapping core/ole/COleControlSite.cpp uses).  Retail dereferences it without
// a null check; so does this.  Deviation: retail does not initialise pCursor
// before the slot-5 call (it returns whatever is in the stack slot if the
// callee does not write it); it is initialised to NULL here.
extern "C" void* MS_ABI impl__GetCursor_CDataSourceControl__UEAAPEAUIUnknown__XZ(void* pThis) {
    S_DataSourceControl* p = DSC(pThis);
    if (p->m_pDataSource != nullptr)
        return p->m_pRowset != nullptr ? p->m_pDataSource : nullptr;
    if (p->m_pCursorMove != nullptr) {
        void* pCursor = nullptr;
        HRESULT hr = reinterpret_cast<PFN_QueryInterface>(Vtbl(p->m_pCursorMove)[0])(
            p->m_pCursorMove, &kIID_ICursor, &pCursor);
        if (FAILED(hr)) return nullptr;
        return pCursor;
    }
    void* pObject = p->m_pClientSite->m_lpObject;
    void* pDSC = nullptr;
    HRESULT hr = reinterpret_cast<PFN_QueryInterface>(Vtbl(pObject)[0])(pObject, &kIID_IVBDSC, &pDSC);
    if (FAILED(hr)) return nullptr;
    void* pCursor = nullptr;
    reinterpret_cast<PFN_CreateCursor>(Vtbl(pDSC)[5])(pDSC, &pCursor);
    reinterpret_cast<PFN_Release>(Vtbl(pDSC)[2])(pDSC);
    return pCursor;
}

// Symbol: ?GetMetaData@CDataSourceControl@@QEAAJXZ
// Not implemented.  Retail RVA 0x23d9e0 (mfc140u) is a ~400-instruction body
// that builds m_pMetaRowData (0x40-byte METAROWTYPE entries, filled through
// CopyColumnID), calling two non-exported helpers (ANSI 0x236f2c and
// 0x23f550) and AfxThrowMemoryException on allocation failure.  It relies on
// the ATL OLE DB consumer objects (m_pRowset / m_pDynamicAccessor) that
// OpenMFC does not have; it was not decoded further.
extern "C" long MS_ABI impl__GetMetaData_CDataSourceControl__QEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?Initialize@CDataSourceControl@@QEAAJXZ
// Not implemented.  Retail RVA 0x23d690 (mfc140u) starts from the control
// window at retail site+0x50 (m_pWndCtrl -- no OpenMFC equivalent) and its
// CWnd::GetTopLevelParent(); a null site, window or top-level parent ends in
// AfxThrowInvalidArgException.  If the top-level window is not
// IsWindowVisible it is moved to the centre of the desktop
// (GetDesktopWindow/GetWindowRect, CWnd::MoveWindow) and shown, then moved
// back and hidden again before returning -- except on the cursor-path failure
// exits, which return without restoring it.  In between: query the site's
// IOleObject (retail +0x80) for the OLE DB DataSource interface
// {7c0ffab3-cd84-11d0-949a-00a0c91110ed} into m_pDataSource; if that QI
// succeeds, call its slot 3 (getDataMember) for
// {0c733a94-2a1c-11ce-ade5-00aa0044773d} (IRowPosition) into m_pRowPosition,
// and fail with E_POINTER if m_pRowPosition stayed NULL, or with that
// HRESULT if it failed.  Only when the DataSource QI itself fails does it fall back to
// the VB DSC interface's CreateCursor (E_FAIL on a NULL cursor),
// QueryInterface the cursor for ICursorMove into m_pCursorMove and
// m_pCursorMove for {d14216a0-d188-11cd-ad48-00aa003c9cb6}
// (ICursorUpdateARow) into m_pCursorUpdateARow.  Either successful path then
// returns GetMetaData().  The site's control window member is missing in
// OpenMFC, and GetMetaData is itself a stub.
extern "C" long MS_ABI impl__Initialize_CDataSourceControl__QEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?UpdateControls@CDataSourceControl@@UEAAJXZ
// Not implemented.  Retail (vtable slot 5, mfc140u 0x23ee50; ANSI twin
// 0x23d450) sets m_bUpdateInProgress, and for every site in every column's
// client list sets retail site+0xd0 to 1; if the site's IOleObject (+0x80) is
// non-null and yields IDispatch it does Invoke(DISPATCH_PROPERTYPUT) of the
// row value on the bound dispid at retail site+0xf0, clears +0xd0 and
// advances the value index (neither happens when the QI fails); the OLE DB
// path converts the value via ToVariant first.  It clears
// m_bUpdateInProgress and returns S_OK.  Site members +0xd0/+0xf0 have no
// OpenMFC equivalent.
extern "C" long MS_ABI impl__UpdateControls_CDataSourceControl__UEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?UpdateCursor@CDataSourceControl@@UEAAJXZ
// Not implemented.  Retail (vtable slot 6, mfc140u 0x23f070; ANSI twin
// 0x23d670) is a ~400-instruction body.  On the OLE DB path it walks every
// column's client list, reads per-site binding state at retail offsets
// +0x100/+0x108 of each listed site, looks the column up in the
// CDynamicAccessor (m_pDynamicAccessor), and writes the values back, checking
// HRESULTs with AfxCheckError and building COleVariant values.  Those site
// members have no OpenMFC equivalent, and OpenMFC has no ATL OLE DB consumer
// layer.  The cursor path (from 0x23fe2c (mfc140u)) was only skimmed: it too
// reads and clears site+0x100 and reads the VARIANT at site+0x108, so it is
// blocked on the same members.
extern "C" long MS_ABI impl__UpdateCursor_CDataSourceControl__UEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}
