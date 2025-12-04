#include "shared_types.h"
#include <afxwin.h>
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

// Minimal inline patching of _CxxThrowException to capture ThrowInfo.
namespace {
using ThrowFn = void(__stdcall*)(void*, ThrowInfo*);

ThrowFn g_orig_throw = nullptr;
ExceptionDump g_last_dump{};
BYTE g_saved_bytes[16] = {};

void write_jump(void* target, void* dest) {
    DWORD oldProtect;
    VirtualProtect(target, 16, PAGE_EXECUTE_READWRITE, &oldProtect);
    BYTE* p = static_cast<BYTE*>(target);
    p[0] = 0x48; // mov rax, imm64
    p[1] = 0xB8;
    *reinterpret_cast<void**>(p + 2) = dest;
    p[10] = 0xFF; // jmp rax
    p[11] = 0xE0;
    for (int i = 12; i < 16; ++i) p[i] = 0x90;
    VirtualProtect(target, 16, oldProtect, &oldProtect);
}

void restore_bytes(void* target) {
    DWORD oldProtect;
    VirtualProtect(target, 16, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(target, g_saved_bytes, sizeof(g_saved_bytes));
    VirtualProtect(target, 16, oldProtect, &oldProtect);
}

void __stdcall my_throw(void* obj, ThrowInfo* info) {
    g_last_dump.throw_info = info;
    g_last_dump.obj = obj;
    restore_bytes(reinterpret_cast<void*>(g_orig_throw));
    g_orig_throw(obj, info);
}

void hook_throw() {
    HMODULE vcrt = GetModuleHandleW(L"VCRUNTIME140.dll");
    if (!vcrt) vcrt = GetModuleHandleW(L"vcruntime140.dll");
    if (!vcrt) return;
    auto sym = GetProcAddress(vcrt, "_CxxThrowException");
    if (!sym) return;
    g_orig_throw = reinterpret_cast<ThrowFn>(sym);
    memcpy(g_saved_bytes, sym, sizeof(g_saved_bytes));
    write_jump(sym, reinterpret_cast<void*>(&my_throw));
}
}

static std::string hex_bytes(const void* ptr, size_t len) {
    std::ostringstream oss;
    auto* p = static_cast<const unsigned char*>(ptr);
    for (size_t i = 0; i < len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(p[i]);
    }
    return oss.str();
}

struct CatchableEntry {
    std::string name;
    int offset = 0;
};

struct ExceptionNode {
    std::string mangled_name;
    std::string throw_info_bytes;
    std::vector<CatchableEntry> catchables;
};

struct LayoutInfo {
    int size = 0;
    std::vector<std::pair<std::string, int>> members;
};

struct TypeInfoView {
    std::string name;
};

TypeInfoView view_typedesc(void* td) {
    if (!td) return {};
    const char* name = *reinterpret_cast<const char* const*>(static_cast<char*>(td) + sizeof(void*) * 2);
    if (reinterpret_cast<uintptr_t>(name) > 0x10000) {
        return {std::string(name)};
    }
    return {};
}

ExceptionNode dump_exception(const char* label) {
    ExceptionNode node;
    node.mangled_name = label;
    auto* ti = reinterpret_cast<const ThrowInfo*>(g_last_dump.throw_info);
    if (!ti) return node;
    node.throw_info_bytes = hex_bytes(ti, 64);
    auto* arr = ti->pCatchableTypeArray;
    if (arr && arr->nCatchableTypes > 0) {
        for (int i = 0; i < arr->nCatchableTypes; ++i) {
            auto* ct = arr->types[i];
            if (!ct) continue;
            auto tv = view_typedesc(ct->pType);
            CatchableEntry ent;
            ent.name = tv.name;
            ent.offset = ct->thisDisplacement.mdisp;
            node.catchables.push_back(ent);
        }
    }
    return node;
}

void harvest_layouts(std::vector<std::pair<std::string, LayoutInfo>>& layouts) {
    LayoutInfo obj; obj.size = static_cast<int>(sizeof(CObject));
    layouts.push_back({"CObject", obj});

    LayoutInfo exc; exc.size = static_cast<int>(sizeof(CException));
    layouts.push_back({"CException", exc});

    LayoutInfo mem; mem.size = static_cast<int>(sizeof(CMemoryException));
    layouts.push_back({"CMemoryException", mem});

    LayoutInfo file; file.size = static_cast<int>(sizeof(CFileException));
    layouts.push_back({"CFileException", file});

    LayoutInfo wnd; wnd.size = static_cast<int>(sizeof(CWnd));
    wnd.members.push_back({"m_hWnd", static_cast<int>(offsetof(CWnd, m_hWnd))});
    wnd.members.push_back({"m_pfnWndProc", static_cast<int>(offsetof(CWnd, m_pfnWndProc))});
    layouts.push_back({"CWnd", wnd});

    LayoutInfo app; app.size = static_cast<int>(sizeof(CWinApp));
    layouts.push_back({"CWinApp", app});
}

std::string quote(const std::string& s) {
    std::ostringstream oss;
    oss << '"';
    for (char c : s) {
        if (c == '"' || c == '\\') oss << '\\' << c;
        else if (c == '\n') oss << "\\n";
        else oss << c;
    }
    oss << '"';
    return oss.str();
}

void write_exceptions(const std::vector<std::pair<std::string, ExceptionNode>>& ex, const std::string& path) {
    std::ostringstream oss;
    oss << "{\n";
    for (size_t i = 0; i < ex.size(); ++i) {
        const auto& kv = ex[i];
        oss << "  " << quote(kv.first) << ": {\n";
        oss << "    \"mangled_name\": " << quote(kv.second.mangled_name) << ",\n";
        oss << "    \"throw_info_bytes\": " << quote(kv.second.throw_info_bytes) << ",\n";
        oss << "    \"catchable_types\": [";
        for (size_t j = 0; j < kv.second.catchables.size(); ++j) {
            const auto& ct = kv.second.catchables[j];
            oss << "{\"name\": " << quote(ct.name) << ", \"offset\": " << ct.offset << "}";
            if (j + 1 < kv.second.catchables.size()) oss << ", ";
        }
        oss << "]\n  }";
        if (i + 1 < ex.size()) oss << ",";
        oss << "\n";
    }
    oss << "}\n";
    std::ofstream(path, std::ios::binary) << oss.str();
}

void write_layouts(const std::vector<std::pair<std::string, LayoutInfo>>& layouts, const std::string& path) {
    std::ostringstream oss;
    oss << "{\n";
    for (size_t i = 0; i < layouts.size(); ++i) {
        const auto& kv = layouts[i];
        oss << "  " << quote(kv.first) << ": {\n";
        oss << "    \"sizeof\": " << kv.second.size << ",\n";
        oss << "    \"members\": {";
        for (size_t j = 0; j < kv.second.members.size(); ++j) {
            const auto& m = kv.second.members[j];
            oss << quote(m.first) << ": " << m.second;
            if (j + 1 < kv.second.members.size()) oss << ", ";
        }
        oss << "}\n  }";
        if (i + 1 < layouts.size()) oss << ",";
        oss << "\n";
    }
    oss << "}\n";
    std::ofstream(path, std::ios::binary) << oss.str();
}

int main() {
    hook_throw();

    std::vector<std::pair<std::string, ExceptionNode>> exceptions;
    try {
        throw CMemoryException();
    } catch (...) {
        exceptions.push_back({"CMemoryException", dump_exception(".?AVCMemoryException@@")});
    }

    try {
        throw CFileException();
    } catch (...) {
        exceptions.push_back({"CFileException", dump_exception(".?AVCFileException@@")});
    }

    std::vector<std::pair<std::string, LayoutInfo>> layouts;
    harvest_layouts(layouts);

    write_exceptions(exceptions, "exceptions.json");
    write_layouts(layouts, "layouts.json");
    return 0;
}
