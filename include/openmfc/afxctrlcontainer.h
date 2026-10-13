#pragma once
#include "afxwin.h"
// The native container embeds a 40-byte object array and a 56-byte WORD map.
class CMFCControlContainer : public CObject {
public:
    explicit CMFCControlContainer(CWnd*);
    ~CMFCControlContainer() override;
    BOOL SubclassDlgControls();
    void FreeSubclassedControls();
    BOOL IsSubclassedFeaturePackControl(HWND);
    BOOL ReSubclassControl(HWND,WORD,CWnd&);
    void SetControlData(WORD,DWORD,BYTE*);
    BOOL GetControlData(WORD,DWORD&,BYTE*&);
    void ClearControlData();
    static int __stdcall UTF8ToString(const char*,CString&,int);
    CWnd* m_pWnd;
    CObArray m_arSubclassedCtrls;
protected:
    CMapWordToOb m_mapControlData;
    CWnd* CreateDlgControl(const wchar_t*);
    void PreUnsubclassControl(CWnd*);
};
static_assert(sizeof(CMFCControlContainer)==112,"native feature-pack control container");
