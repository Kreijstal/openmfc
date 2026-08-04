#include "CollectionsStringsSupport.h"

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
IMPLEMENT_DYNAMIC(CMFCControlRenderer, CObject)
namespace openmfc { namespace detail { namespace collectionsstrings {
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
std::wstring Trim_CollectionsStrings(const std::wstring& value) {
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
        parts.push_back(Trim_CollectionsStrings(value.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start)));
        if (end == std::wstring::npos) break;
        start = end + 1;
    }
    return parts;
}
bool ParseBoolText(const wchar_t* text, int* value) {
    if (!text || !value) return false;
    std::wstring lowered = Trim_CollectionsStrings(text);
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
std::wstring NormalizeSettingPath(const std::wstring& base, const wchar_t* child) {
    std::wstring path = Trim_CollectionsStrings(base);
    std::wstring extra = Trim_CollectionsStrings(child ? child : L"");
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
bool ParseKeyValueList(const wchar_t* text, std::map<std::wstring, std::wstring>* values, std::vector<std::wstring>* positional) {
    if (!text || !values) return false;
    std::vector<std::wstring> parts = SplitList(text);
    for (const std::wstring& part : parts) {
        if (part.empty()) continue;
        size_t eq = part.find_first_of(L"=:");
        if (eq == std::wstring::npos) {
            if (positional) positional->push_back(part);
            continue;
        }
        std::wstring key = Trim_CollectionsStrings(part.substr(0, eq));
        std::wstring value = Trim_CollectionsStrings(part.substr(eq + 1));
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
    std::wstring resource = !positional.empty() ? positional[0] : Trim_CollectionsStrings(text);
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
void SetPictureState(void* self, void* picture, short type, int width, int height, bool owns) {
    PictureHolderState& state = g_pictureHolderStates[self];
    state.picture = picture;
    state.type = type;
    state.width = width;
    state.height = height;
    state.ownsHandle = owns;
}
std::wstring MakeTagEnd(const std::wstring& tag) {
    std::wstring name = Trim_CollectionsStrings(tag);
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
        if (trim) *value = Trim_CollectionsStrings(*value);
        return true;
    }

    std::wstring key = Trim_CollectionsStrings(tag) + L"=";
    size_t keyPos = buffer.find(key);
    if (keyPos != std::wstring::npos) {
        size_t valueStart = keyPos + key.size();
        size_t endPos = buffer.find_first_of(L"\r\n", valueStart);
        *value = buffer.substr(valueStart, endPos == std::wstring::npos ? std::wstring::npos : endPos - valueStart);
        if (trim) *value = Trim_CollectionsStrings(*value);
        return true;
    }

    return false;
}
bool GetTagManagerValue(void* self, const CString* tag, std::wstring* value, bool trim) {
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
std::wstring MakeWindowClassName(const wchar_t* requested) {
    if (requested && *requested) return requested;

    unsigned long counter = 0;
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
} } }  // namespace openmfc::detail::collectionsstrings
