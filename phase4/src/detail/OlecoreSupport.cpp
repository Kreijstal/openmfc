#include "OlecoreSupport.h"

IMPLEMENT_DYNAMIC(CControlBar, CWnd)
IMPLEMENT_DYNAMIC(CDocItem, CObject)
void CCmdUI_Delegated(CCmdUI* pThis, void (CCmdUI::*fn)(int), int arg) {
    if (pThis && pThis->m_pOther && pThis->m_pOther != pThis) {
        (pThis->m_pOther->*fn)(arg);
    }
}
void CCmdUI_DelegatedText(CCmdUI* pThis, void (CCmdUI::*fn)(const wchar_t*), const wchar_t* text) {
    if (pThis && pThis->m_pOther && pThis->m_pOther != pThis) {
        (pThis->m_pOther->*fn)(text);
    }
}
extern "C" ULONG MS_ABI impl__AddRef_COleConnPtContainer__UEAAKXZ(
    COleConnPtContainer* pThis
) {
    (void)pThis;
    return 1;
}
HRESULT DataSourceDataObject::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) return E_POINTER;
    if (riid == IID_IUnknown || riid == IID_IDataObject) {
        *ppvObject = static_cast<IDataObject*>(this);
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}
HRESULT DataSourceDataObject::GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium) {
    if (!pformatetcIn || !pmedium) return E_POINTER;
    DataCacheEntry* entry = FindCacheEntry(m_state, *pformatetcIn);
    if (!entry) return DATA_E_FORMATETC;

    if (!entry->hasMedium) {
        if (entry->delayRender && m_state->owner->OnRenderGlobalData(&entry->format, reinterpret_cast<void**>(&entry->medium.hGlobal))) {
            entry->medium.tymed = TYMED_HGLOBAL;
            entry->medium.pUnkForRelease = nullptr;
            entry->hasMedium = true;
        } else if (entry->delayRenderFile && MakeRenderedFileMedium(m_state->owner, &entry->format, &entry->medium)) {
            entry->hasMedium = true;
        } else {
            return DATA_E_FORMATETC;
        }
    }

    return CopyStorageMedium(entry->medium, pmedium) ? S_OK : STG_E_MEDIUMFULL;
}
HRESULT DataSourceDataObject::QueryGetData(FORMATETC* pformatetc) {
    if (!pformatetc) return E_POINTER;
    return FindCacheEntry(m_state, *pformatetc) ? S_OK : DATA_E_FORMATETC;
}
HRESULT DataSourceDataObject::SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease) {
    if (!pformatetc || !pmedium) return E_POINTER;

    DataCacheEntry* existing = FindCacheEntry(m_state, *pformatetc);
    if (!existing) {
        m_state->entries.push_back(DataCacheEntry());
        existing = &m_state->entries.back();
    } else if (existing->hasMedium) {
        ReleaseStgMedium(&existing->medium);
        existing->hasMedium = false;
    }

    existing->format = *pformatetc;
    existing->delayRender = false;
    existing->delayRenderFile = false;
    if (fRelease) {
        existing->medium = *pmedium;
        memset(pmedium, 0, sizeof(*pmedium));
        existing->hasMedium = true;
        return S_OK;
    }
    existing->hasMedium = CopyStorageMedium(*pmedium, &existing->medium);
    return existing->hasMedium ? S_OK : STG_E_MEDIUMFULL;
}
HRESULT DataSourceDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc) {
    if (!ppenumFormatEtc) return E_POINTER;
    *ppenumFormatEtc = nullptr;
    if (dwDirection != DATADIR_GET) return S_FALSE;

    CEnumFormatEtc* enumerator = new CEnumFormatEtc();
    for (const DataCacheEntry& entry : m_state->entries) {
        enumerator->AddFormat(entry.format);
    }
    *ppenumFormatEtc = enumerator;
    return S_OK;
}
HRESULT DropTargetAdapter::DragEnter(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    *effect = DROPEFFECT_NONE;
    if (!m_target) return S_OK;

    m_dataObject.Release();
    if (dataObject) m_dataObject.Attach(dataObject, FALSE);
    *effect = m_target->OnDragEnter(m_target->m_pWnd, &m_dataObject, keyState, MakePoint(point));
    return S_OK;
}
HRESULT DropTargetAdapter::DragOver(DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    *effect = m_target ? m_target->OnDragOver(m_target->m_pWnd, &m_dataObject, keyState, MakePoint(point)) : DROPEFFECT_NONE;
    return S_OK;
}
HRESULT DropTargetAdapter::DragLeave() {
    if (m_target) m_target->OnDragLeave(m_target->m_pWnd);
    m_dataObject.Release();
    return S_OK;
}
HRESULT DropTargetAdapter::Drop(IDataObject* dataObject, DWORD keyState, POINTL point, DWORD* effect) {
    if (!effect) return E_POINTER;
    DROPEFFECT requested = *effect;
    *effect = DROPEFFECT_NONE;
    if (!m_target) return S_OK;

    COleDataObject dropData;
    COleDataObject* data = &m_dataObject;
    if (dataObject) {
        dropData.Attach(dataObject, FALSE);
        data = &dropData;
    }

    if (m_target->OnDrop(m_target->m_pWnd, data, requested, MakePoint(point))) {
        *effect = requested;
    }
    m_dataObject.Release();
    (void)keyState;
    return S_OK;
}
extern "C" {
MS_ABI void impl__AfxOleInit() {
    if (!g_bOleInitialized) {
        HRESULT hr = OleInitialize(nullptr);
        if (SUCCEEDED(hr)) {
            g_bOleInitialized = TRUE;
            // Create default message filter
            COleMessageFilter* pFilter = new COleMessageFilter();
            g_pMessageFilter = pFilter;
        }
    }
}
MS_ABI void impl__AfxOleTerm(int bExplicit) {
    if (g_bOleInitialized) {
        if (g_pMessageFilter) {
            delete g_pMessageFilter;
            g_pMessageFilter = nullptr;
        }
        if (bExplicit) {
            OleUninitialize();
        }
        g_bOleInitialized = FALSE;
    }
}
MS_ABI void impl__AfxOleLockApp() {
    g_nOleLockCount++;
    CoAddRefServerProcess();
}
MS_ABI void impl__AfxOleUnlockApp() {
    if (g_nOleLockCount > 0) {
        g_nOleLockCount--;
        CoReleaseServerProcess();
    }
}
MS_ABI int impl__AfxOleCanExitApp() {
    return (g_nOleLockCount == 0) ? TRUE : FALSE;
}
MS_ABI int impl__AfxOleGetMessageFilter() {
    return (g_pMessageFilter != nullptr) ? TRUE : FALSE;
}
}
IMPLEMENT_DYNAMIC(COleDataSource, CCmdTarget)
IMPLEMENT_DYNAMIC(COleDropTarget, CCmdTarget)
IMPLEMENT_DYNAMIC(COleDropSource, CCmdTarget)
IMPLEMENT_DYNAMIC(COleMessageFilter, CCmdTarget)
IMPLEMENT_DYNAMIC(COleFrameHook, CCmdTarget)
IMPLEMENT_DYNAMIC(COleResizeBar, CControlBar)
IMPLEMENT_DYNAMIC(COlePropertyPage, CDialog)
IMPLEMENT_DYNAMIC(COleDocument, CDocument)
IMPLEMENT_DYNAMIC(COleLinkingDoc, COleDocument)
IMPLEMENT_DYNAMIC(COleServerDoc, COleLinkingDoc)
IMPLEMENT_DYNAMIC(COleClientItem, CDocItem)
IMPLEMENT_DYNAMIC(COleServerItem, CDocItem)
IMPLEMENT_DYNAMIC(COleObjectFactory, CCmdTarget)
IMPLEMENT_DYNAMIC(COleTemplateServer, COleObjectFactory)
IMPLEMENT_DYNAMIC(COleControlSite, CCmdTarget)
IMPLEMENT_DYNAMIC(COleControlContainer, CCmdTarget)
IMPLEMENT_DYNAMIC(COleIPFrameWnd, CFrameWnd)
IMPLEMENT_DYNAMIC(COleDialog, CDialog)
IMPLEMENT_DYNAMIC(COleBusyDialog, COleDialog)
IMPLEMENT_DYNAMIC(COleChangeIconDialog, COleDialog)
IMPLEMENT_DYNAMIC(COleChangeSourceDialog, COleDialog)
IMPLEMENT_DYNAMIC(COleConvertDialog, COleDialog)
IMPLEMENT_DYNAMIC(COleInsertDialog, COleDialog)
IMPLEMENT_DYNAMIC(COleLinksDialog, COleDialog)
IMPLEMENT_DYNAMIC(COlePasteSpecialDialog, COleDialog)
IMPLEMENT_DYNAMIC(COlePropertiesDialog, COleDialog)
IMPLEMENT_DYNAMIC(COleUpdateDialog, COleDialog)
IMPLEMENT_DYNAMIC(COleControl, CWnd)
static_assert(sizeof(COleControl) == 912, "COleControl must match retail sizeof");
static_assert(sizeof(CFontHolder) == 24, "CFontHolder must match retail sizeof");
static_assert(offsetof(COleControl, m_piidPrimary) == 232, "m_piidPrimary @232");
static_assert(offsetof(COleControl, m_ambientDispDriver) == 256, "m_ambientDispDriver @256");
static_assert(offsetof(COleControl, m_rcPos) == 288, "m_rcPos @288");
static_assert(offsetof(COleControl, m_cxExtent) == 328, "m_cxExtent @328");
static_assert(offsetof(COleControl, m_clrBackColor) == 356, "m_clrBackColor @356");
static_assert(offsetof(COleControl, m_strText) == 368, "m_strText @368");
static_assert(offsetof(COleControl, m_font) == 376, "m_font @376");
static_assert(offsetof(COleControl, m_pClientSite) == 472, "m_pClientSite @472");
static_assert(offsetof(COleControl, m_pControlSite) == 488, "m_pControlSite @488");
static_assert(offsetof(COleControl, m_frameInfo) == 528, "m_frameInfo @528");
static_assert(offsetof(COleControl, m_xPersistStorage) == 584, "m_xPersistStorage @584");
static_assert(offsetof(COleControl, m_xEventConnPt) == 720, "m_xEventConnPt @720");
static_assert(offsetof(COleControl, m_xPropConnPt) == 816, "m_xPropConnPt @816");
extern "C" {
MS_ABI int impl__AfxOleRegisterTypeLib(REFGUID guid, const wchar_t* lpszFileName) {
    if (!lpszFileName) return FALSE;
    ITypeLib* pTypeLib = nullptr;
    HRESULT hr = LoadTypeLib(lpszFileName, &pTypeLib);
    if (FAILED(hr)) return FALSE;
    hr = RegisterTypeLib(pTypeLib, (OLECHAR*)lpszFileName, nullptr);
    pTypeLib->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
MS_ABI int impl__AfxOleRegisterServerClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                            const wchar_t* lpszShortTypeName,
                                            const wchar_t* lpszLongTypeName,
                                            int nAppType, const wchar_t** rglpszRegister,
                                            const wchar_t** rglpszOverwrite) {
    (void)clsid; (void)lpszClassName; (void)lpszShortTypeName;
    (void)lpszLongTypeName; (void)nAppType; (void)rglpszRegister; (void)rglpszOverwrite;
    return FALSE;
}
MS_ABI int impl__AfxOleRegisterControlClass(REFCLSID clsid, const wchar_t* lpszClassName,
                                             const wchar_t* lpszShortTypeName,
                                             const wchar_t* lpszLongTypeName,
                                             DWORD dwMiscStatus, DWORD dwFlags, DWORD dwVersion) {
    (void)clsid; (void)lpszClassName; (void)lpszShortTypeName;
    (void)lpszLongTypeName; (void)dwMiscStatus; (void)dwFlags; (void)dwVersion;
    return FALSE;
}
MS_ABI int impl__AfxOleUnregisterClass(REFCLSID clsid, const wchar_t* lpszClassName) {
    (void)clsid; (void)lpszClassName;
    return TRUE;
}
MS_ABI int impl__AfxOleSetEditMenu(COleClientItem* pItem, CMenu* pMenu, UINT iMenuItem,
                                    UINT nIDVerbMin, UINT nIDVerbMax, UINT nIDConvert) {
    (void)pItem; (void)pMenu; (void)iMenuItem; (void)nIDVerbMin; (void)nIDVerbMax; (void)nIDConvert;
    return FALSE;
}
MS_ABI int impl__AfxOleSetUserCtrl(BOOL bUserCtrl) {
    (void)bUserCtrl;
    return FALSE;
}
MS_ABI COLORREF impl__AfxOleTranslateColor(OLE_COLOR clrColor, HPALETTE hpal) {
    (void)hpal;
    // OLE_COLOR is a COLORREF for standard colors
    // ARGB -> ABGR conversion for OLE_COLOR type
    if (clrColor & 0x80000000) {
        // Palette entry, extract from hpal
        return clrColor;
    }
    return clrColor;
}
MS_ABI int impl__AfxOleUnregisterTypeLib(REFGUID guid, WORD wVerMajor, WORD wVerMinor, LCID lcid) {
    HRESULT hr = UnRegisterTypeLib(guid, wVerMajor, wVerMinor, lcid, SYS_WIN32);
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
}
IMPLEMENT_DYNAMIC(COleDocObjectItem, COleClientItem)
namespace openmfc { namespace detail { namespace olecore {
CMenu* CCmdUI_Menu(const CCmdUI* pThis) {
    auto* ui = reinterpret_cast<const CCmdUIShim_Olecore*>(pThis);
    if (!ui) return nullptr;
    if (ui->m_pSubMenu != nullptr) {
        return ui->m_pSubMenu;
    }
    return ui->m_pMenu;
}
bool CCmdUI_IsValidMenu(const CCmdUI* pThis) {
    auto* ui = reinterpret_cast<const CCmdUIShim_Olecore*>(pThis);
    return ui != nullptr && CCmdUI_Menu(pThis) != nullptr && CCmdUI_Menu(pThis)->m_hMenu != nullptr;
}
bool CCmdUI_IdValid(UINT id) {
    return id != 0U && id != static_cast<UINT>(-1);
}
bool CCmdUI_HasByPositionTarget(const CCmdUI* pThis, UINT* itemID, UINT* flags) {
    auto* ui = reinterpret_cast<const CCmdUIShim_Olecore*>(pThis);
    if (!ui) return false;
    if (ui->m_nIndex > 0) {
        *itemID = static_cast<UINT>(ui->m_nIndex);
        *flags = MF_BYPOSITION;
        return true;
    }
    if (!CCmdUI_IdValid(ui->m_nID)) {
        return false;
    }
    *itemID = ui->m_nID;
    *flags = MF_BYCOMMAND;
    return true;
}
void CCmdUIEnableForMenu(CCmdUI* pThis, BOOL bOn) {
    auto* ui = reinterpret_cast<CCmdUIShim_Olecore*>(pThis);
    if (!ui || pThis->m_pOther) {
        CCmdUI_Delegated(pThis, &CCmdUI::Enable, bOn);
        return;
    }

    CMenu* pMenu = CCmdUI_Menu(pThis);
    if (!CCmdUI_IsValidMenu(pThis)) {
        return;
    }

    UINT itemID = 0;
    UINT flags = MF_BYCOMMAND;
    if (!CCmdUI_HasByPositionTarget(pThis, &itemID, &flags)) return;

    UINT state = bOn ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
    if (CCmdUI_IsValidMenu(pThis)) {
        ::EnableMenuItem(pMenu->m_hMenu, itemID, flags | state);
    }
}
void CCmdUICheckForMenu(CCmdUI* pThis, int nCheck) {
    auto* ui = reinterpret_cast<CCmdUIShim_Olecore*>(pThis);
    if (!ui || pThis->m_pOther) {
        CCmdUI_Delegated(pThis, &CCmdUI::SetCheck, nCheck);
        return;
    }

    if (!CCmdUI_IsValidMenu(pThis)) {
        return;
    }
    CMenu* pMenu = CCmdUI_Menu(pThis);

    UINT itemID = 0;
    UINT flags = MF_BYCOMMAND;
    if (!CCmdUI_HasByPositionTarget(pThis, &itemID, &flags)) return;
    ::CheckMenuItem(pMenu->m_hMenu, itemID, flags | (nCheck ? MF_CHECKED : MF_UNCHECKED));
}
void CCmdUISetTextForMenu(CCmdUI* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    auto* ui = reinterpret_cast<CCmdUIShim_Olecore*>(pThis);
    if (!ui || pThis->m_pOther) {
        CCmdUI_DelegatedText(pThis, &CCmdUI::SetText, lpszText);
        return;
    }

    if (!CCmdUI_IsValidMenu(pThis)) {
        return;
    }
    CMenu* pMenu = CCmdUI_Menu(pThis);

    UINT itemID = 0;
    UINT flags = MF_BYCOMMAND;
    if (!CCmdUI_HasByPositionTarget(pThis, &itemID, &flags)) return;

    MENUITEMINFOW mii = {};
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE | MIIM_STRING;
    mii.dwTypeData = const_cast<wchar_t*>(lpszText ? lpszText : L"");
    mii.cch = static_cast<UINT>(lpszText ? (wcslen(lpszText) + 1) : 0);
    ::SetMenuItemInfoW(pMenu->m_hMenu, itemID, flags == MF_BYPOSITION, &mii);
}
int g_bOleInitialized = FALSE;
int g_nOleLockCount = 0;
BOOL g_bOleUserCtrl = FALSE;
COleMessageFilter* g_pMessageFilter = nullptr;
COleDataSource* g_pClipboardOwner = nullptr;
std::map<CString, CString> g_oleFactoryLicenseKeys;
BOOL g_userOleControlMode = TRUE;
std::map<void*, COleControl*> g_reflectorControls_Olecore;
FORMATETC MakeFormatEtc(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (lpFormatEtc) {
        FORMATETC fmt = *lpFormatEtc;
        fmt.cfFormat = cfFormat ? cfFormat : fmt.cfFormat;
        return fmt;
    }

    FORMATETC fmt = {};
    fmt.cfFormat = cfFormat;
    fmt.ptd = nullptr;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_HGLOBAL;
    return fmt;
}
bool FormatMatches(const FORMATETC& cached, const FORMATETC& requested) {
    if (cached.cfFormat != requested.cfFormat) return false;
    if ((cached.tymed & requested.tymed) == 0) return false;
    if (requested.dwAspect != 0 && cached.dwAspect != requested.dwAspect) return false;
    if (requested.lindex != -1 && cached.lindex != requested.lindex) return false;
    return true;
}
HGLOBAL DuplicateGlobalMemory(HGLOBAL source) {
    if (!source) return nullptr;

    SIZE_T size = GlobalSize(source);
    HGLOBAL copy = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!copy) return nullptr;

    void* src = GlobalLock(source);
    void* dst = GlobalLock(copy);
    if (!src || !dst) {
        if (src) GlobalUnlock(source);
        if (dst) GlobalUnlock(copy);
        GlobalFree(copy);
        return nullptr;
    }

    memcpy(dst, src, size);
    GlobalUnlock(copy);
    GlobalUnlock(source);
    return copy;
}
bool CopyStorageMedium(const STGMEDIUM& source, STGMEDIUM* dest) {
    if (!dest) return false;
    memset(dest, 0, sizeof(*dest));
    dest->tymed = source.tymed;
    dest->pUnkForRelease = nullptr;

    switch (source.tymed) {
    case TYMED_HGLOBAL:
        dest->hGlobal = DuplicateGlobalMemory(source.hGlobal);
        return dest->hGlobal != nullptr;
    case TYMED_ISTREAM:
        dest->pstm = source.pstm;
        if (dest->pstm) dest->pstm->AddRef();
        return dest->pstm != nullptr;
    case TYMED_ISTORAGE:
        dest->pstg = source.pstg;
        if (dest->pstg) dest->pstg->AddRef();
        return dest->pstg != nullptr;
    case TYMED_GDI:
        dest->hBitmap = static_cast<HBITMAP>(CopyImage(source.hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
        return dest->hBitmap != nullptr;
    case TYMED_MFPICT:
        dest->hMetaFilePict = source.hMetaFilePict;
        return dest->hMetaFilePict != nullptr;
    case TYMED_ENHMF:
        dest->hEnhMetaFile = CopyEnhMetaFileW(source.hEnhMetaFile, nullptr);
        return dest->hEnhMetaFile != nullptr;
    case TYMED_FILE:
        dest->lpszFileName = source.lpszFileName ? static_cast<LPOLESTR>(CoTaskMemAlloc((wcslen(source.lpszFileName) + 1) * sizeof(OLECHAR))) : nullptr;
        if (source.lpszFileName && !dest->lpszFileName) return false;
        if (source.lpszFileName) wcscpy(dest->lpszFileName, source.lpszFileName);
        return true;
    default:
        dest->tymed = TYMED_NULL;
        return false;
    }
}
LPOLESTR CopyOleString(const wchar_t* text) {
    if (!text) return nullptr;
    const size_t bytes = (wcslen(text) + 1) * sizeof(OLECHAR);
    LPOLESTR copy = static_cast<LPOLESTR>(CoTaskMemAlloc(bytes));
    if (copy) memcpy(copy, text, bytes);
    return copy;
}
bool MakeRenderedFileMedium(COleDataSource* source, FORMATETC* format, STGMEDIUM* medium) {
    if (!source || !medium) return false;

    wchar_t tempPath[MAX_PATH] = {};
    wchar_t tempName[MAX_PATH] = {};
    if (!GetTempPathW(MAX_PATH, tempPath)) return false;
    if (!GetTempFileNameW(tempPath, L"omf", 0, tempName)) return false;

    bool ok = false;
    {
        CFile file(tempName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeBinary);
        ok = source->OnRenderFileData(format, &file) != 0;
    }

    if (!ok) {
        DeleteFileW(tempName);
        return false;
    }

    memset(medium, 0, sizeof(*medium));
    medium->tymed = TYMED_FILE;
    medium->lpszFileName = CopyOleString(tempName);
    if (!medium->lpszFileName) {
        DeleteFileW(tempName);
        return false;
    }
    return true;
}
void InitializeDocumentView(COleDocObjectItem* item, IOleDocumentView* view) {
    if (!item || !view) return;

    IOleClientSite* clientSite = nullptr;
    if (item->m_lpObject &&
        SUCCEEDED(item->m_lpObject->GetClientSite(&clientSite)) && clientSite) {
        IOleInPlaceSite* inPlaceSite = nullptr;
        if (SUCCEEDED(clientSite->QueryInterface(IID_IOleInPlaceSite,
                                                 reinterpret_cast<void**>(&inPlaceSite))) &&
            inPlaceSite) {
            view->SetInPlaceSite(inPlaceSite);
            inPlaceSite->Release();
        }
        clientSite->Release();
    }

    CRect itemRect;
    item->OnGetItemPosition(itemRect);
    RECT rect = { itemRect.left, itemRect.top, itemRect.right, itemRect.bottom };
    view->SetRect(&rect);
    view->UIActivate(TRUE);
}
int CountDispatchParams(const BYTE* pbParamInfo) {
    if (!pbParamInfo) return 0;
    int count = 0;
    while (pbParamInfo[count] != 0) ++count;
    return count;
}
HRESULT MakeDispatchVariant(VARTYPE vt, va_list* args, VARIANTARG* var) {
    if (!var) return E_POINTER;
    VariantInit(var);
    switch (vt) {
    case VT_I2:
        var->vt = VT_I2;
        var->iVal = static_cast<SHORT>(va_arg(*args, int));
        return S_OK;
    case VT_I4:
        var->vt = VT_I4;
        var->lVal = va_arg(*args, long);
        return S_OK;
    case VT_R4:
        var->vt = VT_R4;
        var->fltVal = static_cast<float>(va_arg(*args, double));
        return S_OK;
    case VT_R8:
        var->vt = VT_R8;
        var->dblVal = va_arg(*args, double);
        return S_OK;
    case VT_BOOL:
        var->vt = VT_BOOL;
        var->boolVal = va_arg(*args, int) ? VARIANT_TRUE : VARIANT_FALSE;
        return S_OK;
    case VT_BSTR:
        var->vt = VT_BSTR;
        var->bstrVal = SysAllocString(va_arg(*args, const wchar_t*));
        return var->bstrVal ? S_OK : E_OUTOFMEMORY;
    case VT_DISPATCH:
        var->vt = VT_DISPATCH;
        var->pdispVal = va_arg(*args, LPDISPATCH);
        return S_OK;
    case VT_UNKNOWN:
        var->vt = VT_UNKNOWN;
        var->punkVal = va_arg(*args, LPUNKNOWN);
        return S_OK;
    case VT_VARIANT: {
        VARIANT* src = va_arg(*args, VARIANT*);
        return src ? VariantCopy(var, src) : S_OK;
    }
    case VT_UI1:
        var->vt = VT_UI1;
        var->bVal = static_cast<BYTE>(va_arg(*args, int));
        return S_OK;
    case VT_UI2:
        var->vt = VT_UI2;
        var->uiVal = static_cast<USHORT>(va_arg(*args, int));
        return S_OK;
    case VT_UI4:
        var->vt = VT_UI4;
        var->ulVal = va_arg(*args, unsigned long);
        return S_OK;
    default:
        return DISP_E_TYPEMISMATCH;
    }
}
HRESULT CopyDispatchResult(VARTYPE vt, void* pvRet, VARIANT* result) {
    if (vt == VT_EMPTY || !pvRet) return S_OK;
    switch (vt) {
    case VT_I2:
        *static_cast<short*>(pvRet) = result->iVal;
        return S_OK;
    case VT_I4:
        *static_cast<long*>(pvRet) = result->lVal;
        return S_OK;
    case VT_R4:
        *static_cast<float*>(pvRet) = result->fltVal;
        return S_OK;
    case VT_R8:
        *static_cast<double*>(pvRet) = result->dblVal;
        return S_OK;
    case VT_BOOL:
        *static_cast<BOOL*>(pvRet) = (result->boolVal == VARIANT_TRUE);
        return S_OK;
    case VT_BSTR:
        *static_cast<BSTR*>(pvRet) = result->bstrVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_DISPATCH:
        *static_cast<LPDISPATCH*>(pvRet) = result->pdispVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_UNKNOWN:
        *static_cast<LPUNKNOWN*>(pvRet) = result->punkVal;
        result->vt = VT_EMPTY;
        return S_OK;
    case VT_VARIANT:
        return VariantCopy(static_cast<VARIANT*>(pvRet), result);
    case VT_UI1:
        *static_cast<BYTE*>(pvRet) = result->bVal;
        return S_OK;
    case VT_UI2:
        *static_cast<USHORT*>(pvRet) = result->uiVal;
        return S_OK;
    case VT_UI4:
        *static_cast<ULONG*>(pvRet) = result->ulVal;
        return S_OK;
    default:
        return DISP_E_TYPEMISMATCH;
    }
}
void ResetOleVariant(COleVariant* pThis) {
    if (!pThis) return;
    VARIANT* var = static_cast<VARIANT*>(pThis);
    VariantClear(var);
    VariantInit(var);
}
void AssignI2Variant(COleVariant* pThis, short value, VARTYPE vt) {
    if (!pThis) return;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    if (vt == VT_I2 || vt == VT_EMPTY) {
        var->vt = VT_I2;
        var->iVal = value;
        return;
    }
    VARIANT src;
    VariantInit(&src);
    src.vt = VT_I2;
    src.iVal = value;
    if (FAILED(VariantChangeType(var, &src, 0, vt))) {
        *var = src;
        VariantInit(&src);
    }
}
void AssignI4Variant(COleVariant* pThis, long value, VARTYPE vt) {
    if (!pThis) return;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    if (vt == VT_I4 || vt == VT_EMPTY) {
        var->vt = VT_I4;
        var->lVal = value;
        return;
    }
    VARIANT src;
    VariantInit(&src);
    src.vt = VT_I4;
    src.lVal = value;
    if (FAILED(VariantChangeType(var, &src, 0, vt))) {
        *var = src;
        VariantInit(&src);
    }
}
void SetOleVariantString(COleVariant* pThis, const wchar_t* text, VARTYPE vt) {
    if (!pThis) return;
    const wchar_t* srcText = text ? text : L"";
    if (vt == VT_EMPTY) vt = VT_BSTR;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    if (vt == VT_BSTR) {
        var->vt = VT_BSTR;
        var->bstrVal = SysAllocString(srcText);
        return;
    }

    VARIANT src;
    VariantInit(&src);
    src.vt = VT_BSTR;
    src.bstrVal = SysAllocString(srcText);
    if (!src.bstrVal && srcText[0] != L'\0') {
        return;
    }
    if (FAILED(VariantChangeType(var, &src, 0, vt))) {
        *var = src;
        VariantInit(&src);
        return;
    }
    VariantClear(&src);
}
const COleVariant* AssignOleVariant(COleVariant* pThis, const VARIANT* pSrc) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    if (pSrc) {
        VariantCopy(static_cast<VARIANT*>(pThis), const_cast<VARIANT*>(pSrc));
    }
    return pThis;
}
COleClientItem* FindDocumentItemByOleObject(const COleDocument* doc, IOleObject* object) {
    if (!doc || !object) return nullptr;
    POSITION pos = reinterpret_cast<POSITION>(doc->GetStartPosition());
    while (pos) {
        COleClientItem* item = doc->GetNextClientItem(pos);
        if (item && item->GetObject() == object) return item;
    }
    return nullptr;
}
void CopyVariantByteArray(COleVariant* pThis, CByteArray* bytes) {
    if (!bytes) return;
    bytes->RemoveAll();
    if (!pThis) return;

    VARIANT* var = static_cast<VARIANT*>(pThis);
    if ((var->vt & VT_ARRAY) == 0 || !var->parray) return;
    VARTYPE elementType = VT_EMPTY;
    if (FAILED(SafeArrayGetVartype(var->parray, &elementType))) {
        elementType = static_cast<VARTYPE>(var->vt & VT_TYPEMASK);
    }
    if (elementType != VT_UI1 && elementType != VT_I1) return;

    LONG lower = 0;
    LONG upper = -1;
    if (FAILED(SafeArrayGetLBound(var->parray, 1, &lower)) ||
        FAILED(SafeArrayGetUBound(var->parray, 1, &upper)) ||
        upper < lower) {
        return;
    }

    BYTE* data = nullptr;
    if (FAILED(SafeArrayAccessData(var->parray, reinterpret_cast<void**>(&data))) || !data) return;
    const LONG count = upper - lower + 1;
    bytes->SetSize(count);
    for (LONG i = 0; i < count; ++i) {
        bytes->SetAt(i, data[i]);
    }
    SafeArrayUnaccessData(var->parray);
}
OleDispatchDriverLayout* DispatchDriverLayout(COleDispatchDriver* pThis) {
    return reinterpret_cast<OleDispatchDriverLayout*>(pThis);
}
const OleDispatchDriverLayout* DispatchDriverLayout(const COleDispatchDriver* pThis) {
    return reinterpret_cast<const OleDispatchDriverLayout*>(pThis);
}
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_AEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    AssignOleVariant(pThis, pSrc);
    return pThis;
}
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_AEBV0__Z(
    COleVariant* pThis, const COleVariant* pSrc
) {
    return impl___0COleVariant__QEAA_AEBUtagVARIANT___Z(
        pThis, reinterpret_cast<const VARIANT*>(pSrc)
    );
}
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_FG_Z(
    COleVariant* pThis, short value, VARTYPE vt
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    AssignI2Variant(pThis, value, vt);
    return pThis;
}
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_JG_Z(
    COleVariant* pThis, long value, VARTYPE vt
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    AssignI4Variant(pThis, value, vt);
    return pThis;
}
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEB_WG_Z(
    COleVariant* pThis, const wchar_t* text, VARTYPE vt
) {
    if (!pThis) return nullptr;
    new (pThis) COleVariant();
    SetOleVariantString(pThis, text, vt);
    return pThis;
}
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    return impl___0COleVariant__QEAA_AEBUtagVARIANT___Z(pThis, pSrc);
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBV0__Z(
    COleVariant* pThis, const COleVariant* pSrc
) {
    return AssignOleVariant(pThis, reinterpret_cast<const VARIANT*>(pSrc));
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    return AssignOleVariant(pThis, pSrc);
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_PEBUtagVARIANT___Z(
    COleVariant* pThis, const VARIANT* pSrc
) {
    return AssignOleVariant(pThis, pSrc);
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_F_Z(
    COleVariant* pThis, short value
) {
    AssignI2Variant(pThis, value, VT_I2);
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_J_Z(
    COleVariant* pThis, long value
) {
    AssignI4Variant(pThis, value, VT_I4);
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_M_Z(
    COleVariant* pThis, float value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_R4;
    pThis->fltVal = value;
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_N_Z(
    COleVariant* pThis, double value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_R8;
    pThis->dblVal = value;
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_QEB_W_Z(
    COleVariant* pThis, const wchar_t* text
) {
    SetOleVariantString(pThis, text, VT_BSTR);
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0__J_Z(
    COleVariant* pThis, __int64 value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_I8;
    pThis->llVal = value;
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0__K_Z(
    COleVariant* pThis, unsigned __int64 value
) {
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    pThis->vt = VT_UI8;
    pThis->ullVal = value;
    return pThis;
}
extern "C" void MS_ABI impl__SetString_COleVariant__QEAAXPEB_WG_Z(
    COleVariant* pThis, const wchar_t* text, VARTYPE vt
) {
    SetOleVariantString(pThis, text, vt);
}
extern "C" void MS_ABI impl__Attach_COleVariant__QEAAXAEAUtagVARIANT___Z(
    COleVariant* pThis, VARIANT* pSrc
) {
    if (!pThis || !pSrc) return;
    ResetOleVariant(pThis);
    *static_cast<VARIANT*>(pThis) = *pSrc;
    VariantInit(pSrc);
}
extern "C" VARIANT* MS_ABI impl__Detach_COleVariant__QEAA_AUtagVARIANT__XZ(
    VARIANT* pRet, COleVariant* pThis
) {
    if (!pRet) return nullptr;
    VariantInit(pRet);
    if (!pThis) return pRet;
    *pRet = *static_cast<VARIANT*>(pThis);
    VariantInit(static_cast<VARIANT*>(pThis));
    return pRet;
}
extern "C" void MS_ABI impl__ChangeType_COleVariant__QEAAXGPEAUtagVARIANT___Z(
    COleVariant* pThis, VARTYPE vtNew, VARIANT* pSrc
) {
    if (!pThis) return;
    VARIANT converted;
    VariantInit(&converted);
    VARIANT* source = pSrc ? pSrc : static_cast<VARIANT*>(pThis);
    if (FAILED(VariantChangeType(&converted, source, 0, vtNew))) {
        return;
    }
    ResetOleVariant(pThis);
    *static_cast<VARIANT*>(pThis) = converted;
}
BOOL _AfxCompareSafeArrays_Olecore(SAFEARRAY* parray1, SAFEARRAY* parray2)
{
    if (!parray1 || !parray2) return parray1 == parray2;
    DWORD dwDim1 = SafeArrayGetDim(parray1);
    DWORD dwDim2 = SafeArrayGetDim(parray2);
    if (dwDim1 != dwDim2) return FALSE;
    if (dwDim1 == 0) return TRUE;
    DWORD dwSize1 = SafeArrayGetElemsize(parray1);
    DWORD dwSize2 = SafeArrayGetElemsize(parray2);
    if (dwSize1 != dwSize2) return FALSE;

    BOOL bCompare = FALSE;
    long* pLBound1 = new (std::nothrow) long[dwDim1];
    long* pLBound2 = new (std::nothrow) long[dwDim2];
    long* pUBound1 = new (std::nothrow) long[dwDim1];
    long* pUBound2 = new (std::nothrow) long[dwDim2];
    void* pData1 = nullptr;
    void* pData2 = nullptr;

    if (!pLBound1 || !pLBound2 || !pUBound1 || !pUBound2) goto cleanup;

    {
        size_t nTotalElements = 1;
        for (DWORD i = 0; i < dwDim1; ++i) {
            if (FAILED(SafeArrayGetLBound(parray1, i+1, &pLBound1[i])) ||
                FAILED(SafeArrayGetLBound(parray2, i+1, &pLBound2[i])) ||
                FAILED(SafeArrayGetUBound(parray1, i+1, &pUBound1[i])) ||
                FAILED(SafeArrayGetUBound(parray2, i+1, &pUBound2[i])))
                goto cleanup;
            if (pUBound1[i] - pLBound1[i] != pUBound2[i] - pLBound2[i])
                goto cleanup;
            nTotalElements *= (size_t)(pUBound1[i] - pLBound1[i] + 1);
        }
        if (FAILED(SafeArrayAccessData(parray1, &pData1)) || !pData1) goto cleanup;
        if (FAILED(SafeArrayAccessData(parray2, &pData2)) || !pData2) goto cleanup;
        size_t nSize = nTotalElements * dwSize1;
        bCompare = (memcmp(pData1, pData2, nSize) == 0);
        SafeArrayUnaccessData(parray1);
        SafeArrayUnaccessData(parray2);
    }

cleanup:
    if (pData1) SafeArrayUnaccessData(parray1);
    if (pData2) SafeArrayUnaccessData(parray2);
    delete[] pLBound1;
    delete[] pLBound2;
    delete[] pUBound1;
    delete[] pUBound2;
    return bCompare;
}
void _AfxCreateOneDimArray(COleVariant* pThis, DWORD dwSize)
{
    VARIANT* varSrc = static_cast<VARIANT*>(pThis);
    if (varSrc->vt != (VT_UI1 | VT_ARRAY) || SafeArrayGetDim(varSrc->parray) != 1) {
        VariantClear(varSrc);
        varSrc->vt = VT_UI1 | VT_ARRAY;
        SAFEARRAYBOUND bound = { dwSize, 0 };
        varSrc->parray = SafeArrayCreate(VT_UI1, 1, &bound);
        if (!varSrc->parray) AfxThrowMemoryException();
    } else {
        long lLower = 0, lUpper = 0;
        SafeArrayGetLBound(varSrc->parray, 1, &lLower);
        SafeArrayGetUBound(varSrc->parray, 1, &lUpper);
        long lSize = lUpper - lLower;
        if (lSize < 0) lSize = 0;
        if ((DWORD)lSize != dwSize) {
            SAFEARRAYBOUND bound = { dwSize, lLower };
            SafeArrayRedim(varSrc->parray, &bound);
        }
    }
}
void _AfxCopyBinaryData(COleVariant* pThis, const void* pvSrc, DWORD dwSize)
{
    SAFEARRAY* parray = static_cast<VARIANT*>(pThis)->parray;
    if (!parray) return;
    void* pDest = nullptr;
    if (SUCCEEDED(SafeArrayAccessData(parray, &pDest)) && pDest) {
        memcpy(pDest, pvSrc, dwSize);
        SafeArrayUnaccessData(parray);
    }
}
extern "C" COleVariant* MS_ABI impl___0COleVariant__QEAA_PEFBU_ITEMIDLIST___Z(
    COleVariant* pThis, const ITEMIDLIST* pidl)
{
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    if (pidl) {
        UINT cbTotal = 0;
        LPCITEMIDLIST pidlWalker = pidl;
        while (pidlWalker->mkid.cb) {
            cbTotal += pidlWalker->mkid.cb;
            pidlWalker = reinterpret_cast<LPCITEMIDLIST>(
                reinterpret_cast<const BYTE*>(pidlWalker) + pidlWalker->mkid.cb);
        }
        cbTotal += sizeof(ITEMIDLIST);
        SAFEARRAY* psa = SafeArrayCreateVector(VT_UI1, 0, cbTotal);
        if (psa) {
            memcpy(psa->pvData, pidl, cbTotal);
            static_cast<VARIANT*>(pThis)->vt = VT_ARRAY | VT_UI1;
            static_cast<VARIANT*>(pThis)->parray = psa;
        }
    }
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    COleVariant* pThis, const CString* strSrc)
{
    if (!pThis) return nullptr;
    ResetOleVariant(pThis);
    VARIANT* var = static_cast<VARIANT*>(pThis);
    var->vt = VT_BSTR;
    var->bstrVal = SysAllocString(strSrc ? strSrc->GetString() : L"");
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCByteArray___Z(
    COleVariant* pThis, const CByteArray* arrSrc)
{
    if (!pThis || !arrSrc) return pThis;
    INT_PTR nSize = arrSrc->GetSize();
    if (nSize > LONG_MAX) AfxThrowMemoryException();
    _AfxCreateOneDimArray(pThis, static_cast<DWORD>(nSize));
    _AfxCopyBinaryData(pThis, arrSrc->GetData(), static_cast<DWORD>(nSize));
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCLongBinary___Z(
    COleVariant* pThis, const CLongBinary* lbSrc)
{
    if (!pThis || !lbSrc) return pThis;
    if (lbSrc->m_dwDataLength > LONG_MAX) AfxThrowMemoryException();
    _AfxCreateOneDimArray(pThis, lbSrc->m_dwDataLength);
    BYTE* pData = static_cast<BYTE*>(GlobalLock(lbSrc->m_hData));
    if (pData) {
        _AfxCopyBinaryData(pThis, pData, lbSrc->m_dwDataLength);
        GlobalUnlock(lbSrc->m_hData);
    }
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCOleCurrency___Z(
    COleVariant* pThis, const COleCurrency* curSrc)
{
    if (!pThis) return nullptr;
    if (static_cast<VARIANT*>(pThis)->vt != VT_CY) {
        ResetOleVariant(pThis);
        static_cast<VARIANT*>(pThis)->vt = VT_CY;
    }
    static_cast<VARIANT*>(pThis)->cyVal = curSrc->m_cur;
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_AEBVCOleDateTime_ATL___Z(
    COleVariant* pThis, const COleDateTime* dateSrc)
{
    if (!pThis) return nullptr;
    if (static_cast<VARIANT*>(pThis)->vt != VT_DATE) {
        ResetOleVariant(pThis);
        static_cast<VARIANT*>(pThis)->vt = VT_DATE;
    }
    static_cast<VARIANT*>(pThis)->date = dateSrc->m_dt;
    return pThis;
}
extern "C" const COleVariant* MS_ABI impl___4COleVariant__QEAAAEBV0_E_Z(
    COleVariant* pThis, unsigned char nSrc)
{
    if (!pThis) return nullptr;
    if (static_cast<VARIANT*>(pThis)->vt != VT_UI1) {
        ResetOleVariant(pThis);
        static_cast<VARIANT*>(pThis)->vt = VT_UI1;
    }
    static_cast<VARIANT*>(pThis)->bVal = nSrc;
    return pThis;
}
extern "C" CArchive* MS_ABI impl___5_YAAEAVCArchive__AEAV0_AEAVCOleVariant___Z(
    CArchive* ar, COleVariant* varSrc)
{
    if (!ar || !varSrc) return ar;
    LPVARIANT pSrc = static_cast<VARIANT*>(varSrc);
    if (pSrc->vt != VT_EMPTY)
        VariantClear(pSrc);
    *ar >> pSrc->vt;
    if (pSrc->vt & VT_BYREF || pSrc->vt & VT_ARRAY)
        return ar;
    switch (pSrc->vt) {
    case VT_BOOL: { WORD v = 0; *ar >> v; V_BOOL(pSrc) = v; return ar; }
    case VT_I1:   *ar >> pSrc->cVal; return ar;
    case VT_UI1:  *ar >> pSrc->bVal; return ar;
    case VT_I2:   *ar >> pSrc->iVal; return ar;
    case VT_UI2:  *ar >> pSrc->uiVal; return ar;
    case VT_I4:   *ar >> pSrc->lVal; return ar;
    case VT_UI4:  *ar >> pSrc->ulVal; return ar;
    case VT_I8:   ar->Read(&pSrc->llVal, sizeof(LONGLONG)); return ar;
    case VT_UI8:  ar->Read(&pSrc->ullVal, sizeof(ULONGLONG)); return ar;
    case VT_CY:   *ar >> pSrc->cyVal.Lo; *ar >> pSrc->cyVal.Hi; return ar;
    case VT_R4:   *ar >> pSrc->fltVal; return ar;
    case VT_R8:   *ar >> pSrc->dblVal; return ar;
    case VT_DATE: *ar >> pSrc->date; return ar;
    case VT_BSTR: {
        DWORD nLen = 0;
        *ar >> nLen;
        if (nLen > 0) {
            pSrc->bstrVal = SysAllocStringByteLen(nullptr, nLen);
            if (!pSrc->bstrVal) AfxThrowMemoryException();
            ar->Read(pSrc->bstrVal, nLen * sizeof(BYTE));
        } else {
            pSrc->bstrVal = nullptr;
        }
        return ar;
    }
    case VT_ERROR: *ar >> pSrc->scode; return ar;
    case VT_DISPATCH:
    case VT_UNKNOWN: {
        CLSID clsid = {};
        *ar >> clsid.Data1;
        *ar >> clsid.Data2;
        *ar >> clsid.Data3;
        ar->Read(&clsid.Data4[0], sizeof(clsid.Data4));
        HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL | CLSCTX_REMOTE_SERVER,
            pSrc->vt == VT_UNKNOWN ? IID_IUnknown : IID_IDispatch,
            reinterpret_cast<void**>(&pSrc->punkVal));
        if (hr == E_INVALIDARG)
            hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL & ~CLSCTX_REMOTE_SERVER,
                pSrc->vt == VT_UNKNOWN ? IID_IUnknown : IID_IDispatch,
                reinterpret_cast<void**>(&pSrc->punkVal));
        if (FAILED(hr)) AfxThrowOleException(hr);
        IPersistStream* pPS = nullptr;
        hr = pSrc->punkVal->QueryInterface(IID_IPersistStream, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr))
            hr = pSrc->punkVal->QueryInterface(IID_IPersistStreamInit, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr)) { pSrc->punkVal->Release(); AfxThrowOleException(hr); }
        CArchiveStream stm(ar);
        hr = pPS->Load(&stm);
        pPS->Release();
        if (FAILED(hr)) { pSrc->punkVal->Release(); AfxThrowOleException(hr); }
        return ar;
    }
    case VT_EMPTY:
    case VT_NULL:
    default:
        return ar;
    }
}
extern "C" CArchive* MS_ABI impl___6_YAAEAVCArchive__AEAV0_VCOleVariant___Z(
    CArchive* ar, const VARIANT* varSrc)
{
    if (!ar || !varSrc) return ar;
    *ar << varSrc->vt;
    if (varSrc->vt & VT_BYREF || varSrc->vt & VT_ARRAY)
        return ar;
    switch (varSrc->vt) {
    case VT_BOOL: *ar << static_cast<WORD>(V_BOOL(varSrc)); return ar;
    case VT_I1:   *ar << varSrc->cVal; return ar;
    case VT_UI1:  *ar << varSrc->bVal; return ar;
    case VT_I2:   *ar << varSrc->iVal; return ar;
    case VT_UI2:  *ar << varSrc->uiVal; return ar;
    case VT_I4:   *ar << varSrc->lVal; return ar;
    case VT_UI4:  *ar << varSrc->ulVal; return ar;
    case VT_I8:   ar->Write(&varSrc->llVal, sizeof(LONGLONG)); return ar;
    case VT_UI8:  ar->Write(&varSrc->ullVal, sizeof(ULONGLONG)); return ar;
    case VT_CY:   *ar << varSrc->cyVal.Lo; *ar << varSrc->cyVal.Hi; return ar;
    case VT_R4:   *ar << varSrc->fltVal; return ar;
    case VT_R8:   *ar << varSrc->dblVal; return ar;
    case VT_DATE: *ar << varSrc->date; return ar;
    case VT_BSTR: {
        DWORD nLen = SysStringByteLen(varSrc->bstrVal);
        *ar << nLen;
        if (nLen > 0)
            ar->Write(varSrc->bstrVal, nLen * sizeof(BYTE));
        return ar;
    }
    case VT_ERROR: *ar << varSrc->scode; return ar;
    case VT_DISPATCH:
    case VT_UNKNOWN: {
        IPersistStream* pPS = nullptr;
        HRESULT hr = varSrc->punkVal->QueryInterface(IID_IPersistStream, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr))
            hr = varSrc->punkVal->QueryInterface(IID_IPersistStreamInit, reinterpret_cast<void**>(&pPS));
        if (FAILED(hr)) AfxThrowOleException(hr);
        CLSID clsid;
        hr = pPS->GetClassID(&clsid);
        if (FAILED(hr)) { pPS->Release(); AfxThrowOleException(hr); }
        *ar << clsid.Data1;
        *ar << clsid.Data2;
        *ar << clsid.Data3;
        ar->Write(&clsid.Data4[0], sizeof(clsid.Data4));
        CArchiveStream stm(ar);
        hr = pPS->Save(&stm, TRUE);
        pPS->Release();
        if (FAILED(hr)) AfxThrowOleException(hr);
        return ar;
    }
    case VT_EMPTY:
    case VT_NULL:
    default:
        return ar;
    }
}
extern "C" int MS_ABI impl___8COleVariant__QEBAHAEBUtagVARIANT___Z(
    const COleVariant* pThis, const VARIANT* var)
{
    if (!pThis || !var) return FALSE;
    if (static_cast<const VARIANT*>(pThis) == var) return TRUE;
    if (var->vt != static_cast<const VARIANT*>(pThis)->vt) return FALSE;
    switch (var->vt) {
    case VT_EMPTY:
    case VT_NULL:
        return TRUE;
    case VT_BOOL:  return V_BOOL(var) == V_BOOL(pThis);
    case VT_I1:    return var->cVal == static_cast<const VARIANT*>(pThis)->cVal;
    case VT_UI1:   return var->bVal == static_cast<const VARIANT*>(pThis)->bVal;
    case VT_I2:    return var->iVal == static_cast<const VARIANT*>(pThis)->iVal;
    case VT_UI2:   return var->uiVal == static_cast<const VARIANT*>(pThis)->uiVal;
    case VT_I4:    return var->lVal == static_cast<const VARIANT*>(pThis)->lVal;
    case VT_UI4:   return var->ulVal == static_cast<const VARIANT*>(pThis)->ulVal;
    case VT_I8:    return var->llVal == static_cast<const VARIANT*>(pThis)->llVal;
    case VT_UI8:   return var->ullVal == static_cast<const VARIANT*>(pThis)->ullVal;
    case VT_CY:
        return var->cyVal.Hi == static_cast<const VARIANT*>(pThis)->cyVal.Hi &&
               var->cyVal.Lo == static_cast<const VARIANT*>(pThis)->cyVal.Lo;
    case VT_R4:    return var->fltVal == static_cast<const VARIANT*>(pThis)->fltVal;
    case VT_R8:    return var->dblVal == static_cast<const VARIANT*>(pThis)->dblVal;
    case VT_DATE:  return var->date == static_cast<const VARIANT*>(pThis)->date;
    case VT_BSTR:
        return SysStringByteLen(var->bstrVal) == SysStringByteLen(static_cast<const VARIANT*>(pThis)->bstrVal) &&
               memcmp(var->bstrVal, static_cast<const VARIANT*>(pThis)->bstrVal,
                      SysStringByteLen(static_cast<const VARIANT*>(pThis)->bstrVal)) == 0;
    case VT_ERROR:
        return var->scode == static_cast<const VARIANT*>(pThis)->scode;
    case VT_DISPATCH:
    case VT_UNKNOWN:
        return var->punkVal == static_cast<const VARIANT*>(pThis)->punkVal;
    default:
        if ((var->vt & VT_ARRAY) && !(var->vt & VT_BYREF))
            return _AfxCompareSafeArrays_Olecore(var->parray, static_cast<const VARIANT*>(pThis)->parray);
        return FALSE;
    }
}
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_XZ(
    COleDispatchDriver* pThis
) {
    if (!pThis) return nullptr;
    new (pThis) COleDispatchDriver();
    return pThis;
}
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_PEAUIDispatch__H_Z(
    COleDispatchDriver* pThis, LPDISPATCH lpDispatch, BOOL bAutoRelease
) {
    if (!pThis) return nullptr;
    new (pThis) COleDispatchDriver(lpDispatch, bAutoRelease);
    return pThis;
}
extern "C" COleDispatchDriver* MS_ABI impl___0COleDispatchDriver__QEAA_AEBV0__Z(
    COleDispatchDriver* pThis, const COleDispatchDriver* pSrc
) {
    if (!pThis) return nullptr;
    if (!pSrc) return impl___0COleDispatchDriver__QEAA_XZ(pThis);
    const OleDispatchDriverLayout* src = DispatchDriverLayout(pSrc);
    new (pThis) COleDispatchDriver(src->m_lpDispatch, src->m_bAutoRelease);
    OleDispatchDriverLayout* dst = DispatchDriverLayout(pThis);
    if (dst->m_lpDispatch && dst->m_bAutoRelease) {
        dst->m_lpDispatch->AddRef();
    }
    return pThis;
}
extern "C" const COleDispatchDriver* MS_ABI impl___4COleDispatchDriver__QEAAAEBV0_AEBV0__Z(
    COleDispatchDriver* pThis, const COleDispatchDriver* pSrc
) {
    if (!pThis || !pSrc || pThis == pSrc) return pThis;
    const OleDispatchDriverLayout* src = DispatchDriverLayout(pSrc);
    LPDISPATCH srcDispatch = src->m_lpDispatch;
    if (srcDispatch && src->m_bAutoRelease) {
        srcDispatch->AddRef();
    }
    pThis->ReleaseDispatch();
    OleDispatchDriverLayout* dst = DispatchDriverLayout(pThis);
    dst->m_lpDispatch = srcDispatch;
    dst->m_bAutoRelease = src->m_bAutoRelease;
    return pThis;
}
extern "C" void MS_ABI impl__AttachDispatch_COleDispatchDriver__QEAAXPEAUIDispatch__H_Z(
    COleDispatchDriver* pThis, LPDISPATCH lpDispatch, BOOL bAutoRelease
) {
    if (!pThis) return;
    pThis->AttachDispatch(lpDispatch, bAutoRelease);
}
extern "C" LPDISPATCH MS_ABI impl__DetachDispatch_COleDispatchDriver__QEAAPEAUIDispatch__XZ(
    COleDispatchDriver* pThis
) {
    return pThis ? pThis->DetachDispatch() : nullptr;
}
extern "C" void MS_ABI impl__ReleaseDispatch_COleDispatchDriver__QEAAXXZ(
    COleDispatchDriver* pThis
) {
    if (!pThis) return;
    pThis->ReleaseDispatch();
}
extern "C" int MS_ABI impl__CreateDispatch_COleDispatchDriver__QEAAHAEBU_GUID__PEAVCOleException___Z(
    COleDispatchDriver* pThis, REFCLSID clsid, COleException* pError
) {
    return pThis ? pThis->CreateDispatch(clsid, pError) : FALSE;
}
extern "C" int MS_ABI impl__CreateDispatch_COleDispatchDriver__QEAAHPEB_WPEAVCOleException___Z(
    COleDispatchDriver* pThis, const wchar_t* progId, COleException* pError
) {
    return pThis ? pThis->CreateDispatch(progId, pError) : FALSE;
}
extern "C" void MS_ABI impl__InvokeHelper_COleDispatchDriver__QEAAXJGGPEAXPEBEZZ(
    COleDispatchDriver* pThis, DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
    void* pvRet, const BYTE* pbParamInfo, ...
) {
    if (!pThis) return;
    va_list args;
    va_start(args, pbParamInfo);
    pThis->InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
    va_end(args);
}
extern "C" void MS_ABI impl__InvokeHelperV_COleDispatchDriver__QEAAXJGGPEAXPEBEPEAD_Z(
    COleDispatchDriver* pThis, DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
    void* pvRet, const BYTE* pbParamInfo, va_list args
) {
    if (!pThis) return;
    pThis->InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, args);
}
extern "C" void MS_ABI impl__SetProperty_COleDispatchDriver__QEAAXJGZZ(
    COleDispatchDriver* pThis, DISPID dwDispID, VARTYPE vtProp, ...
) {
    if (!pThis) return;
    BYTE params[2] = { static_cast<BYTE>(vtProp), 0 };
    va_list args;
    va_start(args, vtProp);
    pThis->InvokeHelperV(dwDispID, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, params, args);
    va_end(args);
}
extern "C" void MS_ABI impl__GetProperty_COleDispatchDriver__QEBAXJGPEAX_Z(
    const COleDispatchDriver* pThis, DISPID dwDispID, VARTYPE vtProp, void* pvProp
) {
    if (!pThis) return;
    const_cast<COleDispatchDriver*>(pThis)->InvokeHelper(
        dwDispID, DISPATCH_PROPERTYGET, vtProp, pvProp, nullptr
    );
}
extern "C" HRESULT MS_ABI impl__QueryInterface_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAX_Z(
    COleConnPtContainer* pThis, REFIID riid, void** ppv
) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IConnectionPointContainer)) {
        *ppv = pThis;
        impl__AddRef_COleConnPtContainer__UEAAKXZ(pThis);
        return S_OK;
    }
    return E_NOINTERFACE;
}
extern "C" ULONG MS_ABI impl__Release_COleConnPtContainer__UEAAKXZ(
    COleConnPtContainer* pThis
) {
    (void)pThis;
    return 1;
}
extern "C" HRESULT MS_ABI impl__EnumConnectionPoints_COleConnPtContainer__UEAAJPEAPEAUIEnumConnectionPoints___Z(
    COleConnPtContainer* pThis, IEnumConnectionPoints** ppEnum
) {
    (void)pThis;
    if (!ppEnum) return E_POINTER;
    *ppEnum = new EmptyEnumConnectionPoints();
    return *ppEnum ? S_OK : E_OUTOFMEMORY;
}
extern "C" HRESULT MS_ABI impl__FindConnectionPoint_COleConnPtContainer__UEAAJAEBU_GUID__PEAPEAUIConnectionPoint___Z(
    COleConnPtContainer* pThis, REFIID riid, IConnectionPoint** ppCP
) {
    (void)pThis;
    (void)riid;
    if (!ppCP) return E_POINTER;
    *ppCP = nullptr;
    return OLE_E_NOCONNECTION;
}
std::vector<DropTargetState> g_dropTargetStates;
std::vector<DocumentState> g_documentStates;
std::vector<ClientItemState> g_clientItemStates;
std::vector<ServerDocState> g_serverDocStates;
std::vector<ServerItemState> g_serverItemStates;
std::vector<OleControlState> g_oleControlStates;
std::vector<COleObjectFactory*> g_oleObjectFactories;
CString ClsidKey(REFCLSID clsid) {
    wchar_t clsidText[64] = {};
    if (StringFromGUID2(clsid, clsidText, 64) == 0) return CString();
    return CString(clsidText);
}
// The default argument is stated once, on the declaration in the header.
CString FindFactoryLicense(REFCLSID clsid, const CString& fallbackProgId) {
    if (!fallbackProgId.IsEmpty()) {
        auto it = g_oleFactoryLicenseKeys.find(fallbackProgId);
        if (it != g_oleFactoryLicenseKeys.end()) return it->second;
    }

    CString clsidText = ClsidKey(clsid);
    if (!clsidText.IsEmpty()) {
        auto it = g_oleFactoryLicenseKeys.find(clsidText);
        if (it != g_oleFactoryLicenseKeys.end()) return it->second;
    }
    return CString();
}
BSTR AllocateLicenseBstr(const CString& value) {
    return value.IsEmpty() ? nullptr : ::SysAllocString(value.GetString());
}
DropTargetState* GetDropTargetState(COleDropTarget* target, bool create) {
    if (!target) return nullptr;
    for (DropTargetState& state : g_dropTargetStates) {
        if (state.target == target) return &state;
    }
    if (!create) return nullptr;
    g_dropTargetStates.push_back(DropTargetState{target, nullptr});
    return &g_dropTargetStates.back();
}
void RemoveDropTargetState(COleDropTarget* target) {
    g_dropTargetStates.erase(
        std::remove_if(g_dropTargetStates.begin(), g_dropTargetStates.end(),
                       [target](const DropTargetState& state) { return state.target == target; }),
        g_dropTargetStates.end());
}
DocumentState* GetDocumentState(COleDocument* document, bool create) {
    if (!document) return nullptr;
    for (DocumentState& state : g_documentStates) {
        if (state.document == document) return &state;
    }
    if (!create) return nullptr;
    g_documentStates.push_back(DocumentState());
    g_documentStates.back().document = document;
    return &g_documentStates.back();
}
void RemoveDocumentState(COleDocument* document) {
    g_documentStates.erase(
        std::remove_if(g_documentStates.begin(), g_documentStates.end(),
                       [document](const DocumentState& state) { return state.document == document; }),
        g_documentStates.end());
}
ClientItemState* GetClientItemState(COleClientItem* item, bool create) {
    if (!item) return nullptr;
    for (ClientItemState& state : g_clientItemStates) {
        if (state.item == item) return &state;
    }
    if (!create) return nullptr;
    g_clientItemStates.push_back(ClientItemState());
    g_clientItemStates.back().item = item;
    return &g_clientItemStates.back();
}
ClientItemState* FindClientItemState(const COleClientItem* item) {
    for (ClientItemState& state : g_clientItemStates) {
        if (state.item == item) return &state;
    }
    return nullptr;
}
void RemoveClientItemState(COleClientItem* item) {
    g_clientItemStates.erase(
        std::remove_if(g_clientItemStates.begin(), g_clientItemStates.end(),
                       [item](const ClientItemState& state) { return state.item == item; }),
        g_clientItemStates.end());
}
void AddOleObjectFactory(COleObjectFactory* factory) {
    if (!factory) return;
    if (std::find(g_oleObjectFactories.begin(), g_oleObjectFactories.end(), factory) == g_oleObjectFactories.end()) {
        g_oleObjectFactories.push_back(factory);
    }
}
void RemoveOleObjectFactory(COleObjectFactory* factory) {
    g_oleObjectFactories.erase(std::remove(g_oleObjectFactories.begin(), g_oleObjectFactories.end(), factory),
                               g_oleObjectFactories.end());
}
ServerDocState* GetServerDocState(COleServerDoc* document, bool create) {
    if (!document) return nullptr;
    for (ServerDocState& state : g_serverDocStates) {
        if (state.document == document) return &state;
    }
    if (!create) return nullptr;
    g_serverDocStates.push_back(ServerDocState());
    g_serverDocStates.back().document = document;
    return &g_serverDocStates.back();
}
void RemoveServerDocState(COleServerDoc* document) {
    g_serverDocStates.erase(
        std::remove_if(g_serverDocStates.begin(), g_serverDocStates.end(),
                       [document](const ServerDocState& state) { return state.document == document; }),
        g_serverDocStates.end());
}
ServerItemState* GetServerItemState(COleServerItem* item, bool create) {
    if (!item) return nullptr;
    for (ServerItemState& state : g_serverItemStates) {
        if (state.item == item) return &state;
    }
    if (!create) return nullptr;
    g_serverItemStates.push_back(ServerItemState());
    g_serverItemStates.back().item = item;
    return &g_serverItemStates.back();
}
void RemoveServerItemState(COleServerItem* item) {
    g_serverItemStates.erase(
        std::remove_if(g_serverItemStates.begin(), g_serverItemStates.end(),
                       [item](const ServerItemState& state) { return state.item == item; }),
        g_serverItemStates.end());
}
BOOL EnsureLinkingDocMoniker(COleLinkingDoc* document, const wchar_t* fileName, BOOL setModified) {
    if (!document) return FALSE;

    const wchar_t* monikerPath = fileName;
    if (!monikerPath || !*monikerPath) {
        monikerPath = document->GetPathName();
    }
    if (!monikerPath || !*monikerPath) return FALSE;

    if (document->m_lpMoniker) {
        document->m_lpMoniker->Release();
        document->m_lpMoniker = nullptr;
    }

    LPMONIKER moniker = nullptr;
    HRESULT hr = CreateFileMoniker(monikerPath, &moniker);
    if (FAILED(hr) || !moniker) {
        document->m_bRegistered = FALSE;
        return FALSE;
    }

    document->m_lpMoniker = moniker;
    document->m_bRegistered = TRUE;
    if (setModified) document->SetModifiedFlag(TRUE);
    return TRUE;
}
OleControlState* GetOleControlState(COleControl* control, bool create) {
    if (!control) return nullptr;
    for (OleControlState& state : g_oleControlStates) {
        if (state.control == control) return &state;
    }
    if (!create) return nullptr;
    g_oleControlStates.push_back(OleControlState());
    g_oleControlStates.back().control = control;
    return &g_oleControlStates.back();
}
COleControl* GetControlFromDataSource(const COleControl::CControlDataSource* pSource) {
    if (!pSource) return nullptr;
    for (OleControlState& state : g_oleControlStates) {
        if (state.control && state.control->m_pDataSource == pSource) return state.control;
    }
    return nullptr;
}
OleControlDataSourceCacheEntry* FindControlDataSourceEntry(OleControlState* state,
                                                                const FORMATETC& format,
                                                                bool create) {
    if (!state) return nullptr;
    for (OleControlDataSourceCacheEntry& entry : state->dataSourceEntries) {
        if (FormatMatches(entry.format, format)) return &entry;
    }
    if (!create) return nullptr;
    state->dataSourceEntries.push_back(OleControlDataSourceCacheEntry{format});
    return &state->dataSourceEntries.back();
}
OleControlDataSourceCacheEntry* FindControlDataSourceEntry(COleControl* control,
                                                                const FORMATETC& format,
                                                                bool create) {
    OleControlState* state = GetOleControlState(control, create);
    return FindControlDataSourceEntry(state, format, create);
}
void ReleaseDataSourceEntryMedium(OleControlDataSourceCacheEntry& entry) {
    if (!entry.hasMedium) return;
    ReleaseStgMedium(&entry.medium);
    entry.hasMedium = false;
}
COleControlSite* MfcSiteOf(const COleControl* control) {
    OleControlState* state = GetOleControlState(const_cast<COleControl*>(control), false);
    return state ? state->mfcSite : nullptr;
}
void RemoveOleControlState(COleControl* control) {
    auto it = std::remove_if(g_oleControlStates.begin(), g_oleControlStates.end(),
                             [control](const OleControlState& state) { return state.control == control; });
    for (auto releaseIt = it; releaseIt != g_oleControlStates.end(); ++releaseIt) {
        for (auto& sink : releaseIt->eventSinks) {
            if (sink.sink) sink.sink->Release();
        }
        for (auto* sink : releaseIt->propSinks) {
            if (sink) sink->Release();
        }
        for (OleControlDataSourceCacheEntry& entry : releaseIt->dataSourceEntries) {
            ReleaseDataSourceEntryMedium(entry);
        }
    }
    g_oleControlStates.erase(it, g_oleControlStates.end());
}
void AddDocumentItem(COleDocument* document, COleClientItem* item) {
    DocumentState* state = GetDocumentState(document, true);
    if (!state || !item) return;
    if (std::find(state->items.begin(), state->items.end(), item) == state->items.end()) {
        state->items.push_back(item);
    }
    item->m_pContainerDoc = document;
    item->m_pDocument = document;
}
void RemoveDocumentItem(COleDocument* document, COleClientItem* item) {
    DocumentState* state = GetDocumentState(document, false);
    if (!state || !item) return;
    state->items.erase(std::remove(state->items.begin(), state->items.end(), item), state->items.end());
    if (item->m_pContainerDoc == document) item->m_pContainerDoc = nullptr;
    if (item->m_pDocument == document) item->m_pDocument = nullptr;
}
void AddServerDocItem(COleServerDoc* document, COleServerItem* item) {
    ServerDocState* state = GetServerDocState(document, true);
    if (!state || !item) return;
    if (std::find(state->items.begin(), state->items.end(), item) == state->items.end()) {
        state->items.push_back(item);
    }
    item->m_pServerDoc = document;
    item->m_pDocument = document;
}
void RemoveServerDocItem(COleServerDoc* document, COleServerItem* item) {
    ServerDocState* state = GetServerDocState(document, false);
    if (!state || !item) return;
    state->items.erase(std::remove(state->items.begin(), state->items.end(), item), state->items.end());
    if (item->m_pServerDoc == document) item->m_pServerDoc = nullptr;
    if (item->m_pDocument == document) item->m_pDocument = nullptr;
}
size_t ParseLinkedItemIndex(const wchar_t* itemName) {
    if (!itemName || !*itemName) return 0;
    wchar_t* end = nullptr;
    unsigned long value = std::wcstoul(itemName, &end, 10);
    if (!end || *end != L'\0' || value == 0) return 0;
    return static_cast<size_t>(value);
}
DataCacheState* GetDataCacheState(COleDataSource* source, bool create) {
    if (!source) return nullptr;
    DataCacheState* state = static_cast<DataCacheState*>(source->m_pDataCache);
    if (!state && create) {
        state = new DataCacheState(source);
        source->m_pDataCache = state;
    }
    return state;
}
DataCacheEntry* FindCacheEntry(DataCacheState* state, const FORMATETC& format) {
    if (!state) return nullptr;
    for (DataCacheEntry& entry : state->entries) {
        if (FormatMatches(entry.format, format)) return &entry;
    }
    return nullptr;
}
thread_local OleMessageFilterAdapter* g_messageFilterAdapter = nullptr;
thread_local IMessageFilter* g_previousMessageFilter = nullptr;
STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::AddRef() {
    return InterlockedIncrement(&m_refCount);
}
STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) m_refCount = 1;
    return ref;
}
STDMETHODIMP_(ULONG) COleDropSource::XDropSource::AddRef() {
    return InterlockedIncrement(&m_refCount);
}
STDMETHODIMP_(ULONG) COleDropSource::XDropSource::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) m_refCount = 1;
    return ref;
}
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::AddRef() {
    return InterlockedIncrement(&m_refCount);
}
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) m_refCount = 1;
    return ref;
}
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::HandleInComingCall(
    DWORD dwCallType, HTASK htaskCaller, DWORD dwTickCount, LPINTERFACEINFO lpInterfaceInfo) {
    (void)dwCallType; (void)htaskCaller; (void)dwTickCount; (void)lpInterfaceInfo;
    return SERVERCALL_ISHANDLED;
}
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::RetryRejectedCall(
    HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType) {
    (void)htaskCallee; (void)dwTickCount;
    if (!m_pMessageFilter || !m_pMessageFilter->m_bEnableBusy) return (ULONG)-1;
    if (m_pMessageFilter->m_nRetryReply != 0) return m_pMessageFilter->m_nRetryReply;
    return dwRejectType == SERVERCALL_RETRYLATER ? 250 : (ULONG)-1;
}
STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::MessagePending(
    HTASK htaskCallee, DWORD dwTickCount, DWORD dwType) {
    (void)htaskCallee; (void)dwTickCount; (void)dwType;
    MSG msg;
    if (m_pMessageFilter && ::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (m_pMessageFilter->IsSignificantMessage(&msg)) return PENDINGMSG_WAITDEFPROCESS;
        if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) return PENDINGMSG_CANCELCALL;
        ::DispatchMessageW(&msg);
    }
    return PENDINGMSG_WAITDEFPROCESS;
}
STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::AddRef() {
    return InterlockedIncrement(&m_refCount);
}
STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::Release() {
    ULONG ref = InterlockedDecrement(&m_refCount);
    if (ref == 0) {
        // In real MFC, factory objects are typically global and never deleted.
        // We reset the ref count to prevent re-deletion.
        m_refCount = 1;
    }
    return ref;
}
HWND GetSiteParentWindow(const COleControlSite* pSite) {
    if (!pSite) {
        return nullptr;
    }
    if (pSite->m_pCtrlCont && pSite->m_pCtrlCont->GetWnd()) {
        HWND hwnd = pSite->m_pCtrlCont->GetWnd()->GetSafeHwnd();
        if (hwnd) {
            return hwnd;
        }
    }
    if (pSite->m_hWnd) {
        HWND hwndParent = ::GetParent(pSite->m_hWnd);
        return hwndParent ? hwndParent : pSite->m_hWnd;
    }
    return nullptr;
}
RECT GetSitePositionRect(const COleControlSite* pSite) {
    RECT rc = {};
    HWND hwndParent = GetSiteParentWindow(pSite);
    if (pSite && pSite->m_hWnd) {
        ::GetWindowRect(pSite->m_hWnd, &rc);
        if (hwndParent) {
            ::MapWindowPoints(nullptr, hwndParent, reinterpret_cast<POINT*>(&rc), 2);
        }
        return rc;
    }
    if (hwndParent) {
        ::GetClientRect(hwndParent, &rc);
    }
    return rc;
}
void SetVariantBool(VARIANT* pVar, BOOL value) {
    VariantInit(pVar);
    pVar->vt = VT_BOOL;
    pVar->boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
}
BOOL TryGetAmbientOverride(COleControlSite* pSite, DISPID dispid, VARIANT* pVarResult) {
    if (!pSite || !pVarResult || !pSite->m_pCtrlCont || !pSite->m_pCtrlCont->GetWnd()) {
        return FALSE;
    }
    VariantInit(pVarResult);
    CWnd* pWnd = pSite->m_pCtrlCont->GetWnd();
    if (impl__OnAmbientProperty_CWnd__UEAAHPEAVCOleControlSite__JPEAUtagVARIANT___Z(
            pWnd, pSite, dispid, pVarResult)) {
        return TRUE;
    }
    VariantClear(pVarResult);
    return FALSE;
}
std::map<const COleControlSite*, OleControlSiteState> g_oleControlSiteState;
void SetControlSiteId(const COleControlSite* pSite, UINT controlId) {
    if (!pSite) {
        return;
    }
    g_oleControlSiteState[pSite].controlId = controlId;
}
void RemoveControlSiteState(const COleControlSite* pSite) {
    auto it = g_oleControlSiteState.find(pSite);
    if (it != g_oleControlSiteState.end()) {
        if (it->second.adapter) {
            it->second.adapter->DetachSite();
            it->second.adapter->Release();
        }
        g_oleControlSiteState.erase(it);
    }
}
BOOL TryGetControlSiteId(const COleControlSite* pSite, UINT* pControlId) {
    if (!pSite || !pControlId) {
        return FALSE;
    }
    auto it = g_oleControlSiteState.find(pSite);
    if (it == g_oleControlSiteState.end()) {
        return FALSE;
    }
    *pControlId = it->second.controlId;
    return TRUE;
}
ControlSiteAdapter* GetControlSiteAdapter(COleControlSite* pSite, bool create) {
    if (!pSite) {
        return nullptr;
    }
    auto it = g_oleControlSiteState.find(pSite);
    if (it == g_oleControlSiteState.end()) {
        if (!create) {
            return nullptr;
        }
        it = g_oleControlSiteState.emplace(pSite, OleControlSiteState{}).first;
    }
    if (!it->second.adapter && create) {
        it->second.adapter = new(std::nothrow) ControlSiteAdapter(pSite);
    }
    return it->second.adapter;
}
STDMETHODIMP_(ULONG) CEnumFormatEtc::AddRef() { return ++m_refCount; }
STDMETHODIMP_(ULONG) CEnumFormatEtc::Release() {
    ULONG ref = --m_refCount;
    if (ref == 0) delete this;
    return ref;
}
} } }  // namespace openmfc::detail::olecore
