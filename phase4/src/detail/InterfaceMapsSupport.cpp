#define OPENMFC_APPCORE_IMPL
#include "InterfaceMapsSupport.h"
#include "openmfc/afxole.h"

namespace openmfc { namespace detail { namespace interfacemaps {
const AFX_INTERFACEMAP* AFXAPI gib_CDocObjectServer() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_CDocObjectServer = { gib_CDocObjectServer, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_CEnumConnPoints() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_CEnumConnPoints = { gib_CEnumConnPoints, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_CEnumConnections() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_CEnumConnections = { gib_CEnumConnections, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_CEnumUnknown() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_CEnumUnknown = { gib_CEnumUnknown, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_CHtmlControlSite() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_CHtmlControlSite = { gib_CHtmlControlSite, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_CMFCBaseAccessibleObject() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_CMFCBaseAccessibleObject = { gib_CMFCBaseAccessibleObject, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleClientItem() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
namespace {
constexpr size_t itemTail = offsetof(COleClientItem, m_retailTail);
constexpr size_t clientSite = itemTail + offsetof(COleClientItem::RetailTailStorage, m_xOleClientSite);
constexpr size_t adviseSink = itemTail + offsetof(COleClientItem::RetailTailStorage, m_xAdviseSink);
constexpr size_t inPlaceSite = itemTail + offsetof(COleClientItem::RetailTailStorage, m_xOleIPSite);
static_assert(clientSite == 0xd8 && adviseSink == 0xe0 && inPlaceSite == 0xe8);
const AFX_INTERFACEMAP_ENTRY clientItemInterfaces[] = {
    { &IID_IOleClientSite, clientSite },
    { &IID_IAdviseSink, adviseSink },
    { &IID_IOleWindow, inPlaceSite },
    { &IID_IOleInPlaceSite, inPlaceSite },
    { nullptr, static_cast<size_t>(-1) }
};
}
const AFX_INTERFACEMAP g_imap_COleClientItem = { gib_COleClientItem, clientItemInterfaces };
const AFX_INTERFACEMAP* AFXAPI gib_COleControl() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleControl = { gib_COleControl, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleControlContainer() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleControlContainer = { gib_COleControlContainer, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleControlSite() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleControlSite = { gib_COleControlSite, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleDocObjectItem() { return impl__GetThisInterfaceMap_COleClientItem__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleDocObjectItem = { gib_COleDocObjectItem, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleFrameHook() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
namespace {
constexpr size_t framePart = offsetof(COleFrameHook, m_retailTail) +
                             offsetof(COleFrameHook::RetailTailStorage, m_xOleInPlaceFrame);
constexpr size_t commandPart = offsetof(COleFrameHook, m_retailTail) +
                               offsetof(COleFrameHook::RetailTailStorage, m_xOleCommandTarget);
const AFX_INTERFACEMAP_ENTRY frameHookInterfaces[] = {
    { &IID_IOleWindow, framePart },
    { &IID_IOleInPlaceUIWindow, framePart },
    { &IID_IOleInPlaceFrame, framePart },
    { &IID_IOleCommandTarget, commandPart },
    { nullptr, static_cast<size_t>(-1) }
};
}
const AFX_INTERFACEMAP g_imap_COleFrameHook = { gib_COleFrameHook, frameHookInterfaces };
const AFX_INTERFACEMAP* AFXAPI gib_COleLinkingDoc() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
namespace {
constexpr size_t persistFile = offsetof(COleLinkingDoc, m_xPersistFile);
constexpr size_t itemContainer = offsetof(COleLinkingDoc, m_xOleItemContainer);
static_assert(persistFile == 0x220 && itemContainer == 0x228);
const AFX_INTERFACEMAP_ENTRY linkingDocInterfaces[] = {
    { &IID_IPersistFile, persistFile },
    { &IID_IPersist, persistFile },
    { &IID_IParseDisplayName, itemContainer },
    { &IID_IOleContainer, itemContainer },
    { &IID_IOleItemContainer, itemContainer },
    { nullptr, static_cast<size_t>(-1) }
};
}
const AFX_INTERFACEMAP g_imap_COleLinkingDoc = { gib_COleLinkingDoc, linkingDocInterfaces };
const AFX_INTERFACEMAP* AFXAPI gib_COleMessageFilter() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleMessageFilter = { gib_COleMessageFilter, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleObjectFactory() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleObjectFactory = { gib_COleObjectFactory, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COlePropertyPage() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COlePropertyPage = { gib_COlePropertyPage, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleServerDoc() { return impl__GetThisInterfaceMap_COleLinkingDoc__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleServerDoc = { gib_COleServerDoc, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_COleServerItem() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_COleServerItem = { gib_COleServerItem, g_ifaceEnd };
const AFX_INTERFACEMAP* AFXAPI gib_CRichEditView() { return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ(); }
const AFX_INTERFACEMAP g_imap_CRichEditView = { gib_CRichEditView, g_ifaceEnd };
} } }  // namespace openmfc::detail::interfacemaps
