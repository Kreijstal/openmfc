#pragma once
#include "openmfc/afxwin.h"

// Export bridges need qualified access without changing the SDK's protected
// member declarations or redispatching an imported base entry to an override.
struct OpenMfcCFrameWndAccess {
    static void Add(CFrameWnd* self) {self->AddFrameWnd();}
    static void Remove(CFrameWnd* self) {self->RemoveFrameWnd();}
    static void Show(CFrameWnd* self) {self->CFrameWnd::OnShowMenuBar();}
    static void Hide(CFrameWnd* self) {self->CFrameWnd::OnHideMenuBar();}
    static BOOL Create(CFrameWnd* self,CREATESTRUCTW* create) {return self->OnCreate(create);}
    static BOOL CreateClient(CFrameWnd* self,CREATESTRUCTW* create,CCreateContext* context) {
        return self->CFrameWnd::OnCreateClient(create,context);
    }
    static BOOL CreateHelper(CFrameWnd* self,CREATESTRUCTW* create,CCreateContext* context) {
        return self->OnCreateHelper(create,context);
    }
};
