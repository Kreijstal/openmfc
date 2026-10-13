// Native feature-pack container ownership and DLGINIT control data.
#define OPENMFC_APPCORE_IMPL
#include "detail/CMFCControlContainerSupport.h"
#include <memory>
CMFCControlContainer::CMFCControlContainer(CWnd* parent):m_pWnd(parent),m_mapControlData(10) {}
CMFCControlContainer::~CMFCControlContainer(){ClearControlData();FreeSubclassedControls();}
void CMFCControlContainer::FreeSubclassedControls() {
    for(INT_PTR i=0;i<m_arSubclassedCtrls.GetSize();++i)delete m_arSubclassedCtrls[i];
    m_arSubclassedCtrls.RemoveAll();
}
void CMFCControlContainer::ClearControlData() {
    auto pos=m_mapControlData.GetStartPosition();
    while(pos) {WORD id;CObject* value;m_mapControlData.GetNextAssoc(pos,id,value);delete value;}
    m_mapControlData.RemoveAll();
}
BOOL CMFCControlContainer::GetControlData(WORD id,DWORD& size,BYTE*& data) {
    CObject* object;if(!m_mapControlData.Lookup(id,object))return FALSE;
    auto* bytes=static_cast<CByteArray*>(object);size=static_cast<DWORD>(bytes->GetSize());data=bytes->GetData();return TRUE;
}
void CMFCControlContainer::SetControlData(WORD id,DWORD size,BYTE* data) {
    std::unique_ptr<CByteArray> bytes(new(std::nothrow) CByteArray);
    if(!bytes)impl__AfxThrowMemoryException__YAXXZ();
    bytes->SetSize(size);
    if(size && data)std::memcpy(bytes->GetData(),data,size);
    // Retail assigns the new byte array directly to the map's value slot.
    m_mapControlData[id]=bytes.get();bytes.release();
}
BOOL CMFCControlContainer::IsSubclassedFeaturePackControl(HWND window) {
    if(!window)return FALSE;
    for(INT_PTR i=0;i<m_arSubclassedCtrls.GetSize();++i) {
        auto* control=static_cast<CWnd*>(m_arSubclassedCtrls[i]);
        if(control && control->m_hWnd==window)return TRUE;
    }
    return FALSE;
}
void CMFCControlContainer::PreUnsubclassControl(CWnd* control) {
    if(!control || !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(control,impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ()))return;
    using GetWindow=CWnd* (MS_ABI*)(CWnd*);
    auto getter=openmfc::detail::WindowSlot<GetWindow>(control,92);
    CWnd* inner=getter(control);
    if(inner && inner->m_hWnd)impl__UnsubclassWindow_CWnd__QEAAPEAUHWND____XZ(getter(control));
}
BOOL CMFCControlContainer::ReSubclassControl(HWND window,WORD id,CWnd& replacement) {
    if(!window)return FALSE;
    for(INT_PTR i=0;i<m_arSubclassedCtrls.GetSize();++i) {
        auto* old=static_cast<CWnd*>(m_arSubclassedCtrls[i]);
        if(!old || old->m_hWnd!=window)continue;
        if(!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(old,CWnd::GetThisClass()) || !old->m_hWnd)return FALSE;
        DWORD size=0;BYTE* data=nullptr;GetControlData(id,size,data);
        m_arSubclassedCtrls[i]=nullptr;PreUnsubclassControl(old);
        impl__UnsubclassWindow_CWnd__QEAAPEAUHWND____XZ(old);delete old;
        if(!impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(&replacement,window)) {impl__AfxThrowNotSupportedException__YAXXZ();return FALSE;}
        if(size)::SendMessageA(replacement.m_hWnd,0x37c,size,reinterpret_cast<LPARAM>(data));
        return TRUE;
    }
    return FALSE;
}
CWnd* CMFCControlContainer::CreateDlgControl(const wchar_t* name) {return impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(this,name);}
BOOL CMFCControlContainer::SubclassDlgControls() {
    for(HWND child=::GetWindow(m_pWnd->m_hWnd,GW_CHILD);child;child=::GetWindow(child,GW_HWNDNEXT)) {
        wchar_t name[1024]{};::GetClassNameW(child,name,1024);
        CWnd* control=CreateDlgControl(name);
        if(control) {m_arSubclassedCtrls.Add(control);impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(control,child);}
    }
    return TRUE;
}
int __stdcall CMFCControlContainer::UTF8ToString(const char* source,CString& result,int size) {
    int count=::MultiByteToWideChar(CP_UTF8,0,source,size,nullptr,0);if(!count)return 0;
    auto* out=result.GetBuffer(count);
    ::MultiByteToWideChar(CP_UTF8,0,source,size,out,count);out[count]=L'\0';result.ReleaseBuffer(-1);return count;
}
void OpenMfcDeleteControlContainer(CMFCControlContainer* container) {delete container;}
// Symbol: ?CreateDlgControl@CMFCControlContainer@@IEAAPEAVCWnd@@PEB_W@Z
extern "C" CWnd* MS_ABI impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(
    void* pThis, const wchar_t* /*lpszClassName*/)
{
    if (pThis == nullptr) return nullptr;
    // TODO(clean-room): transcribed partially -- retail compares the class
    // name (ATL CString, 0x18000dcb0 / CString::Compare 0x1802c7770) against
    // "MFCButton", "MFCColorButton", "MFCEditBrowse", "MFCFontComboBox",
    // "MFCLink", "MFCMaskedEdit", "MFCMenuButton", "MFCPropertyGrid",
    // "MFCShellList", "MFCShellTree" and "MFCVSListBox" (0x18033ed18..
    // 0x18033ee40), allocates the matching object (0xb20/0xbc8/0x150/0x100/
    // 0xb40/0x128/0xb48/0x6b8/0x278/0x100/0x348 bytes) and runs its
    // constructor (0x18001c930/0x180027ff0/0x18005f270/0x180060ef0/
    // 0x1800781b0/0x180078d70/0x18008e2c0/0x1800c5ef0/0x18012a760/
    // 0x18012d010/0x1801c6490).  The constructors are not all real here, so
    // the not-created result (matching an unrecognized name) is returned.
    return nullptr;
}

