// CBitmapButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ?AutoLoad@CBitmapButton@@QEAAHIPEAVCWnd@@@Z
extern "C" int MS_ABI impl__AutoLoad_CBitmapButton__QEAAHIPEAVCWnd___Z(
    void* pThis, unsigned int nId, void* pWnd) {
    if (pThis == nullptr || pWnd == nullptr || nId == 0) {
        return 0;
    }

    auto* button = static_cast<CWnd*>(pThis);
    if (button->GetSafeHwnd() == nullptr) {
        return 0;
    }

    return LoadBitmapButtonStateImages(
        pThis,
        MAKEINTRESOURCEW(nId),
        MAKEINTRESOURCEW(nId),
        MAKEINTRESOURCEW(nId),
        MAKEINTRESOURCEW(nId)) ? 1 : 0;
}
// Symbol: ?DrawItem@CBitmapButton@@MEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CBitmapButton__MEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, void* pDrawItemStruct) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr || pDrawItemStruct == nullptr) {
        return;
    }

    auto* pDis = static_cast<DRAWITEMSTRUCT*>(pDrawItemStruct);
    HDC hdc = pDis->hDC;
    if (hdc == nullptr) {
        return;
    }

    {
        HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
        ::FillRect(hdc, &pDis->rcItem, brush);
        ::DeleteObject(brush);
    }

    HBITMAP selectedBitmap = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_bitmapButtonStateMutex);
        auto it = g_bitmapButtonStates.find(pThis);
        if (it != g_bitmapButtonStates.end()) {
            selectedBitmap = PickBitmapForBitmapButton(it->second, pDis->itemState);
        }
    }

    if (selectedBitmap != nullptr) {
        DrawBitmapCentered(hdc, selectedBitmap, pDis->rcItem);
    }

    if ((pDis->itemState & ODS_FOCUS) != 0) {
        RECT focusRect = pDis->rcItem;
        ::InflateRect(&focusRect, -1, -1);
        if (!::IsRectEmpty(&focusRect)) {
            ::DrawFocusRect(hdc, &focusRect);
        }
    }
}
// Symbol: ?LoadBitmaps@CBitmapButton@@QEAAHPEB_W000@Z
extern "C" int MS_ABI impl__LoadBitmaps_CBitmapButton__QEAAHPEB_W000_Z(
    void* pThis, const wchar_t* normal, const wchar_t* selected,
    const wchar_t* disabled, const wchar_t* focus) {
    return LoadBitmapButtonStateImages(pThis, normal, selected, disabled, focus) ? 1 : 0;
}
// Symbol: ?SizeToContent@CBitmapButton@@QEAAXXZ
extern "C" void MS_ABI impl__SizeToContent_CBitmapButton__QEAAXXZ(void* pThis) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr || pWnd->GetSafeHwnd() == nullptr) {
        return;
    }

    HBITMAP hBitmap = nullptr;
    {
        std::lock_guard<std::mutex> lock(g_bitmapButtonStateMutex);
        auto it = g_bitmapButtonStates.find(pThis);
        if (it != g_bitmapButtonStates.end()) {
            hBitmap = PickBitmapForBitmapButton(it->second, 0);
        }
    }

    if (hBitmap == nullptr) {
        return;
    }

    BITMAP bmp {};
    if (!::GetObjectW(hBitmap, sizeof(bmp), &bmp)) {
        return;
    }

    CRect rect {};
    rect.left = 0;
    rect.top = 0;
    rect.right = static_cast<int>(bmp.bmWidth);
    rect.bottom = static_cast<int>(bmp.bmHeight);
    pWnd->SetWindowPos(nullptr, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
