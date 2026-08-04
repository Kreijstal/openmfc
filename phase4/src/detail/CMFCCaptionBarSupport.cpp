#include "CMFCCaptionBarSupport.h"

static_assert(sizeof(CB) == 0x6b8, "CMFCCaptionBar mirror must be 0x6B8 bytes");
static_assert(offsetof(CB, m_hWnd) == 0x40, "CMFCCaptionBar m_hWnd @0x40");
static_assert(offsetof(CB, m_nTransparent) == 0x4f0, "CMFCCaptionBar m_nTransparent @0x4f0");
static_assert(offsetof(CB, m_nBitmapIndex) == 0x5b0, "CMFCCaptionBar m_nBitmapIndex @0x5b0");
static_assert(offsetof(CB, m_nButtonAlign) == 0x5b4, "CMFCCaptionBar m_nButtonAlign @0x5b4");
static_assert(offsetof(CB, m_rectText) == 0x624, "CMFCCaptionBar m_rectText @0x624");
static_assert(offsetof(CB, m_nTextAligned) == 0x634, "CMFCCaptionBar m_nTextAligned @0x634");
static_assert(offsetof(CB, m_nID) == 0x650, "CMFCCaptionBar m_nID @0x650");
static_assert(offsetof(CB, m_nTextAlign) == 0x654, "CMFCCaptionBar m_nTextAlign @0x654");
static_assert(offsetof(CB, m_bButtonHighlight) == 0x670, "CMFCCaptionBar m_bButtonHighlight @0x670");
static_assert(offsetof(CB, m_bIsClick) == 0x67c, "CMFCCaptionBar m_bIsClick @0x67c");
static_assert(offsetof(CB, m_rectGripper) == 0x6a4, "CMFCCaptionBar m_rectGripper @0x6a4");
