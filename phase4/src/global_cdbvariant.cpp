// OpenMFC — CDBVariant real implementation
// Layout (MSVC /d1reportSingleClassLayout): size(24)
//   0  {vfptr}
//   8  m_dwType (DWORD)
//  12  <alignment padding> (4)
//  16  union { BOOL/uchar/short/long/float/double/pointers } (8)
#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ODBC DBVT_* value-type tags (mfc afxdb-style enum).
enum {
    DBVT_NULL    = 0,
    DBVT_BOOL    = 1,
    DBVT_UCHAR   = 2,
    DBVT_SHORT   = 3,
    DBVT_LONG    = 4,
    DBVT_SINGLE  = 5,
    DBVT_DOUBLE  = 6,
    DBVT_DATE    = 7,
    DBVT_STRING  = 8,
    DBVT_BINARY  = 9,
    DBVT_ASTRING = 10,
    DBVT_WSTRING = 11
};

namespace {

struct S {
    void*  vfptr;       // 0
    DWORD  m_dwType;    // 8
    char   _pad[4];     // 12 (alignment)
    union {             // 16
        int           m_boolVal;
        unsigned char m_chVal;
        short         m_iVal;
        long          m_lVal;
        float         m_fltVal;
        double        m_dblVal;
        void*         m_ptr;   // covers m_pdate / m_pstring / m_pbinary / m_pstringA / m_pstringW
    };
};

static_assert(sizeof(S) == 24, "CDBVariant layout size");
static_assert(offsetof(S, m_dwType) == 8, "m_dwType offset");
static_assert(offsetof(S, m_ptr) == 16, "union offset");

// Free any resource owned by the current value-type, then mark NULL.
inline void ClearImpl(S* s) {
    switch (s->m_dwType) {
    case DBVT_DATE:    // TIMESTAMP_STRUCT*
    case DBVT_STRING:  // CString*
    case DBVT_BINARY:  // CLongBinary*
    case DBVT_ASTRING: // CStringA*
    case DBVT_WSTRING: // CStringW*
        if (s->m_ptr) {
            ::operator delete(s->m_ptr);
            s->m_ptr = nullptr;
        }
        break;
    default:
        break;
    }
    s->m_dwType = DBVT_NULL;
}

// Vector-deleting-destructor: run the ~CDBVariant behavior, then (flag&1) free.
void* MS_ABI vdtor(void* p, unsigned f) {
    ClearImpl(reinterpret_cast<S*>(p));
    if (f & 1) ::operator delete(p);
    return p;
}

// vtable: single slot (deleting destructor), per harvested vtable order.
void* const g_CDBVariant_vtbl[1] = {
    reinterpret_cast<void*>(&vdtor)
};

} // namespace

extern "C" {

// Symbol: ??0CDBVariant@@QEAA@XZ
void* MS_ABI impl___0CDBVariant__QEAA_XZ(void* pThis) {
    S* s = reinterpret_cast<S*>(pThis);
    s->vfptr = (void*)&g_CDBVariant_vtbl[0];
    s->m_dwType = DBVT_NULL;
    s->m_ptr = nullptr;
    return pThis;
}

// Symbol: ??1CDBVariant@@UEAA@XZ
void MS_ABI impl___1CDBVariant__UEAA_XZ(void* pThis) {
    // ~CDBVariant frees any owned value, matching Clear().
    ClearImpl(reinterpret_cast<S*>(pThis));
}

// Symbol: ?Clear@CDBVariant@@QEAAXXZ
void MS_ABI impl__Clear_CDBVariant__QEAAXXZ(void* pThis) {
    ClearImpl(reinterpret_cast<S*>(pThis));
}

} // extern "C"
