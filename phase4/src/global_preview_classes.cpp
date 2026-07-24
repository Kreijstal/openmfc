#if defined(__GNUC__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

#include "openmfc/afxwin.h"
#include <cstddef>
#include <cstring>
#include <new>

namespace {

// Lightweight shims for classes that are declared in public headers only in
// name or are otherwise opaque in this compatibility layer.
struct CPrintInfoShim : public CObject {
};

struct CPrintPreviewStateShim {
};

struct CPreviewViewShim : public CScrollView {
};

struct CPreviewViewExShim : public CPreviewViewShim {
};

struct XRibbonInfoParserCollectionShim : public CObject {
};

template <class Base, std::size_t Size>
CObject* CreateSizedObject() {
    static_assert(sizeof(Base) <= Size, "base class must fit harvested object size");
    void* p = ::operator new(Size, std::nothrow);
    if (!p) {
        return nullptr;
    }
    std::memset(p, 0, Size);
    return static_cast<CObject*>(new (p) Base());
}

} // namespace

// ?m_bScaleLargeImages@CPreviewViewEx@@1HA
extern "C" int MS_ABI impl__m_bScaleLargeImages_CPreviewViewEx__1HA = {};

// ??0CPrintInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPrintInfo__QEAA_XZ(void* pThis) {
    return pThis ? new (pThis) CPrintInfoShim() : nullptr;
}

// ??1CPrintInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___1CPrintInfo__QEAA_XZ(void* pThis) {
    if (pThis) {
        static_cast<CPrintInfoShim*>(pThis)->~CPrintInfoShim();
    }
    return pThis;
}

// ??0CPrintPreviewState@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPrintPreviewState__QEAA_XZ(void* pThis) {
    return pThis ? new (pThis) CPrintPreviewStateShim() : nullptr;
}

// ??0CPreviewView@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPreviewView__QEAA_XZ(void* pThis) {
    return pThis ? new (pThis) CPreviewViewShim() : nullptr;
}

// ??1CPreviewView@@UEAA@XZ
extern "C" void* MS_ABI impl___1CPreviewView__UEAA_XZ(void* pThis) {
    if (pThis) {
        static_cast<CPreviewViewShim*>(pThis)->~CPreviewViewShim();
    }
    return pThis;
}

// ??0CPreviewViewEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0CPreviewViewEx__IEAA_XZ(void* pThis) {
    return pThis ? new (pThis) CPreviewViewExShim() : nullptr;
}

// ??1CPreviewViewEx@@MEAA@XZ
extern "C" void* MS_ABI impl___1CPreviewViewEx__MEAA_XZ(void* pThis) {
    if (pThis) {
        static_cast<CPreviewViewExShim*>(pThis)->~CPreviewViewExShim();
    }
    return pThis;
}

// ??0XRibbonInfoParserCollection@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParserCollection_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis ? new (pThis) XRibbonInfoParserCollectionShim() : nullptr;
}

// ??1XRibbonInfoParserCollection@CMFCRibbonInfo@@UEAA@XZ
extern "C" void* MS_ABI impl___1XRibbonInfoParserCollection_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    if (pThis) {
        static_cast<XRibbonInfoParserCollectionShim*>(pThis)->~XRibbonInfoParserCollectionShim();
    }
    return pThis;
}

// ??0XRibbonInfoParserRoot@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParserRoot_CMFCRibbonInfo__IEAA_XZ() {
    return nullptr;
}

// ??1XRibbonInfoParserRoot@CMFCRibbonInfo@@UEAA@XZ
extern "C" void* MS_ABI impl___1XRibbonInfoParserRoot_CMFCRibbonInfo__UEAA_XZ() {
    return nullptr;
}

// ?CalcPageDisplaySize@CPreviewView@@IEAA?AVCSize@@XZ
extern "C" void* MS_ABI impl__CalcPageDisplaySize_CPreviewView__IEAA_AVCSize__XZ(void* /*class*/* p0) {
    return nullptr;
}

