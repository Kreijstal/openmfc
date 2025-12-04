#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

static HMODULE (WINAPI *real_GetModuleHandleW)(LPCWSTR) = NULL;
static char trace_path[MAX_PATH] = "candidate_trace.json";

static void log_call(const char* msg) {
    FILE* f = fopen(trace_path, "a");
    if (!f) return;
    fprintf(f, "%s\n", msg);
    fclose(f);
}

HMODULE WINAPI GetModuleHandleW(LPCWSTR lpModuleName) {
    if (!real_GetModuleHandleW) {
        HMODULE k32 = GetModuleHandleW(L"kernel32.dll");
        real_GetModuleHandleW = (HMODULE (WINAPI*)(LPCWSTR))GetProcAddress(k32, "GetModuleHandleW");
    }
    log_call("GetModuleHandleW");
    return real_GetModuleHandleW ? real_GetModuleHandleW(lpModuleName) : NULL;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    (void)hinstDLL; (void)lpReserved;
    if (fdwReason == DLL_PROCESS_ATTACH) {
        char* env = getenv("TRACE_OUT");
        if (env) {
            strncpy(trace_path, env, sizeof(trace_path)-1);
            trace_path[sizeof(trace_path)-1] = '\0';
        }
    }
    return TRUE;
}
