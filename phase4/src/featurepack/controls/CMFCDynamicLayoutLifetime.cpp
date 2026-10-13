#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxlayout.h"
#include <cstdlib>
#include <cstring>
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

extern "C" {
void MS_ABI impl___3_YAXPEAX_Z(void*);
int MS_ABI impl__Create_CMFCDynamicLayout__QEAAHPEAVCWnd___Z(void*, CWnd*);
void MS_ABI impl__Adjust_CMFCDynamicLayout__QEAAXXZ(void*);
int MS_ABI impl__AddItem_CMFCDynamicLayout__QEAAHIUMoveSettings_1_USizeSettings_1__Z(void*, UINT, unsigned long long, unsigned long long);
int MS_ABI impl__AddItem_CMFCDynamicLayout__QEAAHPEAUHWND____UMoveSettings_1_USizeSettings_1__Z(void*, HWND, unsigned long long, unsigned long long);
AFX_DYNAMIC_LAYOUT_ITEM* MS_ABI impl__FindItem_CMFCDynamicLayout__IEAAPEAUAFX_DYNAMIC_LAYOUT_ITEM__PEAUHWND_____Z(void*, HWND);
void MS_ABI impl__GetHostWndRect_CMFCDynamicLayout__QEBAXAEAVCRect___Z(const void*, RECT*);
void MS_ABI impl__CorrectItem_CMFCDynamicLayout__IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(const void*, AFX_DYNAMIC_LAYOUT_ITEM*);
int MS_ABI impl__PrepareItem_CMFCDynamicLayout__IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(const void*, AFX_DYNAMIC_LAYOUT_ITEM*);
UINT MS_ABI impl__AdjustItemRect_CMFCDynamicLayout__IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM__AEAVCRect___Z(const void*, AFX_DYNAMIC_LAYOUT_ITEM*, RECT*);
RECT* MS_ABI impl__GetItemRect_CMFCDynamicLayout__IEBA_AVCRect__AEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(const void*, RECT*, AFX_DYNAMIC_LAYOUT_ITEM*);
int MS_ABI impl__LoadResource_CMFCDynamicLayout__SAHPEAVCWnd__PEAXK_Z(CWnd*, void*, DWORD);
void MS_ABI impl__CleanUp_CMFCDynamicLayoutData__QEAAXXZ(void*);
int MS_ABI impl__ReadResource_CMFCDynamicLayoutData__QEAAHPEAXI_Z(void*, void*, UINT);
int MS_ABI impl__ApplyLayoutDataTo_CMFCDynamicLayoutData__QEAAHPEAVCWnd__H_Z(void*, void*, int);
}

