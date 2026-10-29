// CMFCPropertyGridFileProperty — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <new>

namespace {

// CMFCPropertyGridFileProperty is not declared in include/openmfc. Its retail
// constructors at RVA 0xc54b0 and 0xc55f0 (mfc140u) initialize the derived tail
// after the 0x160-byte property base:
// BOOL folder, BOOL open-dialog, DWORD dialog flags, then two CString members.
// The two CString offsets are also read by the retail destructor.
struct FilePropertyTail {
    BOOL m_bIsFolder;
    BOOL m_bOpenFileDialog;
    DWORD m_dwFileOpenFlags;
    DWORD m_padding;
    CString m_strDefExt;
    CString m_strFilter;
};
constexpr std::size_t kFilePropertyBaseSize = 0x160;
static_assert(sizeof(CString) == 8, "retail CString occupies one pointer");
static_assert(offsetof(FilePropertyTail, m_bIsFolder) == 0x00, "m_bIsFolder");
static_assert(offsetof(FilePropertyTail, m_bOpenFileDialog) == 0x04, "m_bOpenFileDialog");
static_assert(offsetof(FilePropertyTail, m_dwFileOpenFlags) == 0x08, "m_dwFileOpenFlags");
static_assert(offsetof(FilePropertyTail, m_strDefExt) == 0x10, "m_strDefExt");
static_assert(offsetof(FilePropertyTail, m_strFilter) == 0x18, "m_strFilter");
static_assert(sizeof(FilePropertyTail) == 0x20, "retail file-property tail size");
static_assert(kFilePropertyBaseSize + offsetof(FilePropertyTail, m_strDefExt) == 0x170,
              "retail m_strDefExt offset");
static_assert(kFilePropertyBaseSize + offsetof(FilePropertyTail, m_strFilter) == 0x178,
              "retail m_strFilter offset");
static_assert(sizeof(CMFCPropertyGridProperty) <= kFilePropertyBaseSize,
              "OpenMFC property base must fit before the retail derived tail");

inline FilePropertyTail* FileTail(void* pThis) {
    return reinterpret_cast<FilePropertyTail*>(
        static_cast<unsigned char*>(pThis) + kFilePropertyBaseSize);
}

} // namespace

// Verified in phase4/src/featurepack/propertygrid/Thunks.cpp.
extern "C" void MS_ABI impl___1CMFCPropertyGridProperty__UEAA_XZ(void* pThis);
// The corresponding base-constructor thunk is defined in
// featurepack/propertygrid/CMFCPropertyGridProperty.cpp.
extern "C" void* MS_ABI impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
    void* pThis, const CString* name, const COleVariant* value,
    const wchar_t* description, unsigned long long data,
    const wchar_t* editMask, const wchar_t* editTemplate, const wchar_t* validChars);
extern "C" void MS_ABI impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(
    void* pThis, const wchar_t* text);

// Symbol: ??0CMFCPropertyGridFileProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@0_KPEB_W@Z
// Transcribed from retail entry RVA 0xc54b0 (mfc140u): make a VT_BSTR value
// from the second CString, call the base constructor with the name, description
// and data, initialize the two empty tail strings, then set the folder flag.
// OpenMFC has no derived MSVC vftable or equivalent base m_dwFlags storage, so
// the retail vfptr and +0x40 value 2 are not reproduced.
extern "C" void* MS_ABI impl___0CMFCPropertyGridFileProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__0_KPEB_W_Z(
    void* pThis, const CString* name, const CString* value, unsigned long long data,
    const wchar_t* description) {
    COleVariant varValue(value->GetString());
    impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
        pThis, name, &varValue, description, data, nullptr, nullptr, nullptr);

    FilePropertyTail* tail = FileTail(pThis);
    new (&tail->m_strDefExt) CString();
    new (&tail->m_strFilter) CString();
    tail->m_bIsFolder = TRUE;
    return pThis;
}
// Symbol: ??0CMFCPropertyGridFileProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H0PEB_WK11_K@Z
// Transcribed from retail entry RVA 0xc55f0 (mfc140u): initialize the base
// from the second CString and initial-directory string, then initialize the
// dialog type, flags, default extension, and filter at their verified tail
// offsets. The retail derived vfptr and base +0x40 value 2 are not reproduced:
// OpenMFC has no matching MSVC vftable or m_dwFlags storage.
extern "C" void* MS_ABI impl___0CMFCPropertyGridFileProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H0PEB_WK11_K_Z(
    void* pThis, const CString* name, int openFileDialog, const CString* value,
    const wchar_t* defaultExtension, unsigned long dialogFlags,
    const wchar_t* filter, const wchar_t* initialDirectory,
    unsigned long long data) {
    COleVariant varValue(value->GetString());
    impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
        pThis, name, &varValue, initialDirectory, data, nullptr, nullptr, nullptr);

    FilePropertyTail* tail = FileTail(pThis);
    new (&tail->m_strDefExt) CString();
    new (&tail->m_strFilter) CString();
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(&tail->m_strDefExt, defaultExtension);
    impl__SetString___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(&tail->m_strFilter, filter);
    tail->m_bIsFolder = FALSE;
    tail->m_bOpenFileDialog = openFileDialog;
    tail->m_dwFileOpenFlags = dialogFlags;
    return pThis;
}
// Symbol: ??1CMFCPropertyGridFileProperty@@UEAA@XZ
// Transcribed from retail entry RVA 0xc5760 (mfc140u): the destructor releases
// m_strFilter (+0x178), then m_strDefExt (+0x170), and tail-calls the
// CMFCPropertyGridProperty destructor. The retail vftable store is omitted:
// OpenMFC has no MSVC-layout vftable for this undeclared derived class.
extern "C" void MS_ABI impl___1CMFCPropertyGridFileProperty__UEAA_XZ(void* pThis) {
    FilePropertyTail* tail = FileTail(pThis);
    tail->m_strFilter.~CString();
    tail->m_strDefExt.~CString();
    impl___1CMFCPropertyGridProperty__UEAA_XZ(pThis);
}
// Symbol: ?OnClickButton@CMFCPropertyGridFileProperty@@UEAAXVCPoint@@@Z
// STUB: retail entry RVA 0xc57d0 (mfc140u) sets the base button-down flag
// (+0x64), copies the current value at +0x10 into a temporary CString, and
// branches on m_bIsFolder (+0x160). The folder path consults a global dialog
// object and invokes retail helper routines; the file path builds and runs a
// dialog using m_bOpenFileDialog (+0x164), flags (+0x168), default extension
// (+0x170), filter (+0x178), and the base path (+0x118). On a successful
// selection either path updates through base members at +0x100 and +0x08; both
// paths then clear the flag and
// run further base helpers. OpenMFC has no retail base-member layout or matching
// folder/file-dialog helper infrastructure to reproduce these paths faithfully.
// CPoint is passed by value (an 8-byte MS ABI aggregate), not as a pointer.
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridFileProperty__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}
