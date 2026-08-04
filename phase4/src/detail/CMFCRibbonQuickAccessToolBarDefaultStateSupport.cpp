#include "CMFCRibbonQuickAccessToolBarDefaultStateSupport.h"

static_assert(sizeof(CArrayS) == 40, "CArray layout");
static_assert(sizeof(S_Cmfcribbonquickaccesstoolbardefaultstate) == 80, "CMFCRibbonQuickAccessToolBarDefaultState size");
static_assert(offsetof(S_Cmfcribbonquickaccesstoolbardefaultstate, m_arVisibleState) == 40, "m_arVisibleState offset");
static_assert(offsetof(CArrayS, m_pData) == 8, "m_pData offset");
static_assert(offsetof(CArrayS, m_nSize) == 16, "m_nSize offset");
namespace openmfc { namespace detail { namespace cmfcribbonquickaccesstoolbardefaultstate {
void arr_SetSize(CArrayS* a, long long nNewSize, size_t elem)
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
        if (p == nullptr) return;   // OOM: keep the existing buffer intact, don't deref null
        memset((char*)p + a->m_nSize * (long long)elem, 0,
               (size_t)((nNewMax - a->m_nSize) * (long long)elem));
        a->m_pData = p;
        a->m_nSize = nNewSize;
        a->m_nMaxSize = nNewMax;
    }
}
void arr_Add4(CArrayS* a, unsigned val)
{
    long long idx = a->m_nSize;
    arr_SetSize(a, idx + 1, 4);
    ((unsigned*)a->m_pData)[idx] = val;
}
void arr_Copy(CArrayS* dst, const CArrayS* src, size_t elem)
{
    arr_SetSize(dst, src->m_nSize, elem);
    if (src->m_nSize > 0) {
        memcpy(dst->m_pData, src->m_pData, (size_t)(src->m_nSize * (long long)elem));
    }
}
} } }  // namespace openmfc::detail::cmfcribbonquickaccesstoolbardefaultstate
