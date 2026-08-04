#pragma once
// Shared internals of the former dbcore.cpp translation unit.
// Definitions live in detail/CRecordsetSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace crecordset {} } }
using namespace openmfc::detail::crecordset;
// Database Implementation
// Provides MFC wrappers around ODBC API
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxole.h"
#include <algorithm>
#include <cstdint>
#include <cwctype>
#include <cstring>
#include <limits>
#include <mutex>
#include <new>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace openmfc { namespace detail { namespace crecordset {
extern std::mutex g_recordsetStateMutex;
} } }
namespace openmfc { namespace detail { namespace crecordset {
extern std::unordered_map<const CRecordset*, CString> g_recordsetSql;
} } }

namespace openmfc { namespace detail { namespace crecordset {
struct OdbcFieldInfoCompat {
    CString m_strName;
    short m_nSQLType = SQL_VARCHAR;
    unsigned long m_nPrecision = 0;
    short m_nScale = 0;
    short m_nNullability = SQL_NULLABLE_UNKNOWN;
};
} } }

namespace openmfc { namespace detail { namespace crecordset {
struct LongBinaryCompat {
    HGLOBAL m_hData = nullptr;
    DWORD m_dwDataLength = 0;
};
} } }

namespace openmfc { namespace detail { namespace crecordset {
struct DbState {
    CString databaseName;
    CString connectInfo;
    RETCODE lastRetCode = SQL_SUCCESS;
};
} } }

namespace openmfc { namespace detail { namespace crecordset {
struct RecordsetState {
    CString baseSql;
    CString currentSql;
    std::vector<OdbcFieldInfoCompat> fields;
    std::unordered_set<unsigned long> dirtyFields;
    std::unordered_set<unsigned long> nullFields;
    std::unordered_set<unsigned long> nullParams;
    std::unordered_map<void*, int> boundFieldIndexes;
    std::unordered_map<void*, int> boundParamIndexes;
    std::vector<SQLLEN> fieldLengths;
    std::vector<unsigned char> dataCache;
    long bookmark = 0;
    bool bookmarksEnabled = false;
    bool rowsetAllocated = false;
    bool updatePrepared = false;
    bool fieldsLoaded = false;
    RETCODE lastRetCode = SQL_SUCCESS;
    unsigned int lockingMode = 0;
};
} } }

namespace openmfc { namespace detail { namespace crecordset {
extern std::unordered_map<const CDatabase*, DbState> g_databaseStates;
} } }
namespace openmfc { namespace detail { namespace crecordset {
extern std::unordered_map<const CRecordset*, RecordsetState> g_recordsetStates;
} } }
namespace openmfc { namespace detail { namespace crecordset {
extern int g_loginTimeoutSeconds;
} } }
namespace openmfc { namespace detail { namespace crecordset {
extern int g_queryTimeoutSeconds;
} } }

namespace openmfc { namespace detail { namespace crecordset {
std::wstring WideOf(const wchar_t* value);
} } }

namespace openmfc { namespace detail { namespace crecordset {
std::wstring LowerSql(const wchar_t* value);
} } }

namespace openmfc { namespace detail { namespace crecordset {
bool SqlSucceeded(RETCODE rc);
} } }

namespace openmfc { namespace detail { namespace crecordset {
inline SQLPOINTER CastOdbcOptionValue(std::uint64_t value) {
    return reinterpret_cast<SQLPOINTER>(static_cast<std::uintptr_t>(value));
}
} } }

namespace openmfc { namespace detail { namespace crecordset {
RecordsetState& EnsureRecordsetState(const CRecordset* recordset);
} } }

namespace openmfc { namespace detail { namespace crecordset {
void LoadRecordsetFields(CRecordset* recordset);
} } }

namespace openmfc { namespace detail { namespace crecordset {
int FindFieldByName(CRecordset* recordset, const wchar_t* name);
} } }

namespace openmfc { namespace detail { namespace crecordset {
CString GetStoredSql(CRecordset* recordset);
} } }

namespace openmfc { namespace detail { namespace crecordset {
void StoreSql(CRecordset* recordset, const CString& sql);
} } }

//=============================================================================
// CDBException
//=============================================================================
// No IMPLEMENT_DYNAMIC needed - CDBException doesn't derive from CObject

//=============================================================================
// CDatabase
//=============================================================================

















//=============================================================================
// CFieldExchange
//=============================================================================






//=============================================================================
// CRecordset
//=============================================================================



































//=============================================================================
// CRecordView
//=============================================================================


















































namespace openmfc { namespace detail { namespace crecordset {
unsigned int AppendCsvName(CString* sql, const wchar_t* name);
} } }



namespace openmfc { namespace detail { namespace crecordset {
void GetFieldTextByIndex(CRecordset* pThis, short index, CString* out);
} } }

