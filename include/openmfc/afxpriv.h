#pragma once
#include "afxstat_.h"

extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
void AFXAPI AfxResetMsgCache();

class CPushRoutingFrame {
protected:
    CFrameWnd* pOldRoutingFrame;
    _AFX_THREAD_STATE* pThreadState;
    CPushRoutingFrame* pOldPushRoutingFrame;
public:
    explicit CPushRoutingFrame(CFrameWnd* frame) : pThreadState(AfxGetThreadState()) {
        pOldRoutingFrame=pThreadState->m_pRoutingFrame;
        pOldPushRoutingFrame=pThreadState->m_pPushRoutingFrame;
        pThreadState->m_pRoutingFrame=frame;
        pThreadState->m_pPushRoutingFrame=this;
    }
    ~CPushRoutingFrame() { if(pThreadState)Restore(); }
    void Pop() {
        if(!pThreadState) { impl__AfxThrowInvalidArgException__YAXXZ();return; }
        Restore();pThreadState=nullptr;
    }
private:
    void Restore() {
        pThreadState->m_pRoutingFrame=pOldRoutingFrame;
        pThreadState->m_pPushRoutingFrame=pOldPushRoutingFrame;
    }
};

class CPushRoutingView {
protected:
    CView* pOldRoutingView;
    _AFX_THREAD_STATE* pThreadState;
    CPushRoutingView* pOldPushRoutingView;
public:
    explicit CPushRoutingView(CView* view) : pThreadState(AfxGetThreadState()) {
        if(pThreadState) {
            pOldRoutingView=pThreadState->m_pRoutingView;
            pOldPushRoutingView=pThreadState->m_pPushRoutingView;
            pThreadState->m_pRoutingView=view;
            pThreadState->m_pPushRoutingView=this;
        }
    }
    ~CPushRoutingView() {
        if(pThreadState) {
            pThreadState->m_pRoutingView=pOldRoutingView;
            pThreadState->m_pPushRoutingView=pOldPushRoutingView;
        }
    }
};
static_assert(sizeof(CPushRoutingFrame)==24 && sizeof(CPushRoutingView)==24,"native routing scopes");

class CTestCmdUI : public CCmdUI {
public:
    CTestCmdUI() : m_bEnabled(TRUE) {}
    void Enable(BOOL enabled) override { m_bEnabled=enabled;m_bEnableChanged=TRUE; }
    void SetCheck(int) override {}
    void SetRadio(BOOL) override {}
    void SetText(const wchar_t*) override {}
    BOOL m_bEnabled;
};
static_assert(sizeof(CTestCmdUI)==72,"native test command UI");

class CToolCmdUI : public CCmdUI {
public:
    void Enable(BOOL enabled) override;
    void SetCheck(int check) override;
    void SetText(const wchar_t*) override {}
};
