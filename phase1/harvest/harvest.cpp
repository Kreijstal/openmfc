#ifndef _AFXDLL
#define _AFXDLL
#endif
#include "shared_types.h"
#include <afxwin.h>
#include <afxext.h>    // CControlBar/CToolBar/CStatusBar/CSplitterWnd
#include <afxadv.h>    // CDockState/CRecentFileList/CSharedFile
#include <afxole.h>    // COle* document/server/client + factory + data transfer
#include <afxdisp.h>   // COleVariant/COleCurrency/COleDateTime(Span)/COleException
#include <Windows.h>
#include <algorithm>
#include <cstdio>
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

    // --- Batch 2: sizeof ground truth for the classes whose RTTI descriptors /
    // value methods are currently blocked by unknown layout. sizeof() is a pure
    // compile-time probe (no instance needed), so this is safe in CI even for
    // abstract or window-backed classes. Member offsets are added only where the
    // member is public (offsetof requires accessibility).
#define ADD_SIZE(C) do { LayoutInfo _l; _l.size = static_cast<int>(sizeof(C)); \
    layouts.push_back({#C, _l}); } while (0)

    // Command/target + thread core
    ADD_SIZE(CCmdTarget);
    ADD_SIZE(CWinThread);

    // Document / view / frame hierarchy
    ADD_SIZE(CDocument);
    ADD_SIZE(CDocTemplate);
    ADD_SIZE(CSingleDocTemplate);
    ADD_SIZE(CMultiDocTemplate);
    ADD_SIZE(CDocManager);
    ADD_SIZE(CView);
    ADD_SIZE(CCtrlView);
    ADD_SIZE(CScrollView);
    ADD_SIZE(CFormView);
    ADD_SIZE(CFrameWnd);
    ADD_SIZE(CMDIFrameWnd);
    ADD_SIZE(CMDIChildWnd);
    ADD_SIZE(CDialog);

    // Control bars / splitter
    ADD_SIZE(CControlBar);
    ADD_SIZE(CStatusBar);
    ADD_SIZE(CToolBar);
    ADD_SIZE(CSplitterWnd);
    ADD_SIZE(CDockState);

    // OLE document / server / client / transfer
    ADD_SIZE(COleDocument);
    ADD_SIZE(COleLinkingDoc);
    ADD_SIZE(COleServerDoc);
    ADD_SIZE(COleServerItem);
    ADD_SIZE(COleClientItem);
    ADD_SIZE(COleDocObjectItem);
    ADD_SIZE(COleObjectFactory);
    ADD_SIZE(COleDataSource);
    ADD_SIZE(COleDropTarget);
    ADD_SIZE(COleDropSource);
    ADD_SIZE(COleDispatchDriver);

    // OLE value types (already partly implemented; pin real sizeof)
    ADD_SIZE(COleVariant);
    ADD_SIZE(COleCurrency);
    { LayoutInfo _l; _l.size = static_cast<int>(sizeof(ATL::COleDateTime));
      layouts.push_back({"COleDateTime", _l}); }
    { LayoutInfo _l; _l.size = static_cast<int>(sizeof(ATL::COleDateTimeSpan));
      layouts.push_back({"COleDateTimeSpan", _l}); }
    ADD_SIZE(COleSafeArray);

    // Misc value/util
    ADD_SIZE(CDynLinkLibrary);
    ADD_SIZE(CRecentFileList);
    ADD_SIZE(CCommandLineInfo);

#undef ADD_SIZE
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
    uintptr_t rva = 0;       // offset within the containing module
    std::string module;      // containing module basename (e.g. "mfc140u.dll")
    std::string symbol;      // "@<ordinal>", "name", or "module+0xrva"
};

struct VTableInfo {
    std::string name;
    std::string note;        // diagnostics when a walk yields nothing useful
    std::vector<VTableSlot> slots;
};

namespace {
std::vector<std::pair<uintptr_t, std::string>> g_exportMap; // sorted by address
uintptr_t g_modBase = 0;
uintptr_t g_modEnd = 0;

// Parse the loaded module's export table. mfc140u.dll exports almost everything
// by ORDINAL (NONAME), so AddressOfNames is sparse -- we therefore index EVERY
// function in AddressOfFunctions by ordinal, then overlay names where present.
// Explicitly use the 64-bit PE headers (IMAGE_NT_HEADERS can resolve to the
// 32-bit variant under some toolchains, which misreads SizeOfImage and the
// export DataDirectory -> empty map + truncated module range).
void build_export_map(const wchar_t* moduleName) {
    HMODULE h = GetModuleHandleW(moduleName);
    if (!h) return;
    auto base = reinterpret_cast<uintptr_t>(h);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return;
    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE ||
        nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) return;
    g_modBase = base;
    g_modEnd = base + nt->OptionalHeader.SizeOfImage;
    const auto& dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (!dir.VirtualAddress) return;
    auto* exp = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(base + dir.VirtualAddress);
    auto* funcs = reinterpret_cast<DWORD*>(base + exp->AddressOfFunctions);
    auto* names = reinterpret_cast<DWORD*>(base + exp->AddressOfNames);
    auto* nameOrds = reinterpret_cast<WORD*>(base + exp->AddressOfNameOrdinals);

    // 1) every function by ordinal -> "@<ordinal>" (handles NONAME)
    for (DWORD i = 0; i < exp->NumberOfFunctions; ++i) {
        if (!funcs[i]) continue; // empty slot
        uintptr_t addr = base + funcs[i];
        std::ostringstream o;
        o << "@" << (exp->Base + i);
        g_exportMap.push_back({addr, o.str()});
    }
    // 2) overlay the (sparse) named exports so resolution prefers real names
    for (DWORD i = 0; i < exp->NumberOfNames; ++i) {
        const char* nm = reinterpret_cast<const char*>(base + names[i]);
        uintptr_t addr = base + funcs[nameOrds[i]];
        g_exportMap.push_back({addr, nm});
    }
    // sort so named entries (which sort after "@...") win on exact-address ties
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

// Is `a` inside committed, executable memory? A vtable slot must point at code;
// the first slot that does NOT (data export, RTTI, padding, garbage) marks the
// end of the vtable. This also naturally drops the trailing data-export
// over-read that a fixed [modBase,modEnd) range check used to include.
bool is_exec_addr(uintptr_t a) {
    MEMORY_BASIC_INFORMATION mbi{};
    if (VirtualQuery(reinterpret_cast<void*>(a), &mbi, sizeof(mbi)) != sizeof(mbi)) return false;
    if (mbi.State != MEM_COMMIT) return false;
    DWORD prot = mbi.Protect & 0xFF;
    return prot == PAGE_EXECUTE || prot == PAGE_EXECUTE_READ ||
           prot == PAGE_EXECUTE_READWRITE || prot == PAGE_EXECUTE_WRITECOPY;
}

// A vtable built in harvest.exe for a derived shim points its inherited slots at
// LOCAL import thunks (jmp qword ptr [__imp_...]) or ILT stubs (jmp rel32), not
// directly at the mfc140u function. Follow up to a few hops so the slot resolves
// to the real mfc140u export instead of "harvest.exe+0x...".
uintptr_t follow_thunk(uintptr_t addr, int maxHops = 4) {
    for (int hop = 0; hop < maxHops; ++hop) {
        if (IsBadReadPtr(reinterpret_cast<void*>(addr), 6)) break;
        const unsigned char* p = reinterpret_cast<const unsigned char*>(addr);
        if (p[0] == 0xFF && p[1] == 0x25) {            // jmp qword ptr [rip+disp32]
            int32_t disp; memcpy(&disp, p + 2, 4);
            uintptr_t iat = addr + 6 + static_cast<intptr_t>(disp);
            if (IsBadReadPtr(reinterpret_cast<void*>(iat), sizeof(uintptr_t))) break;
            addr = *reinterpret_cast<const uintptr_t*>(iat); // real target from IAT
            continue;
        }
        if (p[0] == 0xE9) {                            // jmp rel32 (ILT stub)
            int32_t rel; memcpy(&rel, p + 1, 4);
            addr = addr + 5 + static_cast<intptr_t>(rel);
            continue;
        }
        break;
    }
    return addr;
}

std::string module_basename(HMODULE m) {
    wchar_t buf[MAX_PATH] = {};
    GetModuleFileNameW(m, buf, MAX_PATH);
    std::wstring w(buf);
    size_t pos = w.find_last_of(L"\\/");
    std::wstring b = (pos == std::wstring::npos) ? w : w.substr(pos + 1);
    std::string out;
    for (wchar_t c : b) out += static_cast<char>(c & 0x7f); // ascii filenames
    return out;
}

// Resolve `addr` to a symbol and report its containing module + module-relative
// rva. Inside mfc140u.dll we use the export table (ordinal/name); elsewhere we
// emit "<module>+0x<rva>" so a vtable whose functions live outside mfc140u (the
// CFile/CArchive case) is captured instead of silently dropped.
std::string resolve_in_module(uintptr_t addr, std::string& moduleOut, uintptr_t& rvaOut) {
    // If the slot points outside mfc140u (e.g. a harvest.exe import thunk for a
    // derived-shim vtable), follow the thunk to its real target before resolving.
    uintptr_t target = addr;
    if (!(g_modBase && addr >= g_modBase && addr < g_modEnd)) {
        uintptr_t followed = follow_thunk(addr);
        if (g_modBase && followed >= g_modBase && followed < g_modEnd) target = followed;
    }
    HMODULE m = nullptr;
    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                           GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           reinterpret_cast<LPCWSTR>(target), &m) && m) {
        moduleOut = module_basename(m);
        rvaOut = target - reinterpret_cast<uintptr_t>(m);
    } else {
        moduleOut = "?";
        rvaOut = target;
    }
    if (g_modBase && target >= g_modBase && target < g_modEnd) return resolve_addr(target);
    std::ostringstream o;
    o << moduleOut << "+0x" << std::hex << rvaOut;
    return o.str();
}

