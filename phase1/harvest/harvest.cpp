#ifndef _AFXDLL
#define _AFXDLL
#endif
#include "shared_types.h"
#include <afxwin.h>
#include <Windows.h>
#include <algorithm>
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

// ---------------------------------------------------------------------------
// VTable slot harvesting
//
// MFC does not export its vtables as named symbols (no ??_7Class@@6B@ exports),
// so we cannot GetProcAddress a vtable. Instead we read the vptr off a *live*
// instance (offset 0), walk the slots, and resolve each slot's function address
// against mfc140u.dll's export table -- the methods ARE exported even though the
// vtables are not. The output schema matches what tools/gen_vtable.py consumes.
// ---------------------------------------------------------------------------
struct VTableSlot {
    int index = 0;
    uintptr_t rva = 0;
    std::string symbol;
};

struct VTableInfo {
    std::string name;
    std::vector<VTableSlot> slots;
};

namespace {
std::vector<std::pair<uintptr_t, std::string>> g_exportMap; // sorted by address
uintptr_t g_modBase = 0;
uintptr_t g_modEnd = 0;

void build_export_map(const wchar_t* moduleName) {
    HMODULE h = GetModuleHandleW(moduleName);
    if (!h) return;
    auto base = reinterpret_cast<uintptr_t>(h);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return;
    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    g_modBase = base;
    g_modEnd = base + nt->OptionalHeader.SizeOfImage;
    const auto& dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (!dir.VirtualAddress) return;
    auto* exp = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(base + dir.VirtualAddress);
    auto* names = reinterpret_cast<DWORD*>(base + exp->AddressOfNames);
    auto* ords = reinterpret_cast<WORD*>(base + exp->AddressOfNameOrdinals);
    auto* funcs = reinterpret_cast<DWORD*>(base + exp->AddressOfFunctions);
    g_exportMap.reserve(exp->NumberOfNames);
    for (DWORD i = 0; i < exp->NumberOfNames; ++i) {
        const char* nm = reinterpret_cast<const char*>(base + names[i]);
        uintptr_t addr = base + funcs[ords[i]];
        g_exportMap.push_back({addr, nm});
    }
    std::sort(g_exportMap.begin(), g_exportMap.end());
}

std::string resolve_addr(uintptr_t addr) {
    if (g_exportMap.empty()) return "?";
    auto it = std::lower_bound(
        g_exportMap.begin(), g_exportMap.end(), addr,
        [](const std::pair<uintptr_t, std::string>& e, uintptr_t a) { return e.first < a; });
    if (it != g_exportMap.end() && it->first == addr) return it->second; // exact export
    if (it != g_exportMap.begin()) {                                     // nearest lower + offset
        --it;
        std::ostringstream o;
        o << it->second << "+0x" << std::hex << (addr - it->first);
        return o.str();
    }
    return "?";
}

VTableInfo harvest_vtable(const std::string& className, const void* instance, int maxSlots = 64) {
    VTableInfo vt;
    vt.name = className;
    if (!instance || !g_modBase) return vt;
    auto* vptr = *reinterpret_cast<void* const*>(instance);
    if (!vptr) return vt;
    auto* slots = reinterpret_cast<void* const*>(vptr);
    for (int i = 0; i < maxSlots; ++i) {
        auto a = reinterpret_cast<uintptr_t>(slots[i]);
        if (a < g_modBase || a >= g_modEnd) break; // pointer left the module => end of vtable
        VTableSlot s;
        s.index = i;
        s.rva = a - g_modBase;
        s.symbol = resolve_addr(a);
        vt.slots.push_back(s);
    }
    return vt;
}
} // namespace

void write_vtables(const std::vector<VTableInfo>& vts, const std::string& path) {
    std::ostringstream oss;
    oss << "{\n  \"classes\": [\n";
    for (size_t i = 0; i < vts.size(); ++i) {
        oss << "    {\n      \"name\": " << quote(vts[i].name) << ",\n      \"slots\": [\n";
        for (size_t j = 0; j < vts[i].slots.size(); ++j) {
            const auto& s = vts[i].slots[j];
            oss << "        {\"index\": " << s.index
                << ", \"rva\": \"0x" << std::hex << s.rva << std::dec
                << "\", \"symbol\": " << quote(s.symbol) << "}";
            if (j + 1 < vts[i].slots.size()) oss << ",";
            oss << "\n";
        }
        oss << "      ]\n    }";
        if (i + 1 < vts.size()) oss << ",";
        oss << "\n";
    }
    oss << "  ]\n}\n";
    std::ofstream(path, std::ios::binary) << oss.str();
}

int main() {
    hook_throw();
    build_export_map(L"mfc140u.dll");
    std::vector<VTableInfo> vtables;

    std::vector<std::pair<std::string, ExceptionNode>> exceptions;
    try {
        AfxThrowMemoryException();
    } catch (CMemoryException* e) {
        exceptions.push_back({"CMemoryException", dump_exception(".?AVCMemoryException@@")});
        if (e) vtables.push_back(harvest_vtable("CMemoryException", e));
        if (e) e->Delete();
    }

    try {
        AfxThrowFileException(CFileException::generic, 0, nullptr);
    } catch (CFileException* e) {
        exceptions.push_back({"CFileException", dump_exception(".?AVCFileException@@")});
        if (e) vtables.push_back(harvest_vtable("CFileException", e));
        if (e) e->Delete();
    }

    try {
        AfxThrowArchiveException(CArchiveException::generic, nullptr);
    } catch (CArchiveException* e) {
        if (e) vtables.push_back(harvest_vtable("CArchiveException", e));
        if (e) e->Delete();
    }

    std::vector<std::pair<std::string, LayoutInfo>> layouts;
    harvest_layouts(layouts);

    write_exceptions(exceptions, "exceptions.json");
    write_layouts(layouts, "layouts.json");
    write_vtables(vtables, "vtable_slots.json");
    return 0;
}
