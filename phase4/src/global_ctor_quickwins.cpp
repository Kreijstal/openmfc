// Hand-written real implementations for header-defined classes that were one
// export short of complete. Constructors placement-new into the caller-provided
// storage so the openmfc C++ constructor establishes the validated member
// layout. Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) removes the weak/typed stubs for these ordinals
// automatically.

#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#include <new>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Runtime-class descriptors used by COleException::Process, defined alongside
// the exception RTTI tables (ole_oleexception_rtti.cpp,
// global_simple_exceptions_rtti.cpp).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();

// CWnd::CWnd(HWND) — private constructor MFC uses internally to wrap an existing
// window handle. Default-initializes the CWnd members (validated layout) and
// then binds the handle, matching the observable state of the real ctor.
// Symbol: ??0CWnd@@AEAA@PEAUHWND__@@@Z
extern "C" void* MS_ABI impl___0CWnd__AEAA_PEAUHWND_____Z(void* pThis, HWND hWnd)
{
    CWnd* p = new (pThis) CWnd();
    p->m_hWnd = hWnd;
    return p;
}

// CFileFind::CFileFind(ATL::CAtlTransactionManager*) — the transacted-file
// variant. OpenMFC's CFileFind does not model transactions, so this initializes
// identically to CFileFind::CFileFind(); the transaction-manager argument is
// accepted for ABI compatibility and otherwise unused.
// Symbol: ??0CFileFind@@QEAA@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" void* MS_ABI impl___0CFileFind__QEAA_PEAVCAtlTransactionManager_ATL___Z(void* pThis, void* /*pTM*/)
{
    return new (pThis) CFileFind();
}

// CPen::CPen(int nPenStyle, int nWidth, const LOGBRUSH*, int nStyleCount,
//            const DWORD* lpStyle) — the geometric/cosmetic ExtCreatePen form.
// Mirrors the existing CPen(int,int,COLORREF) thunk in gdicore.cpp, which stores
// the created handle straight into CGdiObject::m_hObject.
// Symbol: ??0CPen@@QEAA@HHPEBUtagLOGBRUSH@@HPEBK@Z
extern "C" CPen* MS_ABI impl___0CPen__QEAA_HHPEBUtagLOGBRUSH__HPEBK_Z(
    CPen* pThis, int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
    int nStyleCount, const DWORD* lpStyle)
{
    if (!pThis) return nullptr;
    pThis->m_hObject = ::ExtCreatePen(static_cast<DWORD>(nPenStyle),
                                      static_cast<DWORD>(nWidth),
                                      pLogBrush,
                                      static_cast<DWORD>(nStyleCount),
                                      lpStyle);
    return pThis;
}

// CGopherFile::CGopherFile(HINTERNET, CGopherLocator&, CGopherConnection*) —
// protected ctor; the openmfc C++ ctor is defined in inetcore.cpp.
// Symbol: ??0CGopherFile@@IEAA@PEAXAEAVCGopherLocator@@PEAVCGopherConnection@@@Z
extern "C" void* MS_ABI impl___0CGopherFile__IEAA_PEAXAEAVCGopherLocator__PEAVCGopherConnection___Z(
    void* pThis, void* hFile, CGopherLocator* pLocator, CGopherConnection* pConnection)
{
    return new (pThis) CGopherFile(static_cast<HINTERNET>(hFile), *pLocator, pConnection);
}

// CGopherLocator::CGopherLocator(const wchar_t* pstrLocator, DWORD dwLen) —
// private ctor that takes ownership of a copy of the locator buffer. dwLen is the
// locator length in characters; the buffer is malloc'd to match ~CGopherLocator,
// which releases it with free().
// Symbol: ??0CGopherLocator@@AEAA@PEB_WK@Z
extern "C" void* MS_ABI impl___0CGopherLocator__AEAA_PEB_WK_Z(
    void* pThis, const wchar_t* pstrLocator, DWORD dwLen)
{
    CGopherLocator* p = new (pThis) CGopherLocator();
    if (pstrLocator) {
        wchar_t* buf = static_cast<wchar_t*>(std::malloc((static_cast<size_t>(dwLen) + 1) * sizeof(wchar_t)));
        if (buf) {
            std::memcpy(buf, pstrLocator, static_cast<size_t>(dwLen) * sizeof(wchar_t));
            buf[dwLen] = L'\0';
            p->m_lpBuffer = buf;
            p->m_dwBufferLength = dwLen;
        }
    }
    return p;
}

