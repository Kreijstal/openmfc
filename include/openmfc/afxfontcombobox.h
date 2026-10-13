#pragma once
#include "afxtoolbarfontcombobox.h"
class CMFCFontComboBox:public CComboBox {
public:
    CMFCFontComboBox();
    ~CMFCFontComboBox() override;
    static BOOL m_bDrawUsingFont;
    BOOL Setup(int=DEVICE_FONTTYPE|RASTER_FONTTYPE|TRUETYPE_FONTTYPE,BYTE=DEFAULT_CHARSET,BYTE=DEFAULT_PITCH);
    BOOL SelectFont(CMFCFontInfo*);
    BOOL SelectFont(const wchar_t*,BYTE=DEFAULT_CHARSET);
    CMFCFontInfo* GetSelFont()const;
    int DeleteString(UINT);
    int CompareItem(COMPAREITEMSTRUCT*) override;
    void DrawItem(DRAWITEMSTRUCT*) override;
    void MeasureItem(MEASUREITEMSTRUCT*) override;
    BOOL PreTranslateMessage(MSG*) override;
    void PreSubclassWindow() override;
    const AFX_MSGMAP* GetMessageMap()const override;
    static const AFX_MSGMAP* GetThisMessageMap();
protected:
    CImageList m_Images;
    BOOL m_bToolBarMode;
    void Init();void CleanUp();
    int OnCreate(CREATESTRUCTW*);void OnDestroy();LRESULT OnInitControl(WPARAM,LPARAM);
};
static_assert(sizeof(CMFCFontComboBox)==256,"native font combo layout");
