#pragma once
#include <windows.h>
#include <sql.h>
// The process database state starts without an allocated ODBC environment.
// Connection allocation owns initialization; diagnostic lookup does not.
HENV& OpenMfcDatabaseEnvironment();
LONG& OpenMfcAllocatedDatabaseConnections();
