#include "detail/WindowActivationSupport.h"

// Release-x64 isolation-aware window helpers (mfc140u RVAs 0x1c294,
// 0x1c45c, 0x2934d4 and 0x2935f0). Both wrappers share this module cache.
namespace openmfc { namespace detail {
WindowActivationState windowActivation;
BOOL InitializeWindowActivation() {
    auto& state=windowActivation;
    if(state.unavailable || state.context!=INVALID_HANDLE_VALUE)return TRUE;
    ACTIVATION_CONTEXT_BASIC_INFORMATION information;
    if(!::QueryActCtxW(0x80000010u,&state.context,nullptr,ActivationContextBasicInformation,
                      &information,sizeof(information),nullptr))return FALSE;
    HANDLE context=information.hActCtx;
    if(!context) {
        HMODULE module;
        if(!::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<const wchar_t*>(&state.context),&module))return FALSE;
        wchar_t filename[MAX_PATH+1];
        DWORD length=::GetModuleFileNameW(module,filename,MAX_PATH+1);
        if(!length)return FALSE;
        if(length>=MAX_PATH+1){::SetLastError(ERROR_BUFFER_OVERFLOW);return FALSE;}
        ACTCTXW description;
        description.cbSize=sizeof(description);
        description.dwFlags=ACTCTX_FLAG_RESOURCE_NAME_VALID|ACTCTX_FLAG_HMODULE_VALID;
        description.lpSource=filename;
        description.lpResourceName=MAKEINTRESOURCEW(3);
        description.hModule=module;
        context=::CreateActCtxW(&description);
        if(context==INVALID_HANDLE_VALUE) {
            DWORD error=::GetLastError();
            if(error-ERROR_RESOURCE_DATA_NOT_FOUND>3u && error-ERROR_FILE_NOT_FOUND>1u)return FALSE;
            context=nullptr;
        }
    }
    state.context=context;
    ULONG_PTR cookie=0;
    if(::ActivateActCtx(context,&cookie)) {
        __try {
            ACTCTX_SECTION_KEYED_DATA section;
            section.cbSize=sizeof(section);
            if(::FindActCtxSectionStringW(0,nullptr,ACTIVATION_CONTEXT_SECTION_DLL_REDIRECTION,
                                         L"Comctl32.dll",&section))::LoadLibraryW(L"Comctl32.dll");
        } __finally {::DeactivateActCtx(0,cookie);}
    }
    return TRUE;
}
BOOL ActivateWindowContext(ULONG_PTR* cookie) {
    auto& state=windowActivation;
    if(state.cleanupStarted)::OutputDebugStringA("IsolationAware function called after IsolationAwareCleanup\n");
    if(state.unavailable)return TRUE;
    if((state.cleanupStarted || InitializeWindowActivation()) && ::ActivateActCtx(state.context,cookie))return TRUE;
    DWORD error=::GetLastError();
    if(error==ERROR_MOD_NOT_FOUND || error==ERROR_PROC_NOT_FOUND || error==ERROR_CALL_NOT_IMPLEMENTED ||
       error==ERROR_INVALID_FUNCTION || error==ERROR_NOT_SUPPORTED) {
        state.unavailable=TRUE;
        return TRUE;
    }
    return FALSE;
}
namespace {
void FinishContext(ULONG_PTR cookie,BOOL failed) {
    if(windowActivation.bypass && windowActivation.unavailable)return;
    DWORD error=failed?::GetLastError():0;
    ::DeactivateActCtx(0,cookie);
    if(failed)::SetLastError(error);
}
}
HWND ContextCreateWindow(CREATESTRUCTW create) {
    ULONG_PTR cookie=0;
    HWND result=nullptr;
    if(!windowActivation.bypass && !windowActivation.unavailable && !ActivateWindowContext(&cookie))return nullptr;
    // The native isolation-aware wrapper uses SEH finally, including for
    // foreign API exceptions and structured exceptions.
    __try {
        result=::CreateWindowExW(create.dwExStyle,create.lpszClass,create.lpszName,create.style,
            create.x,create.y,create.cx,create.cy,create.hwndParent,create.hMenu,create.hInstance,create.lpCreateParams);
    } __finally {FinishContext(cookie,result==nullptr);}
    return result;
}
BOOL ContextMenuClassInfo(WNDCLASSEXW* info) {
    ULONG_PTR cookie=0;
    BOOL result=FALSE;
    if(!windowActivation.bypass && !windowActivation.unavailable && !ActivateWindowContext(&cookie))return FALSE;
    __try {result=::GetClassInfoExW(nullptr,L"#32768",info);}
    __finally {FinishContext(cookie,!result);}
    return result;
}
} }
