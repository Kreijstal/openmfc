#pragma once
#include "afxwin.h"

struct AFX_DYNAMIC_LAYOUT_ITEM;
namespace openmfc { namespace detail {
// Native CList storage shared by the pointer and resource-item lists.
struct DynamicLayoutList : CObject {
    void* head = nullptr;
    void* tail = nullptr;
    INT_PTR count = 0;
    void* freeNodes = nullptr;
    void* blocks = nullptr;
    INT_PTR blockSize = 10;
    ~DynamicLayoutList();
};
} }

class CMFCDynamicLayout : public CObject {
    DECLARE_DYNAMIC(CMFCDynamicLayout)
public:
    struct MoveSettings {
        MoveSettings() : m_nXRatio(0), m_nYRatio(0) {}
        BOOL IsHorizontal() const { return m_nXRatio > 0; }
        BOOL IsVertical() const { return m_nYRatio > 0; }
        BOOL IsNone() const { return !IsHorizontal() && !IsVertical(); }
        int m_nXRatio, m_nYRatio;
    };
    struct SizeSettings {
        SizeSettings() : m_nXRatio(0), m_nYRatio(0) {}
        BOOL IsHorizontal() const { return m_nXRatio > 0; }
        BOOL IsVertical() const { return m_nYRatio > 0; }
        BOOL IsNone() const { return !IsHorizontal() && !IsVertical(); }
        int m_nXRatio, m_nYRatio;
    };
    CMFCDynamicLayout();
    virtual ~CMFCDynamicLayout();
    BOOL Create(CWnd* host);
    void Adjust();
    static MoveSettings MoveNone();
    static MoveSettings MoveHorizontal(int ratio);
    static MoveSettings MoveVertical(int ratio);
    static MoveSettings MoveHorizontalAndVertical(int x, int y);
    static SizeSettings SizeNone();
    static SizeSettings SizeHorizontal(int ratio);
    static SizeSettings SizeVertical(int ratio);
    static SizeSettings SizeHorizontalAndVertical(int x, int y);
    BOOL AddItem(UINT id, MoveSettings move, SizeSettings size);
    BOOL AddItem(HWND window, MoveSettings move, SizeSettings size);
    BOOL HasItem(HWND window) { return FindItem(window) != nullptr; }
    BOOL IsEmpty() const { return m_listWnd.count == 0; }
    void GetHostWndRect(CRect& rect) const;
    static BOOL LoadResource(CWnd* host, LPVOID resource, DWORD bytes);
    CWnd* GetHostWnd() { return m_pHostWnd; }
    CSize GetMinSize() const { return m_MinSize; }
    void SetMinSize(CSize size) { m_MinSize = size; }
protected:
    AFX_DYNAMIC_LAYOUT_ITEM* FindItem(HWND window);
    void CorrectItem(AFX_DYNAMIC_LAYOUT_ITEM& item) const;
    BOOL PrepareItem(AFX_DYNAMIC_LAYOUT_ITEM& item) const;
    UINT AdjustItemRect(AFX_DYNAMIC_LAYOUT_ITEM& item, CRect& rect) const;
    CRect GetItemRect(AFX_DYNAMIC_LAYOUT_ITEM& item) const;
    CWnd* m_pHostWnd;
    CSize m_MinSize;
    openmfc::detail::DynamicLayoutList m_listWnd;
};

class CMFCDynamicLayoutData {
public:
    struct Item {
        CMFCDynamicLayout::MoveSettings m_moveSettings;
        CMFCDynamicLayout::SizeSettings m_sizeSettings;
    };
    void CleanUp();
    BOOL ReadResource(LPVOID resource, UINT bytes);
    BOOL ApplyLayoutDataTo(CWnd* host, BOOL update);
protected:
    openmfc::detail::DynamicLayoutList m_listCtrls;
};
static_assert(sizeof(openmfc::detail::DynamicLayoutList) == 56, "native CList layout");
static_assert(sizeof(CMFCDynamicLayout) == 80, "native dynamic layout");
static_assert(sizeof(CMFCDynamicLayoutData) == 56, "native layout resource data");