VTableInfo harvest_vtable(const std::string& className, const void* instance, int maxSlots = 256) {
    VTableInfo vt;
    vt.name = className;
    if (!instance) { vt.note = "null instance"; return vt; }
    if (IsBadReadPtr(instance, sizeof(void*))) { vt.note = "unreadable instance"; return vt; }
    auto* vptr = *reinterpret_cast<void* const*>(instance);
    if (!vptr) { vt.note = "null vptr"; return vt; }
    if (IsBadReadPtr(vptr, sizeof(void*))) { vt.note = "unreadable vptr"; return vt; }
    auto* slots = reinterpret_cast<void* const*>(vptr);
    for (int i = 0; i < maxSlots; ++i) {
        if (IsBadReadPtr(&slots[i], sizeof(void*))) break;
        auto a = reinterpret_cast<uintptr_t>(slots[i]);
        if (!a || !is_exec_addr(a)) break; // slot no longer points to code => vtable end
        VTableSlot s;
        s.index = i;
        s.symbol = resolve_in_module(a, s.module, s.rva);
        vt.slots.push_back(s);
    }
    if (vt.slots.empty()) {
        std::ostringstream o;
        o << "vptr=0x" << std::hex << reinterpret_cast<uintptr_t>(vptr)
          << " first slot not executable";
        vt.note = o.str();
    }
    return vt;
}
} // namespace

