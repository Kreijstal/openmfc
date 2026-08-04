// CDialogTemplate — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CDialogTemplate@@QEAA@PEAX@Z
extern "C" void MS_ABI impl___0CDialogTemplate__QEAA_PEAX_Z(void* self, void* tmpl) {
    SetDialogTemplateState(self, static_cast<const DLGTEMPLATE*>(tmpl));
}
// Symbol: ??0CDialogTemplate@@QEAA@PEBUDLGTEMPLATE@@@Z
extern "C" void MS_ABI impl___0CDialogTemplate__QEAA_PEBUDLGTEMPLATE___Z(void* self, const DLGTEMPLATE* tmpl) {
    SetDialogTemplateState(self, tmpl);
}
// Symbol: ??1CDialogTemplate@@QEAA@XZ
extern "C" void MS_ABI impl___1CDialogTemplate__QEAA_XZ(void* self) {
    g_dialogTemplateStates.erase(self);
}
// Symbol: ?SetTemplate@CDialogTemplate@@IEAAHPEBUDLGTEMPLATE@@I@Z
extern "C" int MS_ABI impl__SetTemplate_CDialogTemplate__IEAAHPEBUDLGTEMPLATE__I_Z(void* self, const DLGTEMPLATE* tmpl, unsigned int) {
    SetDialogTemplateState(self, tmpl);
    return tmpl ? TRUE : FALSE;
}
// Symbol: ?GetFont@CDialogTemplate@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAG@Z
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    void* self, CString* fontName, unsigned short* fontSize) {
    auto it = g_dialogTemplateStates.find(self);
    if (it == g_dialogTemplateStates.end() || !it->second.hasFont || !fontName || !fontSize) return FALSE;
    *fontName = it->second.fontName;
    *fontSize = it->second.fontSize;
    return TRUE;
}
// Symbol: ?GetFont@CDialogTemplate@@SAHPEBUDLGTEMPLATE@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAG@Z
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__SAHPEBUDLGTEMPLATE__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize) {
    return ReadDialogFont(tmpl, fontName, fontSize) ? TRUE : FALSE;
}
