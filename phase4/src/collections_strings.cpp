#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>
#include <cwchar>
#include <cwctype>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
struct TagManagerState {
    std::wstring buffer;
};

struct DialogTemplateState {
    CString fontName;
    unsigned short fontSize = 0;
    bool hasFont = false;
};

struct D2DTextState {
    CString fontFamilyName;
    CString localeName;
};

struct MenuTearOffState {
    std::wstring registryKey;
    unsigned int firstId = 0;
    unsigned int lastId = 0;
    std::set<unsigned int> inUse;
};

struct ControlRendererInfoState {
    std::wstring resourceId;
    unsigned int resourceNumericId = 0;
    unsigned long transparentColor = CLR_INVALID;
    CRect image;
    CRect corners;
    CRect sides;
    CRect interior;
    bool hasImage = false;
    bool preMultiplyCheck = false;
};

struct ControlRendererState {
    ControlRendererInfoState info;
    bool valid = false;
    bool mirrored = false;
    double imageScale = 1.0;
};

struct ToolTipInfoState {
    unsigned long fillColor = CLR_INVALID;
    unsigned long borderColor = CLR_INVALID;
    unsigned long textColor = CLR_INVALID;
    unsigned long descriptionColor = CLR_INVALID;
    int maxDescrWidth = 0;
    int roundedCorners = FALSE;
    int balloonTooltip = FALSE;
};

struct SettingsValue {
    enum Kind { String, Dword, Binary, Rect, ObjectPtr } kind = String;
    CString stringValue;
    unsigned long dwordValue = 0;
    std::vector<unsigned char> binaryValue;
    CRect rectValue;
    void* objectValue = nullptr;
};

struct SettingsStoreState {
    std::wstring path;
    bool open = false;
    bool readOnly = false;
};

struct TabViewEntry {
    CView* view = nullptr;
    CString title;
    int image = -1;
};

struct TabViewState {
    std::vector<TabViewEntry> tabs;
    int active = -1;
};

struct FontHolderState {
    void* font = nullptr;
    void* notifySink = nullptr;
    HFONT hFont = nullptr;
    CString faceName;
};

struct PictureHolderState {
    void* picture = nullptr;
    short type = 0;
    int width = 0;
    int height = 0;
    bool ownsHandle = false;
};

std::mutex g_tagManagerMutex;
std::map<void*, TagManagerState> g_tagManagerStates;
std::map<void*, DialogTemplateState> g_dialogTemplateStates;
std::map<void*, D2DTextState> g_d2dTextFormatStates;
std::map<void*, D2DTextState> g_d2dTextLayoutStates;
std::map<void*, MenuTearOffState> g_menuTearOffStates;
std::map<void*, ControlRendererInfoState> g_controlRendererInfoStates;
std::map<void*, ControlRendererState> g_controlRendererStates;
std::map<void*, ToolTipInfoState> g_toolTipInfoStates;
std::map<void*, SettingsStoreState> g_settingsStoreStates;
std::map<std::wstring, std::map<std::wstring, SettingsValue>> g_settingsStoreValues;
std::map<void*, TabViewState> g_tabViewStates;
std::map<void*, FontHolderState> g_fontHolderStates;
std::map<void*, PictureHolderState> g_pictureHolderStates;

const wchar_t* CStringText(const CString* value) {
    return value ? static_cast<const wchar_t*>(*value) : L"";
}

std::wstring Trim(const std::wstring& value) {
    size_t first = value.find_first_not_of(L" \t\r\n");
    if (first == std::wstring::npos) return L"";
    size_t last = value.find_last_not_of(L" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool ParseUnsignedHex(const wchar_t* text, unsigned long* value) {
    if (!text || !value) return false;
    while (*text == L' ' || *text == L'\t' || *text == L'#') ++text;
    if (text[0] == L'0' && (text[1] == L'x' || text[1] == L'X')) text += 2;

    wchar_t* end = nullptr;
    unsigned long parsed = std::wcstoul(text, &end, 16);
    if (end == text) return false;
    while (*end == L' ' || *end == L'\t') ++end;
    if (*end != L'\0') return false;

    *value = parsed;
    return true;
}

bool ParseUnsignedDecimalOrHex(const wchar_t* text, unsigned long* value) {
    if (!text || !value) return false;
    while (*text == L' ' || *text == L'\t') ++text;
    int base = 10;
    if (*text == L'#') {
        ++text;
        base = 16;
    } else if (text[0] == L'0' && (text[1] == L'x' || text[1] == L'X')) {
        base = 16;
    }

    wchar_t* end = nullptr;
    unsigned long parsed = std::wcstoul(text, &end, base);
    if (end == text) return false;
    while (*end == L' ' || *end == L'\t') ++end;
    if (*end != L'\0') return false;

    *value = parsed;
    return true;
}

bool ParseInts(const wchar_t* text, int* values, int count) {
    if (!text || !values || count <= 0) return false;
    const wchar_t* cur = text;
    for (int i = 0; i < count; ++i) {
        while (*cur == L' ' || *cur == L'\t' || *cur == L',' || *cur == L';') ++cur;
        wchar_t* end = nullptr;
        long parsed = std::wcstol(cur, &end, 10);
        if (end == cur) return false;
        values[i] = static_cast<int>(parsed);
        cur = end;
    }
    while (*cur == L' ' || *cur == L'\t' || *cur == L',' || *cur == L';') ++cur;
    return *cur == L'\0';
}

bool ParseSingleInt(const wchar_t* text, int* value) {
    if (!text || !value) return false;
    int parsed = 0;
    if (!ParseInts(text, &parsed, 1)) return false;
    *value = parsed;
    return true;
}

std::vector<std::wstring> SplitList(const wchar_t* text) {
    std::vector<std::wstring> parts;
    if (!text) return parts;
    std::wstring value = text;
    size_t start = 0;
    while (start <= value.size()) {
        size_t end = value.find_first_of(L",;", start);
        parts.push_back(Trim(value.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start)));
        if (end == std::wstring::npos) break;
        start = end + 1;
    }
    return parts;
}

bool ParseBoolText(const wchar_t* text, int* value) {
    if (!text || !value) return false;
    std::wstring lowered = Trim(text);
    for (wchar_t& ch : lowered) ch = static_cast<wchar_t>(std::towlower(ch));
    if (lowered == L"true" || lowered == L"yes" || lowered == L"on" || lowered == L"1") {
        *value = TRUE;
        return true;
    }
    if (lowered == L"false" || lowered == L"no" || lowered == L"off" || lowered == L"0") {
        *value = FALSE;
        return true;
    }
    return false;
}

std::wstring NormalizeSettingPath(const std::wstring& base, const wchar_t* child = nullptr) {
    std::wstring path = Trim(base);
    std::wstring extra = Trim(child ? child : L"");
    if (path.empty()) path = L"Software\\OpenMFC";
    if (!extra.empty()) {
        if (!path.empty() && path.back() != L'\\') path += L'\\';
        path += extra;
    }
    return path;
}

std::map<std::wstring, SettingsValue>& CurrentSettingsValues(void* self) {
    SettingsStoreState& state = g_settingsStoreStates[self];
    state.path = NormalizeSettingPath(state.path);
    state.open = true;
    return g_settingsStoreValues[state.path];
}

bool ParseKeyValueList(const wchar_t* text, std::map<std::wstring, std::wstring>* values, std::vector<std::wstring>* positional = nullptr) {
    if (!text || !values) return false;
    std::vector<std::wstring> parts = SplitList(text);
    for (const std::wstring& part : parts) {
        if (part.empty()) continue;
        size_t eq = part.find_first_of(L"=:");
        if (eq == std::wstring::npos) {
            if (positional) positional->push_back(part);
            continue;
        }
        std::wstring key = Trim(part.substr(0, eq));
        std::wstring value = Trim(part.substr(eq + 1));
        for (wchar_t& ch : key) ch = static_cast<wchar_t>(std::towlower(ch));
        (*values)[key] = value;
    }
    return !values->empty() || (positional && !positional->empty());
}

bool ParseRectText(const std::map<std::wstring, std::wstring>& values, const wchar_t* key, CRect* rect) {
    if (!rect || !key) return false;
    auto it = values.find(key);
    if (it == values.end()) return false;
    int parts[4] = {};
    if (!ParseInts(it->second.c_str(), parts, 4)) return false;
    rect->SetRect(parts[0], parts[1], parts[2], parts[3]);
    return true;
}

bool ParseControlRendererInfoText(const wchar_t* text, ControlRendererInfoState* info) {
    if (!text || !info) return false;
    std::map<std::wstring, std::wstring> kv;
    std::vector<std::wstring> positional;
    if (!ParseKeyValueList(text, &kv, &positional)) return false;

    ControlRendererInfoState parsed;
    auto resource = kv.find(L"res");
    if (resource == kv.end()) resource = kv.find(L"resource");
    if (resource == kv.end()) resource = kv.find(L"id");
    if (resource != kv.end()) parsed.resourceId = resource->second;
    else if (!positional.empty()) parsed.resourceId = positional[0];

    unsigned long numeric = 0;
    if (!parsed.resourceId.empty() && ParseUnsignedDecimalOrHex(parsed.resourceId.c_str(), &numeric)) {
        parsed.resourceNumericId = static_cast<unsigned int>(numeric);
    }

    parsed.hasImage = ParseRectText(kv, L"image", &parsed.image) || ParseRectText(kv, L"rect", &parsed.image);
    ParseRectText(kv, L"corners", &parsed.corners);
    ParseRectText(kv, L"sides", &parsed.sides);
    ParseRectText(kv, L"interior", &parsed.interior);
    auto transparent = kv.find(L"transparent");
    if (transparent == kv.end()) transparent = kv.find(L"color");
    if (transparent != kv.end()) ParseUnsignedDecimalOrHex(transparent->second.c_str(), &parsed.transparentColor);
    auto premultiply = kv.find(L"premultiply");
    int boolValue = FALSE;
    if (premultiply != kv.end() && ParseBoolText(premultiply->second.c_str(), &boolValue)) parsed.preMultiplyCheck = boolValue != FALSE;

    *info = parsed;
    return !parsed.resourceId.empty() || parsed.hasImage;
}

