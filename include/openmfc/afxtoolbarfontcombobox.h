#pragma once
#include "afxwin.h"
class CMFCFontInfo:public CObject {
public:
    CMFCFontInfo(const wchar_t* name,const wchar_t* script,BYTE charset,BYTE pitch,int type):m_strName(name),m_strScript(script),m_nCharSet(charset),m_nPitchAndFamily(pitch),m_nType(type){}
    CMFCFontInfo(const CMFCFontInfo&)=default;
    const CString m_strName,m_strScript;
    const BYTE m_nCharSet,m_nPitchAndFamily;
    const int m_nType;
    CString GetFullName() const {CString name=m_strName;if(!m_strScript.IsEmpty()) {name+=L" (";name+=m_strScript;name+=L")";}return name;}
    bool operator==(const CMFCFontInfo& other)const{return m_strName==other.m_strName && m_strScript==other.m_strScript && m_nCharSet==other.m_nCharSet && m_nPitchAndFamily==other.m_nPitchAndFamily && m_nType==other.m_nType;}
};
static_assert(sizeof(CMFCFontInfo)==32,"native font descriptor");