// CWindowlessDC::CWindowlessDC(HDC, CPoint&) — the DC wrapper windowless OLE
// controls use to draw into a host-supplied device context. The harvested
// descriptor (global_taskdialog_dc_rtti.cpp) gives sizeof 40 over base CDC
// (sizeof 32), so the CDC subobject is constructed in place and bound to the
// caller's HDC. The viewport offset carried by the CPoint is deliberately NOT
// applied: mutating the caller's DC origin here would be a destructive guess,
// and binding the handle is the state clients actually observe (previously
// m_hDC was left unset, so any use of the object faulted).
// Symbol: ??0CWindowlessDC@@QEAA@PEAUHDC__@@AEAVCPoint@@@Z
extern "C" void* MS_ABI impl___0CWindowlessDC__QEAA_PEAUHDC____AEAVCPoint___Z(
    void* pThis, HDC hDC, CPoint* /*pPoint*/)
{
    static_assert(sizeof(CDC) <= 40, "CDC must fit the harvested CWindowlessDC size");
    CDC* p = new (pThis) CDC();
    p->m_hDC = hDC;
    p->m_hAttribDC = hDC;
    return p;
}

// CEnumFormatEtc::OnNext(void*) — MFC's CEnumArray hook that fetches the single
// next element into the caller's buffer and advances the cursor, returning TRUE
// while elements remain. Implemented over the class's own public IEnumFORMATETC
// ::Next, so it stays consistent with m_position/m_count bookkeeping.
// Symbol: ?OnNext@CEnumFormatEtc@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumFormatEtc__MEAAHPEAX_Z(CEnumFormatEtc* pThis, void* pv)
{
    if (!pThis || !pv) return FALSE;
    ULONG fetched = 0;
    HRESULT hr = pThis->Next(1, static_cast<FORMATETC*>(pv), &fetched);
    return (hr == S_OK && fetched == 1) ? TRUE : FALSE;
}

// CMFCRibbonSpinButtonCtrl::OnDeltapos(NMHDR*, LRESULT*) — UDN_DELTAPOS
// reflection handler. Retail (mfc140u, verified at the export's RVA) first
// tests its owning ribbon-edit pointer at this+0x108 and, when that is null,
// takes the early-out `mov qword ptr [pResult],0 / ret`. OpenMFC never
// populates that pointer (the ribbon edit itself is unimplemented), so the
// early-out is the path retail would always take here, and it is reproduced
// exactly rather than dereferencing state we do not model.
//
// Zeroing pResult matters: the generated stub left it untouched, so the common
// control read whatever the caller had on the stack and could reject the spin.
// Symbol: ?OnDeltapos@CMFCRibbonSpinButtonCtrl@@AEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnDeltapos_CMFCRibbonSpinButtonCtrl__AEAAXPEAUtagNMHDR__PEA_J_Z(
    void* /*pThis*/, void* /*pNMHDR*/, LRESULT* pResult)
{
    if (pResult) *pResult = 0;
}

// CBaseKeyFrame::AddToStoryboard(IUIAnimationStoryboard*, BOOL) — the base
// keyframe represents UI_ANIMATION_KEYFRAME_STORYBOARD_START, which is already
// part of every storyboard, so there is nothing to add. Retail mfc140u compiles
// this to exactly `mov eax,1 / ret` (verified at the export's RVA): it touches
// no members and ignores both arguments. Derived keyframes override it.
// Symbol: ?AddToStoryboard@CBaseKeyFrame@@UEAAHPEAUIUIAnimationStoryboard@@H@Z
extern "C" int MS_ABI impl__AddToStoryboard_CBaseKeyFrame__UEAAHPEAUIUIAnimationStoryboard__H_Z(
    void* /*pThis*/, void* /*pStoryboard*/, int /*bDeepAdd*/)
{
    return TRUE;
}

// COleException::Process(const CException*) — maps an MFC exception onto the
// SCODE the OLE layer reports. This is MFC's documented mapping: a COleException
// yields its own m_sc, the standard exception types map to their COM equivalents,
// and anything else is E_UNEXPECTED.
// Symbol: ?Process@COleException@@SAJPEBVCException@@@Z
extern "C" LONG MS_ABI impl__Process_COleException__SAJPEBVCException___Z(const CException* pAnyException)
{
    if (!pAnyException) return E_UNEXPECTED;

    if (pAnyException->IsKindOf(impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ()))
        return static_cast<const COleException*>(pAnyException)->m_sc;
    if (pAnyException->IsKindOf(impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ()))
        return E_OUTOFMEMORY;
    if (pAnyException->IsKindOf(impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ()))
        return E_NOTIMPL;
    if (pAnyException->IsKindOf(impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ()))
        return E_INVALIDARG;

    return E_UNEXPECTED;
}