bool ParseToolTipInfoText(const wchar_t* text, ToolTipInfoState* info) {
    if (!text || !info) return false;
    std::map<std::wstring, std::wstring> kv;
    std::vector<std::wstring> positional;
    if (!ParseKeyValueList(text, &kv, &positional)) return false;
    auto parseColor = [&](const wchar_t* key, unsigned long* color) {
        auto it = kv.find(key);
        if (it != kv.end()) ParseUnsignedDecimalOrHex(it->second.c_str(), color);
    };
    auto parseBool = [&](const wchar_t* key, int* value) {
        auto it = kv.find(key);
        if (it != kv.end()) ParseBoolText(it->second.c_str(), value);
    };
    auto parseInt = [&](const wchar_t* key, int* value) {
        auto it = kv.find(key);
        if (it != kv.end()) ParseSingleInt(it->second.c_str(), value);
    };
    parseColor(L"fill", &info->fillColor);
    parseColor(L"border", &info->borderColor);
    parseColor(L"text", &info->textColor);
    parseColor(L"description", &info->descriptionColor);
    parseColor(L"descr", &info->descriptionColor);
    parseInt(L"maxwidth", &info->maxDescrWidth);
    parseInt(L"maxdescrwidth", &info->maxDescrWidth);
    parseBool(L"rounded", &info->roundedCorners);
    parseBool(L"balloon", &info->balloonTooltip);
    return true;
}

bool LoadToolBarImagesFromText(const wchar_t* text, CMFCToolBarImages* images, HINSTANCE instance) {
    if (!text || !images) return false;
    std::map<std::wstring, std::wstring> kv;
    std::vector<std::wstring> positional;
    ParseKeyValueList(text, &kv, &positional);
    std::wstring resource = !positional.empty() ? positional[0] : Trim(text);
    auto it = kv.find(L"res");
    if (it == kv.end()) it = kv.find(L"resource");
    if (it == kv.end()) it = kv.find(L"file");
    if (it == kv.end()) it = kv.find(L"id");
    if (it != kv.end()) resource = it->second;
    if (resource.empty()) return false;

    unsigned long numeric = 0;
    if (ParseUnsignedDecimalOrHex(resource.c_str(), &numeric)) {
        return images->Load(static_cast<UINT>(numeric), instance, FALSE) != FALSE;
    }
    if (images->Load(resource.c_str(), 0)) return true;
    return images->LoadStr(resource.c_str(), instance, FALSE) != FALSE;
}

bool ParseFontText(const wchar_t* text, LOGFONTW* font) {
    if (!text || !font) return false;
    std::vector<std::wstring> parts = SplitList(text);
    if (parts.empty() || parts[0].empty()) return false;

    ZeroMemory(font, sizeof(*font));
    wcsncpy(font->lfFaceName, parts[0].c_str(), LF_FACESIZE - 1);
    if (parts.size() > 1 && !parts[1].empty()) font->lfHeight = std::wcstol(parts[1].c_str(), nullptr, 10);
    if (parts.size() > 2 && !parts[2].empty()) font->lfWeight = std::wcstol(parts[2].c_str(), nullptr, 10);
    if (parts.size() > 3 && !parts[3].empty()) {
        int italic = FALSE;
        if (ParseBoolText(parts[3].c_str(), &italic) || ParseSingleInt(parts[3].c_str(), &italic)) {
            font->lfItalic = italic ? TRUE : FALSE;
        }
    }
    return true;
}

CString FontFaceFromHandle(HFONT font) {
    if (!font) return CString();
    LOGFONTW logFont = {};
    if (::GetObjectW(font, sizeof(logFont), &logFont) != sizeof(logFont)) return CString();
    return logFont.lfFaceName;
}

HFONT EnsureHolderFont(void* self) {
    FontHolderState& state = g_fontHolderStates[self];
    if (state.hFont) return state.hFont;
    state.hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    if (state.faceName.IsEmpty()) state.faceName = FontFaceFromHandle(state.hFont);
    return state.hFont;
}

void SetPictureState(void* self, void* picture, short type, int width = 0, int height = 0, bool owns = false) {
    PictureHolderState& state = g_pictureHolderStates[self];
    state.picture = picture;
    state.type = type;
    state.width = width;
    state.height = height;
    state.ownsHandle = owns;
}

std::wstring MakeTagEnd(const std::wstring& tag) {
    std::wstring name = Trim(tag);
    if (!name.empty() && name.front() == L'<') {
        size_t start = 1;
        if (start < name.size() && name[start] == L'/') ++start;
        size_t end = name.find_first_of(L" \t\r\n>", start);
        name = name.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start);
    }
    return name.empty() ? L"" : (L"</" + name + L">");
}

bool ExtractNamedValue(const std::wstring& buffer, const wchar_t* tagName, std::wstring* value, bool trim) {
    if (!tagName || !*tagName || !value) return false;
    std::wstring tag = tagName;

    std::vector<std::wstring> beginCandidates;
    if (!tag.empty() && tag.front() == L'<') {
        beginCandidates.push_back(tag);
    } else {
        beginCandidates.push_back(L"<" + tag + L">");
        beginCandidates.push_back(L"<" + tag + L" ");
    }

    for (const std::wstring& begin : beginCandidates) {
        size_t beginPos = buffer.find(begin);
        if (beginPos == std::wstring::npos) continue;
        size_t valueStart = beginPos + begin.size();
        if (!begin.empty() && begin.back() != L'>') {
            size_t close = buffer.find(L'>', valueStart);
            if (close == std::wstring::npos) continue;
            valueStart = close + 1;
        }

        std::wstring endTag = MakeTagEnd(tag);
        size_t endPos = endTag.empty() ? std::wstring::npos : buffer.find(endTag, valueStart);
        if (endPos == std::wstring::npos) endPos = buffer.size();
        *value = buffer.substr(valueStart, endPos - valueStart);
        if (trim) *value = Trim(*value);
        return true;
    }

    std::wstring key = Trim(tag) + L"=";
    size_t keyPos = buffer.find(key);
    if (keyPos != std::wstring::npos) {
        size_t valueStart = keyPos + key.size();
        size_t endPos = buffer.find_first_of(L"\r\n", valueStart);
        *value = buffer.substr(valueStart, endPos == std::wstring::npos ? std::wstring::npos : endPos - valueStart);
        if (trim) *value = Trim(*value);
        return true;
    }

    return false;
}

bool GetTagManagerValue(void* self, const CString* tag, std::wstring* value, bool trim = true) {
    std::wstring buffer;
    {
        std::lock_guard<std::mutex> lock(g_tagManagerMutex);
        auto it = g_tagManagerStates.find(self);
        if (it == g_tagManagerStates.end()) return false;
        buffer = it->second.buffer;
    }
    return ExtractNamedValue(buffer, CStringText(tag), value, trim);
}

bool ReadFileToWideString(const wchar_t* path, std::wstring* output) {
    if (!path || !*path || !output) return false;
    HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;

    LARGE_INTEGER size = {};
    if (!GetFileSizeEx(file, &size) || size.QuadPart < 0 || size.QuadPart > 64 * 1024 * 1024) {
        CloseHandle(file);
        return false;
    }

    std::vector<char> bytes(static_cast<size_t>(size.QuadPart));
    DWORD read = 0;
    BOOL ok = bytes.empty() ? TRUE : ReadFile(file, bytes.data(), static_cast<DWORD>(bytes.size()), &read, nullptr);
    CloseHandle(file);
    if (!ok) return false;
    bytes.resize(read);

    if (bytes.size() >= 2 &&
        static_cast<unsigned char>(bytes[0]) == 0xFF &&
        static_cast<unsigned char>(bytes[1]) == 0xFE) {
        const wchar_t* wide = reinterpret_cast<const wchar_t*>(bytes.data() + 2);
        output->assign(wide, wide + ((bytes.size() - 2) / sizeof(wchar_t)));
        return true;
    }

    UINT cp = CP_UTF8;
    int chars = MultiByteToWideChar(cp, 0, bytes.data(), static_cast<int>(bytes.size()), nullptr, 0);
    if (chars <= 0) {
        cp = CP_ACP;
        chars = MultiByteToWideChar(cp, 0, bytes.data(), static_cast<int>(bytes.size()), nullptr, 0);
    }
    if (chars <= 0) return false;
    output->assign(chars, L'\0');
    return MultiByteToWideChar(cp, 0, bytes.data(), static_cast<int>(bytes.size()), output->data(), chars) > 0;
}

bool ResourceToWideString(HRSRC resource, std::wstring* output) {
    if (!resource || !output) return false;
    HMODULE module = GetModuleHandleW(nullptr);
    HGLOBAL loaded = LoadResource(module, resource);
    if (!loaded) return false;
    DWORD size = SizeofResource(module, resource);
    const char* bytes = static_cast<const char*>(LockResource(loaded));
    if (!bytes) return false;

    if (size >= 2 &&
        static_cast<unsigned char>(bytes[0]) == 0xFF &&
        static_cast<unsigned char>(bytes[1]) == 0xFE) {
        const wchar_t* wide = reinterpret_cast<const wchar_t*>(bytes + 2);
        output->assign(wide, wide + ((size - 2) / sizeof(wchar_t)));
        return true;
    }

    int chars = MultiByteToWideChar(CP_UTF8, 0, bytes, static_cast<int>(size), nullptr, 0);
    UINT cp = CP_UTF8;
    if (chars <= 0) {
        cp = CP_ACP;
        chars = MultiByteToWideChar(cp, 0, bytes, static_cast<int>(size), nullptr, 0);
    }
    if (chars <= 0) return false;
    output->assign(chars, L'\0');
    return MultiByteToWideChar(cp, 0, bytes, static_cast<int>(size), output->data(), chars) > 0;
}

bool ReadDialogFont(const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize);

void SetDialogTemplateState(void* self, const DLGTEMPLATE* tmpl) {
    DialogTemplateState state;
    state.hasFont = ReadDialogFont(tmpl, &state.fontName, &state.fontSize);
    g_dialogTemplateStates[self] = state;
}

const D2DTextState* GetD2DFormatState(const void* self) {
    auto it = g_d2dTextFormatStates.find(const_cast<void*>(self));
    return it == g_d2dTextFormatStates.end() ? nullptr : &it->second;
}

const D2DTextState* GetD2DLayoutState(const void* self) {
    auto it = g_d2dTextLayoutStates.find(const_cast<void*>(self));
    return it == g_d2dTextLayoutStates.end() ? nullptr : &it->second;
}

