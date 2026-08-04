#include "SockcoreSupport.h"

int AfxSocketInit(WSADATA* lpwsaData) {
    if (g_bSocketsInitialized) {
        if (lpwsaData) {
            *lpwsaData = g_wsaData;
        }
        return TRUE;
    }

    int result = WSAStartup(MAKEWORD(2, 2), &g_wsaData);
    if (result != 0) {
        return FALSE;
    }

    g_bSocketsInitialized = TRUE;

    if (lpwsaData) {
        *lpwsaData = g_wsaData;
    }
    return TRUE;
}
IMPLEMENT_DYNAMIC(CAsyncSocket, CObject)
IMPLEMENT_DYNAMIC(CSocket, CAsyncSocket)
BEGIN_MESSAGE_MAP(CSocketWnd, CWnd)
END_MESSAGE_MAP()
namespace openmfc { namespace detail { namespace sockcore {
int g_bSocketsInitialized = FALSE;
WSADATA g_wsaData;
std::map<SOCKET, CAsyncSocket*> g_socketMap;
std::mutex g_socketMapMutex;
std::deque<AuxSocketMessage> g_auxQueue;
std::mutex g_auxQueueMutex;
} } }  // namespace openmfc::detail::sockcore
