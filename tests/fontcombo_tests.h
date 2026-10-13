#pragma once
#include "openmfc/afxfontcombobox.h"
extern "C" {
CWnd* MS_ABI impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(void*,const wchar_t*);
void* MS_ABI impl___0CMFCFontComboBox__QEAA_XZ(void*);
void MS_ABI impl___1CMFCFontComboBox__UEAA_XZ(CMFCFontComboBox*);
void MS_ABI impl__CleanUp_CMFCFontComboBox__IEAAXXZ(void*);
}
namespace fontcombo_test {
class Access:public CMFCFontComboBox {
public:using CMFCFontComboBox::m_Images;using CMFCFontComboBox::m_bToolBarMode;
};
class OwnerDraw:public CComboBox {
public:int draw=0,measure=0,compare=0,deleted=0;
    void DrawItem(DRAWITEMSTRUCT*)override {++draw;}
    void MeasureItem(MEASUREITEMSTRUCT* item)override {++measure;item->itemHeight=37;}
    int CompareItem(COMPAREITEMSTRUCT*)override {++compare;return -7;}
    void DeleteItem(DELETEITEMSTRUCT*)override {++deleted;}
};
static void reflection() {
    OwnerDraw control;LRESULT result=99;DRAWITEMSTRUCT draw{};MEASUREITEMSTRUCT measure{};COMPAREITEMSTRUCT compare{};DELETEITEMSTRUCT deleted{};
    check(control.OnChildNotify(WM_DRAWITEM,0,reinterpret_cast<LPARAM>(&draw),&result) && control.draw==1 && result==99,"combo reflection calls the derived DrawItem and preserves the result slot");
    check(control.OnChildNotify(WM_MEASUREITEM,0,reinterpret_cast<LPARAM>(&measure),&result) && control.measure==1 && measure.itemHeight==37,"combo reflection invokes the derived MeasureItem at native slot 93");
    check(control.OnChildNotify(WM_COMPAREITEM,0,reinterpret_cast<LPARAM>(&compare),&result) && control.compare==1 && result==-7,"combo reflection sign-extends the derived comparison result from native slot 94");
    check(control.OnChildNotify(WM_DELETEITEM,0,reinterpret_cast<LPARAM>(&deleted),&result) && control.deleted==1 && result==-7,"combo reflection invokes the derived DeleteItem at native slot 95");
}
static void lifecycle() {
    alignas(Access) BYTE raw[264];memset(raw,0xa5,sizeof(raw));
    auto* combo=static_cast<Access*>(impl___0CMFCFontComboBox__QEAA_XZ(raw));
    check(sizeof(CMFCFontComboBox)==256 && !combo->m_hWnd && !combo->m_bToolBarMode && !combo->m_Images.m_hImageList &&
        combo->m_Images.GetRuntimeClass()==CImageList::GetThisClass() && combo->GetRuntimeClass()==CComboBox::GetThisClass() &&
        *reinterpret_cast<UINT64*>(raw+256)==0xa5a5a5a5a5a5a5a5ULL,
        "font combo construction supplies native 256-byte storage, its own vtable and a real embedded image list");
    check(combo->GetMessageMap()==CMFCFontComboBox::GetThisMessageMap() && combo->GetMessageMap()->lpEntries[0].nMessage==WM_CREATE &&
        combo->GetMessageMap()->lpEntries[1].nMessage==WM_DESTROY && combo->GetMessageMap()->lpEntries[2].nMessage==0x37c,
        "font combo message map routes creation, destruction and DLGINIT control data");
    BOOL savedDraw=CMFCFontComboBox::m_bDrawUsingFont;CMFCFontComboBox::m_bDrawUsingFont=TRUE;
    check(CMFCFontComboBox::m_bDrawUsingFont==TRUE,"public draw-using-font static resolves to the actual exported BOOL storage");
    CMFCFontComboBox::m_bDrawUsingFont=savedDraw;
    combo->m_Images.m_hImageList=ImageList_Create(4,4,ILC_COLOR32,1,1);
    check(combo->m_Images.m_hImageList!=nullptr,"create a real owned image list for font combo destruction");
    impl___1CMFCFontComboBox__UEAA_XZ(combo);
    CWnd parent;CMFCControlContainer container(&parent);
    CWnd* created=impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(&container,L"MFCFontComboBox");
    check(created && created->GetMessageMap()==CMFCFontComboBox::GetThisMessageMap(),"control factory creates the restored native font combo class");delete created;
    check(!impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(&container,L"mfcfontcombobox") &&
        !impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(&container,L"Unknown"),"control factory preserves case-sensitive class matching and unknown-class failure");
}
static void setup() {
    CWnd parent;parent.m_hWnd=CreateWindowExW(0,L"STATIC",L"Fonts",WS_OVERLAPPEDWINDOW,0,0,300,250,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    Access combo;combo.m_hWnd=CreateWindowExW(0,L"COMBOBOX",L"",WS_CHILD|CBS_DROPDOWNLIST|CBS_SORT,0,0,250,200,parent.m_hWnd,reinterpret_cast<HMENU>(101),GetModuleHandleW(nullptr),nullptr);
    check(combo.m_hWnd && combo.Setup(),"font Setup enumerates the production toolbar catalog into a real combo box");
    int count=combo.GetCount();check(count>0,"font Setup produces selectable installed fonts");
    if(count>0) {
        auto* first=reinterpret_cast<CMFCFontInfo*>(combo.GetItemData(0));
        check(first && first!=reinterpret_cast<CMFCFontInfo*>(CB_ERR) && !first->m_strName.IsEmpty() && first->m_nType && sizeof(*first)==32,
            "font item data contains owned native font descriptors with face, script, charset and type");
        CString name=first->m_strName;
        check(combo.SelectFont(name.GetString(),first->m_nCharSet) && combo.GetSelFont()==first,"face and charset selection returns the stored native descriptor");
        CMFCFontInfo copy=*first;check(combo.SelectFont(&copy) && combo.GetSelFont()==first,"descriptor selection matches the native font value fields");
        combo.m_bToolBarMode=TRUE;check(!combo.Setup() && combo.GetCount()==count,"toolbar-owned combos reject Setup without deleting borrowed items");combo.m_bToolBarMode=FALSE;
        check(combo.Setup(TRUETYPE_FONTTYPE),"font Setup rebuilds the list with the requested type mask");
        bool types=true;for(int i=0;i<combo.GetCount();++i)types&=(reinterpret_cast<CMFCFontInfo*>(combo.GetItemData(i))->m_nType&TRUETYPE_FONTTYPE)!=0;
        check(types && combo.GetCount()>0,"rebuilt font items honor the TrueType filter");
        const char properties[]="<MFCComboBox_DrawUsingFont>TRUE</MFCComboBox_DrawUsingFont><MFCComboBox_ShowTrueTypeFonts>FALSE</MFCComboBox_ShowTrueTypeFonts><MFCComboBox_ShowRasterTypeFonts>FALSE</MFCComboBox_ShowRasterTypeFonts><MFCComboBox_ShowDeviceTypeFonts>FALSE</MFCComboBox_ShowDeviceTypeFonts>";
        LRESULT result=-1;
        check(combo.OnWndMsg(0x37c,sizeof(properties)-1,reinterpret_cast<LPARAM>(properties),&result) && result==0 && !combo.GetCount() && CMFCFontComboBox::m_bDrawUsingFont,
            "font combo DLGINIT message parses real properties, updates exported drawing state and applies an empty type mask");
        check(combo.Setup(TRUETYPE_FONTTYPE),"font Setup repopulates after DLGINIT reconfiguration");
        impl__CleanUp_CMFCFontComboBox__IEAAXXZ(&combo);check(!combo.GetCount(),"font cleanup deletes owned descriptors and resets the actual control");
    }
    DestroyWindow(combo.m_hWnd);combo.m_hWnd=nullptr;DestroyWindow(parent.m_hWnd);parent.m_hWnd=nullptr;
}
}
static void fontCombo(){fontcombo_test::reflection();fontcombo_test::lifecycle();fontcombo_test::setup();}