unsigned int NextTearOffId(MenuTearOffState& state) {
    if (state.firstId == 0 && state.lastId == 0) return 0;
    for (unsigned int id = state.firstId; id <= state.lastId; ++id) {
        if (state.inUse.count(id) == 0) return id;
        if (id == 0xFFFFFFFFu) break;
    }
    return 0;
}

const wchar_t* SkipDialogString(const wchar_t* cur) {
    if (!cur) return nullptr;
    if (*cur == 0xFFFF) return cur + 2;
    while (*cur) ++cur;
    return cur + 1;
}

bool ReadDialogFont(const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize) {
    if (!tmpl || !fontName || !fontSize) return false;

    const DWORD DS_SETFONT_STYLE = 0x40;
    const DWORD DS_SHELLFONT_STYLE = DS_SETFONT_STYLE | 0x48;

    const BYTE* bytes = reinterpret_cast<const BYTE*>(tmpl);
    const WORD* words = reinterpret_cast<const WORD*>(tmpl);
    bool extended = words[0] == 1 && words[1] == 0xFFFF;

    DWORD style = 0;
    const wchar_t* cur = nullptr;
    if (extended) {
        struct DlgTemplateExPrefix {
            WORD dlgVer;
            WORD signature;
            DWORD helpID;
            DWORD exStyle;
            DWORD style;
            WORD cDlgItems;
            short x;
            short y;
            short cx;
            short cy;
        };
        const auto* ex = reinterpret_cast<const DlgTemplateExPrefix*>(bytes);
        style = ex->style;
        cur = reinterpret_cast<const wchar_t*>(bytes + sizeof(DlgTemplateExPrefix));
    } else {
        style = tmpl->style;
        cur = reinterpret_cast<const wchar_t*>(bytes + sizeof(DLGTEMPLATE));
    }

    if ((style & DS_SETFONT_STYLE) == 0 && (style & DS_SHELLFONT_STYLE) != DS_SHELLFONT_STYLE) return false;

    cur = SkipDialogString(cur);
    cur = SkipDialogString(cur);
    cur = SkipDialogString(cur);
    if (!cur) return false;

    const WORD* fontInfo = reinterpret_cast<const WORD*>(cur);
    *fontSize = fontInfo[0];
    cur = reinterpret_cast<const wchar_t*>(fontInfo + 1);
    if (extended) {
        const BYTE* exFontInfo = reinterpret_cast<const BYTE*>(cur);
        cur = reinterpret_cast<const wchar_t*>(exFontInfo + sizeof(WORD) + sizeof(BYTE) + sizeof(BYTE));
    }

    *fontName = CString(cur);
    return true;
}

std::wstring MakeWindowClassName(const wchar_t* requested) {
    if (requested && *requested) return requested;

    static unsigned long counter = 0;
    wchar_t buffer[80] = {};
    wsprintfW(buffer, L"OpenMFCWindowClass%lu", ++counter);
    return buffer;
}

bool ExtractTaggedText(CString* buffer, const wchar_t* beginTag, const wchar_t* endTag) {
    if (!buffer || !beginTag || !*beginTag) return false;
    std::wstring text = static_cast<const wchar_t*>(*buffer);
    std::wstring begin = beginTag;
    size_t beginPos = text.find(begin);
    if (beginPos == std::wstring::npos) return false;
    beginPos += begin.size();

    size_t endPos = std::wstring::npos;
    if (endTag && *endTag) endPos = text.find(endTag, beginPos);
    if (endPos == std::wstring::npos) endPos = text.size();

    *buffer = CString(text.substr(beginPos, endPos - beginPos).c_str());
    return true;
}

const wchar_t* DocumentTitleOrPathName(CDocument* doc) {
    if (!doc) return L"";
    const wchar_t* title = doc->GetTitle();
    if (title && *title) return title;

    const wchar_t* path = doc->GetPathName();
    if (!path || !*path) return L"";
    const wchar_t* file = path;
    for (const wchar_t* cur = path; *cur; ++cur) {
        if (*cur == L'\\' || *cur == L'/') file = cur + 1;
    }
    return file;
}

std::wstring SanitizedFileName(const wchar_t* value) {
    std::wstring result = (value && *value) ? value : L"Untitled";
    for (wchar_t& ch : result) {
        if (ch == L'\\' || ch == L'/' || ch == L':' || ch == L'*' || ch == L'?' ||
            ch == L'"' || ch == L'<' || ch == L'>' || ch == L'|') {
            ch = L'_';
        }
    }
    return result;
}
}

IMPLEMENT_DYNAMIC(CMFCControlRenderer, CObject)

CMFCControlRendererInfo::CMFCControlRendererInfo() {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    CommonInit();
}

CMFCControlRendererInfo::CMFCControlRendererInfo(const CMFCControlRendererInfo& src) {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    g_controlRendererInfoStates[this] = g_controlRendererInfoStates[const_cast<CMFCControlRendererInfo*>(&src)];
}

CMFCControlRendererInfo::CMFCControlRendererInfo(UINT nID, const CRect& rectImage, const CRect& rectCorners,
                                                 const CRect& rectSides, const CRect& rectInter) {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.resourceNumericId = nID;
    wchar_t buffer[32] = {};
    wsprintfW(buffer, L"%u", nID);
    state.resourceId = buffer;
    state.image = rectImage;
    state.corners = rectCorners;
    state.sides = rectSides;
    state.interior = rectInter;
    state.hasImage = true;
}

CMFCControlRendererInfo::CMFCControlRendererInfo(UINT nID, DWORD clrTransparent, const CRect& rectImage,
                                                 const CRect& rectCorners, const CRect& rectSides,
                                                 const CRect& rectInter, BOOL bPreMultiplyCheck)
    : CMFCControlRendererInfo(nID, rectImage, rectCorners, rectSides, rectInter) {
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.transparentColor = clrTransparent;
    state.preMultiplyCheck = bPreMultiplyCheck != FALSE;
}

CMFCControlRendererInfo::CMFCControlRendererInfo(const wchar_t* lpszID, const CRect& rectImage,
                                                 const CRect& rectCorners, const CRect& rectSides,
                                                 const CRect& rectInter) {
    memset(_controlrendererinfo_padding, 0, sizeof(_controlrendererinfo_padding));
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.resourceId = lpszID ? lpszID : L"";
    unsigned long numeric = 0;
    if (ParseUnsignedDecimalOrHex(state.resourceId.c_str(), &numeric)) state.resourceNumericId = static_cast<UINT>(numeric);
    state.image = rectImage;
    state.corners = rectCorners;
    state.sides = rectSides;
    state.interior = rectInter;
    state.hasImage = true;
}

CMFCControlRendererInfo::CMFCControlRendererInfo(const wchar_t* lpszID, DWORD clrTransparent,
                                                 const CRect& rectImage, const CRect& rectCorners,
                                                 const CRect& rectSides, const CRect& rectInter,
                                                 BOOL bPreMultiplyCheck)
    : CMFCControlRendererInfo(lpszID, rectImage, rectCorners, rectSides, rectInter) {
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.transparentColor = clrTransparent;
    state.preMultiplyCheck = bPreMultiplyCheck != FALSE;
}

CMFCControlRendererInfo::~CMFCControlRendererInfo() {
    g_controlRendererInfoStates.erase(this);
}

CMFCControlRendererInfo& CMFCControlRendererInfo::operator=(const CMFCControlRendererInfo& src) {
    if (this != &src) g_controlRendererInfoStates[this] = g_controlRendererInfoStates[const_cast<CMFCControlRendererInfo*>(&src)];
    return *this;
}

void CMFCControlRendererInfo::CommonInit() {
    g_controlRendererInfoStates[this] = ControlRendererInfoState();
}

const wchar_t* CMFCControlRendererInfo::GetResourceID() const {
    auto it = g_controlRendererInfoStates.find(const_cast<CMFCControlRendererInfo*>(this));
    return it == g_controlRendererInfoStates.end() ? L"" : it->second.resourceId.c_str();
}

void CMFCControlRendererInfo::SetResourceID(const wchar_t* lpszID) {
    ControlRendererInfoState& state = g_controlRendererInfoStates[this];
    state.resourceId = lpszID ? lpszID : L"";
    unsigned long numeric = 0;
    state.resourceNumericId = ParseUnsignedDecimalOrHex(state.resourceId.c_str(), &numeric) ? static_cast<UINT>(numeric) : 0;
}

CMFCControlRenderer::CMFCControlRenderer() {
    memset(_controlrenderer_padding, 0, sizeof(_controlrenderer_padding));
    g_controlRendererStates[this] = ControlRendererState();
}

CMFCControlRenderer::~CMFCControlRenderer() {
    g_controlRendererStates.erase(this);
}

BOOL CMFCControlRenderer::Create(const CMFCControlRendererInfo& info, BOOL bFlip) {
    ControlRendererState& state = g_controlRendererStates[this];
    state.info = g_controlRendererInfoStates[const_cast<CMFCControlRendererInfo*>(&info)];
    state.valid = !state.info.resourceId.empty() || state.info.hasImage;
    state.mirrored = bFlip != FALSE;
    return state.valid ? TRUE : FALSE;
}

void CMFCControlRenderer::CleanUp() {
    g_controlRendererStates[this] = ControlRendererState();
}

void CMFCControlRenderer::Draw(CDC* pDC, CRect rect, UINT, BYTE) {
    if (pDC && pDC->GetSafeHdc()) ::Rectangle(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom);
}

