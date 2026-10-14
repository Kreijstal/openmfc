#pragma once
#include "openmfc/afxdb.h"
#include <cstdlib>
namespace openmfc { namespace detail {
inline void FreeRecordsetProxies(CRecordset& recordset) {
    if(recordset.m_pvFieldProxy) {
        for(UINT i=0;i<recordset.m_nProxyFields;++i)std::free(recordset.m_pvFieldProxy[i]);
        std::free(recordset.m_pvFieldProxy);recordset.m_pvFieldProxy=nullptr;recordset.m_nProxyFields=0;
    }
    if(recordset.m_pvParamProxy) {
        for(UINT i=0;i<recordset.m_nProxyParams;++i)std::free(recordset.m_pvParamProxy[i]);
        std::free(recordset.m_pvParamProxy);recordset.m_pvParamProxy=nullptr;recordset.m_nProxyParams=0;
    }
}
} }
