#pragma once
extern "C" LRESULT MS_ABI impl__WindowProc_CWnd__MEAA_JI_K_J_Z(CWnd*,UINT,WPARAM,LPARAM);
extern "C" UINT impl__AFX_WM_DRAW2D__3IA;
extern "C" UINT impl__AFX_WM_RECREATED2DRESOURCES__3IA;
extern "C" void MS_ABI impl__EnableD2DSupport_CWnd__QEAAXHH_Z(CWnd*,int,int);
extern "C" int MS_ABI impl__DoD2DPaint_CWnd__IEAAHXZ(CWnd*);
extern "C" int MS_ABI impl__IsD2DSupportEnabled_CWnd__QEAAHXZ(CWnd*);
extern "C" CHwndRenderTarget* MS_ABI impl__GetRenderTarget_CWnd__QEAAPEAVCHwndRenderTarget__XZ(CWnd*);
extern "C" CDCRenderTarget* MS_ABI impl__GetDCRenderTarget_CWnd__QEAAPEAVCDCRenderTarget__XZ(CWnd*);
extern "C" void MS_ABI impl__OnPaint_CWnd__IEAAXXZ(CWnd*);

struct WindowPaintFixture {
    CWnd window;
    HWND hwnd=nullptr;
    int drawings=0, notifications=0;
    bool handled=true, disableDuringDraw=false, fakeTarget=false;
    CRenderTarget* lastTarget=nullptr;
    static LRESULT CALLBACK Proc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) {
        auto* fixture=reinterpret_cast<WindowPaintFixture*>(GetWindowLongPtrW(hwnd,GWLP_USERDATA));
        if(message==WM_NCCREATE) {
            fixture=static_cast<WindowPaintFixture*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
            SetWindowLongPtrW(hwnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(fixture));
            fixture->window.m_hWnd=hwnd;
        }
        if(fixture) {
            if(message==impl__AFX_WM_DRAW2D__3IA) {
                ++fixture->drawings;
                fixture->lastTarget=reinterpret_cast<CRenderTarget*>(lParam);
                check(wParam==0 && fixture->lastTarget==fixture->window.m_pRenderTarget,"draw message carries zero WPARAM and the actual owned target in LPARAM");
                if(fixture->disableDuringDraw) {
                    fixture->window.EnableD2DSupport(FALSE);
                    check(!fixture->window.IsD2DSupportEnabled() && fixture->lastTarget->IsValid(),"disabling support inside the draw callback preserves the active native target until EndDraw");
                }
                if(!fixture->fakeTarget) fixture->lastTarget->Clear(CD2DColorF{0,1,0,1});
                return fixture->handled;
            }
            if(message==impl__AFX_WM_RECREATED2DRESOURCES__3IA) {
                ++fixture->notifications;
                check(wParam==0 && reinterpret_cast<CRenderTarget*>(lParam)==fixture->window.m_pRenderTarget,
                      "device-loss notification carries the window's render target");
                return 0;
            }
            if(message==WM_NCDESTROY) {
                LRESULT result=impl__WindowProc_CWnd__MEAA_JI_K_J_Z(&fixture->window,message,wParam,lParam);
                fixture->window.EnableD2DSupport(FALSE);
                fixture->window.m_hWnd=nullptr;
                return result;
            }
            return impl__WindowProc_CWnd__MEAA_JI_K_J_Z(&fixture->window,message,wParam,lParam);
        }
        return DefWindowProcW(hwnd,message,wParam,lParam);
    }
    explicit WindowPaintFixture(bool useDC) {
        window.EnableD2DSupport(TRUE,useDC);
        static ATOM atom=[] {
            WNDCLASSW cls{};cls.lpfnWndProc=Proc;cls.hInstance=GetModuleHandleW(nullptr);
            cls.lpszClassName=L"OpenMFC_D2D_Window_Test";cls.hbrBackground=static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
            return RegisterClassW(&cls);
        }();
        if(atom) hwnd=CreateWindowExW(0,L"OpenMFC_D2D_Window_Test",L"Native D2D window",WS_POPUP|WS_VISIBLE,
            20,20,96,64,nullptr,nullptr,GetModuleHandleW(nullptr),this);
        check(hwnd!=nullptr,"create a native window with production CWnd message routing");
    }
    ~WindowPaintFixture() { if(hwnd) DestroyWindow(hwnd); }
    void Invalidate() { InvalidateRect(hwnd,nullptr,FALSE); }
};
struct LostTargetProbe { void** table;int begins=0,ends=0,releases=0; };
static void STDMETHODCALLTYPE lostBegin(LostTargetProbe* self) { ++self->begins; }
static HRESULT STDMETHODCALLTYPE lostEnd(LostTargetProbe* self,D2D1_TAG*,D2D1_TAG*) { ++self->ends;return D2DERR_RECREATE_TARGET; }
static ULONG STDMETHODCALLTYPE lostRelease(LostTargetProbe* self) { ++self->releases;return 0; }

