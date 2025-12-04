#pragma once
#include "afx.h"
#include "afxstr.h"
#include <windows.h>

class CException : public CObject {
public:
    virtual BOOL GetErrorMessage(wchar_t*, UINT, UINT* = nullptr) const { return FALSE; }
};

class CMemoryException : public CException {};
class CFileException : public CException {
public:
    enum { generic = 0 } m_cause = generic;
};

class CWinThread : public CObject {};
class CWinApp : public CWinThread {
public:
    virtual BOOL InitInstance() { return TRUE; }
    virtual int ExitInstance() { return 0; }
};

class CWnd : public CObject {
public:
    CWnd() : m_hWnd(nullptr) {}
    virtual ~CWnd() = default;

    HWND m_hWnd;
};
