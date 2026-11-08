#pragma once
#include "openmfc/afxole.h"
namespace openmfc { namespace detail {
// Preserve the SDK's protected API while checking its physical ABI and allowing
// focused fixtures to inspect the actual C++ fields without derived casts.
struct OleDropSourceAccess {
    static CRect& Rectangle(COleDropSource* self) {return self->m_rectStartDrag;}
    static BOOL& Started(COleDropSource* self) {return self->m_bDragStarted;}
    static DWORD& Cancel(COleDropSource* self) {return self->m_dwButtonCancel;}
    static DWORD& Drop(COleDropSource* self) {return self->m_dwButtonDrop;}
    static UINT& Delay() {return COleDropSource::nDragDelay;}
    static UINT& Distance() {return COleDropSource::nDragMinDist;}
    static_assert(offsetof(COleDropSource,m_rectStartDrag)==0x48 && offsetof(COleDropSource,m_bDragStarted)==0x58 &&
                  offsetof(COleDropSource,m_dwButtonCancel)==0x5c && offsetof(COleDropSource,m_dwButtonDrop)==0x60,
                  "native drag source protected fields");
};
}}