void CMFCControlRenderer::DrawFrame(CDC* pDC, CRect rect, UINT, BYTE) {
    if (pDC && pDC->GetSafeHdc()) {
        RECT nativeRect = {rect.left, rect.top, rect.right, rect.bottom};
        ::FrameRect(pDC->GetSafeHdc(), &nativeRect, static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
    }
}

void CMFCControlRenderer::FillInterior(CDC* pDC, CRect rect, UINT, BYTE) {
    if (pDC && pDC->GetSafeHdc()) {
        RECT nativeRect = {rect.left, rect.top, rect.right, rect.bottom};
        ::FillRect(pDC->GetSafeHdc(), &nativeRect, static_cast<HBRUSH>(::GetStockObject(LTGRAY_BRUSH)));
    }
}

void CMFCControlRenderer::FillInterior(CDC* pDC, CRect rect, int, int, UINT index, BYTE alphaSrc) {
    FillInterior(pDC, rect, index, alphaSrc);
}

void CMFCControlRenderer::Mirror() {
    g_controlRendererStates[this].mirrored = !g_controlRendererStates[this].mirrored;
}

void CMFCControlRenderer::OnSysColorChange() {}

BOOL CMFCControlRenderer::SmoothResize(double dblImageScale) {
    g_controlRendererStates[this].imageScale = dblImageScale;
    return TRUE;
}

// Symbol: ??0CMFCControlRendererInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_XZ(void* self) { return new (self) CMFCControlRendererInfo(); }
// Symbol: ??0CMFCControlRendererInfo@@QEAA@AEBV0@@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_AEBV0__Z(void* self, const CMFCControlRendererInfo* src) { return new (self) CMFCControlRendererInfo(*src); }
// Symbol: ??0CMFCControlRendererInfo@@QEAA@IAEBVCRect@@000@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_IAEBVCRect__000_Z(void* self, unsigned int id, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty);
}
// Symbol: ??0CMFCControlRendererInfo@@QEAA@IKAEBVCRect@@000H@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_IKAEBVCRect__000H_Z(void* self, unsigned int id, unsigned long color, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior, int pre) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, color, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty, pre);
}
// Symbol: ??0CMFCControlRendererInfo@@QEAA@PEB_WAEBVCRect@@111@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_PEB_WAEBVCRect__111_Z(void* self, const wchar_t* id, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty);
}
// Symbol: ??0CMFCControlRendererInfo@@QEAA@PEB_WKAEBVCRect@@111H@Z
extern "C" void* MS_ABI impl___0CMFCControlRendererInfo__QEAA_PEB_WKAEBVCRect__111H_Z(void* self, const wchar_t* id, unsigned long color, const CRect* image, const CRect* corners, const CRect* sides, const CRect* interior, int pre) {
    CRect empty;
    return new (self) CMFCControlRendererInfo(id, color, image ? *image : empty, corners ? *corners : empty, sides ? *sides : empty, interior ? *interior : empty, pre);
}
// Symbol: ??1CMFCControlRendererInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1CMFCControlRendererInfo__QEAA_XZ(CMFCControlRendererInfo* self) { if (self) self->~CMFCControlRendererInfo(); }
// Symbol: ??4CMFCControlRendererInfo@@QEAAAEAV0@AEBV0@@Z
extern "C" CMFCControlRendererInfo* MS_ABI impl___4CMFCControlRendererInfo__QEAAAEAV0_AEBV0__Z(CMFCControlRendererInfo* self, const CMFCControlRendererInfo* src) { return self && src ? &(*self = *src) : self; }
// Symbol: ?CommonInit@CMFCControlRendererInfo@@QEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCControlRendererInfo__QEAAXXZ(CMFCControlRendererInfo* self) { if (self) self->CommonInit(); }
// Symbol: ?GetResourceID@CMFCControlRendererInfo@@QEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetResourceID_CMFCControlRendererInfo__QEBAPEB_WXZ(const CMFCControlRendererInfo* self) { return self ? self->GetResourceID() : L""; }
// Symbol: ?SetResourceID@CMFCControlRendererInfo@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetResourceID_CMFCControlRendererInfo__QEAAXPEB_W_Z(CMFCControlRendererInfo* self, const wchar_t* id) { if (self) self->SetResourceID(id); }

// Symbol: ??0CMFCControlRenderer@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCControlRenderer__QEAA_XZ(void* self) { return new (self) CMFCControlRenderer(); }
// Symbol: ??1CMFCControlRenderer@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCControlRenderer__UEAA_XZ(CMFCControlRenderer* self) { if (self) self->~CMFCControlRenderer(); }
// Symbol: ?Create@CMFCControlRenderer@@UEAAHAEBVCMFCControlRendererInfo@@H@Z
extern "C" int MS_ABI impl__Create_CMFCControlRenderer__UEAAHAEBVCMFCControlRendererInfo__H_Z(CMFCControlRenderer* self, const CMFCControlRendererInfo* info, int flip) { return self && info ? self->Create(*info, flip) : FALSE; }
// Symbol: ?CleanUp@CMFCControlRenderer@@UEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCControlRenderer__UEAAXXZ(CMFCControlRenderer* self) { if (self) self->CleanUp(); }
// Symbol: ?Mirror@CMFCControlRenderer@@UEAAXXZ
extern "C" void MS_ABI impl__Mirror_CMFCControlRenderer__UEAAXXZ(CMFCControlRenderer* self) { if (self) self->Mirror(); }
// Symbol: ?OnSysColorChange@CMFCControlRenderer@@UEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCControlRenderer__UEAAXXZ(CMFCControlRenderer* self) { if (self) self->OnSysColorChange(); }
// Symbol: ?SmoothResize@CMFCControlRenderer@@QEAAHN@Z
extern "C" int MS_ABI impl__SmoothResize_CMFCControlRenderer__QEAAHN_Z(CMFCControlRenderer* self, double scale) { return self ? self->SmoothResize(scale) : FALSE; }
// Symbol: ?Draw@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__Draw_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, unsigned int index, unsigned char alpha) { if (self) self->Draw(dc, rect, index, alpha); }
// Symbol: ?DrawFrame@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__DrawFrame_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, unsigned int index, unsigned char alpha) { if (self) self->DrawFrame(dc, rect, index, alpha); }
// Symbol: ?FillInterior@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__FillInterior_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, unsigned int index, unsigned char alpha) { if (self) self->FillInterior(dc, rect, index, alpha); }
// Symbol: ?FillInterior@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@W4ImageAlignHorz@CMFCToolBarImages@@W4ImageAlignVert@5@IE@Z
extern "C" void MS_ABI impl__FillInterior_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__W4ImageAlignHorz_CMFCToolBarImages__W4ImageAlignVert_5_IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, int horz, int vert, unsigned int index, unsigned char alpha) { if (self) self->FillInterior(dc, rect, horz, vert, index, alpha); }

// Symbol: ??0CTagManager@@QEAA@PEB_W@Z
extern "C" void MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer) {
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer ? buffer : L"";
}

// Symbol: ??1CTagManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CTagManager__UEAA_XZ(void* self) {
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates.erase(self);
}

// Symbol: ?SetBuffer@CTagManager@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetBuffer_CTagManager__QEAAXPEB_W_Z(void* self, const wchar_t* buffer) {
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer ? buffer : L"";
}

// Symbol: ?LoadFromFile@CTagManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadFromFile_CTagManager__QEAAHPEB_W_Z(void* self, const wchar_t* path) {
    std::wstring buffer;
    if (!ReadFileToWideString(path, &buffer)) return FALSE;
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer;
    return TRUE;
}

// Symbol: ?LoadFromResource@CTagManager@@QEAAHIPEB_W@Z
extern "C" int MS_ABI impl__LoadFromResource_CTagManager__QEAAHIPEB_W_Z(void* self, unsigned int resourceId, const wchar_t* type) {
    HMODULE module = GetModuleHandleW(nullptr);
    HRSRC resource = FindResourceW(module, MAKEINTRESOURCEW(resourceId), type);
    std::wstring buffer;
    if (!ResourceToWideString(resource, &buffer)) return FALSE;
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer;
    return TRUE;
}

// Symbol: ?LoadFromResource@CTagManager@@QEAAHPEB_W0@Z
extern "C" int MS_ABI impl__LoadFromResource_CTagManager__QEAAHPEB_W0_Z(void* self, const wchar_t* resourceName, const wchar_t* type) {
    HMODULE module = GetModuleHandleW(nullptr);
    HRSRC resource = FindResourceW(module, resourceName, type);
    std::wstring buffer;
    if (!ResourceToWideString(resource, &buffer)) return FALSE;
    std::lock_guard<std::mutex> lock(g_tagManagerMutex);
    g_tagManagerStates[self].buffer = buffer;
    return TRUE;
}

// Symbol: ??0CDialogTemplate@@QEAA@PEAX@Z
extern "C" void MS_ABI impl___0CDialogTemplate__QEAA_PEAX_Z(void* self, void* tmpl) {
    SetDialogTemplateState(self, static_cast<const DLGTEMPLATE*>(tmpl));
}

// Symbol: ??0CDialogTemplate@@QEAA@PEBUDLGTEMPLATE@@@Z
extern "C" void MS_ABI impl___0CDialogTemplate__QEAA_PEBUDLGTEMPLATE___Z(void* self, const DLGTEMPLATE* tmpl) {
    SetDialogTemplateState(self, tmpl);
}

// Symbol: ??1CDialogTemplate@@QEAA@XZ
extern "C" void MS_ABI impl___1CDialogTemplate__QEAA_XZ(void* self) {
    g_dialogTemplateStates.erase(self);
}

// Symbol: ?SetTemplate@CDialogTemplate@@IEAAHPEBUDLGTEMPLATE@@I@Z
extern "C" int MS_ABI impl__SetTemplate_CDialogTemplate__IEAAHPEBUDLGTEMPLATE__I_Z(void* self, const DLGTEMPLATE* tmpl, unsigned int) {
    SetDialogTemplateState(self, tmpl);
    return tmpl ? TRUE : FALSE;
}

// Symbol: ?GetFont@CDialogTemplate@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAG@Z
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    void* self, CString* fontName, unsigned short* fontSize) {
    auto it = g_dialogTemplateStates.find(self);
    if (it == g_dialogTemplateStates.end() || !it->second.hasFont || !fontName || !fontSize) return FALSE;
    *fontName = it->second.fontName;
    *fontSize = it->second.fontSize;
    return TRUE;
}

// Symbol: ??0CD2DTextFormat@@QEAA@PEAVCRenderTarget@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@MW4DWRITE_FONT_WEIGHT@@W4DWRITE_FONT_STYLE@@W4DWRITE_FONT_STRETCH@@1PEAUIDWriteFontCollection@@H@Z
extern "C" void MS_ABI impl___0CD2DTextFormat__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_A___MW4DWRITE_FONT_WEIGHT__W4DWRITE_FONT_STYLE__W4DWRITE_FONT_STRETCH__1PEAUIDWriteFontCollection__H_Z(
    void* self, void*, const CString* fontFamilyName, float, unsigned int, unsigned int, unsigned int, const CString* localeName, void*, int) {
    D2DTextState& state = g_d2dTextFormatStates[self];
    state.fontFamilyName = CStringText(fontFamilyName);
    state.localeName = localeName && *CStringText(localeName) ? CStringText(localeName) : L"";
}

