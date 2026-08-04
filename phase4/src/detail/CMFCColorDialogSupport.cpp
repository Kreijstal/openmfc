#include "CMFCColorDialogSupport.h"

static_assert(sizeof(CPalHolder_Cmfccolordialog) == 0x10, "CPalHolder_Cmfccolordialog must be 0x10 bytes");
static_assert(offsetof(CColorDlg, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(CColorDlg, m_pPalette) == 0x188, "palette @0x188");
static_assert(offsetof(CColorDlg, m_nCurrentColor) == 0x190, "current color @0x190");
static_assert(offsetof(CColorDlg, m_nNewColor) == 0x194, "new color @0x194");
static_assert(offsetof(CColorDlg, m_hCursor) == 0x198, "cursor @0x198");
static_assert(offsetof(CColorDlg, m_nTracking) == 0x1a4, "tracking @0x1a4");
