#pragma once

#include <cstddef>
#include "openmfc/afxole.h"

class CDocObjectServer;
extern "C" long __cdecl impl__OnActivateView_CDocObjectServer__MEAAJXZ(void*);

namespace openmfc::detail::olecore {
HRESULT QueryDocObjectServer(CDocObjectServer*, bool queryOwner, REFIID, void**);
ULONG AddRefDocObjectServer(CDocObjectServer*);
ULONG ReleaseDocObjectServer(CDocObjectServer*);

template<class Interface, size_t Offset, bool QueryOwner = false>
struct DocObjectServerPart : Interface {
    CDocObjectServer* Server() {
        return reinterpret_cast<CDocObjectServer*>(reinterpret_cast<BYTE*>(this) - Offset);
    }
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** result) override {
        return QueryDocObjectServer(Server(), QueryOwner, iid, result);
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return AddRefDocObjectServer(Server()); }
    ULONG STDMETHODCALLTYPE Release() override { return ReleaseDocObjectServer(Server()); }
};

} // namespace openmfc::detail::olecore

class CDocObjectServer : public CCmdTarget {
public:
    explicit CDocObjectServer(COleServerDoc* owner, IOleDocumentSite* site = nullptr);
    // A successful factory transfers ownership of the document-site reference.
    void SetDocSite(IOleDocumentSite* site);
    void ReleaseDocSite();
    void ActivateDocObject();
    HRESULT RequestActivation();
    ~CDocObjectServer() override;
    CRuntimeClass* GetRuntimeClass() const override;

    IOleDocumentSite* m_pDocSite;
    COleServerDoc* m_pOwner;
    LONG m_nFirstPage = -1;
    IContinueCallback* m_pContinueCallback = nullptr;
    IOleInPlaceSite* m_pViewSite = nullptr;

    struct XOleObject : openmfc::detail::olecore::DocObjectServerPart<IOleObject, 0x68, true> {
        HRESULT STDMETHODCALLTYPE SetClientSite(IOleClientSite*) override;
        HRESULT STDMETHODCALLTYPE GetClientSite(IOleClientSite**) override;
        HRESULT STDMETHODCALLTYPE SetHostNames(LPCOLESTR, LPCOLESTR) override;
        HRESULT STDMETHODCALLTYPE Close(DWORD) override;
        HRESULT STDMETHODCALLTYPE SetMoniker(DWORD, IMoniker*) override;
        HRESULT STDMETHODCALLTYPE GetMoniker(DWORD, DWORD, IMoniker**) override;
        HRESULT STDMETHODCALLTYPE InitFromData(IDataObject*, BOOL, DWORD) override;
        HRESULT STDMETHODCALLTYPE GetClipboardData(DWORD, IDataObject**) override;
        HRESULT STDMETHODCALLTYPE DoVerb(LONG, MSG*, IOleClientSite*, LONG, HWND, LPCRECT) override;
        HRESULT STDMETHODCALLTYPE EnumVerbs(IEnumOLEVERB**) override;
        HRESULT STDMETHODCALLTYPE Update() override;
        HRESULT STDMETHODCALLTYPE IsUpToDate() override;
        HRESULT STDMETHODCALLTYPE GetUserClassID(CLSID*) override;
        HRESULT STDMETHODCALLTYPE GetUserType(DWORD, LPOLESTR*) override;
        HRESULT STDMETHODCALLTYPE SetExtent(DWORD, SIZEL*) override;
        HRESULT STDMETHODCALLTYPE GetExtent(DWORD, SIZEL*) override;
        HRESULT STDMETHODCALLTYPE Advise(IAdviseSink*, DWORD*) override;
        HRESULT STDMETHODCALLTYPE Unadvise(DWORD) override;
        HRESULT STDMETHODCALLTYPE EnumAdvise(IEnumSTATDATA**) override;
        HRESULT STDMETHODCALLTYPE GetMiscStatus(DWORD, DWORD*) override;
        HRESULT STDMETHODCALLTYPE SetColorScheme(LOGPALETTE*) override;
    } m_xOleObject;

