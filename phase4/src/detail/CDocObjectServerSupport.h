#pragma once

#include <cstddef>
#include "openmfc/afxole.h"

// CDocObjectServer has no public OpenMFC class declaration. This ABI shadow is
// deliberately a POD view: the object is allocated by the retail-facing thunk,
// and its constructor establishes these fields at the fixed offsets below.
// Evidence for the member stores is the function entry RVA 0x256ce0 (mfc140u).
namespace openmfc { namespace detail { namespace olecore {
struct RetailCDocObjectServerLayout {
    BYTE m_ccmdTargetPrefix[0x40];
    IOleDocumentSite* m_pDocSite;           // +0x40; RVA 0x256ce0 (mfc140u)
    COleServerDoc* m_pOwner;               // +0x48; RVA 0x256ce0 (mfc140u)
    LONG m_nFirstPage;                     // +0x50; RVA 0x256ce0 (mfc140u)
    void* m_pContinueCallback;             // +0x58; retail IContinueCallback*
    IOleInPlaceSite* m_pViewSite;           // +0x60; RVA 0x256ce0 (mfc140u)
    void* m_xOleObjectVftable;              // +0x68; RVA 0x256ce0 (mfc140u)
    void* m_xOleDocumentVftable;            // +0x70; RVA 0x256ce0 (mfc140u)
    void* m_xOleDocumentViewVftable;        // +0x78; RVA 0x256ce0 (mfc140u)
    void* m_xOleCommandTargetVftable;       // +0x80; RVA 0x256ce0 (mfc140u)
    void* m_xPrintVftable;                  // +0x88; RVA 0x256ce0 (mfc140u)
};

#define ASSERT_DOC_OBJECT_SERVER(member, offset) \
    static_assert(offsetof(RetailCDocObjectServerLayout, member) == (offset), \
                  "RetailCDocObjectServerLayout::" #member)
ASSERT_DOC_OBJECT_SERVER(m_ccmdTargetPrefix, 0x00);
ASSERT_DOC_OBJECT_SERVER(m_pDocSite, 0x40);
ASSERT_DOC_OBJECT_SERVER(m_pOwner, 0x48);
ASSERT_DOC_OBJECT_SERVER(m_nFirstPage, 0x50);
ASSERT_DOC_OBJECT_SERVER(m_pContinueCallback, 0x58);
ASSERT_DOC_OBJECT_SERVER(m_pViewSite, 0x60);
ASSERT_DOC_OBJECT_SERVER(m_xOleObjectVftable, 0x68);
ASSERT_DOC_OBJECT_SERVER(m_xOleDocumentVftable, 0x70);
ASSERT_DOC_OBJECT_SERVER(m_xOleDocumentViewVftable, 0x78);
ASSERT_DOC_OBJECT_SERVER(m_xOleCommandTargetVftable, 0x80);
ASSERT_DOC_OBJECT_SERVER(m_xPrintVftable, 0x88);
#undef ASSERT_DOC_OBJECT_SERVER
static_assert(sizeof(RetailCDocObjectServerLayout) == 0x90,
              "retail CDocObjectServer object size");
} } }
