#pragma once
#include "openmfc/afxwin.h"
#include "openmfc/afxtls_.h"
class COleObjectFactory;
struct AFX_EXTENSION_MODULE {
    BOOL bInitialized;
    HINSTANCE hModule;
    HINSTANCE hResource;
    CRuntimeClass* pFirstSharedClass;
    COleObjectFactory* pFirstSharedFactory;
};
class CDynLinkLibrary : public CCmdTarget {
    DECLARE_DYNAMIC(CDynLinkLibrary)
public:
    explicit CDynLinkLibrary(AFX_EXTENSION_MODULE&,BOOL system=FALSE);
    CDynLinkLibrary(HINSTANCE module,HINSTANCE resources);
    ~CDynLinkLibrary() override;
    HINSTANCE m_hModule;
    HINSTANCE m_hResource;
    CTypedSimpleList<CRuntimeClass*> m_classList;
    CTypedSimpleList<COleObjectFactory*> m_factoryList;
    BOOL m_bSystem;
    CDynLinkLibrary* m_pNextDLL;
};
static_assert(sizeof(AFX_EXTENSION_MODULE)==40 && sizeof(CDynLinkLibrary)==128,"native extension module and library sizes");
static_assert(offsetof(CDynLinkLibrary,m_hModule)==0x40 && offsetof(CDynLinkLibrary,m_classList)==0x50 &&
              offsetof(CDynLinkLibrary,m_factoryList)==0x60 && offsetof(CDynLinkLibrary,m_bSystem)==0x70 &&
              offsetof(CDynLinkLibrary,m_pNextDLL)==0x78,"native extension library fields");