// Symbol: ??1CD2DTextFormat@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DTextFormat__UEAA_XZ(void* self) {
    g_d2dTextFormatStates.erase(self);
}

// Symbol: ?GetFontFamilyName@CD2DTextFormat@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFontFamilyName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* ret, const void* self) {
    const D2DTextState* state = GetD2DFormatState(self);
    new (ret) CString(state ? state->fontFamilyName : CString());
}

// Symbol: ?GetLocaleName@CD2DTextFormat@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetLocaleName_CD2DTextFormat__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* ret, const void* self) {
    const D2DTextState* state = GetD2DFormatState(self);
    new (ret) CString(state ? state->localeName : CString());
}

// Symbol: ??0CD2DTextLayout@@QEAA@PEAVCRenderTarget@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCD2DTextFormat@@AEBVCD2DSizeF@@H@Z
extern "C" void MS_ABI impl___0CD2DTextLayout__QEAA_PEAVCRenderTarget__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCD2DTextFormat__AEBVCD2DSizeF__H_Z(
    void* self, void*, const CString*, void* format, const CD2DSizeF*, int) {
    D2DTextState& state = g_d2dTextLayoutStates[self];
    if (const D2DTextState* formatState = GetD2DFormatState(format)) {
        state = *formatState;
    }
}

// Symbol: ??1CD2DTextLayout@@UEAA@XZ
extern "C" void MS_ABI impl___1CD2DTextLayout__UEAA_XZ(void* self) {
    g_d2dTextLayoutStates.erase(self);
}

// Symbol: ?SetFontFamilyName@CD2DTextLayout@@QEAAHPEB_WUDWRITE_TEXT_RANGE@@@Z
extern "C" int MS_ABI impl__SetFontFamilyName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(
    void* self, const wchar_t* fontFamilyName, unsigned long long) {
    g_d2dTextLayoutStates[self].fontFamilyName = fontFamilyName ? fontFamilyName : L"";
    return TRUE;
}

// Symbol: ?SetLocaleName@CD2DTextLayout@@QEAAHPEB_WUDWRITE_TEXT_RANGE@@@Z
extern "C" int MS_ABI impl__SetLocaleName_CD2DTextLayout__QEAAHPEB_WUDWRITE_TEXT_RANGE___Z(
    void* self, const wchar_t* localeName, unsigned long long) {
    g_d2dTextLayoutStates[self].localeName = localeName ? localeName : L"";
    return TRUE;
}

// Symbol: ?GetFontFamilyName@CD2DTextLayout@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEAUDWRITE_TEXT_RANGE@@@Z
extern "C" void MS_ABI impl__GetFontFamilyName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(
    CString* ret, const void* self, unsigned int, void*) {
    const D2DTextState* state = GetD2DLayoutState(self);
    new (ret) CString(state ? state->fontFamilyName : CString());
}

// Symbol: ?GetLocaleName@CD2DTextLayout@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEAUDWRITE_TEXT_RANGE@@@Z
extern "C" void MS_ABI impl__GetLocaleName_CD2DTextLayout__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEAUDWRITE_TEXT_RANGE___Z(
    CString* ret, const void* self, unsigned int, void*) {
    const D2DTextState* state = GetD2DLayoutState(self);
    new (ret) CString(state ? state->localeName : CString());
}

// Symbol: ??0CMenuTearOffManager@@QEAA@XZ
extern "C" void MS_ABI impl___0CMenuTearOffManager__QEAA_XZ(void* self) {
    g_menuTearOffStates[self] = MenuTearOffState();
}

// Symbol: ??1CMenuTearOffManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CMenuTearOffManager__UEAA_XZ(void* self) {
    g_menuTearOffStates.erase(self);
}

// Symbol: ?Initialize@CMenuTearOffManager@@QEAAHPEB_WII@Z
extern "C" int MS_ABI impl__Initialize_CMenuTearOffManager__QEAAHPEB_WII_Z(
    void* self, const wchar_t* registryKey, unsigned int firstId, unsigned int lastId) {
    MenuTearOffState& state = g_menuTearOffStates[self];
    state.registryKey = registryKey ? registryKey : L"";
    state.firstId = firstId;
    state.lastId = lastId;
    state.inUse.clear();
    return firstId <= lastId ? TRUE : FALSE;
}

// Symbol: ?SetInUse@CMenuTearOffManager@@QEAAXIH@Z
extern "C" void MS_ABI impl__SetInUse_CMenuTearOffManager__QEAAXIH_Z(void* self, unsigned int id, int inUse) {
    MenuTearOffState& state = g_menuTearOffStates[self];
    if (inUse) state.inUse.insert(id);
    else state.inUse.erase(id);
}

// Symbol: ?GetFreeTearOffID@CMenuTearOffManager@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__GetFreeTearOffID_CMenuTearOffManager__IEAAIXZ(void* self) {
    return NextTearOffId(g_menuTearOffStates[self]);
}

// Symbol: ?Build@CMenuTearOffManager@@QEAAXIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__Build_CMenuTearOffManager__QEAAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* self, unsigned int id, CString* value) {
    if (!value) return;
    wchar_t buffer[32] = {};
    wsprintfW(buffer, L"%u", id);
    *value = CString(buffer);
    g_menuTearOffStates[self].inUse.insert(id);
}

// Symbol: ?Parse@CMenuTearOffManager@@QEAAIAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" unsigned int MS_ABI impl__Parse_CMenuTearOffManager__QEAAIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* self, CString* value) {
    unsigned long parsed = 0;
    if (!value || !ParseUnsignedDecimalOrHex(CStringText(value), &parsed)) return 0;
    g_menuTearOffStates[self].inUse.insert(static_cast<unsigned int>(parsed));
    return static_cast<unsigned int>(parsed);
}

// Symbol: ?ParseColor@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ParseColor_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(
    const CString* value, unsigned long* color) {
    return ParseUnsignedDecimalOrHex(CStringText(value), color) ? TRUE : FALSE;
}

// Symbol: ?ParseColorHEX@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ParseColorHEX_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(
    const CString* value, unsigned long* color) {
    return ParseUnsignedHex(CStringText(value), color) ? TRUE : FALSE;
}

// Symbol: ?ParsePoint@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCPoint@@@Z
extern "C" int MS_ABI impl__ParsePoint_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCPoint___Z(
    const CString* value, CPoint* point) {
    int parts[2] = {};
    if (!point || !ParseInts(CStringText(value), parts, 2)) return FALSE;
    point->SetPoint(parts[0], parts[1]);
    return TRUE;
}

// Symbol: ?ParseSize@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCSize@@@Z
extern "C" int MS_ABI impl__ParseSize_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCSize___Z(
    const CString* value, CSize* size) {
    int parts[2] = {};
    if (!size || !ParseInts(CStringText(value), parts, 2)) return FALSE;
    size->SetSize(parts[0], parts[1]);
    return TRUE;
}

// Symbol: ?ParseRect@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__ParseRect_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCRect___Z(
    const CString* value, CRect* rect) {
    int parts[4] = {};
    if (!rect || !ParseInts(CStringText(value), parts, 4)) return FALSE;
    rect->SetRect(parts[0], parts[1], parts[2], parts[3]);
    return TRUE;
}

// Symbol: ?ParseFont@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagLOGFONTW@@@Z
extern "C" int MS_ABI impl__ParseFont_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagLOGFONTW___Z(
    const CString* value, LOGFONTW* font) {
    return ParseFontText(CStringText(value), font) ? TRUE : FALSE;
}

// Symbol: ?ParseString@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@0AEAVCStringArray@@HH@Z
extern "C" int MS_ABI impl__ParseString_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__0AEAVCStringArray__HH_Z(
    const CString* value, const CString* separators, CStringArray* output, int trim, int removeEmpty) {
    if (!value || !output) return FALSE;
    output->RemoveAll();

    const wchar_t* sep = separators ? static_cast<const wchar_t*>(*separators) : L",;";
    std::wstring text = static_cast<const wchar_t*>(*value);
    size_t start = 0;
    while (start <= text.size()) {
        size_t end = text.find_first_of(sep, start);
        std::wstring token = text.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start);
        if (trim) {
            size_t first = token.find_first_not_of(L" \t\r\n");
            size_t last = token.find_last_not_of(L" \t\r\n");
            token = first == std::wstring::npos ? L"" : token.substr(first, last - first + 1);
        }
        if (!removeEmpty || !token.empty()) output->Add(CString(token.c_str()));
        if (end == std::wstring::npos) break;
        start = end + 1;
    }
    return TRUE;
}

// Symbol: ?ExcludeTag@AFX_GLOBAL_DATA@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W0H@Z
extern "C" int MS_ABI impl__ExcludeTag_AFX_GLOBAL_DATA__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0H_Z(
    void*, CString* buffer, const wchar_t* beginTag, const wchar_t* endTag, int) {
    return ExtractTaggedText(buffer, beginTag, endTag) ? TRUE : FALSE;
}

// Symbol: ?ExcludeTag@CTagManager@@QEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" int MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* self, const wchar_t* tag, CString* value, int trim) {
    if (!value) return FALSE;
    std::wstring extracted;
    std::wstring buffer;
    {
        std::lock_guard<std::mutex> lock(g_tagManagerMutex);
        auto it = g_tagManagerStates.find(self);
        if (it == g_tagManagerStates.end()) return FALSE;
        buffer = it->second.buffer;
    }
    if (!ExtractNamedValue(buffer, tag, &extracted, trim != FALSE)) return FALSE;
    *value = CString(extracted.c_str());
    return TRUE;
}

// Symbol: ?ReadBool@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadBool_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* self, const CString* tag, int* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseBoolText(text.c_str(), value) ? TRUE : FALSE;
}

// Symbol: ?ReadColor@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ReadColor_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(
    void* self, const CString* tag, unsigned long* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseUnsignedDecimalOrHex(text.c_str(), value) ? TRUE : FALSE;
}

// Symbol: ?ReadFont@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagLOGFONTW@@@Z
extern "C" int MS_ABI impl__ReadFont_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagLOGFONTW___Z(
    void* self, const CString* tag, LOGFONTW* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseFontText(text.c_str(), value) ? TRUE : FALSE;
}

// Symbol: ?ReadInt@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadInt_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* self, const CString* tag, int* value) {
    std::wstring text;
    return value && GetTagManagerValue(self, tag, &text) && ParseSingleInt(text.c_str(), value) ? TRUE : FALSE;
}

