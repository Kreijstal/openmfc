#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MSVC_DIR="${MSVC_DIR:-/tmp/msvc/unpack}"
BUILD_DIR="${BUILD_DIR:-/tmp/openmfc-rendertarget-drawing}"
MSVC_VER="$(ls -1 "$MSVC_DIR/VC/Tools/MSVC" | sort -V | tail -1)"
SDK_VER="$(ls -1 "$MSVC_DIR/Windows Kits/10/Include" | sort -V | tail -1)"
mkdir -p "$BUILD_DIR"

# Export aliases have C linkage but preserve the throwing C++ MFC contract.
# /EHsc would suppress catching/unwinding through those alias declarations.
FLAGS=(/nologo /EHs /MT /std:c++17 /Gy /Gw /W0
    /DUNICODE /D_UNICODE /DNOMINMAX /D_WIN32_WINNT=0x0601
    -imsvc "$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/include"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/ucrt"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/um"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/shared"
    -I"$ROOT/include" -I"$ROOT/phase4/src")
SOURCES=(tests/test_rendertarget_drawing.cpp
    phase4/src/detail/GdicoreSupport.cpp
    phase4/src/detail/CollectionStateSupport.cpp phase4/src/core/collections/CObList.cpp phase4/src/core/collections/CPtrArrayStorage.cpp phase4/src/core/ole/COleConnPtContainer.cpp phase4/src/core/ole/CConnectionPoint.cpp phase4/src/core/ole/CEnumConnections.cpp phase4/src/core/ole/CEnumArrayInterfaces.cpp
    phase4/src/core/d2d/CD2DResource.cpp phase4/src/core/d2d/RuntimeClasses.cpp
    phase4/src/core/runtime/CCmdTargetVirtuals.cpp phase4/src/core/runtime/CCmdTargetDispatch.cpp phase4/src/core/runtime/CCmdTargetInvoke.cpp phase4/src/core/runtime/CCmdTargetEvents.cpp phase4/src/core/runtime/CCmdTargetTypeInfo.cpp phase4/src/core/runtime/CCmdTargetVerbs.cpp phase4/src/core/runtime/CommandRouting.cpp phase4/src/core/runtime/_AFX_THREAD_STATE.cpp phase4/src/core/ole/CEnumOleVerb.cpp phase4/src/core/ole/CEnumArray.cpp phase4/src/core/ole/CTypeLibCache.cpp
    phase4/src/core/ole/COleDispatchImpl.cpp phase4/src/core/ole/OleExceptionTranslation.cpp phase4/src/core/exceptions/COleDispatchException.cpp phase4/src/detail/OleOleexceptionRttiSupport.cpp phase4/src/core/runtime/InterfaceMaps.cpp
    phase4/src/core/exceptions/CExceptionVirtuals.cpp phase4/src/core/exceptions/CSimpleException.cpp phase4/src/core/exceptions/CUserException.cpp phase4/src/detail/CsimpleexceptionRttiSupport.cpp phase4/src/core/dialog/CDataExchangeLifecycle.cpp phase4/src/core/window/CWndDataExchange.cpp phase4/src/core/runtime/DdxExchange.cpp
    phase4/src/core/collections/NativeArrayStorage.cpp phase4/src/core/collections/CByteArray.cpp phase4/src/core/collections/CObArray.cpp phase4/src/core/collections/CMapWordToObStorage.cpp phase4/src/core/collections/CMapWordToOb.cpp phase4/src/featurepack/controls/CMFCControlContainer.cpp phase4/src/core/window/CWndDlgInit.cpp
    phase4/src/core/app/CWinThreadVirtualDispatch.cpp
    phase4/src/core/dialog/CDialogLifetime.cpp phase4/src/core/dialog/CDialogInitialization.cpp phase4/src/core/dialog/CDialogVirtuals.cpp phase4/src/core/dialog/AfxHelpEnabled.cpp
    phase4/src/core/gdi/CImageList.cpp phase4/src/core/gdi/CImageListLifetime.cpp phase4/src/core/controls/CComboBox.cpp phase4/src/featurepack/controls/CMFCFontComboBox.cpp phase4/src/featurepack/controls/CMFCFontComboBoxLifetime.cpp phase4/src/featurepack/controls/StaticData.cpp phase4/src/featurepack/toolbar/StaticData.cpp phase4/src/featurepack/toolbar/CMFCToolBarFontComboBox.cpp phase4/src/featurepack/toolbar/CMFCToolBarComboBoxButton.cpp phase4/src/featurepack/toolbar/CMFCToolBarButtonLifetime.cpp phase4/src/detail/ToolbarComboState.cpp phase4/src/core/collections/CStringList.cpp phase4/src/core/collections/CStringArray.cpp
    phase4/src/featurepack/controls/CTagManager.cpp phase4/src/featurepack/controls/CTagManagerProperties.cpp phase4/src/detail/CollectionsStringsSupport.cpp
    phase4/src/core/collections/CMapPtrToPtrStorage.cpp phase4/src/core/collections/CMapPtrToPtr.cpp
    phase4/src/core/db/CRecordsetFieldStorage.cpp
    phase4/src/core/db/CFieldExchangeLifecycle.cpp
    phase4/src/featurepack/ribbon/CMFCRibbonCmdUI.cpp phase4/src/detail/CMFCRibbonCmdUISupport.cpp
    phase4/src/core/cmdui/CCmdUI.cpp phase4/src/core/cmdui/COleCmdUI.cpp phase4/src/core/cmdui/CTestCmdUI.cpp phase4/src/core/cmdui/CToolCmdUI.cpp
    phase4/src/core/window/CWndMessageMap.cpp phase4/src/core/window/WindowMessageMapDispatch.cpp phase4/src/core/runtime/CommandMessageDispatch.cpp phase4/src/core/gdi/CDC.cpp phase4/src/core/gdi/CDCMessageSupport.cpp
    phase4/src/core/window/CWndDynamicLayout.cpp phase4/src/featurepack/controls/CMFCDynamicLayout.cpp phase4/src/featurepack/controls/CMFCDynamicLayoutLifetime.cpp phase4/src/featurepack/controls/CMFCDynamicLayoutData.cpp phase4/src/detail/CMFCDynamicLayoutDataSupport.cpp
    phase4/src/core/window/CWndMessageDispatch.cpp phase4/src/core/window/CWndInput.cpp phase4/src/core/window/CWndVirtuals.cpp phase4/src/core/window/WindowMessageContext.cpp phase4/src/core/dialog/DialogMessageCallbacks.cpp phase4/src/core/window/CWndD2D.cpp phase4/src/core/window/CWndAccessibility.cpp
    phase4/src/core/window/CWndAccessibilityInterfaces.cpp phase4/src/core/window/CWndAccessibilityProxy.cpp
    phase4/src/core/runtime/D2DMessages.cpp
    phase4/src/core/d2d/_AFX_D2D_STATE.cpp phase4/src/core/d2d/CDCRenderTarget.cpp
    phase4/src/core/d2d/CHwndRenderTarget.cpp phase4/src/core/d2d/CBitmapRenderTarget.cpp
    phase4/src/core/d2d/CD2DGeometry.cpp phase4/src/core/d2d/CD2DPathGeometry.cpp
    phase4/src/core/d2d/CD2DLayer.cpp phase4/src/core/d2d/CD2DMesh.cpp
    phase4/src/core/d2d/CD2DBrush.cpp phase4/src/core/d2d/CD2DSolidColorBrush.cpp
    phase4/src/core/d2d/CD2DBitmap.cpp phase4/src/core/d2d/CD2DBitmapBrush.cpp
    phase4/src/core/d2d/CD2DGradientBrush.cpp phase4/src/core/d2d/CD2DLinearGradientBrush.cpp
    phase4/src/core/d2d/CD2DRadialGradientBrush.cpp phase4/src/core/runtime/AfxFindResourceHandle.cpp
    phase4/src/core/d2d/CD2DPointF.cpp phase4/src/core/d2d/CD2DSizeF.cpp
    phase4/src/core/d2d/CD2DRectF.cpp phase4/src/core/d2d/CD2DEllipse.cpp
    phase4/src/core/d2d/CD2DRoundedRect.cpp)
