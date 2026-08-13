// COleObjectFactory — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

// Export wrappers (placement-new the C++ ctors above; matches .def aliases).
// Symbol: ??0COleObjectFactory@@QEAA@AEBU_GUID@@PEAUCRuntimeClass@@HPEB_W@Z
// Ordinal: 850
extern "C" COleObjectFactory* MS_ABI impl___0COleObjectFactory__QEAA_AEBU_GUID__PEAUCRuntimeClass__HPEB_W_Z(
    COleObjectFactory* pThis, const GUID* pClsid, CRuntimeClass* pRuntimeClass,
    int bMultiInstance, const wchar_t* lpszProgID) {
    return new(pThis) COleObjectFactory(*pClsid, pRuntimeClass, bMultiInstance, lpszProgID);
}
// Symbol: ??0COleObjectFactory@@QEAA@AEBU_GUID@@PEAUCRuntimeClass@@HHPEB_W@Z
// Ordinal: 849
extern "C" COleObjectFactory* MS_ABI impl___0COleObjectFactory__QEAA_AEBU_GUID__PEAUCRuntimeClass__HHPEB_W_Z(
    COleObjectFactory* pThis, const GUID* pClsid, CRuntimeClass* pRuntimeClass,
    int bMultiInstance, int bFreeOnRelease, const wchar_t* lpszProgID) {
    return new(pThis) COleObjectFactory(*pClsid, pRuntimeClass, bMultiInstance,
                                         bFreeOnRelease, lpszProgID);
}
// Symbol: ?CreateInstance@XClassFactory@COleObjectFactory@@UEAAJPEAUIUnknown@@AEBU_GUID@@PEAPEAX@Z
// Ordinal: 3312
extern "C" HRESULT MS_ABI impl__CreateInstance_XClassFactory_COleObjectFactory__UEAAJPEAUIUnknown__AEBU_GUID__PEAPEAX_Z(
    COleObjectFactory::XClassFactory* pThis, IUnknown* pUnkOuter,
    const GUID* priid, void** ppv) {
    return pThis->CreateInstance(pUnkOuter, *priid, ppv);
}
// Symbol: ?CreateInstanceLic@XClassFactory@COleObjectFactory@@UEAAJPEAUIUnknown@@0AEBU_GUID@@PEA_WPEAPEAX@Z
// Ordinal: 3313
extern "C" HRESULT MS_ABI impl__CreateInstanceLic_XClassFactory_COleObjectFactory__UEAAJPEAUIUnknown__0AEBU_GUID__PEA_WPEAPEAX_Z(
    COleObjectFactory::XClassFactory* pThis, IUnknown* pUnkOuter,
    IUnknown* pUnkReserved, const GUID* priid, wchar_t* bstrKey, void** ppv) {
    return pThis->CreateInstanceLic(pUnkOuter, pUnkReserved, *priid, bstrKey, ppv);
}
// Symbol: ?GetLicInfo@XClassFactory@COleObjectFactory@@UEAAJPEAUtagLICINFO@@@Z
// Ordinal: 5685
extern "C" HRESULT MS_ABI impl__GetLicInfo_XClassFactory_COleObjectFactory__UEAAJPEAUtagLICINFO___Z(
    COleObjectFactory::XClassFactory* pThis, LICINFO* pLicInfo) {
    return pThis->GetLicInfo(pLicInfo);
}
// Symbol: ?LockServer@XClassFactory@COleObjectFactory@@UEAAJH@Z
// Ordinal: 8199
extern "C" HRESULT MS_ABI impl__LockServer_XClassFactory_COleObjectFactory__UEAAJH_Z(
    COleObjectFactory::XClassFactory* pThis, int fLock) {
    return pThis->LockServer(fLock);
}
// Symbol: ?RequestLicKey@XClassFactory@COleObjectFactory@@UEAAJKPEAPEA_W@Z
// Ordinal: 12488
extern "C" HRESULT MS_ABI impl__RequestLicKey_XClassFactory_COleObjectFactory__UEAAJKPEAPEA_W_Z(
    COleObjectFactory::XClassFactory* pThis, unsigned long dwReserved,
    wchar_t** pbstrKey) {
    return pThis->RequestLicKey(dwReserved, pbstrKey);
}
COleObjectFactory::COleObjectFactory()
    : m_pRuntimeClass(nullptr), m_bMultiInstance(FALSE), m_strProgID(L""),
      m_dwRegister(0), m_pTemplate(nullptr),
      m_xClassFactory(this) {
    memset(&m_clsid, 0, sizeof(m_clsid));
    memset(_oleobjectfactory_padding, 0, sizeof(_oleobjectfactory_padding));
    AddOleObjectFactory(this);
}
COleObjectFactory::COleObjectFactory(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                                      BOOL bMultiInstance, const wchar_t* lpszProgID)
    : m_clsid(clsid), m_pRuntimeClass(pRuntimeClass),
      m_bMultiInstance(bMultiInstance), m_strProgID(lpszProgID ? lpszProgID : L""),
      m_dwRegister(0), m_pTemplate(nullptr),
      m_xClassFactory(this) {
    memset(_oleobjectfactory_padding, 0, sizeof(_oleobjectfactory_padding));
    AddOleObjectFactory(this);
}
COleObjectFactory::COleObjectFactory(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                                      BOOL bMultiInstance, BOOL bFreeOnRelease,
                                      const wchar_t* lpszProgID)
    : COleObjectFactory() {
    CommonConstruct(clsid, pRuntimeClass, bMultiInstance, bFreeOnRelease, lpszProgID);
}
COleObjectFactory::~COleObjectFactory() {
    Revoke();
    RemoveOleObjectFactory(this);
}
void COleObjectFactory::CommonConstruct(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
                                         BOOL bMultiInstance, BOOL bFreeOnRelease,
                                         const wchar_t* lpszProgID) {
    (void)bFreeOnRelease;
    m_clsid = clsid;
    m_pRuntimeClass = pRuntimeClass;
    m_bMultiInstance = bMultiInstance;
    m_strProgID = lpszProgID ? lpszProgID : L"";
    m_dwRegister = 0;
    m_pTemplate = nullptr;
    AddOleObjectFactory(this);
}
BOOL COleObjectFactory::Register() {
    AfxOleInit();
    HRESULT hr = CoRegisterClassObject(m_clsid, &m_xClassFactory,
                                        CLSCTX_LOCAL_SERVER,
                                        REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED,
                                        &m_dwRegister);
    return SUCCEEDED(hr);
}
void COleObjectFactory::Revoke() {
    if (m_dwRegister) {
        CoRevokeClassObject(m_dwRegister);
        m_dwRegister = 0;
    }
}
BOOL COleObjectFactory::IsRegistered() const {
    return m_dwRegister != 0;
}
BOOL COleObjectFactory::IsLicenseValid() {
    return VerifyUserLicense();
}
int COleObjectFactory::RegisterAll() {
    int count = 0;
    for (COleObjectFactory* factory : g_oleObjectFactories) {
        if (factory && factory->Register()) ++count;
    }
    return count;
}
BOOL COleObjectFactory::UpdateRegistryAll(BOOL bRegister) {
    BOOL ok = TRUE;
    for (COleObjectFactory* factory : g_oleObjectFactories) {
        if (!factory) continue;
        factory->UpdateRegistry(bRegister);
    }
    return ok;
}
void COleObjectFactory::RevokeAll() {
    for (COleObjectFactory* factory : g_oleObjectFactories) {
        if (factory) factory->Revoke();
    }
}
BOOL COleObjectFactory::Unregister() {
    Revoke();
    return TRUE;
}
BOOL COleObjectFactory::UnregisterAll() {
    RevokeAll();
    return TRUE;
}
CCmdTarget* COleObjectFactory::OnCreateObject() {
    if (!m_pRuntimeClass) {
        return nullptr;
    }
    CObject* pObject = m_pRuntimeClass->CreateObject();
    return pObject ? static_cast<CCmdTarget*>(pObject) : nullptr;
}
BOOL COleObjectFactory::OnVerifyFile(LPCTSTR lpszFileName) {
    return lpszFileName && *lpszFileName;
}
BOOL COleObjectFactory::UpdateRegistry(BOOL bRegister) {
    if (m_strProgID.IsEmpty()) return FALSE;

    wchar_t clsidText[64] = {};
    if (StringFromGUID2(m_clsid, clsidText, 64) == 0) return FALSE;

    CString clsidKey = CString(L"CLSID\\") + clsidText;
    if (!bRegister) {
        LONG progResult = RegDeleteTreeW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(m_strProgID));
        LONG clsidResult = RegDeleteTreeW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(clsidKey));
        return (progResult == ERROR_SUCCESS || progResult == ERROR_FILE_NOT_FOUND) &&
               (clsidResult == ERROR_SUCCESS || clsidResult == ERROR_FILE_NOT_FOUND);
    }

    BOOL success = TRUE;
    HKEY hKey = nullptr;
    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(m_strProgID), 0, nullptr,
                        REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        HKEY hClsid = nullptr;
        if (RegCreateKeyExW(hKey, L"CLSID", 0, nullptr, REG_OPTION_NON_VOLATILE,
                            KEY_SET_VALUE, nullptr, &hClsid, nullptr) == ERROR_SUCCESS) {
            RegSetValueExW(hClsid, nullptr, 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(clsidText),
                           static_cast<DWORD>((wcslen(clsidText) + 1) * sizeof(wchar_t)));
            RegCloseKey(hClsid);
        } else {
            success = FALSE;
        }
        RegCloseKey(hKey);
    } else {
        success = FALSE;
    }

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, static_cast<const wchar_t*>(clsidKey), 0, nullptr,
                        REG_OPTION_NON_VOLATILE, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        const wchar_t* progID = static_cast<const wchar_t*>(m_strProgID);
        RegSetValueExW(hKey, nullptr, 0, REG_SZ,
                       reinterpret_cast<const BYTE*>(progID),
                       static_cast<DWORD>((wcslen(progID) + 1) * sizeof(wchar_t)));
        HKEY hProgID = nullptr;
        if (RegCreateKeyExW(hKey, L"ProgID", 0, nullptr, REG_OPTION_NON_VOLATILE,
                            KEY_SET_VALUE, nullptr, &hProgID, nullptr) == ERROR_SUCCESS) {
            RegSetValueExW(hProgID, nullptr, 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(progID),
                           static_cast<DWORD>((wcslen(progID) + 1) * sizeof(wchar_t)));
            RegCloseKey(hProgID);
        }
        RegCloseKey(hKey);
    } else {
        success = FALSE;
    }
    return success;
}
void COleObjectFactory::ConnectTemplate(COleTemplateServer* pTemplate) {
    m_pTemplate = pTemplate;
}
BOOL COleObjectFactory::VerifyUserLicense() {
    return TRUE;
}
BOOL COleObjectFactory::VerifyLicenseKey(BSTR bstrKey) {
    (void)bstrKey;
    return TRUE;
}
BOOL COleObjectFactory::GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey) {
    (void)dwReserved;
    if (pbstrKey) {
        *pbstrKey = nullptr;
    }
    return FALSE;
}
COleObjectFactory::XClassFactory::XClassFactory(COleObjectFactory* pOuter)
    : m_pOuter(pOuter), m_refCount(0), m_lockCount(0) {
}
STDMETHODIMP COleObjectFactory::XClassFactory::QueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory) ||
        IsEqualIID(riid, IID_IClassFactory2)) {
        *ppv = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}