// ?CalcScaleRatio@CPreviewView@@MEAA?AVCSize@@V2@0@Z
extern "C" void* MS_ABI impl__CalcScaleRatio_CPreviewView__MEAA_AVCSize__V2_0_Z(void* /*class*/* p0, void* /*class*/ p1, void* /*class*/* p2) {
    return nullptr;
}

// ?ContextSensitiveHelp@XRichEditOleCallback@CRichEditView@@UEAAJH@Z
extern "C" long MS_ABI impl__ContextSensitiveHelp_XRichEditOleCallback_CRichEditView__UEAAJH_Z(int p0) {
    (void)p0;
    return 0;
}

// ?CreateObject@CPreviewView@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CPreviewView__SAPEAVCObject__XZ() {
    return CreateSizedObject<CPreviewViewShim, 536>();
}

// ?CreateObject@CPreviewViewEx@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CPreviewViewEx__SAPEAVCObject__XZ() {
    return CreateSizedObject<CPreviewViewExShim, 5536>();
}

// ?DeleteObject@XRichEditOleCallback@CRichEditView@@UEAAJPEAUIOleObject@@@Z
extern "C" long MS_ABI impl__DeleteObject_XRichEditOleCallback_CRichEditView__UEAAJPEAUIOleObject___Z(void* /*struct*/* p0) {
    return 0;
}

// ?DoZoom@CPreviewView@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__DoZoom_CPreviewView__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// ?FindPageRect@CPreviewView@@IEAAHAEAVCPoint@@AEAI@Z
extern "C" int MS_ABI impl__FindPageRect_CPreviewView__IEAAHAEAVCPoint__AEAI_Z(void* /*class*/* p0, unsigned int* p1) {
    return 0;
}

// ?Freeze@XViewObject@COleControl@@UEAAJKJPEAXPEAK@Z
extern "C" long MS_ABI impl__Freeze_XViewObject_COleControl__UEAAJKJPEAXPEAK_Z(unsigned long p0, long p1, void* p2, unsigned long* p3) {
    if (p3 != nullptr) {
        *p3 = 0;
    }
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

// ?GetAdvise@XViewObject@COleControl@@UEAAJPEAK0PEAPEAUIAdviseSink@@@Z
extern "C" long MS_ABI impl__GetAdvise_XViewObject_COleControl__UEAAJPEAK0PEAPEAUIAdviseSink___Z(unsigned long* p0, unsigned long* p1, void* /*struct*/** p2) {
    if (p0 != nullptr) {
        *p0 = 0;
    }
    if (p1 != nullptr) {
        *p1 = 0;
    }
    if (p2 != nullptr) {
        *p2 = nullptr;
    }
    return 0;
}

// ?GetDragDropEffect@XRichEditOleCallback@CRichEditView@@UEAAJHKPEAK@Z
extern "C" long MS_ABI impl__GetDragDropEffect_XRichEditOleCallback_CRichEditView__UEAAJHKPEAK_Z(int p0, unsigned long p1, unsigned long* p2) {
    if (p2 != nullptr) {
        *p2 = 0;
    }
    (void)p0;
    (void)p1;
    return 0;
}

// ?GetNewStorage@XRichEditOleCallback@CRichEditView@@UEAAJPEAPEAUIStorage@@@Z
extern "C" long MS_ABI impl__GetNewStorage_XRichEditOleCallback_CRichEditView__UEAAJPEAPEAUIStorage___Z(void* /*struct*/** p0) {
    if (p0 != nullptr) {
        *p0 = nullptr;
    }
    return 0;
}

// ?GetPages@XSpecifyPropertyPages@COleControl@@UEAAJPEAUtagCAUUID@@@Z
extern "C" long MS_ABI impl__GetPages_XSpecifyPropertyPages_COleControl__UEAAJPEAUtagCAUUID___Z(void* /*struct*/* p0) {
    if (p0 != nullptr) {
        struct _tagCAUUID {
            unsigned long cElems;
            void* pElems;
        };
        _tagCAUUID* pCauuid = reinterpret_cast<_tagCAUUID*>(p0);
        pCauuid->cElems = 0;
        pCauuid->pElems = nullptr;
    }
    return 0;
}

// ?GetRect@XViewObject@COleControl@@UEAAJKPEAU_RECTL@@@Z
extern "C" long MS_ABI impl__GetRect_XViewObject_COleControl__UEAAJKPEAU_RECTL___Z(unsigned long p0, void* /*struct*/* p1) {
    if (p1 != nullptr) {
        struct _RECTL {
            unsigned long left;
            unsigned long top;
            unsigned long right;
            unsigned long bottom;
        };
        auto rect = reinterpret_cast<_RECTL*>(p1);
        rect->left = 0;
        rect->top = 0;
        rect->right = 0;
        rect->bottom = 0;
    }
    (void)p0;
    return 0;
}

// ?GetViewStatus@XViewObject@COleControl@@UEAAJPEAK@Z
extern "C" long MS_ABI impl__GetViewStatus_XViewObject_COleControl__UEAAJPEAK_Z(unsigned long* p0) {
    if (p0 != nullptr) {
        *p0 = 0;
    }
    return 0;
}

// ?OnCreate@CPreviewView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CPreviewView__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// ?OnCreate@CPreviewViewEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CPreviewViewEx__IEAAHPEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {
    return 0;
}