class WindowOwnedProbe : public CD2DResource {
public:
    int& deleted;
    WindowOwnedProbe(CRenderTarget* parent,int& observed) : CD2DResource(parent,TRUE),deleted(observed) {}
    ~WindowOwnedProbe() override { ++deleted; }
    HRESULT Create(CRenderTarget*) override { return S_OK; }
    void Destroy() override {}
    BOOL IsValid() const override { return FALSE; }
};
static void windowD2D() {
    static_assert(sizeof(CWnd)==232,"CWnd stays ABI-sized");
    static_assert(offsetof(CWnd,m_pRenderTarget)==224,"retail CWnd inline render-target field");
    {
        CWnd window;
        check(!window.m_pRenderTarget && !window.IsD2DSupportEnabled() && !window.GetRenderTarget() && !window.GetDCRenderTarget() && !window.DoD2DPaint(),
              "CWnd construction initializes its physical target field and disabled getters/paint return empty results");
        impl__EnableD2DSupport_CWnd__QEAAXHH_Z(&window,TRUE,FALSE);
        auto* selected=impl__GetRenderTarget_CWnd__QEAAPEAVCHwndRenderTarget__XZ(&window);
        check(selected && selected==window.m_pRenderTarget && !selected->IsValid() && !window.GetDCRenderTarget() &&
              impl__IsD2DSupportEnabled_CWnd__QEAAHXZ(&window),"enabling before HWND creation owns a lazy HWND target in the retail member");
        impl__EnableD2DSupport_CWnd__QEAAXHH_Z(&window,TRUE,TRUE);
        check(window.GetRenderTarget()==selected && !window.GetDCRenderTarget(),"repeated enable preserves the existing target type and identity");
        window.EnableD2DSupport(FALSE);
        check(!window.m_pRenderTarget && !window.IsD2DSupportEnabled(),"disable releases owned target storage and resets the physical field");
        window.EnableD2DSupport(TRUE,TRUE);
        check(window.GetDCRenderTarget() && !window.GetRenderTarget() && !window.GetDCRenderTarget()->IsValid(),"DC selection produces a typed lazy DC target and never casts it to an HWND target");
    }
    check(impl__AFX_WM_DRAW2D__3IA==RegisterWindowMessageW(L"AFX_WM_DRAW2D") &&
          impl__AFX_WM_RECREATED2DRESOURCES__3IA==RegisterWindowMessageW(L"AFX_WM_RECREATED2DRESOURCES"),
          "exported D2D message variables use the shared registered Win32 message identities");
    {
        WindowPaintFixture fixture(false);if(!fixture.hwnd)return;
        auto* target=fixture.window.GetRenderTarget();
        fixture.Invalidate();
        check(impl__DoD2DPaint_CWnd__IEAAHXZ(&fixture.window) && fixture.drawings>0 && target->IsValid() &&
              target->GetHwnd()==fixture.hwnd && !GetUpdateRect(fixture.hwnd,nullptr,FALSE),
              "HWND paint lazily creates its native target, dispatches drawing and validates a handled update region");
        SetWindowPos(fixture.hwnd,nullptr,0,0,80,50,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
        auto size=target->GetHwndRenderTarget()->GetPixelSize();
        check(size.width==80 && size.height==50,"WM_SIZE routing resizes the actual HWND render target");
        fixture.handled=false;fixture.Invalidate();
        check(!fixture.window.DoD2DPaint() && GetUpdateRect(fixture.hwnd,nullptr,FALSE),"unhandled HWND drawing leaves the update region available for fallback painting");
        impl__OnPaint_CWnd__IEAAXXZ(&fixture.window);
        check(!GetUpdateRect(fixture.hwnd,nullptr,FALSE),"OnPaint falls back to balanced GDI painting when the D2D handler declines the message");
        fixture.handled=true;
        auto* old=target->Detach();if(old)old->Release();
        void* slots[57]{};slots[2]=reinterpret_cast<void*>(lostRelease);slots[48]=reinterpret_cast<void*>(lostBegin);slots[49]=reinterpret_cast<void*>(lostEnd);
        LostTargetProbe lost{slots};target->Attach(reinterpret_cast<ID2D1HwndRenderTarget*>(&lost));
        fixture.fakeTarget=true;
        LifecycleProbe resource(target);
        fixture.Invalidate();
        check(fixture.window.DoD2DPaint() && lost.begins==1 && lost.ends==1 && lost.releases==1 &&
              target->IsValid() && fixture.notifications==1 && resource.calls==std::vector<int>({7,6}),
              "HWND device loss recreates its native target and registered resources before sending the recreated-resources notification");
        fixture.fakeTarget=false;
    }
    {
        WindowPaintFixture fixture(true);if(!fixture.hwnd)return;
        auto* target=impl__GetDCRenderTarget_CWnd__QEAAPEAVCDCRenderTarget__XZ(&fixture.window);
        fixture.Invalidate();
        check(fixture.window.DoD2DPaint() && target->IsValid() && fixture.lastTarget==target && !GetUpdateRect(fixture.hwnd,nullptr,FALSE),
              "DC paint creates and binds a native target to the paint DC and balances BeginPaint/EndPaint");
        HDC dc=GetDC(fixture.hwnd);COLORREF color=dc?GetPixel(dc,4,4):CLR_INVALID;if(dc)ReleaseDC(fixture.hwnd,dc);
        check(color==RGB(0,255,0),"DC Direct2D painting reaches the actual window's green pixels");
        fixture.handled=false;fixture.Invalidate();
        check(!fixture.window.DoD2DPaint() && !GetUpdateRect(fixture.hwnd,nullptr,FALSE),"declined DC drawing still releases its paint DC and validates the paint session");
    }
    {
        WindowPaintFixture fixture(false);if(!fixture.hwnd)return;
        fixture.disableDuringDraw=true;fixture.Invalidate();
        check(fixture.window.DoD2DPaint() && !fixture.window.m_pRenderTarget,"disabling support during a draw callback completes the active paint without dangling target access");
    }
    {
        WindowPaintFixture fixture(false);if(!fixture.hwnd)return;
        fixture.window.EnableD2DSupport(FALSE);
        fixture.window.m_pRenderTarget = new CHwndRenderTarget(fixture.hwnd);
        fixture.disableDuringDraw=true;fixture.Invalidate();
        check(fixture.window.DoD2DPaint() && !fixture.window.m_pRenderTarget,
              "client-installed targets are retained across callbacks and released after active painting");
    }
    {
        auto* window=new CWnd;window->EnableD2DSupport(TRUE);
        int deleted=0;
        auto* owned=new WindowOwnedProbe(window->GetRenderTarget(),deleted);
        (void)owned;
        delete window;
        check(deleted==1,"CWnd destruction before HWND creation releases its auto-owned D2D resources");
    }
}