OBJECTS=()
for source in "${SOURCES[@]}"; do
    object="$BUILD_DIR/$(basename "${source%.cpp}").obj"
    if [[ "$source" == phase4/src/featurepack/toolbar/StaticData.cpp ]]; then object="$BUILD_DIR/ToolbarStaticData.obj"; fi
    clang-cl "${FLAGS[@]}" /c "$ROOT/$source" /Fo"$object"
    OBJECTS+=("$object")
done
# The DLL generator shortens long internal names. Check the actual generator
# aliases, rather than trusting Symbol markers or direct-call tests alone.
python3 - "$ROOT" "$BUILD_DIR" <<'PY'
import re
import subprocess
import sys
from pathlib import Path

root = Path(sys.argv[1])
sys.path.insert(0, str(root / 'tools'))
import gen_weak_stubs
for name in ('CRenderTarget', 'CD2DTextFormat', 'CD2DTextLayout',
             'CDCRenderTarget', 'CHwndRenderTarget', 'CBitmapRenderTarget',
             'CD2DResource', 'CD2DGeometry', 'CD2DPathGeometry',
             'CD2DLayer', 'CD2DMesh', 'CD2DBrush', 'CD2DSolidColorBrush',
             'CD2DBitmap', 'CD2DBitmapBrush', 'CD2DGradientBrush',
             'CD2DLinearGradientBrush', 'CD2DRadialGradientBrush'):
    obj = 'test_rendertarget_drawing' if name in (
        'CRenderTarget', 'CD2DTextFormat', 'CD2DTextLayout') else name
    defined = {line.split()[-1] for line in subprocess.check_output(
        ['llvm-nm', '--defined-only', str(Path(sys.argv[2]) / (obj + '.obj'))],
        text=True).splitlines() if line.split()}
    source = (root / 'phase4/src/core/d2d' / (name + '.cpp')).read_text()
    for symbol in re.findall(r'^// Symbol: (.+)$', source, re.M):
        alias = gen_weak_stubs.generate_stub_name(symbol)
        if name == 'CRenderTarget' and '?VerifyResource@' in symbol:
            continue  # Only this one thunk is doubled in the forwarding test.
        assert alias in defined, 'Missing compiled export alias: ' + alias