// ?OnDisplayPageNumber@CPreviewView@@MEAAXII@Z
extern "C" void MS_ABI impl__OnDisplayPageNumber_CPreviewView__MEAAXII_Z(unsigned int p0, unsigned int p1) {}

// ?OnDisplayPageNumber@CPreviewViewEx@@MEAAXII@Z
extern "C" void MS_ABI impl__OnDisplayPageNumber_CPreviewViewEx__MEAAXII_Z(unsigned int p0, unsigned int p1) {}

// ?OnDraw@CPreviewView@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CPreviewView__MEAAXPEAVCDC___Z(void* /*class*/* p0) {}

// ?OnEraseBkgnd@CPreviewView@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPreviewView__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// ?OnEraseBkgnd@CPreviewViewEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPreviewViewEx__IEAAHPEAVCDC___Z(void* /*class*/* p0) {
    return 0;
}

// ?OnFieldChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@_K1QEA_KKKH@Z
extern "C" long MS_ABI impl__OnFieldChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___K1QEA_KKKH_Z(void* /*struct*/* p0, unsigned __int64 p1, unsigned __int64 p2, void* p3, unsigned char p4, unsigned __int64* p5, unsigned long p6, unsigned long p7, int p8) {
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    (void)p7;
    (void)p8;
    return 0;
}

// ?OnHScroll@CPreviewView@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CPreviewView__IEAAXIIPEAVCScrollBar___Z(unsigned int p0, unsigned int p1, void* /*class*/* p2) {}

// ?OnLButtonDown@CPreviewView@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CPreviewView__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// ?OnNextPage@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnNextPage_CPreviewView__IEAAXXZ() {}

// ?OnNumPageChange@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnNumPageChange_CPreviewView__IEAAXXZ() {}

// ?OnPreviewClose@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPreviewClose_CPreviewView__IEAAXXZ() {}

// ?OnPreviewPrint@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPreviewPrint_CPreviewView__IEAAXXZ() {}

// ?OnPrevPage@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPrevPage_CPreviewView__IEAAXXZ() {}

// ?OnRowChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@_KQEB_KKKH@Z
extern "C" long MS_ABI impl__OnRowChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset___KQEB_KKKH_Z(void* /*struct*/* p0, unsigned __int64 p1, void* p2, unsigned char p3, void* p4, unsigned __int64 p5, unsigned long p6, unsigned long p7, int p8) {
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    (void)p6;
    (void)p7;
    (void)p8;
    return 0;
}

// ?OnRowsetChange@XRowsetNotify@COleControlSite@@UEAAJPEAUIRowset@@KKH@Z
extern "C" long MS_ABI impl__OnRowsetChange_XRowsetNotify_COleControlSite__UEAAJPEAUIRowset__KKH_Z(void* /*struct*/* p0, unsigned long p1, unsigned long p2, int p3) {
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    return 0;
}

