#include "CMFCAcceleratorKeySupport.h"

static_assert(sizeof(S_Cmfcacceleratorkey) == 16, "CMFCAcceleratorKey must be 16 bytes");
static_assert(offsetof(S_Cmfcacceleratorkey, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S_Cmfcacceleratorkey, m_lpAccel) == 8, "m_lpAccel at 8");
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
void* MS_ABI slot_GetRuntimeClass_Cmfcacceleratorkey(const void* pThis) {
    return impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(pThis);
}
void MS_ABI slot_Serialize_Cmfcacceleratorkey(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI slot_AssertValid_Cmfcacceleratorkey(const void* pThis) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI slot_Dump_Cmfcacceleratorkey(const void* pThis, void* /*dc*/) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::Dump();
}
void AddVirtKeyStrImpl(CString& str, UINT uiVirtKey, BOOL bLast) {
    UINT uiScanCode = MapVirtualKeyW(uiVirtKey, MAPVK_VK_TO_VSC);
    LONG lParam = (LONG)(uiScanCode << 16);

    // Extended keys need bit 24 set so GetKeyNameText resolves them correctly
    // (e.g. the right-hand Alt/Ctrl and the grey navigation cluster).
    switch (uiVirtKey) {
    case VK_INSERT: case VK_DELETE:
    case VK_HOME:   case VK_END:
    case VK_PRIOR:  case VK_NEXT:
    case VK_LEFT:   case VK_RIGHT:
    case VK_UP:     case VK_DOWN:
    case VK_NUMLOCK: case VK_DIVIDE:
    case VK_RCONTROL: case VK_RMENU:
        lParam |= (1L << 24);
        break;
    default:
        break;
    }

    wchar_t szKey[64];
    szKey[0] = L'\0';
    if (GetKeyNameTextW(lParam, szKey, 64) != 0) {
        str += szKey;
    }

    if (!bLast) {
        str += L"+";
    }
}
void* MS_ABI slot_vecDelDtor_Cmfcacceleratorkey(void* pThis, unsigned flags) {
    // Destructor body is a no-op (borrowed ACCEL); honour the delete flag.
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}
} } }  // namespace openmfc::detail::cmfcacceleratorkey