// Symbol: ?ReadPoint@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCPoint@@@Z
extern "C" int MS_ABI impl__ReadPoint_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCPoint___Z(
    void* self, const CString* tag, CPoint* value) {
    std::wstring text;
    int parts[2] = {};
    if (!value || !GetTagManagerValue(self, tag, &text) || !ParseInts(text.c_str(), parts, 2)) return FALSE;
    value->SetPoint(parts[0], parts[1]);
    return TRUE;
}

// Symbol: ?ReadRect@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCRect@@@Z
extern "C" int MS_ABI impl__ReadRect_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCRect___Z(
    void* self, const CString* tag, CRect* value) {
    std::wstring text;
    int parts[4] = {};
    if (!value || !GetTagManagerValue(self, tag, &text) || !ParseInts(text.c_str(), parts, 4)) return FALSE;
    value->SetRect(parts[0], parts[1], parts[2], parts[3]);
    return TRUE;
}

// Symbol: ?ReadSize@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCSize@@@Z
extern "C" int MS_ABI impl__ReadSize_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCSize___Z(
    void* self, const CString* tag, CSize* value) {
    std::wstring text;
    int parts[2] = {};
    if (!value || !GetTagManagerValue(self, tag, &text) || !ParseInts(text.c_str(), parts, 2)) return FALSE;
    value->SetSize(parts[0], parts[1]);
    return TRUE;
}

// Symbol: ?ParseControlRendererInfo@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRendererInfo@@@Z
extern "C" int MS_ABI impl__ParseControlRendererInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(
    const CString* value, CMFCControlRendererInfo* info) {
    if (!info) return FALSE;
    ControlRendererInfoState parsed;
    if (!ParseControlRendererInfoText(CStringText(value), &parsed)) return FALSE;
    g_controlRendererInfoStates[info] = parsed;
    return TRUE;
}

// Symbol: ?ParseControlRenderer@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@I@Z
extern "C" int MS_ABI impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(
    const CString* value, CMFCControlRenderer* renderer, unsigned int) {
    if (!renderer) return FALSE;
    CMFCControlRendererInfo info;
    if (!impl__ParseControlRendererInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(value, &info)) return FALSE;
    return renderer->Create(info, FALSE);
}

// Symbol: ?ParseControlRenderer@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@PEB_W@Z
extern "C" int MS_ABI impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__PEB_W_Z(
    const CString* value, CMFCControlRenderer* renderer, const wchar_t*) {
    return impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(value, renderer, 0);
}

// Symbol: ?ParseToolBarImages@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@I@Z
extern "C" int MS_ABI impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__I_Z(
    const CString* value, CMFCToolBarImages* images, unsigned int id) {
    if (!images) return FALSE;
    if (CStringText(value)[0] != L'\0') return LoadToolBarImagesFromText(CStringText(value), images, nullptr) ? TRUE : FALSE;
    return images->Load(id, nullptr, FALSE);
}

// Symbol: ?ParseToolBarImages@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@PEB_W@Z
extern "C" int MS_ABI impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__PEB_W_Z(
    const CString* value, CMFCToolBarImages* images, const wchar_t* resourceId) {
    if (!images) return FALSE;
    const wchar_t* source = CStringText(value)[0] != L'\0' ? CStringText(value) : resourceId;
    return LoadToolBarImagesFromText(source, images, nullptr) ? TRUE : FALSE;
}

// Symbol: ?ParseToolTipInfo@CTagManager@@SAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolTipInfo@@@Z
extern "C" int MS_ABI impl__ParseToolTipInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolTipInfo___Z(
    const CString* value, CMFCToolTipInfo* info) {
    if (!info) return FALSE;
    ToolTipInfoState parsed;
    if (!ParseToolTipInfoText(CStringText(value), &parsed)) return FALSE;
    g_toolTipInfoStates[info] = parsed;
    return TRUE;
}

// Symbol: ?ReadControlRendererInfo@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRendererInfo@@@Z
extern "C" int MS_ABI impl__ReadControlRendererInfo_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(
    void* self, const CString* tag, CMFCControlRendererInfo* info) {
    std::wstring text;
    CString value;
    if (!info || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseControlRendererInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRendererInfo___Z(&value, info);
}

// Symbol: ?ReadControlRenderer@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@I@Z
extern "C" int MS_ABI impl__ReadControlRenderer_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(
    void* self, const CString* tag, CMFCControlRenderer* renderer, unsigned int id) {
    std::wstring text;
    CString value;
    if (!renderer || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__I_Z(&value, renderer, id);
}

// Symbol: ?ReadControlRenderer@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCControlRenderer@@PEB_W@Z
extern "C" int MS_ABI impl__ReadControlRenderer_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__PEB_W_Z(
    void* self, const CString* tag, CMFCControlRenderer* renderer, const wchar_t* resourceId) {
    std::wstring text;
    CString value;
    if (!renderer || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseControlRenderer_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCControlRenderer__PEB_W_Z(&value, renderer, resourceId);
}

// Symbol: ?ReadToolBarImages@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@I@Z
extern "C" int MS_ABI impl__ReadToolBarImages_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__I_Z(
    void* self, const CString* tag, CMFCToolBarImages* images, unsigned int id) {
    std::wstring text;
    CString value;
    if (!images || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__I_Z(&value, images, id);
}

// Symbol: ?ReadToolBarImages@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolBarImages@@PEB_W@Z
extern "C" int MS_ABI impl__ReadToolBarImages_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__PEB_W_Z(
    void* self, const CString* tag, CMFCToolBarImages* images, const wchar_t* resourceId) {
    std::wstring text;
    CString value;
    if (!images || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseToolBarImages_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolBarImages__PEB_W_Z(&value, images, resourceId);
}

// Symbol: ?ReadToolTipInfo@CTagManager@@QEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCMFCToolTipInfo@@@Z
extern "C" int MS_ABI impl__ReadToolTipInfo_CTagManager__QEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolTipInfo___Z(
    void* self, const CString* tag, CMFCToolTipInfo* info) {
    std::wstring text;
    CString value;
    if (!info || !GetTagManagerValue(self, tag, &text)) return FALSE;
    value = text.c_str();
    return impl__ParseToolTipInfo_CTagManager__SAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCMFCToolTipInfo___Z(&value, info);
}

// Symbol: ??0CSettingsStore@@IEAA@XZ
extern "C" void* MS_ABI impl___0CSettingsStore__IEAA_XZ(void* self) {
    g_settingsStoreStates[self] = SettingsStoreState();
    return self;
}

// Symbol: ??0CSettingsStore@@QEAA@HH@Z
extern "C" void* MS_ABI impl___0CSettingsStore__QEAA_HH_Z(void* self, int, int readOnly) {
    SettingsStoreState& state = g_settingsStoreStates[self];
    state.readOnly = readOnly != FALSE;
    state.open = true;
    state.path = NormalizeSettingPath(L"Software\\OpenMFC");
    return self;
}

// Symbol: ??1CSettingsStore@@UEAA@XZ
extern "C" void MS_ABI impl___1CSettingsStore__UEAA_XZ(void* self) {
    g_settingsStoreStates.erase(self);
}

// Symbol: ?Open@CSettingsStore@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* path) {
    SettingsStoreState& state = g_settingsStoreStates[self];
    state.path = NormalizeSettingPath(path);
    state.open = true;
    return TRUE;
}

// Symbol: ?CreateKey@CSettingsStore@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* key) {
    SettingsStoreState& state = g_settingsStoreStates[self];
    state.path = NormalizeSettingPath(state.path, key);
    state.open = true;
    (void)CurrentSettingsValues(self);
    return TRUE;
}

// Symbol: ?Close@CSettingsStore@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CSettingsStore__UEAAXXZ(void* self) {
    g_settingsStoreStates[self].open = false;
}

// Symbol: ?DeleteKey@CSettingsStore@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__DeleteKey_CSettingsStore__UEAAHPEB_WH_Z(void* self, const wchar_t* key, int) {
    auto it = g_settingsStoreStates.find(self);
    std::wstring path = NormalizeSettingPath(it == g_settingsStoreStates.end() ? L"" : it->second.path, key);
    return g_settingsStoreValues.erase(path) > 0 ? TRUE : FALSE;
}

// Symbol: ?DeleteValue@CSettingsStore@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__DeleteValue_CSettingsStore__UEAAHPEB_W_Z(void* self, const wchar_t* name) {
    if (!name) return FALSE;
    auto& values = CurrentSettingsValues(self);
    return values.erase(name) > 0 ? TRUE : FALSE;
}

// Symbol: ?Write@CSettingsStore@@UEAAHPEB_W0@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_W0_Z(void* self, const wchar_t* name, const wchar_t* value) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::String;
    stored.stringValue = value ? value : L"";
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}

// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WH_Z(void* self, const wchar_t* name, int value) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Dword;
    stored.dwordValue = static_cast<unsigned long>(value);
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}

// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WK@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WK_Z(void* self, const wchar_t* name, unsigned long value) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Dword;
    stored.dwordValue = value;
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}

// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WPEAEI@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(void* self, const wchar_t* name, const unsigned char* data, unsigned int size) {
    if (!name || (!data && size != 0)) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Binary;
    stored.binaryValue.assign(data, data + size);
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}

// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WAEBVCRect@@@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WAEBVCRect___Z(void* self, const wchar_t* name, const CRect* rect) {
    if (!name || !rect) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::Rect;
    stored.rectValue = *rect;
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}

// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WPEAVCObject@@@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WPEAVCObject___Z(void* self, const wchar_t* name, CObject* object) {
    if (!name) return FALSE;
    SettingsValue stored;
    stored.kind = SettingsValue::ObjectPtr;
    stored.objectValue = object;
    CurrentSettingsValues(self)[name] = stored;
    return TRUE;
}

// Symbol: ?Write@CSettingsStore@@UEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(void* self, const wchar_t* name, CObject* object) {
    return impl__Write_CSettingsStore__UEAAHPEB_WPEAVCObject___Z(self, name, object);
}

// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* self, const wchar_t* name, CString* value) {
    if (!name || !value) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end()) return FALSE;
    if (it->second.kind == SettingsValue::String) *value = it->second.stringValue;
    else {
        wchar_t buffer[64] = {};
        wsprintfW(buffer, L"%lu", it->second.dwordValue);
        *value = buffer;
    }
    return TRUE;
}

// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAH@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAH_Z(void* self, const wchar_t* name, int* value) {
    if (!name || !value) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end()) return FALSE;
    if (it->second.kind == SettingsValue::Dword) *value = static_cast<int>(it->second.dwordValue);
    else if (!ParseSingleInt(it->second.stringValue.GetString(), value)) return FALSE;
    return TRUE;
}

// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAK@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAK_Z(void* self, const wchar_t* name, unsigned long* value) {
    if (!name || !value) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end()) return FALSE;
    if (it->second.kind == SettingsValue::Dword) *value = it->second.dwordValue;
    else if (!ParseUnsignedDecimalOrHex(it->second.stringValue.GetString(), value)) return FALSE;
    return TRUE;
}

// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WPEAPEAEPEAI@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WPEAPEAEPEAI_Z(void* self, const wchar_t* name, unsigned char** data, unsigned int* size) {
    if (!name || !data || !size) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end() || it->second.kind != SettingsValue::Binary) return FALSE;
    *size = static_cast<unsigned int>(it->second.binaryValue.size());
    *data = nullptr;
    if (*size != 0) {
        *data = static_cast<unsigned char*>(::LocalAlloc(LMEM_FIXED, *size));
        if (!*data) return FALSE;
        memcpy(*data, it->second.binaryValue.data(), *size);
    }
    return TRUE;
}

// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAVCRect@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAVCRect___Z(void* self, const wchar_t* name, CRect* rect) {
    if (!name || !rect) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end() || it->second.kind != SettingsValue::Rect) return FALSE;
    *rect = it->second.rectValue;
    return TRUE;
}

// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAPEAVCObject@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAPEAVCObject___Z(void* self, const wchar_t* name, CObject** object) {
    if (!name || !object) return FALSE;
    auto& values = CurrentSettingsValues(self);
    auto it = values.find(name);
    if (it == values.end() || it->second.kind != SettingsValue::ObjectPtr) return FALSE;
    *object = static_cast<CObject*>(it->second.objectValue);
    return TRUE;
}

// Symbol: ?Read@CSettingsStore@@UEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WAEAVCObject___Z(void* self, const wchar_t* name, CObject* object) {
    CObject* stored = nullptr;
    return object && impl__Read_CSettingsStore__UEAAHPEB_WAEAPEAVCObject___Z(self, name, &stored) ? TRUE : FALSE;
}

// Symbol: ??0CTabView@@IEAA@XZ
extern "C" void* MS_ABI impl___0CTabView__IEAA_XZ(void* self) {
    g_tabViewStates[self] = TabViewState();
    return self;
}

// Symbol: ??1CTabView@@MEAA@XZ
extern "C" void MS_ABI impl___1CTabView__MEAA_XZ(void* self) {
    g_tabViewStates.erase(self);
}

// Symbol: ?AddView@CTabView@@QEAAHPEAUCRuntimeClass@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__AddView_CTabView__QEAAHPEAUCRuntimeClass__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEAUCCreateContext___Z(
    void* self, CRuntimeClass* runtimeClass, const CString* title, int image, CCreateContext*) {
    if (!self) return FALSE;
    CObject* object = runtimeClass ? runtimeClass->CreateObject() : nullptr;
    CView* view = dynamic_cast<CView*>(object);
    if (object && !view) {
        delete object;
        return FALSE;
    }
    TabViewState& state = g_tabViewStates[self];
    TabViewEntry entry;
    entry.view = view;
    entry.title = CStringText(title);
    entry.image = image;
    state.tabs.push_back(entry);
    if (state.active < 0) state.active = 0;
    return TRUE;
}

// Symbol: ?FindTab@CTabView@@QEBAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__FindTab_CTabView__QEBAHPEAUHWND_____Z(const void* self, HWND hwnd) {
    auto it = g_tabViewStates.find(const_cast<void*>(self));
    if (it == g_tabViewStates.end()) return -1;
    for (size_t i = 0; i < it->second.tabs.size(); ++i) {
        CView* view = it->second.tabs[i].view;
        if (view && view->GetSafeHwnd() == hwnd) return static_cast<int>(i);
    }
    return -1;
}

// Symbol: ?GetActiveView@CTabView@@QEBAPEAVCView@@XZ
extern "C" CView* MS_ABI impl__GetActiveView_CTabView__QEBAPEAVCView__XZ(const void* self) {
    auto it = g_tabViewStates.find(const_cast<void*>(self));
    if (it == g_tabViewStates.end() || it->second.active < 0 || it->second.active >= static_cast<int>(it->second.tabs.size())) return nullptr;
    return it->second.tabs[it->second.active].view;
}

// Symbol: ?SetActiveView@CTabView@@QEAAHH@Z
extern "C" int MS_ABI impl__SetActiveView_CTabView__QEAAHH_Z(void* self, int index) {
    TabViewState& state = g_tabViewStates[self];
    if (index < 0 || index >= static_cast<int>(state.tabs.size())) return FALSE;
    state.active = index;
    return TRUE;
}

// Symbol: ?RemoveView@CTabView@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveView_CTabView__QEAAHH_Z(void* self, int index) {
    TabViewState& state = g_tabViewStates[self];
    if (index < 0 || index >= static_cast<int>(state.tabs.size())) return FALSE;
    CView* view = state.tabs[index].view;
    state.tabs.erase(state.tabs.begin() + index);
    if (view) delete view;
    if (state.tabs.empty()) state.active = -1;
    else if (state.active >= static_cast<int>(state.tabs.size())) state.active = static_cast<int>(state.tabs.size()) - 1;
    return TRUE;
}

// Symbol: ??0CFontHolder@@QEAA@PEAUIPropertyNotifySink@@@Z
extern "C" void* MS_ABI impl___0CFontHolder__QEAA_PEAUIPropertyNotifySink___Z(void* self, void* notifySink) {
    FontHolderState& state = g_fontHolderStates[self];
    state.notifySink = notifySink;
    state.hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    state.faceName = FontFaceFromHandle(state.hFont);
    return self;
}

// Symbol: ??1CFontHolder@@QEAA@XZ
extern "C" void MS_ABI impl___1CFontHolder__QEAA_XZ(void* self) {
    g_fontHolderStates.erase(self);
}

// Symbol: ?InitializeFont@CFontHolder@@QEAAXPEBUtagFONTDESC@@PEAUIDispatch@@@Z
extern "C" void MS_ABI impl__InitializeFont_CFontHolder__QEAAXPEBUtagFONTDESC__PEAUIDispatch___Z(void* self, const void*, void* dispatch) {
    FontHolderState& state = g_fontHolderStates[self];
    state.font = dispatch;
    state.hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    state.faceName = FontFaceFromHandle(state.hFont);
}

// Symbol: ?SetFont@CFontHolder@@QEAAXPEAUIFont@@@Z
extern "C" void MS_ABI impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(void* self, void* font) {
    FontHolderState& state = g_fontHolderStates[self];
    state.font = font;
    state.hFont = font ? reinterpret_cast<HFONT>(font) : static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    CString face = FontFaceFromHandle(state.hFont);
    if (!face.IsEmpty()) state.faceName = face;
}

// Symbol: ?SetFontNotifySink@CFontHolder@@QEAAXPEAUIPropertyNotifySink@@@Z
extern "C" void MS_ABI impl__SetFontNotifySink_CFontHolder__QEAAXPEAUIPropertyNotifySink___Z(void* self, void* notifySink) {
    g_fontHolderStates[self].notifySink = notifySink;
}

// Symbol: ?GetFontDispatch@CFontHolder@@QEAAPEAUIFontDisp@@XZ
extern "C" void* MS_ABI impl__GetFontDispatch_CFontHolder__QEAAPEAUIFontDisp__XZ(void* self) {
    return g_fontHolderStates[self].font;
}

// Symbol: ?GetFontHandle@CFontHolder@@QEAAPEAUHFONT__@@XZ
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(void* self) {
    return EnsureHolderFont(self);
}

// Symbol: ?GetFontHandle@CFontHolder@@QEAAPEAUHFONT__@@JJ@Z
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____JJ_Z(void* self, long, long) {
    return EnsureHolderFont(self);
}

// Symbol: ?QueryTextMetrics@CFontHolder@@QEAAXPEAUtagTEXTMETRICW@@@Z
extern "C" void MS_ABI impl__QueryTextMetrics_CFontHolder__QEAAXPEAUtagTEXTMETRICW___Z(void* self, TEXTMETRICW* metrics) {
    if (!metrics) return;
    ZeroMemory(metrics, sizeof(*metrics));
    HDC dc = ::GetDC(nullptr);
    if (!dc) return;
    HGDIOBJ oldFont = ::SelectObject(dc, EnsureHolderFont(self));
    ::GetTextMetricsW(dc, metrics);
    if (oldFont) ::SelectObject(dc, oldFont);
    ::ReleaseDC(nullptr, dc);
}

// Symbol: ?Select@CFontHolder@@QEAAPEAVCFont@@PEAVCDC@@JJ@Z
extern "C" CFont* MS_ABI impl__Select_CFontHolder__QEAAPEAVCFont__PEAVCDC__JJ_Z(void* self, CDC* dc, long, long) {
    if (!dc || !dc->GetSafeHdc()) return nullptr;
    ::SelectObject(dc->GetSafeHdc(), EnsureHolderFont(self));
    return nullptr;
}

// Symbol: ?ReleaseFont@CFontHolder@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseFont_CFontHolder__QEAAXXZ(void* self) {
    FontHolderState& state = g_fontHolderStates[self];
    state.font = nullptr;
    state.hFont = nullptr;
    state.faceName.Empty();
}

// Symbol: ?GetDisplayString@CFontHolder@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetDisplayString_CFontHolder__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* self, CString* value) {
    if (!value) return FALSE;
    FontHolderState& state = g_fontHolderStates[self];
    if (state.faceName.IsEmpty()) state.faceName = FontFaceFromHandle(EnsureHolderFont(self));
    *value = state.faceName.IsEmpty() ? CString(L"Font") : state.faceName;
    return TRUE;
}

// Symbol: ??0CPictureHolder@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPictureHolder__QEAA_XZ(void* self) {
    SetPictureState(self, nullptr, 0);
    return self;
}