for path, obj in (('core/db/CRecordsetFieldStorage.cpp', 'CRecordsetFieldStorage'),
                  ('core/collections/CMapPtrToPtr.cpp', 'CMapPtrToPtr'),
                  ('core/db/CFieldExchangeLifecycle.cpp', 'CFieldExchangeLifecycle'),
                  ('featurepack/ribbon/CMFCRibbonCmdUI.cpp', 'CMFCRibbonCmdUI'),
                  ('core/gdi/CImageListLifetime.cpp', 'CImageListLifetime'),
                  ('core/controls/CComboBox.cpp', 'CComboBox'),
                  ('featurepack/controls/CMFCFontComboBox.cpp', 'CMFCFontComboBox'),
                  ('featurepack/controls/CMFCFontComboBoxLifetime.cpp', 'CMFCFontComboBoxLifetime'),
                  ('featurepack/controls/CTagManagerProperties.cpp', 'CTagManagerProperties'),
                  ('core/app/CWinThreadVirtualDispatch.cpp', 'CWinThreadVirtualDispatch'),
                  ('core/dialog/CDialogLifetime.cpp', 'CDialogLifetime'),
                  ('core/dialog/CDialogInitialization.cpp', 'CDialogInitialization'),
                  ('core/dialog/CDialogVirtuals.cpp', 'CDialogVirtuals'),
                  ('core/dialog/AfxHelpEnabled.cpp', 'AfxHelpEnabled'),
                  ('core/collections/CByteArray.cpp', 'CByteArray'),
                  ('core/collections/CObArray.cpp', 'CObArray'),
                  ('core/collections/CMapWordToOb.cpp', 'CMapWordToOb'),
                  ('featurepack/controls/CMFCControlContainer.cpp', 'CMFCControlContainer'),
                  ('core/window/CWndDlgInit.cpp', 'CWndDlgInit'),
                  ('core/exceptions/CSimpleException.cpp', 'CSimpleException'),
                  ('core/exceptions/CUserException.cpp', 'CUserException'),
                  ('core/dialog/CDataExchangeLifecycle.cpp', 'CDataExchangeLifecycle'),
                  ('core/window/CWndDataExchange.cpp', 'CWndDataExchange'),
                  ('core/runtime/CCmdTargetVirtuals.cpp', 'CCmdTargetVirtuals'),
                  ('core/runtime/CCmdTargetDispatch.cpp', 'CCmdTargetDispatch'),
                  ('core/runtime/CCmdTargetInvoke.cpp', 'CCmdTargetInvoke'),
                  ('core/runtime/CCmdTargetEvents.cpp', 'CCmdTargetEvents'),
                  ('core/runtime/CCmdTargetTypeInfo.cpp', 'CCmdTargetTypeInfo'),
                  ('core/runtime/CCmdTargetVerbs.cpp', 'CCmdTargetVerbs'),
                  ('core/runtime/CommandRouting.cpp', 'CommandRouting'),
                  ('core/runtime/_AFX_THREAD_STATE.cpp', '_AFX_THREAD_STATE'),
                  ('core/ole/CEnumOleVerb.cpp', 'CEnumOleVerb'),
                  ('core/ole/CEnumArray.cpp', 'CEnumArray'),
                  ('core/runtime/AfxFindResourceHandle.cpp', 'AfxFindResourceHandle'),
                  ('core/ole/CTypeLibCache.cpp', 'CTypeLibCache'),
                  ('core/ole/COleConnPtContainer.cpp', 'COleConnPtContainer'),
                  ('core/ole/CConnectionPoint.cpp', 'CConnectionPoint'),
                  ('core/ole/CEnumConnections.cpp', 'CEnumConnections'),
                  ('core/ole/CEnumArrayInterfaces.cpp', 'CEnumArrayInterfaces'),
                  ('core/ole/COleDispatchImpl.cpp', 'COleDispatchImpl'),
                  ('core/exceptions/COleDispatchException.cpp', 'COleDispatchException'),
                  ('detail/OleOleexceptionRttiSupport.cpp', 'OleOleexceptionRttiSupport'),
                  ('core/window/CWndD2D.cpp', 'CWndD2D'),
                  ('core/window/WindowMessageContext.cpp', 'WindowMessageContext'),
                  ('core/window/CWndInput.cpp', 'CWndInput'),
                  ('core/window/CWndDynamicLayout.cpp', 'CWndDynamicLayout'),
                  ('core/window/CWndMessageMap.cpp', 'CWndMessageMap'),
                  ('core/cmdui/CCmdUI.cpp', 'CCmdUI'),
                  ('core/cmdui/COleCmdUI.cpp', 'COleCmdUI'),
                  ('core/cmdui/CTestCmdUI.cpp', 'CTestCmdUI'),
                  ('core/cmdui/CToolCmdUI.cpp', 'CToolCmdUI'),
                  ('core/window/WindowMessageMapDispatch.cpp', 'WindowMessageMapDispatch'),
                  ('core/gdi/CDCMessageSupport.cpp', 'CDCMessageSupport'),
                  ('featurepack/controls/CMFCDynamicLayout.cpp', 'CMFCDynamicLayout'),
                  ('featurepack/controls/CMFCDynamicLayoutLifetime.cpp', 'CMFCDynamicLayoutLifetime'),
                  ('featurepack/controls/CMFCDynamicLayoutData.cpp', 'CMFCDynamicLayoutData'),
                  ('core/dialog/DialogMessageCallbacks.cpp', 'DialogMessageCallbacks'),
                  ('core/window/CWndAccessibility.cpp', 'CWndAccessibility'),
                  ('core/window/CWndAccessibilityInterfaces.cpp', 'CWndAccessibilityInterfaces'),
                  ('core/window/CWndAccessibilityProxy.cpp', 'CWndAccessibilityProxy'),
                  ('core/runtime/D2DMessages.cpp', 'D2DMessages')):
    defined = {line.split()[-1] for line in subprocess.check_output(
        ['llvm-nm', '--defined-only', str(Path(sys.argv[2]) / (obj + '.obj'))],
        text=True).splitlines() if line.split()}
    for symbol in re.findall(r'^// Symbol: (.+)$', (root / 'phase4/src' / path).read_text(), re.M):
        assert gen_weak_stubs.generate_stub_name(symbol) in defined, 'Missing window D2D alias: ' + symbol