// ?OnSize@CPreviewView@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CPreviewView__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// ?OnSize@CPreviewViewEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CPreviewViewEx__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// ?OnUpdateNextPage@CPreviewView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNextPage_CPreviewView__IEAAXPEAVCCmdUI___Z(void* /*class*/* p0) {}

// ?OnUpdateNumPageChange@CPreviewView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateNumPageChange_CPreviewView__IEAAXPEAVCCmdUI___Z(void* /*class*/* p0) {}

// ?OnUpdatePreviewNumPage@CPreviewViewEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePreviewNumPage_CPreviewViewEx__IEAAXPEAVCCmdUI___Z(void* /*class*/* p0) {}

// ?OnUpdatePrevPage@CPreviewView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePrevPage_CPreviewView__IEAAXPEAVCCmdUI___Z(void* /*class*/* p0) {}

// ?OnUpdateZoomIn@CPreviewView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateZoomIn_CPreviewView__IEAAXPEAVCCmdUI___Z(void* /*class*/* p0) {}

// ?OnUpdateZoomOut@CPreviewView@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateZoomOut_CPreviewView__IEAAXPEAVCCmdUI___Z(void* /*class*/* p0) {}

// ?OnVScroll@CPreviewView@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CPreviewView__IEAAXIIPEAVCScrollBar___Z(unsigned int p0, unsigned int p1, void* /*class*/* p2) {}

// ?OnZoomIn@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnZoomIn_CPreviewView__IEAAXXZ() {}

// ?OnZoomOut@CPreviewView@@IEAAXXZ
extern "C" void MS_ABI impl__OnZoomOut_CPreviewView__IEAAXXZ() {}

// ?PositionPage@CPreviewView@@MEAAXI@Z
extern "C" void MS_ABI impl__PositionPage_CPreviewView__MEAAXI_Z(unsigned int p0) {}

// ?SetAdvise@XViewObject@COleControl@@UEAAJKKPEAUIAdviseSink@@@Z
extern "C" long MS_ABI impl__SetAdvise_XViewObject_COleControl__UEAAJKKPEAUIAdviseSink___Z(unsigned long p0, unsigned long p1, void* /*struct*/* p2) {
    (void)p0;
    (void)p1;
    (void)p2;
    return 0;
}

// ?SetCurrentPage@CPreviewView@@IEAAXIH@Z
extern "C" void MS_ABI impl__SetCurrentPage_CPreviewView__IEAAXIH_Z(unsigned int p0, int p1) {}

// ?SetPrintView@CPreviewView@@QEAAHPEAVCView@@@Z
extern "C" int MS_ABI impl__SetPrintView_CPreviewView__QEAAHPEAVCView___Z(void* /*class*/* p0) {
    return 0;
}

// ?SetScaledSize@CPreviewView@@IEAAXI@Z
extern "C" void MS_ABI impl__SetScaledSize_CPreviewView__IEAAXI_Z(unsigned int p0) {}

// ?SetToolbarSize@CPreviewViewEx@@IEAAXXZ
extern "C" void MS_ABI impl__SetToolbarSize_CPreviewViewEx__IEAAXXZ() {}

// ?SetZoomState@CPreviewView@@IEAAXIIVCPoint@@@Z
extern "C" void MS_ABI impl__SetZoomState_CPreviewView__IEAAXIIVCPoint___Z(unsigned int p0, unsigned int p1, void* /*class*/ p2) {}

// ?ShowContainerUI@XRichEditOleCallback@CRichEditView@@UEAAJH@Z
extern "C" long MS_ABI impl__ShowContainerUI_XRichEditOleCallback_CRichEditView__UEAAJH_Z(int p0) {
    (void)p0;
    return 0;
}

// ?Unfreeze@XViewObject@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__Unfreeze_XViewObject_COleControl__UEAAJK_Z(unsigned long p0) {
    (void)p0;
    return 0;
}
