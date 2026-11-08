// CGdiObject — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/GdiObjectHandleMapSupport.h"

// CGdiObject::DeleteTempMap (static)
// Symbol: ?DeleteTempMap@CGdiObject@@SAXXZ
// Called during idle processing to clean up temporary GDI object wrappers
extern "C" HGDIOBJ MS_ABI impl__Detach_CGdiObject__QEAAPEAXXZ(CGdiObject*);
extern "C" void MS_ABI impl__DeleteTempMap_CGdiObject__SAXXZ() {
    auto* map = openmfc::detail::GetGdiObjectHandleMap(false);
    if (map) map->DeleteTemp();
}
// CGdiObject::FromHandle
// Symbol: ?FromHandle@CGdiObject@@SAPEAV1@PEAX@Z
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject) {
    auto* map = openmfc::detail::GetGdiObjectHandleMap(true);
    return reinterpret_cast<CGdiObject*>(map->FromHandle(hObject));
}
// CGdiObject::DeleteObject
// Symbol: ?DeleteObject@CGdiObject@@QEAAHXZ
extern "C" int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis) {
    if (!pThis || !pThis->m_hObject) return FALSE;
    return ::DeleteObject(impl__Detach_CGdiObject__QEAAPEAXXZ(pThis));
}
// CGdiObject::Attach
// Symbol: ?Attach@CGdiObject@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject) {
    if (!pThis || !hObject) return FALSE;
    pThis->m_hObject = hObject;
    auto* map = openmfc::detail::GetGdiObjectHandleMap(true);
    // Native reloads the member after obtaining the map; callbacks may have run in between.
    map->m_permanentMap[pThis->m_hObject] = pThis;
    return TRUE;
}
// CGdiObject::Detach
// Symbol: ?Detach@CGdiObject@@QEAAPEAXXZ
extern "C" HGDIOBJ MS_ABI impl__Detach_CGdiObject__QEAAPEAXXZ(CGdiObject* pThis) {
    if (!pThis) return nullptr;
    HGDIOBJ h = pThis->m_hObject;
    if (h) {
        auto* map = openmfc::detail::GetGdiObjectHandleMap(false);
        // Detach returns the earlier snapshot, but native removes by the post-getter member value.
        if (map) map->m_permanentMap.RemoveKey(pThis->m_hObject);
    }
    pThis->m_hObject = nullptr;
    return h;
}

// C++ consumers and destructors share the same maps as imported ABI calls.
int CGdiObject::DeleteObject() { return impl__DeleteObject_CGdiObject__QEAAHXZ(this); }
int CGdiObject::Attach(HGDIOBJ handle) { return impl__Attach_CGdiObject__QEAAHPEAX_Z(this,handle); }
HGDIOBJ CGdiObject::Detach() { return impl__Detach_CGdiObject__QEAAPEAXXZ(this); }
CGdiObject* CGdiObject::FromHandle(HGDIOBJ handle) { return impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(handle); }
void CGdiObject::DeleteTempMap() { impl__DeleteTempMap_CGdiObject__SAXXZ(); }