// Symbol: ??1CPictureHolder@@QEAA@XZ
extern "C" void MS_ABI impl___1CPictureHolder__QEAA_XZ(void* self) {
    g_pictureHolderStates.erase(self);
}

// Symbol: ?CreateEmpty@CPictureHolder@@QEAAHXZ
extern "C" int MS_ABI impl__CreateEmpty_CPictureHolder__QEAAHXZ(void* self) {
    SetPictureState(self, nullptr, 0);
    return TRUE;
}

// Symbol: ?CreateFromBitmap@CPictureHolder@@QEAAHI@Z
extern "C" int MS_ABI impl__CreateFromBitmap_CPictureHolder__QEAAHI_Z(void* self, unsigned int bitmapId) {
    HBITMAP bitmap = ::LoadBitmapW(nullptr, MAKEINTRESOURCEW(bitmapId));
    SetPictureState(self, bitmap, 1, 0, 0, bitmap != nullptr);
    return bitmap != nullptr;
}

// Symbol: ?CreateFromBitmap@CPictureHolder@@QEAAHPEAUHBITMAP__@@PEAUHPALETTE__@@H@Z
extern "C" int MS_ABI impl__CreateFromBitmap_CPictureHolder__QEAAHPEAUHBITMAP____PEAUHPALETTE____H_Z(void* self, HBITMAP bitmap, HPALETTE, int transferOwnership) {
    SetPictureState(self, bitmap, bitmap ? 1 : 0, 0, 0, transferOwnership != FALSE);
    return bitmap != nullptr;
}

// Symbol: ?CreateFromBitmap@CPictureHolder@@QEAAHPEAVCBitmap@@PEAVCPalette@@H@Z
extern "C" int MS_ABI impl__CreateFromBitmap_CPictureHolder__QEAAHPEAVCBitmap__PEAVCPalette__H_Z(void* self, CBitmap* bitmap, CPalette*, int transferOwnership) {
    return impl__CreateFromBitmap_CPictureHolder__QEAAHPEAUHBITMAP____PEAUHPALETTE____H_Z(
        self, bitmap ? static_cast<HBITMAP>(bitmap->GetSafeHandle()) : nullptr, nullptr, transferOwnership);
}

// Symbol: ?CreateFromIcon@CPictureHolder@@QEAAHI@Z
extern "C" int MS_ABI impl__CreateFromIcon_CPictureHolder__QEAAHI_Z(void* self, unsigned int iconId) {
    HICON icon = static_cast<HICON>(::LoadImageW(nullptr, MAKEINTRESOURCEW(iconId), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
    SetPictureState(self, icon, icon ? 3 : 0);
    return icon != nullptr;
}

// Symbol: ?CreateFromIcon@CPictureHolder@@QEAAHPEAUHICON__@@H@Z
extern "C" int MS_ABI impl__CreateFromIcon_CPictureHolder__QEAAHPEAUHICON____H_Z(void* self, HICON icon, int transferOwnership) {
    SetPictureState(self, icon, icon ? 3 : 0, 0, 0, transferOwnership != FALSE);
    return icon != nullptr;
}

// Symbol: ?CreateFromMetafile@CPictureHolder@@QEAAHPEAUHMETAFILE__@@HHH@Z
extern "C" int MS_ABI impl__CreateFromMetafile_CPictureHolder__QEAAHPEAUHMETAFILE____HHH_Z(void* self, HMETAFILE metafile, int width, int height, int transferOwnership) {
    SetPictureState(self, metafile, metafile ? 2 : 0, width, height, transferOwnership != FALSE);
    return metafile != nullptr;
}

// Symbol: ?SetPictureDispatch@CPictureHolder@@QEAAXPEAUIPictureDisp@@@Z
extern "C" void MS_ABI impl__SetPictureDispatch_CPictureHolder__QEAAXPEAUIPictureDisp___Z(void* self, void* picture) {
    SetPictureState(self, picture, picture ? 4 : 0);
}

// Symbol: ?GetPictureDispatch@CPictureHolder@@QEAAPEAUIPictureDisp@@XZ
extern "C" void* MS_ABI impl__GetPictureDispatch_CPictureHolder__QEAAPEAUIPictureDisp__XZ(void* self) {
    return g_pictureHolderStates[self].picture;
}

// Symbol: ?GetType@CPictureHolder@@QEAAFXZ
extern "C" short MS_ABI impl__GetType_CPictureHolder__QEAAFXZ(void* self) {
    return g_pictureHolderStates[self].type;
}

// Symbol: ?Render@CPictureHolder@@QEAAXPEAVCDC@@AEBVCRect@@1@Z
extern "C" void MS_ABI impl__Render_CPictureHolder__QEAAXPEAVCDC__AEBVCRect__1_Z(void* self, CDC* dc, const CRect* dest, const CRect* source) {
    if (!dc || !dc->GetSafeHdc() || !dest) return;
    PictureHolderState& state = g_pictureHolderStates[self];
    if (state.type == 3 && state.picture) {
        ::DrawIconEx(dc->GetSafeHdc(), dest->left, dest->top, static_cast<HICON>(state.picture),
                     dest->Width(), dest->Height(), 0, nullptr, DI_NORMAL);
        return;
    }
    if (state.type == 1 && state.picture) {
        HDC memDC = ::CreateCompatibleDC(dc->GetSafeHdc());
        HGDIOBJ oldBitmap = memDC ? ::SelectObject(memDC, static_cast<HBITMAP>(state.picture)) : nullptr;
        int srcWidth = source ? source->Width() : dest->Width();
        int srcHeight = source ? source->Height() : dest->Height();
        int srcLeft = source ? source->left : 0;
        int srcTop = source ? source->top : 0;
        if (memDC) {
            ::StretchBlt(dc->GetSafeHdc(), dest->left, dest->top, dest->Width(), dest->Height(),
                         memDC, srcLeft, srcTop, srcWidth, srcHeight, SRCCOPY);
            if (oldBitmap) ::SelectObject(memDC, oldBitmap);
            ::DeleteDC(memDC);
        }
        return;
    }
    ::Rectangle(dc->GetSafeHdc(), dest->left, dest->top, dest->right, dest->bottom);
}

// Symbol: ?GetDisplayString@CPictureHolder@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetDisplayString_CPictureHolder__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* self, CString* value) {
    if (!value) return FALSE;
    PictureHolderState& state = g_pictureHolderStates[self];
    switch (state.type) {
    case 1: *value = L"Bitmap"; break;
    case 2: *value = L"Metafile"; break;
    case 3: *value = L"Icon"; break;
    default: *value = state.picture ? L"Picture" : L"Empty"; break;
    }
    return TRUE;
}

// Symbol: ?RegisterWindowClass@AFX_GLOBAL_DATA@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void MS_ABI impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* ret, void*, const wchar_t* requestedClassName) {
    std::wstring className = MakeWindowClassName(requestedClassName);
    HINSTANCE instance = GetModuleHandleW(nullptr);

    WNDCLASSEXW existing = {};
    existing.cbSize = sizeof(existing);
    if (!GetClassInfoExW(instance, className.c_str(), &existing)) {
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(wc);
        wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProcW;
        wc.hInstance = instance;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = className.c_str();
        RegisterClassExW(&wc);
    }

    new (ret) CString(className.c_str());
}

// Symbol: ?DrawTextOnGlass@AFX_GLOBAL_DATA@@QEAAHPEAXPEAVCDC@@HHV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCRect@@KHK@Z
extern "C" int MS_ABI impl__DrawTextOnGlass_AFX_GLOBAL_DATA__QEAAHPEAXPEAVCDC__HHV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__KHK_Z(
    void*, void*, CDC* dc, int, int, const CString* text, CRect rect, unsigned long color, int format, unsigned long) {
    if (!dc || !dc->GetSafeHdc()) return FALSE;
    COLORREF oldColor = SetTextColor(dc->GetSafeHdc(), color);
    int oldBkMode = SetBkMode(dc->GetSafeHdc(), TRANSPARENT);
    RECT winRect = { rect.left, rect.top, rect.right, rect.bottom };
    int result = DrawTextW(dc->GetSafeHdc(), CStringText(text), -1, &winRect, static_cast<UINT>(format));
    SetBkMode(dc->GetSafeHdc(), oldBkMode);
    SetTextColor(dc->GetSafeHdc(), oldColor);
    return result;
}

// Symbol: ?GetFont@CDialogTemplate@@SAHPEBUDLGTEMPLATE@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAG@Z
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__SAHPEBUDLGTEMPLATE__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize) {
    return ReadDialogFont(tmpl, fontName, fontSize) ? TRUE : FALSE;
}

// Symbol: ?DeleteAutosavedFile@CDataRecoveryHandler@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__DeleteAutosavedFile_CDataRecoveryHandler__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void*, const CString* fileName) {
    const wchar_t* path = CStringText(fileName);
    if (!path || !*path) return FALSE;
    if (DeleteFileW(path)) return TRUE;
    return GetLastError() == ERROR_FILE_NOT_FOUND ? TRUE : FALSE;
}

// Symbol: ?GenerateAutosaveFileName@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
extern "C" void MS_ABI impl__GenerateAutosaveFileName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    CString* ret, void*, const CString* documentName) {
    wchar_t tempPath[MAX_PATH] = {};
    if (!GetTempPathW(MAX_PATH, tempPath)) tempPath[0] = L'\0';
    std::wstring fileName = SanitizedFileName(CStringText(documentName));
    new (ret) CString((std::wstring(tempPath) + fileName + L".autosave").c_str());
}

// Symbol: ?GetDocumentListName@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCDocument@@@Z
extern "C" void MS_ABI impl__GetDocumentListName_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    CString* ret, void*, CDocument* document) {
    new (ret) CString(DocumentTitleOrPathName(document));
}

// Symbol: ?GetNormalDocumentTitle@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCDocument@@@Z
extern "C" void MS_ABI impl__GetNormalDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCDocument___Z(
    CString* ret, void*, CDocument* document) {
    new (ret) CString(DocumentTitleOrPathName(document));
}

// Symbol: ?GetRecoveredDocumentTitle@CDataRecoveryHandler@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBV23@@Z
extern "C" void MS_ABI impl__GetRecoveredDocumentTitle_CDataRecoveryHandler__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBV23__Z(
    CString* ret, void*, const CString* documentName) {
    CString title = CStringText(documentName);
    title += L" [Recovered]";
    new (ret) CString(title);
}
