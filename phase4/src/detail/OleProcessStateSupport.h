#pragma once
#include "openmfc/afxstat_.h"
class COleDataSource;
// The private process state has one deleting-destructor slot. Only the clipboard
// field is consumed here; preserve the remaining native state storage.
class _AFX_OLE_STATE : public CNoTrackObject {
    BYTE m_stateBeforeClipboard[8];
public:
    COleDataSource* m_pClipboardSource;
private:
    BYTE m_remainingState[48];
};
static_assert(sizeof(_AFX_OLE_STATE)==72 && offsetof(_AFX_OLE_STATE,m_pClipboardSource)==16,
              "native process OLE state and clipboard owner");
namespace openmfc { namespace detail {
_AFX_OLE_STATE* GetOleProcessState();
} }
