#include "CMFCAcceleratorKeyAssignCtrlSupport.h"

static_assert(offsetof(CAccelKeyAssignCtrl, m_hWnd) == 0x40, "hwnd @0x40");
static_assert(offsetof(CAccelKeyAssignCtrl, m_bAccelFlags) == 0xe8, "flags @0xe8");
static_assert(offsetof(CAccelKeyAssignCtrl, m_flags) == 0xec, "flags byte @0xec");
static_assert(offsetof(CAccelKeyAssignCtrl, m_bKillFocus) == 0x108, "kill-focus @0x108");