STDMETHODIMP COleObjectFactory::XClassFactory::CreateInstance(
    IUnknown* pUnkOuter, REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (pUnkOuter) return CLASS_E_NOAGGREGATION;
    if (!m_pOuter) return E_UNEXPECTED;

    CCmdTarget* pObject = m_pOuter->OnCreateObject();
    if (!pObject) return E_OUTOFMEMORY;

    // CCmdTarget-derived objects in this implementation do not yet expose
    // COM interfaces through their interface map. Return E_NOINTERFACE.
    // Real MFC would walk the object's GetInterfaceMap() to find the
    // matching COM identity (e.g. nested X* class) and QI from it.
    (void)riid;
    delete pObject;
    return E_NOINTERFACE;
}
STDMETHODIMP COleObjectFactory::XClassFactory::LockServer(BOOL fLock) {
    if (fLock)
        InterlockedIncrement(&m_lockCount);
    else
        InterlockedDecrement(&m_lockCount);
    return S_OK;
}
STDMETHODIMP COleObjectFactory::XClassFactory::GetLicInfo(LICINFO* pLicInfo) {
    if (!pLicInfo) return E_POINTER;
    memset(pLicInfo, 0, sizeof(*pLicInfo));
    if (!m_pOuter) return E_UNEXPECTED;
    if (m_pOuter->VerifyUserLicense()) {
        pLicInfo->fLicVerified = TRUE;
        pLicInfo->fRuntimeKeyAvail = m_pOuter->GetLicenseKey(0, nullptr) ? TRUE : FALSE;
    }
    return S_OK;
}
STDMETHODIMP COleObjectFactory::XClassFactory::RequestLicKey(
    DWORD dwReserved, BSTR* pbstrKey) {
    if (!pbstrKey) return E_POINTER;
    if (!m_pOuter) return E_UNEXPECTED;
    return m_pOuter->GetLicenseKey(dwReserved, pbstrKey) ? S_OK : E_FAIL;
}
STDMETHODIMP COleObjectFactory::XClassFactory::CreateInstanceLic(
    IUnknown* pUnkOuter, IUnknown* pUnkReserved, REFIID riid,
    BSTR bstrKey, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (pUnkOuter) return CLASS_E_NOAGGREGATION;
    if (!m_pOuter) return E_UNEXPECTED;
    (void)pUnkReserved;

    // Verify license key
    if (!m_pOuter->VerifyLicenseKey(bstrKey))
        return CLASS_E_NOTLICENSED;

    return CreateInstance(nullptr, riid, ppv);
}

// === Moved from ManualThunks.cpp ===
// Symbol: ?UpdateRegistry@COleObjectFactory@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__UpdateRegistry_COleObjectFactory__QEAAXPEB_W_Z(void* pThis, void* p0) {
    (void)pThis;
    (void)p0;
}