    struct XOleDocument : openmfc::detail::olecore::DocObjectServerPart<IOleDocument, 0x70> {
        HRESULT STDMETHODCALLTYPE CreateView(IOleInPlaceSite*, IStream*, DWORD, IOleDocumentView**) override;
        HRESULT STDMETHODCALLTYPE GetDocMiscStatus(DWORD*) override;
        HRESULT STDMETHODCALLTYPE EnumViews(IEnumOleDocumentViews**, IOleDocumentView**) override;
    } m_xOleDocument;

    struct XOleDocumentView : openmfc::detail::olecore::DocObjectServerPart<IOleDocumentView, 0x78, true> {
        HRESULT STDMETHODCALLTYPE SetInPlaceSite(IOleInPlaceSite*) override;
        HRESULT STDMETHODCALLTYPE GetInPlaceSite(IOleInPlaceSite**) override;
        HRESULT STDMETHODCALLTYPE GetDocument(IUnknown**) override;
        HRESULT STDMETHODCALLTYPE SetRect(RECT*) override;
        HRESULT STDMETHODCALLTYPE GetRect(RECT*) override;
        HRESULT STDMETHODCALLTYPE SetRectComplex(RECT*, RECT*, RECT*, RECT*) override;
        HRESULT STDMETHODCALLTYPE Show(BOOL) override;
        HRESULT STDMETHODCALLTYPE UIActivate(BOOL) override;
        HRESULT STDMETHODCALLTYPE Open() override;
        HRESULT STDMETHODCALLTYPE CloseView(DWORD) override;
        HRESULT STDMETHODCALLTYPE SaveViewState(IStream*) override;
        HRESULT STDMETHODCALLTYPE ApplyViewState(IStream*) override;
        HRESULT STDMETHODCALLTYPE Clone(IOleInPlaceSite*, IOleDocumentView**) override;
    } m_xOleDocumentView;

    struct XOleCommandTarget : openmfc::detail::olecore::DocObjectServerPart<IOleCommandTarget, 0x80, true> {
        HRESULT STDMETHODCALLTYPE QueryStatus(const GUID*, ULONG, OLECMD*, OLECMDTEXT*) override;
        HRESULT STDMETHODCALLTYPE Exec(const GUID*, DWORD, DWORD, VARIANT*, VARIANT*) override;
    } m_xOleCommandTarget;

    struct XPrint : openmfc::detail::olecore::DocObjectServerPart<IPrint, 0x88> {
        HRESULT STDMETHODCALLTYPE SetInitialPageNum(LONG) override;
        HRESULT STDMETHODCALLTYPE GetPageInfo(LONG*, LONG*) override;
        HRESULT STDMETHODCALLTYPE Print(DWORD, DVTARGETDEVICE**, PAGESET**, STGMEDIUM*, IContinueCallback*, LONG, LONG*, LONG*) override;
    } m_xPrint;

protected:
    friend long __cdecl impl__OnActivateView_CDocObjectServer__MEAAJXZ(void*);
    virtual HRESULT OnActivateView();
    const AFX_INTERFACEMAP* GetInterfaceMap() const override;
};

static_assert(sizeof(CDocObjectServer) == 0x90);
static_assert(offsetof(CDocObjectServer, m_pDocSite) == 0x40);
static_assert(offsetof(CDocObjectServer, m_pOwner) == 0x48);
static_assert(offsetof(CDocObjectServer, m_nFirstPage) == 0x50);
static_assert(offsetof(CDocObjectServer, m_pContinueCallback) == 0x58);
static_assert(offsetof(CDocObjectServer, m_pViewSite) == 0x60);
static_assert(offsetof(CDocObjectServer, m_xOleObject) == 0x68);
static_assert(offsetof(CDocObjectServer, m_xOleDocument) == 0x70);
static_assert(offsetof(CDocObjectServer, m_xOleDocumentView) == 0x78);
static_assert(offsetof(CDocObjectServer, m_xOleCommandTarget) == 0x80);
static_assert(offsetof(CDocObjectServer, m_xPrint) == 0x88);
