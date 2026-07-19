// Differential test for the third implementation batch: CMFCRibbonBar layout +
// 3 methods, CMFCToolBar +3, COleControl +7 and the accessor migration off the
// side table onto the ABI-visible members.
//
// Every expectation below is the behaviour decoded from retail mfc140u, and
// the raw offsets are the ones the shipping code addresses, so a layout
// regression fails here as loudly as a logic regression.
#include <windows.h>
#include <cstdio>
#include <cstring>
static int pass=0, fail=0;
#define CHECK(c,msg) do{ if(c){pass++;printf("  PASS %s\n",msg);} \
                         else{fail++;printf("  FAIL %s\n",msg);} }while(0)

#define I32(p,off) (*(int*)((unsigned char*)(p)+(off)))
#define I16(p,off) (*(short*)((unsigned char*)(p)+(off)))
#define U32(p,off) (*(unsigned int*)((unsigned char*)(p)+(off)))
#define PTR(p,off) (*(void**)((unsigned char*)(p)+(off)))

int main(){
    HMODULE h = LoadLibraryA("openmfc.dll");
    if(!h){ printf("cannot load openmfc.dll\n"); return 1; }

    // ---------------- CMFCRibbonBar ----------------
    typedef void* (__stdcall *RbCtor)(void*, int);
    typedef void  (__stdcall *RbEnable1)(void*, int);
    typedef void  (__stdcall *RbEnable2)(void*, int, int);
    typedef int   (__stdcall *RbGetCount)(void*);
    auto RibbonCtor = (RbCtor)    GetProcAddress(h,"??0CMFCRibbonBar@@QEAA@H@Z");
    auto EnKeyTips  = (RbEnable1) GetProcAddress(h,"?EnableKeyTips@CMFCRibbonBar@@QEAAXH@Z");
    auto EnToolTips = (RbEnable2) GetProcAddress(h,"?EnableToolTips@CMFCRibbonBar@@QEAAXHH@Z");
    auto CatCount   = (RbGetCount)GetProcAddress(h,"?GetCategoryCount@CMFCRibbonBar@@QEBAHXZ");
    if(!RibbonCtor||!EnKeyTips||!EnToolTips||!CatCount){ printf("missing ribbon export(s)\n"); return 1; }

    alignas(16) static unsigned char rb[8400];
    memset(rb,0,sizeof rb);
    RibbonCtor(rb, TRUE);

    // Constructor defaults, all read off the retail ctor's own stores.
    CHECK(I32(rb,1028)==-1,  "ribbon m_nHighlightedTab defaults to -1 (0x404)");
    CHECK(I32(rb,1044)==-1,  "ribbon m_nKeyboardNavLevel defaults to -1 (0x414)");
    CHECK(I32(rb,1052)==210, "ribbon m_nTooltipWidthRegular defaults to 210 (0x41c)");
    CHECK(I32(rb,1056)==318, "ribbon m_nTooltipWidthLargeImage defaults to 318 (0x420)");
    CHECK(I32(rb,1060)==1,   "ribbon m_bRecalcCategoryHeight defaults TRUE (0x424)");
    CHECK(I32(rb,1064)==1,   "ribbon m_bRecalcCategoryWidth defaults TRUE (0x428)");
    CHECK(I32(rb,1068)==0,   "ribbon 0x42c stays 0 (rbp write is 8 bytes wide)");
    CHECK(I32(rb,1104)==1,   "ribbon m_bToolTip defaults TRUE (0x450)");
    CHECK(I32(rb,1108)==1,   "ribbon m_bToolTipDescr defaults TRUE (0x454)");
    CHECK(I32(rb,1112)==1,   "ribbon m_bKeyTips defaults TRUE (0x458)");
    CHECK(I32(rb,1128)==1,   "ribbon ctor arg lands in m_bReplaceFrameCaption (0x468)");

    memset(rb,0,sizeof rb); RibbonCtor(rb, FALSE);
    CHECK(I32(rb,1128)==0,   "ribbon ctor arg FALSE lands in m_bReplaceFrameCaption");

    // EnableKeyTips is a bare store -- the value is not normalized to 0/1.
    EnKeyTips(rb, 7);
    CHECK(I32(rb,1112)==7,   "EnableKeyTips stores raw value at 0x458");
    EnKeyTips(rb, 0);
    CHECK(I32(rb,1112)==0,   "EnableKeyTips(0) clears 0x458");

    // Both flags stored unconditionally; the second is not gated on the first.
    EnToolTips(rb, 0, 1);
    CHECK(I32(rb,1104)==0 && I32(rb,1108)==1, "EnableToolTips stores both flags independently");
    EnToolTips(rb, 1, 0);
    CHECK(I32(rb,1104)==1 && I32(rb,1108)==0, "EnableToolTips(1,0) stores both");

    // GetCategoryCount stays side-table backed (see global_ribbonbar_batch1.cpp),
    // so it reports 0 for a ribbon that never went through AddCategory. It is
    // checked here only to pin that documented behaviour.
    CHECK(CatCount(rb)==0,   "GetCategoryCount reports 0 for a ribbon with no categories");

    // ---------------- CMFCToolBar ----------------
    typedef void* (__stdcall *TbCtor)(void*);
    typedef int   (__stdcall *TbPred)(void*);
    typedef void  (__stdcall *TbDock)(void*, DWORD);
    auto TbCtorFn   = (TbCtor)GetProcAddress(h,"??0CMFCToolBar@@QEAA@XZ");
    auto CanRestore = (TbPred)GetProcAddress(h,"?CanBeRestored@CMFCToolBar@@UEBAHXZ");
    auto GetCount   = (TbPred)GetProcAddress(h,"?GetCount@CMFCToolBar@@QEBAHXZ");
    auto EnDocking  = (TbDock)GetProcAddress(h,"?EnableDocking@CMFCToolBar@@UEAAXK@Z");
    if(!TbCtorFn||!CanRestore||!GetCount||!EnDocking){ printf("missing toolbar export(s)\n"); return 1; }

    alignas(16) static unsigned char tb[4944];
    memset(tb,0,sizeof tb);
    TbCtorFn(tb);

    CHECK(CanRestore(tb)==0, "CanBeRestored FALSE when m_uiOriginalResID is 0");
    U32(tb,4872) = 0x1234;
    CHECK(CanRestore(tb)==1, "CanBeRestored normalizes a non-zero res id to 1 (setne)");
    U32(tb,4872) = 0xFFFFFFFFu;
    CHECK(CanRestore(tb)==1, "CanBeRestored returns 1, not the raw id");

    // GetCount stays side-table backed so toolbars built through AddButton keep
    // reporting correctly; it reads 0 for a bar with no buttons added.
    CHECK(GetCount(tb)==0,   "GetCount 0 for a toolbar with no buttons");

    // EnableDocking overwrites m_dwControlBarStyle with a literal 1.
    U32(tb,264) = 0xDEADBEEF;
    EnDocking(tb, 0x55);
    CHECK(U32(tb,256)==0x55, "EnableDocking stores alignment at 0x100");
    CHECK(U32(tb,264)==1,    "EnableDocking overwrites m_dwControlBarStyle with 1");

    // ---------------- COleControl ----------------
    typedef void  (__stdcall *CtlVoid)(void*);
    typedef int   (__stdcall *CtlInvoke)(void*, long);
    typedef void* (__stdcall *CtlPtr)(void*);
    typedef void* (__stdcall *CtlPropPages)(void*, DWORD*);
    typedef DWORD (__stdcall *CtlFlags)(void*);
    auto EnSimple   = (CtlVoid)     GetProcAddress(h,"?EnableSimpleFrame@COleControl@@QEAAXXZ");
    auto InvokeOk   = (CtlInvoke)   GetProcAddress(h,"?IsInvokeAllowed@COleControl@@MEAAHJ@Z");
    auto OuterWnd   = (CtlPtr)      GetProcAddress(h,"?GetOuterWindow@COleControl@@IEBAPEAVCWnd@@XZ");
    auto IntFont    = (CtlPtr)      GetProcAddress(h,"?InternalGetFont@COleControl@@QEAAAEAVCFontHolder@@XZ");
    auto DropTgt    = (CtlPtr)      GetProcAddress(h,"?GetWindowlessDropTarget@COleControl@@UEAAPEAUIDropTarget@@XZ");
    auto Aggregates = (CtlInvoke)   GetProcAddress(h,"?OnCreateAggregates@COleControl@@MEAAHXZ");
    auto PropPages  = (CtlPropPages)GetProcAddress(h,"?GetPropPageIDs@COleControl@@MEAAPEAU_GUID@@AEAK@Z");
    auto CtlFlagsFn = (CtlFlags)    GetProcAddress(h,"?GetControlFlags@COleControl@@UEAAKXZ");
    if(!EnSimple||!InvokeOk||!OuterWnd||!IntFont||!DropTgt||!Aggregates||!PropPages||!CtlFlagsFn){
        printf("missing olecontrol export(s)\n"); return 1;
    }

    // A raw zeroed buffer is enough: none of these dispatch virtually.
    alignas(16) static unsigned char ctl[912];
    memset(ctl,0,sizeof ctl);

    EnSimple(ctl);
    CHECK((U32(ctl,352) & (1u<<16))!=0, "EnableSimpleFrame sets bit 16 of the 0x160 flags word");
    CHECK(U32(ctl,352)==(1u<<16),       "EnableSimpleFrame sets only that bit");

    CHECK(InvokeOk(ctl,0)==0, "IsInvokeAllowed FALSE when bit 18 is clear");
    U32(ctl,352) |= (1u<<18);
    CHECK(InvokeOk(ctl,0)==1,        "IsInvokeAllowed reads bit 18");
    CHECK(InvokeOk(ctl,0x1234)==1,   "IsInvokeAllowed ignores the DISPID argument");

    CHECK(OuterWnd(ctl)==(void*)ctl, "GetOuterWindow falls back to the control itself");
    void* fake = (void*)0x12340000;
    PTR(ctl,336) = fake;
    CHECK(OuterWnd(ctl)==fake,       "GetOuterWindow returns m_pReflect when set (0x150)");
    PTR(ctl,336) = NULL;

    CHECK(IntFont(ctl)==(void*)(ctl+376), "InternalGetFont returns &m_font (0x178)");
    CHECK(DropTgt(ctl)==NULL,             "GetWindowlessDropTarget returns null");
    CHECK(Aggregates(ctl,0)==1,           "OnCreateAggregates returns TRUE");

    DWORD cPages = 0xCCCCCCCC;
    void* ids = PropPages(ctl, &cPages);
    CHECK(ids==NULL,     "GetPropPageIDs returns a null array");
    CHECK(cPages==0,     "GetPropPageIDs writes a zero count through the out-param");

    // GetControlFlags is virtual (UEAA), so it dispatches through the vtable and
    // needs a real constructed object -- a zeroed buffer has a null vptr and
    // faults. Construct one through the exported ctor for this check.
    typedef void* (__stdcall *CtlCtor)(void*);
    auto CtlCtorFn = (CtlCtor)GetProcAddress(h,"??0COleControl@@QEAA@XZ");
    if(!CtlCtorFn){ printf("missing COleControl ctor\n"); return 1; }
    alignas(16) static unsigned char ctl2[912];
    memset(ctl2,0,sizeof ctl2);
    CtlCtorFn(ctl2);
    CHECK(CtlFlagsFn(ctl2)==2, "GetControlFlags returns clipPaintDC (literal 2)");

    // The constructor's own retail defaults, now that they live in the members.
    CHECK(U32(ctl2,356)==0x80000005u, "ctor sets m_clrBackColor to COLOR_WINDOW|0x80000000");
    CHECK(U32(ctl2,360)==0x80000008u, "ctor sets m_clrForeColor to COLOR_WINDOWTEXT|0x80000000");
    CHECK(I32(ctl2,412)==1,           "ctor sets m_bEnabled TRUE");
    CHECK(I32(ctl2,416)==4,           "ctor sets m_lReadyState COMPLETE");

    // The accessor migration: these must read the ABI-visible members, not a
    // side table. Poke the members directly and require the getters to agree.
    typedef DWORD (__stdcall *GetColor)(void*);
    typedef short (__stdcall *GetShort)(void*);
    typedef int   (__stdcall *GetInt)(void*);
    typedef long  (__stdcall *GetLong)(void*);
    auto GetBack  = (GetColor)GetProcAddress(h,"?GetBackColor@COleControl@@QEAAKXZ");
    auto GetFore  = (GetColor)GetProcAddress(h,"?GetForeColor@COleControl@@QEAAKXZ");
    auto GetApp   = (GetShort)GetProcAddress(h,"?GetAppearance@COleControl@@QEAAFXZ");
    auto GetBorder= (GetShort)GetProcAddress(h,"?GetBorderStyle@COleControl@@QEAAFXZ");
    auto GetEn    = (GetInt)  GetProcAddress(h,"?GetEnabled@COleControl@@QEAAHXZ");
    auto GetReady = (GetLong) GetProcAddress(h,"?GetReadyState@COleControl@@QEAAJXZ");
    if(!GetBack||!GetFore||!GetApp||!GetBorder||!GetEn||!GetReady){
        printf("missing accessor export(s)\n"); return 1;
    }

    U32(ctl,356)  = 0x00ABCDEF;   // m_clrBackColor
    U32(ctl,360)  = 0x00123456;   // m_clrForeColor
    I16(ctl,408)  = 3;            // m_sAppearance
    I16(ctl,410)  = 2;            // m_sBorderStyle
    I32(ctl,412)  = 0;            // m_bEnabled
    I32(ctl,416)  = 1;            // m_lReadyState

    CHECK(GetBack(ctl)==0x00ABCDEF,  "GetBackColor reads m_clrBackColor (0x164)");
    CHECK(GetFore(ctl)==0x00123456,  "GetForeColor reads m_clrForeColor (0x168)");
    CHECK(GetApp(ctl)==3,            "GetAppearance reads m_sAppearance (0x198)");
    CHECK(GetBorder(ctl)==2,         "GetBorderStyle reads m_sBorderStyle (0x19a)");
    CHECK(GetEn(ctl)==0,             "GetEnabled reads m_bEnabled (0x19c)");
    CHECK(GetReady(ctl)==1,          "GetReadyState reads m_lReadyState (0x1a0)");

    // And the getters must agree with what the constructor actually stored --
    // the check that would have caught the side-table divergence.
    CHECK(GetBack(ctl2)==0x80000005u, "GetBackColor agrees with the ctor default");
    CHECK(GetFore(ctl2)==0x80000008u, "GetForeColor agrees with the ctor default");
    CHECK(GetEn(ctl2)==1,             "GetEnabled agrees with the ctor default");
    CHECK(GetReady(ctl2)==4,          "GetReadyState agrees with the ctor default");

    printf("%d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}
