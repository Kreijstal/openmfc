// Second batch of COleControl methods implemented on the retail member layout
// transcribed in a28f781. Each was decoded from the retail mfc140u export
// (ordinal -> RVA -> objdump) rather than inferred.
//
// Retail bitfield word at this+0x160; bit positions used below are the ones
// the shipping code tests or sets:
//   bit 16 (0x10) via bts = m_bSimpleFrame
//   bit 18        = the invoke-allowed flag read by IsInvokeAllowed
//
// Other offsets referenced:
//   0x150 = 336 m_pReflect
//   0x178 = 376 m_font
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

static_assert(offsetof(COleControl, m_pReflect) == 336, "m_pReflect @336");
static_assert(offsetof(COleControl, m_font) == 376, "m_font @376");

// The flags word is addressed raw: several of the bits retail tests here have
// no name in this header, and reading the word is exactly what the shipping
// code does.
static inline DWORD* FlagsOf(COleControl* p)
{ return reinterpret_cast<DWORD*>(reinterpret_cast<char*>(p) + 352); }
static inline const DWORD* FlagsOf(const COleControl* p)
{ return reinterpret_cast<const DWORD*>(reinterpret_cast<const char*>(p) + 352); }

// COleControl::EnableSimpleFrame() — retail is a single bit set:
//     bts DWORD PTR [rcx+0x160],0x10 ; ret
// It only raises the flag; it does not create the frame or notify the
// container. There is no matching disable entry point in retail either.
// Symbol: ?EnableSimpleFrame@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__EnableSimpleFrame_COleControl__QEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    *FlagsOf(pThis) |= (1u << 16);
}

// COleControl::IsInvokeAllowed(DISPID) — retail:
//     mov eax,[rcx+0x160] ; shr eax,0x12 ; and eax,1 ; ret
// Bit 18 is returned directly and the DISPID argument is ignored entirely --
// the base class makes no per-member decision, it reports whether the control
// is in a state that permits automation at all. Derived classes override.
// Symbol: ?IsInvokeAllowed@COleControl@@MEAAHJ@Z
extern "C" int MS_ABI impl__IsInvokeAllowed_COleControl__MEAAHJ_Z(
    COleControl* pThis, long /*dispid*/)
{
    if (!pThis) return FALSE;
    return static_cast<int>((*FlagsOf(pThis) >> 18) & 1u);
}

// COleControl::GetOuterWindow() const — retail:
//     mov rax,[rcx+0x150] ; test rax,rax ; cmovne rcx,rax ; mov rax,rcx ; ret
// i.e. the reflected window if one is installed, otherwise the control itself.
// The cmov makes the fallback branchless but the semantics are a plain
// null-check; note it never returns null for a valid control.
// Symbol: ?GetOuterWindow@COleControl@@IEBAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetOuterWindow_COleControl__IEBAPEAVCWnd__XZ(
    const COleControl* pThis)
{
    if (!pThis) return nullptr;
    CWnd* pReflect = pThis->m_pReflect;
    return pReflect != nullptr ? pReflect
                               : const_cast<CWnd*>(static_cast<const CWnd*>(pThis));
}

// COleControl::InternalGetFont() — retail is `lea rax,[rcx+0x178] ; ret`:
// it hands back a reference to the embedded CFontHolder, not a copy and not a
// COM pointer, so callers mutate the control's own font state through it.
// Symbol: ?InternalGetFont@COleControl@@QEAAAEAVCFontHolder@@XZ
extern "C" CFontHolder* MS_ABI impl__InternalGetFont_COleControl__QEAAAEAVCFontHolder__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    return &pThis->m_font;
}

// COleControl::GetWindowlessDropTarget() — retail is `xor eax,eax ; ret`.
// The base control supplies no drop target; a windowless control that wants
// drag-and-drop overrides this to return one.
// Symbol: ?GetWindowlessDropTarget@COleControl@@UEAAPEAUIDropTarget@@XZ
extern "C" void* MS_ABI impl__GetWindowlessDropTarget_COleControl__UEAAPEAUIDropTarget__XZ(
    COleControl* /*pThis*/)
{
    return nullptr;
}

// COleControl::OnCreateAggregates() — retail is `mov eax,0x1 ; ret`.
// The base class aggregates nothing and reports success unconditionally;
// derived controls override to create their aggregates and may fail.
// Symbol: ?OnCreateAggregates@COleControl@@MEAAHXZ
extern "C" int MS_ABI impl__OnCreateAggregates_COleControl__MEAAHXZ(COleControl* /*pThis*/)
{
    return TRUE;
}

// COleControl::GetPropPageIDs(DWORD&) — retail:
//     mov DWORD PTR [rdx],0x0 ; xor eax,eax ; ret
// The base control publishes no property pages: it writes a zero count through
// the out-parameter and returns a null array. Writing the count matters --
// the generated stub left the caller's DWORD untouched, so a container could
// walk a null array using stack garbage as the length.
// Symbol: ?GetPropPageIDs@COleControl@@MEAAPEAU_GUID@@AEAK@Z
extern "C" GUID* MS_ABI impl__GetPropPageIDs_COleControl__MEAAPEAU_GUID__AEAK_Z(
    COleControl* /*pThis*/, DWORD* pcPages)
{
    if (pcPages) *pcPages = 0;
    return nullptr;
}