void write_vtables(const std::vector<VTableInfo>& vts, const std::string& path) {
    std::ostringstream oss;
    oss << "{\n  \"classes\": [\n";
    for (size_t i = 0; i < vts.size(); ++i) {
        oss << "    {\n      \"name\": " << quote(vts[i].name);
        if (!vts[i].note.empty()) oss << ",\n      \"note\": " << quote(vts[i].note);
        oss << ",\n      \"slots\": [\n";
        for (size_t j = 0; j < vts[i].slots.size(); ++j) {
            const auto& s = vts[i].slots[j];
            oss << "        {\"index\": " << s.index
                << ", \"rva\": \"0x" << std::hex << s.rva << std::dec
                << "\", \"module\": " << quote(s.module)
                << ", \"symbol\": " << quote(s.symbol) << "}";
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

// A global CWinApp so AfxGetApp()/module state are valid while we construct MFC
// objects to read their vtables (this is the standard MFC console-app pattern).
CWinApp g_harvestApp;

namespace {
// MFC marks many CObject-derived classes abstract via a PURE-but-DLL-defined
// destructor (e.g. ~CDocument), so they cannot be `new`d directly. A trivial
// derived shim supplies a concrete destructor (overriding the pure base dtor) and
// inherits the base constructors, making the class instantiable. The shim's
// vtable preserves the base slot ORDER; only the destructor slot (and any
// explicitly overridden method) points into harvest.exe — which is flagged in the
// resolved symbol and conveniently identifies that slot.
template <class B> struct Shim : public B { using B::B; };

// Views additionally have a pure virtual OnDraw; override it so the shim is
// concrete. Slot order still matches the base class.
struct HView       : public CView       { void OnDraw(CDC*) override {} };
struct HScrollView : public CScrollView { void OnDraw(CDC*) override {} };

// Construct each class, read the vtable off its live vptr (offset 0), append.
// Objects are intentionally LEAKED — the process exits immediately, and leaking
// avoids any destructor side effects. Release MFC (/MD, no _DEBUG) turns ASSERTs
// into no-ops and the C++ ctors only initialize members (no window is created),
// so construction is safe. Each probe prints to stderr first, so if any ctor
// faults the CI log names the culprit.
void harvest_constructed_vtables(std::vector<VTableInfo>& v) {
#define PROBE(NAME, EXPR) \
    do { std::fprintf(stderr, "[vtbl] %s\n", NAME); std::fflush(stderr); \
         try { const void* _p = (EXPR); \
               if (_p) v.push_back(harvest_vtable(NAME, _p)); \
               else { VTableInfo _vi; _vi.name = NAME; _vi.note = "null instance"; v.push_back(_vi); } } \
         catch (...) { VTableInfo _vi; _vi.name = NAME; _vi.note = "construction threw"; v.push_back(_vi); } \
    } while (0)

    PROBE("CDocument",          new Shim<CDocument>());
    PROBE("COleDocument",       new Shim<COleDocument>());
    PROBE("COleLinkingDoc",     new Shim<COleLinkingDoc>());
    PROBE("CDockState",         new Shim<CDockState>());
    PROBE("CCommandLineInfo",   new Shim<CCommandLineInfo>());
    PROBE("CFrameWnd",          new Shim<CFrameWnd>());
    PROBE("CMDIFrameWnd",       new Shim<CMDIFrameWnd>());
    PROBE("CMDIChildWnd",       new Shim<CMDIChildWnd>());
    PROBE("CDialog",            new Shim<CDialog>());
    PROBE("CView",              new HView());        // slot ORDER == CView
    PROBE("CScrollView",        new HScrollView());  // slot ORDER == CScrollView
    PROBE("CSingleDocTemplate", new Shim<CSingleDocTemplate>(0, nullptr, nullptr, nullptr));
    PROBE("CMultiDocTemplate",  new Shim<CMultiDocTemplate>(0, nullptr, nullptr, nullptr));
    PROBE("COleObjectFactory",  new Shim<COleObjectFactory>(CLSID_NULL, RUNTIME_CLASS(CDocument), FALSE, nullptr));
    PROBE("CRecentFileList",    new Shim<CRecentFileList>(0, _T("Recent File List"), _T("File%d"), 4, 0));
    PROBE("COleDataSource",     new Shim<COleDataSource>());
    PROBE("COleDropTarget",     new Shim<COleDropTarget>());
    PROBE("COleDropSource",     new Shim<COleDropSource>());
#undef PROBE
}
} // namespace

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

    // Persist the crash-proof data FIRST: even if a constructor below faults with
    // an access violation (not catchable by C++ try/catch), these files are already
    // on disk and the `if: always()` upload step will still capture them.
    write_exceptions(exceptions, "exceptions.json");
    write_layouts(layouts, "layouts.json");

    // Initialize MFC module state, then harvest vtable slot order by constructing
    // live instances (MFC exports no ??_7 vtable symbols, so an instance is the
    // only way to reach the vptr).
    if (AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0)) {
        harvest_constructed_vtables(vtables);
    }
    write_vtables(vtables, "vtable_slots.json");
    return 0;
}
