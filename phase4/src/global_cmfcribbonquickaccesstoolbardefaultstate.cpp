// OpenMFC real implementation of CMFCRibbonQuickAccessToolBarDefaultState
//
// Harvested layout (cl.exe /d1reportSingleClassLayout):
//   class CMFCRibbonQuickAccessToolBarDefaultState size(80):
//     +0  CArray<UINT,UINT>  m_arCommands
//     +40 CArray<BOOL,BOOL>  m_arVisibleState
//
// The class itself is NOT polymorphic (no vfptr at offset 0). Its two members
// are MFC CArray objects. A CArray derives from CObject, so each is:
//     +0  vfptr        (CObject vtable pointer; MFC-internal, left null here)
//     +8  m_pData      (TYPE*)
//     +16 m_nSize      (INT_PTR)
//     +24 m_nMaxSize   (INT_PTR)
//     +32 m_nGrowBy    (INT_PTR)
//   => sizeof == 40, two of them == 80.
//
// Both element types (UINT and BOOL) are 4 bytes, so a single set of
// element-size-parameterised helpers faithfully reproduces CArray semantics
// (SetSize grow policy, Add, Copy, RemoveAll).

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

struct CArrayS {
    void*     vfptr;      // 0  CObject vtable pointer
    void*     m_pData;    // 8
    long long m_nSize;    // 16
    long long m_nMaxSize; // 24
    long long m_nGrowBy;  // 32
};

struct S {
    CArrayS m_arCommands;     // 0
    CArrayS m_arVisibleState; // 40
};

static_assert(sizeof(CArrayS) == 40, "CArray layout");
static_assert(sizeof(S) == 80, "CMFCRibbonQuickAccessToolBarDefaultState size");
static_assert(offsetof(S, m_arVisibleState) == 40, "m_arVisibleState offset");
static_assert(offsetof(CArrayS, m_pData) == 8, "m_pData offset");
static_assert(offsetof(CArrayS, m_nSize) == 16, "m_nSize offset");

// Faithful reproduction of MFC CArray::SetSize(nNewSize, /*nGrowBy=*/-1).
static void arr_SetSize(CArrayS* a, long long nNewSize, size_t elem)
{
    if (nNewSize == 0) {
        free(a->m_pData);
        a->m_pData = nullptr;
        a->m_nSize = 0;
        a->m_nMaxSize = 0;
    } else if (a->m_pData == nullptr) {
        a->m_pData = calloc((size_t)nNewSize, elem);
        a->m_nSize = nNewSize;
        a->m_nMaxSize = nNewSize;
    } else if (nNewSize <= a->m_nMaxSize) {
        if (nNewSize > a->m_nSize) {
            memset((char*)a->m_pData + a->m_nSize * (long long)elem, 0,
                   (size_t)((nNewSize - a->m_nSize) * (long long)elem));
        }
        a->m_nSize = nNewSize;
    } else {
        long long nGrowBy = a->m_nGrowBy;
        if (nGrowBy == 0) {
            nGrowBy = a->m_nSize / 8;
            if (nGrowBy < 4) nGrowBy = 4;
            if (nGrowBy > 1024) nGrowBy = 1024;
        }
        long long nNewMax = (nNewSize < a->m_nMaxSize + nGrowBy)
                                ? a->m_nMaxSize + nGrowBy
                                : nNewSize;
        void* p = realloc(a->m_pData, (size_t)(nNewMax * (long long)elem));
        memset((char*)p + a->m_nSize * (long long)elem, 0,
               (size_t)((nNewMax - a->m_nSize) * (long long)elem));
        a->m_pData = p;
        a->m_nSize = nNewSize;
        a->m_nMaxSize = nNewMax;
    }
}

// CArray::Add for a 4-byte element type.
static void arr_Add4(CArrayS* a, unsigned val)
{
    long long idx = a->m_nSize;
    arr_SetSize(a, idx + 1, 4);
    ((unsigned*)a->m_pData)[idx] = val;
}

// CArray::Copy (elements are trivially copyable POD).
static void arr_Copy(CArrayS* dst, const CArrayS* src, size_t elem)
{
    arr_SetSize(dst, src->m_nSize, elem);
    if (src->m_nSize > 0) {
        memcpy(dst->m_pData, src->m_pData, (size_t)(src->m_nSize * (long long)elem));
    }
}

} // namespace

// Symbol: ??0CMFCRibbonQuickAccessToolBarDefaultState@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CMFCRibbonQuickAccessToolBarDefaultState__QEAA_XZ(void* pThis)
{
    S* s = (S*)pThis;
    // Both CArray members default-construct to all-zero (null vfptr, null data,
    // zero sizes/growby) exactly like CObject-derived CArray::CArray().
    memset(s, 0, sizeof(S));
    return pThis;
}

// Symbol: ?AddCommand@CMFCRibbonQuickAccessToolBarDefaultState@@QEAAXIH@Z
extern "C" void MS_ABI
impl__AddCommand_CMFCRibbonQuickAccessToolBarDefaultState__QEAAXIH_Z(
    void* pThis, unsigned uiCmd, int bIsVisible)
{
    S* s = (S*)pThis;
    arr_Add4(&s->m_arCommands, uiCmd);
    arr_Add4(&s->m_arVisibleState, (unsigned)bIsVisible);
}

// Symbol: ?RemoveAll@CMFCRibbonQuickAccessToolBarDefaultState@@QEAAXXZ
extern "C" void MS_ABI
impl__RemoveAll_CMFCRibbonQuickAccessToolBarDefaultState__QEAAXXZ(void* pThis)
{
    S* s = (S*)pThis;
    arr_SetSize(&s->m_arCommands, 0, 4);
    arr_SetSize(&s->m_arVisibleState, 0, 4);
}

// Symbol: ?CopyFrom@CMFCRibbonQuickAccessToolBarDefaultState@@QEAAXAEBV1@@Z
extern "C" void MS_ABI
impl__CopyFrom_CMFCRibbonQuickAccessToolBarDefaultState__QEAAXAEBV1__Z(
    void* pThis, const void* pSrc)
{
    S* s = (S*)pThis;
    const S* src = (const S*)pSrc;
    arr_Copy(&s->m_arCommands, &src->m_arCommands, 4);
    arr_Copy(&s->m_arVisibleState, &src->m_arVisibleState, 4);
}
