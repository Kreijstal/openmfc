#include "ManualThunksSupport.h"

namespace openmfc { namespace detail { namespace manualthunks {
std::mutex g_manualThunkStateMutex;
std::unordered_map<void*, RibbonWriteState> g_ribbonWriterState;
std::unordered_map<void*, int> g_dynamicLayoutState;
std::unordered_map<void*, int> g_dataSourceControlState;
std::wstring ManualThunkCStringText(const CString* text) {
    return text ? std::wstring(static_cast<const wchar_t*>(*text)) : std::wstring();
}
int RecordRibbonWrite(void* parser, const wchar_t* name) {
    std::lock_guard<std::mutex> lock(g_manualThunkStateMutex);
    g_ribbonWriterState[parser].events.emplace_back(name ? name : L"");
    return 1;
}
int RecordRibbonAttribute(void* parser, const CString* name, const std::wstring& value) {
    std::lock_guard<std::mutex> lock(g_manualThunkStateMutex);
    std::wstring event = ManualThunkCStringText(name);
    event += L"=";
    event += value;
    g_ribbonWriterState[parser].events.push_back(event);
    return 1;
}
std::wstring ManualThunkIntText(int value, int defaultValue) {
    wchar_t buffer[64] = {};
    std::swprintf(buffer, 64, L"%d:%d", value, defaultValue);
    return buffer;
}
std::wstring ManualThunkUIntText(unsigned int value, unsigned int defaultValue) {
    wchar_t buffer[64] = {};
    std::swprintf(buffer, 64, L"%u:%u", value, defaultValue);
    return buffer;
}
std::wstring ManualThunkULongText(unsigned long value, unsigned long defaultValue) {
    wchar_t buffer[64] = {};
    std::swprintf(buffer, 64, L"%lu:%lu", value, defaultValue);
    return buffer;
}
std::wstring ManualThunkSizeText(const CSize* value, const CSize* defaultValue) {
    wchar_t buffer[96] = {};
    const int cx = value ? value->cx : 0;
    const int cy = value ? value->cy : 0;
    const int defaultCx = defaultValue ? defaultValue->cx : 0;
    const int defaultCy = defaultValue ? defaultValue->cy : 0;
    std::swprintf(buffer, 96, L"%d,%d:%d,%d", cx, cy, defaultCx, defaultCy);
    return buffer;
}
} } }  // namespace openmfc::detail::manualthunks
