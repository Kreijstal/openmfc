#include <Windows.h>
#include <fstream>
#include <string>

static HMODULE (WINAPI *real_GetModuleHandleW)(LPCWSTR) = GetModuleHandleW;
static std::wstring g_path;
static bool g_hooked = false;

void log_call(const std::wstring& msg) {
    if (g_path.empty()) return;
    std::ofstream f(std::string(g_path.begin(), g_path.end()), std::ios::app | std::ios::binary);
    f << msg.c_str() << "\n";
}

HMODULE WINAPI hook_GetModuleHandleW(LPCWSTR name) {
    log_call(L"GetModuleHandleW" + std::wstring(L";") + (name ? name : L""));
    return real_GetModuleHandleW(name);
}

void install_hook() {
    if (g_hooked) return;
    // Patch IAT of current module to point to our hook.
    HMODULE self = GetModuleHandleW(nullptr);
    auto* base = reinterpret_cast<BYTE*>(self);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    auto dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    auto* imp = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base + dir.VirtualAddress);
    for (; imp->Name; ++imp) {
        auto* thunk = reinterpret_cast<IMAGE_THUNK_DATA*>(base + imp->FirstThunk);
        for (; thunk->u1.Function; ++thunk) {
            auto* fnPtr = reinterpret_cast<void**>(&thunk->u1.Function);
            if (*fnPtr == reinterpret_cast<void*>(GetModuleHandleW)) {
                DWORD old;
                VirtualProtect(fnPtr, sizeof(void*), PAGE_READWRITE, &old);
                *fnPtr = reinterpret_cast<void*>(&hook_GetModuleHandleW);
                VirtualProtect(fnPtr, sizeof(void*), old, &old);
                g_hooked = true;
                return;
            }
        }
    }
}

extern "C" __declspec(dllexport) void tracer_init(const wchar_t* out_path) {
    g_path = out_path ? out_path : L"trace.json";
    install_hook();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        wchar_t buf[MAX_PATH];
        if (GetEnvironmentVariableW(L"TRACE_OUT", buf, MAX_PATH)) {
            g_path = buf;
        }
        install_hook();
    }
    return TRUE;
}