print('Render target and resource export aliases match the DLL generator')
PY
lld-link /OUT:"$BUILD_DIR/test_rendertarget_drawing.exe" /SUBSYSTEM:CONSOLE /OPT:REF /errorlimit:0 \
    "/LIBPATH:$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/lib/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/ucrt/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/um/x64" \
    "${OBJECTS[@]}" comctl32.lib kernel32.lib user32.lib gdi32.lib ole32.lib oleacc.lib oleaut32.lib uuid.lib d2d1.lib windowscodecs.lib
# Link a second executable against production verification and wrapper access.
# The forwarding test above deliberately doubles those calls for COM ABI probes.
NATIVE_OBJECTS=("${OBJECTS[@]:1}")
for source in tests/test_d2d_resources.cpp phase4/src/core/d2d/CRenderTarget.cpp \
    phase4/src/core/d2d/CD2DTextFormat.cpp phase4/src/core/d2d/CD2DTextLayout.cpp; do
    object="$BUILD_DIR/$(basename "${source%.cpp}").obj"
    clang-cl "${FLAGS[@]}" /c "$ROOT/$source" /Fo"$object"
    NATIVE_OBJECTS+=("$object")
done
(
    cd "$BUILD_DIR"
    widl --win64 -t -o typelib_cache_fixture.tlb "$ROOT/tests/typelib_cache_fixture.idl"
    llvm-rc /I "$BUILD_DIR" /fo typelib_cache_fixture.res "$ROOT/tests/typelib_cache_fixture.rc"
)
lld-link /OUT:"$BUILD_DIR/test_d2d_resources.exe" /SUBSYSTEM:CONSOLE /OPT:REF /errorlimit:0 \
    "/LIBPATH:$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/lib/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/ucrt/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/um/x64" \
    "${NATIVE_OBJECTS[@]}" "$BUILD_DIR/typelib_cache_fixture.res" comctl32.lib kernel32.lib user32.lib gdi32.lib ole32.lib oleacc.lib oleaut32.lib uuid.lib d2d1.lib windowscodecs.lib
RUNNER=(wine)
if [[ -z "${DISPLAY:-}" ]] && command -v xvfb-run >/dev/null 2>&1; then
    RUNNER=(xvfb-run -a wine)
fi
WINEDEBUG="${WINEDEBUG:--all}" timeout 45 "${RUNNER[@]}" "$BUILD_DIR/test_rendertarget_drawing.exe"

WINEDEBUG="${WINEDEBUG:--all}" timeout 45 "${RUNNER[@]}" "$BUILD_DIR/test_d2d_resources.exe"