namespace {
struct Block { Block* next; };
struct Node { Node* next; Node* prev; void* item; };
template<class T> unsigned long long Pack(T value) {
    static_assert(sizeof(T)==8,"native ratio argument");
    unsigned long long bits;std::memcpy(&bits,&value,8);return bits;
}
}
openmfc::detail::DynamicLayoutList::~DynamicLayoutList() {
    auto* block=static_cast<Block*>(blocks);
    while(block) { auto* next=block->next;std::free(block);block=next; }
    head=tail=freeNodes=blocks=nullptr;count=0;
}
IMPLEMENT_DYNAMIC(CMFCDynamicLayout,CObject)
CMFCDynamicLayout::CMFCDynamicLayout() : m_pHostWnd(nullptr),m_MinSize(0,0) {}
CMFCDynamicLayout::~CMFCDynamicLayout() {
    for(auto* node=static_cast<Node*>(m_listWnd.head);node;node=node->next)
        impl___3_YAXPEAX_Z(node->item);
}
BOOL CMFCDynamicLayout::Create(CWnd* host) { return impl__Create_CMFCDynamicLayout__QEAAHPEAVCWnd___Z(this,host); }
void CMFCDynamicLayout::Adjust() { impl__Adjust_CMFCDynamicLayout__QEAAXXZ(this); }
BOOL CMFCDynamicLayout::AddItem(UINT id,MoveSettings move,SizeSettings size) {
    return impl__AddItem_CMFCDynamicLayout__QEAAHIUMoveSettings_1_USizeSettings_1__Z(this,id,Pack(move),Pack(size));
}
BOOL CMFCDynamicLayout::AddItem(HWND window,MoveSettings move,SizeSettings size) {
    return impl__AddItem_CMFCDynamicLayout__QEAAHPEAUHWND____UMoveSettings_1_USizeSettings_1__Z(this,window,Pack(move),Pack(size));
}
AFX_DYNAMIC_LAYOUT_ITEM* CMFCDynamicLayout::FindItem(HWND window) { return impl__FindItem_CMFCDynamicLayout__IEAAPEAUAFX_DYNAMIC_LAYOUT_ITEM__PEAUHWND_____Z(this,window); }
void CMFCDynamicLayout::GetHostWndRect(CRect& rect) const { impl__GetHostWndRect_CMFCDynamicLayout__QEBAXAEAVCRect___Z(this,reinterpret_cast<RECT*>(&rect)); }
void CMFCDynamicLayout::CorrectItem(AFX_DYNAMIC_LAYOUT_ITEM& item) const { impl__CorrectItem_CMFCDynamicLayout__IEBAXAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(this,&item); }
BOOL CMFCDynamicLayout::PrepareItem(AFX_DYNAMIC_LAYOUT_ITEM& item) const { return impl__PrepareItem_CMFCDynamicLayout__IEBAHAEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(this,&item); }
UINT CMFCDynamicLayout::AdjustItemRect(AFX_DYNAMIC_LAYOUT_ITEM& item,CRect& rect) const { return impl__AdjustItemRect_CMFCDynamicLayout__IEBAIAEAUAFX_DYNAMIC_LAYOUT_ITEM__AEAVCRect___Z(this,&item,reinterpret_cast<RECT*>(&rect)); }
CRect CMFCDynamicLayout::GetItemRect(AFX_DYNAMIC_LAYOUT_ITEM& item) const { CRect rect;impl__GetItemRect_CMFCDynamicLayout__IEBA_AVCRect__AEAUAFX_DYNAMIC_LAYOUT_ITEM___Z(this,reinterpret_cast<RECT*>(&rect),&item);return rect; }
BOOL CMFCDynamicLayout::LoadResource(CWnd* host,void* resource,DWORD bytes) { return impl__LoadResource_CMFCDynamicLayout__SAHPEAVCWnd__PEAXK_Z(host,resource,bytes); }
CMFCDynamicLayout::MoveSettings CMFCDynamicLayout::MoveNone() { return MoveSettings(); }
CMFCDynamicLayout::MoveSettings CMFCDynamicLayout::MoveHorizontal(int x) { return MoveHorizontalAndVertical(x,0); }
CMFCDynamicLayout::MoveSettings CMFCDynamicLayout::MoveVertical(int y) { return MoveHorizontalAndVertical(0,y); }
CMFCDynamicLayout::MoveSettings CMFCDynamicLayout::MoveHorizontalAndVertical(int x,int y) { MoveSettings result;result.m_nXRatio=x;result.m_nYRatio=y;return result; }
CMFCDynamicLayout::SizeSettings CMFCDynamicLayout::SizeNone() { return SizeSettings(); }
CMFCDynamicLayout::SizeSettings CMFCDynamicLayout::SizeHorizontal(int x) { return SizeHorizontalAndVertical(x,0); }
CMFCDynamicLayout::SizeSettings CMFCDynamicLayout::SizeVertical(int y) { return SizeHorizontalAndVertical(0,y); }
CMFCDynamicLayout::SizeSettings CMFCDynamicLayout::SizeHorizontalAndVertical(int x,int y) { SizeSettings result;result.m_nXRatio=x;result.m_nYRatio=y;return result; }
void CMFCDynamicLayoutData::CleanUp() { impl__CleanUp_CMFCDynamicLayoutData__QEAAXXZ(this); }
BOOL CMFCDynamicLayoutData::ReadResource(void* resource,UINT bytes) { return impl__ReadResource_CMFCDynamicLayoutData__QEAAHPEAXI_Z(this,resource,bytes); }
BOOL CMFCDynamicLayoutData::ApplyLayoutDataTo(CWnd* host,BOOL update) { return impl__ApplyLayoutDataTo_CMFCDynamicLayoutData__QEAAHPEAVCWnd__H_Z(this,host,update); }
// Symbol: ??0CMFCDynamicLayout@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDynamicLayout__QEAA_XZ(void* storage) { return new(storage) CMFCDynamicLayout; }
// Symbol: ??1CMFCDynamicLayout@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMFCDynamicLayout__UEAA_XZ(CMFCDynamicLayout* self) { self->~CMFCDynamicLayout();return self; }
// Symbol: ?GetThisClass@CMFCDynamicLayout@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCDynamicLayout__SAPEAUCRuntimeClass__XZ() { return CMFCDynamicLayout::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCDynamicLayout@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCDynamicLayout__UEBAPEAUCRuntimeClass__XZ(const void*) { return CMFCDynamicLayout::GetThisClass(); }
