#pragma once
// Shared internals of the former collections_strings.cpp translation unit.
// Definitions live in detail/CollectionsStringsSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace collectionsstrings {} } }
using namespace openmfc::detail::collectionsstrings;
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

namespace openmfc { namespace detail { namespace collectionsstrings {
struct TagManagerState {
    std::wstring buffer;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct DialogTemplateState {
    CString fontName;
    unsigned short fontSize = 0;
    bool hasFont = false;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct D2DTextState {
    CString fontFamilyName;
    CString localeName;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct MenuTearOffState {
    std::wstring registryKey;
    unsigned int firstId = 0;
    unsigned int lastId = 0;
    std::set<unsigned int> inUse;
    std::vector<unsigned int> tearOffItems;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
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
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct ControlRendererState {
    ControlRendererInfoState info;
    bool valid = false;
    bool mirrored = false;
    double imageScale = 1.0;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct ToolTipInfoState {
    unsigned long fillColor = CLR_INVALID;
    unsigned long borderColor = CLR_INVALID;
    unsigned long textColor = CLR_INVALID;
    unsigned long descriptionColor = CLR_INVALID;
    int maxDescrWidth = 0;
    int roundedCorners = FALSE;
    int balloonTooltip = FALSE;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct SettingsValue {
    enum Kind { String, Dword, Binary, Rect, ObjectPtr } kind = String;
    CString stringValue;
    unsigned long dwordValue = 0;
    std::vector<unsigned char> binaryValue;
    CRect rectValue;
    void* objectValue = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct SettingsStoreState {
    std::wstring path;
    bool open = false;
    bool readOnly = false;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct TabViewEntry {
    CView* view = nullptr;
    CString title;
    int image = -1;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct TabViewState {
    std::vector<TabViewEntry> tabs;
    int active = -1;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct FontHolderState {
    void* font = nullptr;
    void* notifySink = nullptr;
    HFONT hFont = nullptr;
    CString faceName;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
struct PictureHolderState {
    void* picture = nullptr;
    short type = 0;
    int width = 0;
    int height = 0;
    bool ownsHandle = false;
};
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::mutex g_tagManagerMutex;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, TagManagerState> g_tagManagerStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, DialogTemplateState> g_dialogTemplateStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, D2DTextState> g_d2dTextFormatStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, D2DTextState> g_d2dTextLayoutStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, MenuTearOffState> g_menuTearOffStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, ControlRendererInfoState> g_controlRendererInfoStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, ControlRendererState> g_controlRendererStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, ToolTipInfoState> g_toolTipInfoStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, SettingsStoreState> g_settingsStoreStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<std::wstring, std::map<std::wstring, SettingsValue>> g_settingsStoreValues;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, TabViewState> g_tabViewStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, FontHolderState> g_fontHolderStates;
} } }
namespace openmfc { namespace detail { namespace collectionsstrings {
extern std::map<void*, PictureHolderState> g_pictureHolderStates;
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
const wchar_t* CStringText(const CString* value);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
std::wstring Trim_CollectionsStrings(const std::wstring& value);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseUnsignedHex(const wchar_t* text, unsigned long* value);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseUnsignedDecimalOrHex(const wchar_t* text, unsigned long* value);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseInts(const wchar_t* text, int* values, int count);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseSingleInt(const wchar_t* text, int* value);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
std::vector<std::wstring> SplitList(const wchar_t* text);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseBoolText(const wchar_t* text, int* value);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
std::wstring NormalizeSettingPath(const std::wstring& base, const wchar_t* child = nullptr);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
std::map<std::wstring, SettingsValue>& CurrentSettingsValues(void* self);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseKeyValueList(const wchar_t* text, std::map<std::wstring, std::wstring>* values, std::vector<std::wstring>* positional = nullptr);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseRectText(const std::map<std::wstring, std::wstring>& values, const wchar_t* key, CRect* rect);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseControlRendererInfoText(const wchar_t* text, ControlRendererInfoState* info);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseToolTipInfoText(const wchar_t* text, ToolTipInfoState* info);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool LoadToolBarImagesFromText(const wchar_t* text, CMFCToolBarImages* images, HINSTANCE instance);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ParseFontText(const wchar_t* text, LOGFONTW* font);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
CString FontFaceFromHandle(HFONT font);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
HFONT EnsureHolderFont(void* self);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
void SetPictureState(void* self, void* picture, short type, int width = 0, int height = 0, bool owns = false);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
std::wstring MakeTagEnd(const std::wstring& tag);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ExtractNamedValue(const std::wstring& buffer, const wchar_t* tagName, std::wstring* value, bool trim);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool GetTagManagerValue(void* self, const CString* tag, std::wstring* value, bool trim = true);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ReadFileToWideString(const wchar_t* path, std::wstring* output);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ResourceToWideString(HRSRC resource, std::wstring* output);
} } }

bool ReadDialogFont(const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize);

namespace openmfc { namespace detail { namespace collectionsstrings {
void SetDialogTemplateState(void* self, const DLGTEMPLATE* tmpl);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
const D2DTextState* GetD2DFormatState(const void* self);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
const D2DTextState* GetD2DLayoutState(const void* self);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
unsigned int NextTearOffId(MenuTearOffState& state);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
const wchar_t* SkipDialogString(const wchar_t* cur);
} } }

bool ReadDialogFont(const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize);

namespace openmfc { namespace detail { namespace collectionsstrings {
std::wstring MakeWindowClassName(const wchar_t* requested);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
bool ExtractTaggedText(CString* buffer, const wchar_t* beginTag, const wchar_t* endTag);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
const wchar_t* DocumentTitleOrPathName(CDocument* doc);
} } }

namespace openmfc { namespace detail { namespace collectionsstrings {
std::wstring SanitizedFileName(const wchar_t* value);
} } }


















































































































































