// CGdiObject — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// CGdiObject::DeleteTempMap (static)
// Symbol: ?DeleteTempMap@CGdiObject@@SAXXZ
// Called during idle processing to clean up temporary GDI object wrappers
extern "C" void MS_ABI impl__DeleteTempMap_CGdiObject__SAXXZ() {
    DeleteTempGdiMap();
}
// CGdiObject::FromHandle
// Symbol: ?FromHandle@CGdiObject@@SAPEAV1@PEAX@Z
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject) {
    return GetTempGdiObject(hObject);
}
// CGdiObject::DeleteObject
// Symbol: ?DeleteObject@CGdiObject@@QEAAHXZ
extern "C" int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis) {
    if (!pThis || !pThis->m_hObject) return FALSE;
    int result = ::DeleteObject(pThis->m_hObject);
    pThis->m_hObject = nullptr;
    return result;
}
// CGdiObject::Attach
// Symbol: ?Attach@CGdiObject@@QEAAHPEAX@Z
extern "C" int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject) {
    if (!pThis) return FALSE;
    pThis->m_hObject = hObject;
    return TRUE;
}
// CGdiObject::Detach
// Symbol: ?Detach@CGdiObject@@QEAAPEAXXZ
extern "C" HGDIOBJ MS_ABI impl__Detach_CGdiObject__QEAAPEAXXZ(CGdiObject* pThis) {
    if (!pThis) return nullptr;
    HGDIOBJ h = pThis->m_hObject;
    pThis->m_hObject = nullptr;
    return h;
}
