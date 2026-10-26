#pragma once
#include "openmfc/afxstat_.h"
#include "detail/CPtrListSupport.h"
class CHandleMap;
class CToolTipCtrl;

// Release Unicode x64 per-module, per-thread state (sizeof 0x138).
class AFX_MODULE_THREAD_STATE : public CNoTrackObject {
  public:
    using NewHandler = int(MS_ABI*)(size_t);
    AFX_MODULE_THREAD_STATE();
    ~AFX_MODULE_THREAD_STATE() override;
    CWinThread* m_pCurrentWinThread; // 0x08
    void* m_pReserved10;
    size_t m_nSafetyPoolSize; // 0x18
    int m_nTempMapLock;       // 0x20
    CHandleMap* m_pmapHWND;   // 0x28
    CHandleMap* m_pmapHMENU;
    CHandleMap* m_pmapHDC;
    CHandleMap* m_pmapHGDIOBJ;
    CHandleMap* m_pmapHIMAGELIST;
    NewHandler m_pfnNewHandler; // 0x50
    void* m_pReserved58;
    CMapPtrToPtr m_mapSocketHandle;             // 0x60
    CMapPtrToPtr m_mapDeadSockets;              // 0x98
    OpenMFC_CPtrList m_listSocketNotifications; // 0xd0
    CToolTipCtrl* m_pToolTip;                   // 0x108
    void* m_pReserved110;
    UINT_PTR m_nLastHit;
    void* m_pLastInfo;
    UINT_PTR m_nLastHitMessage;
    BOOL m_bLastStatus;
};
extern "C" AFX_MODULE_THREAD_STATE* MS_ABI
impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
extern "C" AFX_MODULE_THREAD_STATE::NewHandler MS_ABI impl__AfxGetNewHandler__YAP6AH_K_ZXZ();
extern "C" AFX_MODULE_THREAD_STATE::NewHandler
    MS_ABI impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(AFX_MODULE_THREAD_STATE::NewHandler);
namespace openmfc {
namespace detail {
namespace modulethreadstate {
int MS_ABI DefaultAllocationFailure(size_t);
int MS_ABI HandleAllocationFailure(size_t);
} // namespace modulethreadstate
} // namespace detail
} // namespace openmfc
