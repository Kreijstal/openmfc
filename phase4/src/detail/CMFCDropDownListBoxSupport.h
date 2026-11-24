#pragma once

#include "openmfc/afxmfc.h"

namespace openmfc::detail::mfccore {

// Retain the SDK's inline size settings and offsets for the members after CMFCPopupMenu.
class DropDownListBox : public CMFCPopupMenu {
public:
    DropDownListBox();
    ~DropDownListBox() override;
    CRuntimeClass* GetRuntimeClass() const override;

    void Track(CPoint point, CWnd* owner);
    void ResetContent();
    void AddString(const wchar_t* text);
    int GetCount() const;
    int SetCurSel(int index);
    void GetText(int index, CString& text) const;
    CMFCToolBarMenuButton* GetItem(int index) const;
    void HighlightItem(int physicalIndex);
    HWND EditWindow() const;

    virtual void OnChangeHot(int index);
    virtual void OnChooseItem(UINT command);
    virtual void OnDrawItem(CDC* dc, CMFCToolBarMenuButton* item, BOOL hot);
    virtual CSize OnGetItemSize(CDC* dc, CMFCToolBarMenuButton* item, CSize sizeDefault);

    unsigned char beforeAnimationFlags[0x16a0 - sizeof(CMFCPopupMenu)]{};
    BOOL disableAnimation = TRUE;
    BOOL scrollable = FALSE;
    BOOL showScrollBar = TRUE;
    unsigned char beforeMaxHeight[0x16d4 - 0x16ac]{};
    int maxHeight = -1;
    unsigned char afterMaxHeight[0x19c8 - 0x16d8]{};
    CMenu menu;
    int selection = -1;
    int minWidth = -1;
    CWnd* edit = nullptr;
    void* ribbonCombo = nullptr;
};

void SelectDropDownPopupRow(DropDownListBox& list, int index);

} // namespace openmfc::detail::mfccore
