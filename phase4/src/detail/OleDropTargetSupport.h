#pragma once
#include "openmfc/afxole.h"
namespace openmfc { namespace detail {
struct OleDropTargetAccess {
    static HWND& Window(COleDropTarget* p){return p->m_hWnd;}
    static IDataObject*& Data(COleDropTarget* p){return p->m_lpDataObject;}
    static UINT& Timer(COleDropTarget* p){return p->m_nTimerID;}
    static DWORD& Tick(COleDropTarget* p){return p->m_dwLastTick;}
    static UINT& Delay(COleDropTarget* p){return p->m_nScrollDelay;}
    static UINT& ScrollDelay(){return COleDropTarget::nScrollDelay;}
    static UINT& ScrollInterval(){return COleDropTarget::nScrollInterval;}
    static int& ScrollInset(){return COleDropTarget::nScrollInset;}
    static_assert(offsetof(COleDropTarget,m_hWnd)==0x40 && offsetof(COleDropTarget,m_lpDataObject)==0x48 &&
                  offsetof(COleDropTarget,m_nTimerID)==0x50 && offsetof(COleDropTarget,m_dwLastTick)==0x54 &&
                  offsetof(COleDropTarget,m_nScrollDelay)==0x58,"native drop target fields");
};
struct OleDataObjectAccess {
    static BOOL& AutoRelease(COleDataObject* p){return p->m_bAutoRelease;}
    static_assert(offsetof(COleDataObject,m_bAutoRelease)==20,"native data wrapper ownership flag");
};
}}