// Symbol: ??0CMFCControlContainer@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCControlContainer__QEAA_PEAVCWnd___Z(void* self,CWnd* parent) {return new(self) CMFCControlContainer(parent);}
// Symbol: ??1CMFCControlContainer@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCControlContainer__UEAA_XZ(CMFCControlContainer* self) {self->~CMFCControlContainer();}
// Symbol: ?FreeSubclassedControls@CMFCControlContainer@@QEAAXXZ
extern "C" void MS_ABI impl__FreeSubclassedControls_CMFCControlContainer__QEAAXXZ(CMFCControlContainer* self) {self->FreeSubclassedControls();}
// Symbol: ?ClearControlData@CMFCControlContainer@@QEAAXXZ
extern "C" void MS_ABI impl__ClearControlData_CMFCControlContainer__QEAAXXZ(CMFCControlContainer* self) {self->ClearControlData();}
// Symbol: ?GetControlData@CMFCControlContainer@@QEAAHGAEAKAEAPEAE@Z
extern "C" BOOL MS_ABI impl__GetControlData_CMFCControlContainer__QEAAHGAEAKAEAPEAE_Z(CMFCControlContainer* self,WORD id,DWORD& size,BYTE*& data) {return self->GetControlData(id,size,data);}
// Symbol: ?SetControlData@CMFCControlContainer@@QEAAXGKPEAE@Z
extern "C" void MS_ABI impl__SetControlData_CMFCControlContainer__QEAAXGKPEAE_Z(CMFCControlContainer* self,WORD id,DWORD size,BYTE* data) {self->SetControlData(id,size,data);}
// Symbol: ?IsSubclassedFeaturePackControl@CMFCControlContainer@@QEAAHPEAUHWND__@@@Z
extern "C" BOOL MS_ABI impl__IsSubclassedFeaturePackControl_CMFCControlContainer__QEAAHPEAUHWND_____Z(CMFCControlContainer* self,HWND window) {return self->IsSubclassedFeaturePackControl(window);}
// Symbol: ?PreUnsubclassControl@CMFCControlContainer@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__PreUnsubclassControl_CMFCControlContainer__IEAAXPEAVCWnd___Z(CMFCControlContainer* self,CWnd* control) {
    // The protected helper is reached through a layout-identical access class.
    struct Access:CMFCControlContainer {using CMFCControlContainer::PreUnsubclassControl;};
    static_cast<Access*>(self)->PreUnsubclassControl(control);
}
// Symbol: ?ReSubclassControl@CMFCControlContainer@@QEAAHPEAUHWND__@@GAEAVCWnd@@@Z
extern "C" BOOL MS_ABI impl__ReSubclassControl_CMFCControlContainer__QEAAHPEAUHWND____GAEAVCWnd___Z(CMFCControlContainer* self,HWND window,WORD id,CWnd& control) {return self->ReSubclassControl(window,id,control);}
// Symbol: ?SubclassDlgControls@CMFCControlContainer@@QEAAHXZ
extern "C" BOOL MS_ABI impl__SubclassDlgControls_CMFCControlContainer__QEAAHXZ(CMFCControlContainer* self) {return self->SubclassDlgControls();}
// Symbol: ?UTF8ToString@CMFCControlContainer@@SAHPEBDAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" int MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const char* source,CString& result,int size) {return CMFCControlContainer::UTF8ToString(source,result,size);}
