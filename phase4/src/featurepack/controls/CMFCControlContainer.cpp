// CMFCControlContainer — OpenMFC implementation.
// Sources: global_cmfccontrolcontainer.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCControlContainerSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// FreeSubclassedControls(): destroys every control in the CObArray at +0x10
// and empties the array.  Transcribed from retail RVA 0x33790; the per-control
// virtual scalar-deleting destructor dispatch (vtable+0x8 via 0x1802c7b30) is
// not modeled, so controls are left alive (safe: the stub constructor never
// fills the array).
// Symbol: ?FreeSubclassedControls@CMFCControlContainer@@QEAAXXZ
extern "C" void MS_ABI impl__FreeSubclassedControls_CMFCControlContainer__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CContainer* s = reinterpret_cast<CContainer*>(pThis);
    if (s->m_nControls > 0 && s->m_pControls != nullptr) {
        for (long long i = 0; i < s->m_nControls; i++) {
            if (s->m_pControls[i] != nullptr) {
                // TODO(clean-room): transcribed partially -- retail dispatches
                // the virtual deleting destructor (0x1802c7b30, vslot+0x8) on
                // each control; not modeled to avoid calling into an unknown
                // vtable.
            }
        }
    }
    // CObArray::SetSize(&0x10, 0, -1) (0x1801d45d0): release the buffer.
    if (s->m_pControls != nullptr) std::free(s->m_pControls);
    s->m_pControls = nullptr;
    s->m_nControls = 0;
    s->m_nMaxControls = 0;
    s->m_nGrowBy = -1;
}
// ClearControlData(): deletes every value in the CMapWordToOb at +0x38 and
// removes all entries.  Transcribed from retail RVA 0x33de0; the map
// iteration (CMapWordToOb::GetNextAssoc 0x1802343e0) and the per-value
// virtual-destructor dispatch are not modeled, and the CMapPtrToPtr::RemoveAll
// teardown (0x18001ba40) is only performed when the table pointer is present
// so an uninitialized map is never walked.
// Symbol: ?ClearControlData@CMFCControlContainer@@QEAAXXZ
extern "C" void MS_ABI impl__ClearControlData_CMFCControlContainer__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    char* map = reinterpret_cast<char*>(pThis) + 0x38;
    const long long nCount = *reinterpret_cast<long long*>(map + 0x18);  // m_nCount
    if (nCount != 0) {
        // TODO(clean-room): transcribed partially -- retail iterates the map
        // with GetNextAssoc and deletes each CObject* value through its
        // vtable; not modeled.
    }
    // CMapPtrToPtr::RemoveAll(&this+0x38): free the assoc chains and table.
    void** pTable = *reinterpret_cast<void***>(map + 0x8);   // m_pHashTable
    if (pTable != nullptr) {
        const UINT nSize = *reinterpret_cast<UINT*>(map + 0x10);  // m_nHashTableSize
        for (UINT i = 0; i < nSize; i++) {
            void* pAssoc = pTable[i];
            while (pAssoc != nullptr) {
                void* pNext = *reinterpret_cast<void**>(pAssoc);  // pNext at +0
                std::free(pAssoc);
                pAssoc = pNext;
            }
        }
        std::free(pTable);
        *reinterpret_cast<void***>(map + 0x8) = nullptr;
    }
    *reinterpret_cast<long long*>(map + 0x18) = 0;
}
// GetControlData(WORD nID, DWORD& cbData, BYTE*& lpData): looks the control
// up in the map at +0x38 and, on success, writes the value's size (+0x10) and
// data pointer (+0x8).  Transcribed from retail RVA 0x33d80; the
// CMapWordToOb::Lookup (0x180234280) is not modeled, so the not-found result
// is returned (the stub constructor never populates the map).
// Symbol: ?GetControlData@CMFCControlContainer@@QEAAHGAEAKAEAPEAE@Z
extern "C" int MS_ABI impl__GetControlData_CMFCControlContainer__QEAAHGAEAKAEAPEAE_Z(
    void* pThis, unsigned short /*nID*/, unsigned long& /*cbData*/,
    unsigned char*& /*lpData*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): transcribed partially -- retail calls
    // CMapWordToOb::Lookup(&0x38, nID, &value); on success copies
    // value+0x10 (DWORD size) into cbData and value+0x8 (BYTE* data) into
    // lpData and returns TRUE.  The map is unmodeled.
    return 0;
}
// CreateDlgControl(LPCTSTR lpszClassName): class factory that creates the MFC
// control object for one of the eleven recognised class names.  Transcribed
// from retail RVA 0x33800; the constructor calls for the eleven control
// classes are not all available here, so the not-created result is returned.
// Symbol: ?CreateDlgControl@CMFCControlContainer@@IEAAPEAVCWnd@@PEB_W@Z
extern "C" CWnd* MS_ABI impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(
    void* pThis, const wchar_t* /*lpszClassName*/)
{
    if (pThis == nullptr) return nullptr;
    // TODO(clean-room): transcribed partially -- retail compares the class
    // name (ATL CString, 0x18000dcb0 / CString::Compare 0x1802c7770) against
    // "MFCButton", "MFCColorButton", "MFCEditBrowse", "MFCFontComboBox",
    // "MFCLink", "MFCMaskedEdit", "MFCMenuButton", "MFCPropertyGrid",
    // "MFCShellList", "MFCShellTree" and "MFCVSListBox" (0x18033ed18..
    // 0x18033ee40), allocates the matching object (0xb20/0xbc8/0x150/0x100/
    // 0xb40/0x128/0xb48/0x6b8/0x278/0x100/0x348 bytes) and runs its
    // constructor (0x18001c930/0x180027ff0/0x18005f270/0x180060ef0/
    // 0x1800781b0/0x180078d70/0x18008e2c0/0x1800c5ef0/0x18012a760/
    // 0x18012d010/0x1801c6490).  The constructors are not all real here, so
    // the not-created result (matching an unrecognized name) is returned.
    return nullptr;
}
// IsSubclassedFeaturePackControl(HWND hWnd): returns TRUE when one of the
// subclassed controls in the CObArray at +0x10 has the given window handle.
// Transcribed verbatim from retail RVA 0x33af0 (the loop compares each
// control's m_hWnd at +0x40 against hWnd).
// Symbol: ?IsSubclassedFeaturePackControl@CMFCControlContainer@@QEAAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__IsSubclassedFeaturePackControl_CMFCControlContainer__QEAAHPEAUHWND_____Z(
    void* pThis, HWND hWnd)
{
    if (pThis == nullptr || hWnd == nullptr) return 0;
    CContainer* s = reinterpret_cast<CContainer*>(pThis);
    if (s->m_nControls <= 0 || s->m_pControls == nullptr) return 0;
    for (long long i = 0; i < s->m_nControls; i++) {
        const char* pControl = reinterpret_cast<const char*>(s->m_pControls[i]);
        if (pControl != nullptr &&
            *reinterpret_cast<const HWND*>(pControl + 0x40) == hWnd) {
            return 1;
        }
    }
    return 0;
}
// PreUnsubclassControl(CWnd* pWnd): for CMFCShellListCtrl controls the retail
// (RVA 0x33f40) IsKindOf's the control against the shell-list runtime class
// (0x18030eb88), then calls the virtual at vslot 0x2e0 twice and
// UnsubclassWindow's the returned inner list window.  The vslot dispatch is
// not modeled, so the type check is kept and the unsubclass branch is omitted.
// Symbol: ?PreUnsubclassControl@CMFCControlContainer@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__PreUnsubclassControl_CMFCControlContainer__IEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pWnd)
{
    if (pThis == nullptr || pWnd == nullptr) return;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            reinterpret_cast<CObject*>(pWnd),
            impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ())) {
        return;
    }
    // TODO(clean-room): transcribed partially -- retail dispatches the
    // CMFCShellListCtrl virtual at vslot 0x2e0 (twice) and calls
    // CWnd::UnsubclassWindow on the result; the vslot is not modeled.
}
// ReSubclassControl(HWND hWnd, WORD nID, CWnd& pControl): locates the control
// whose m_hWnd matches hWnd, moves its control data (GetControlData, RVA
// 0x33d80) to the replacement, destroys the old control and subclasses the new
// one.  Transcribed from retail RVA 0x33b50; the CMapWordToOb-backed
// GetControlData returns not-found here (the map is unmodeled), so the
// 0x37c data-relay message is never sent.
// Symbol: ?ReSubclassControl@CMFCControlContainer@@QEAAHPEAUHWND__@@GAEAVCWnd@@@Z
extern "C" int MS_ABI impl__ReSubclassControl_CMFCControlContainer__QEAAHPEAUHWND____GAEAVCWnd___Z(
    void* pThis, HWND hWnd, unsigned short nID, CWnd& pControl)
{
    if (pThis == nullptr || hWnd == nullptr) return 0;
    CContainer* s = reinterpret_cast<CContainer*>(pThis);
    if (s->m_nControls <= 0 || s->m_pControls == nullptr) return 0;

    long long idx = -1;
    char* pOld = nullptr;
    for (long long i = 0; i < s->m_nControls; i++) {
        char* c = reinterpret_cast<char*>(s->m_pControls[i]);
        if (c != nullptr && *reinterpret_cast<HWND*>(c + 0x40) == hWnd) {
            idx = i;
            pOld = c;
            break;
        }
    }
    if (idx < 0) return 0;

    // Retail guards with CObject::IsKindOf against the runtime class at
    // 0x180337490 (unidentified) and requires a live m_hWnd.
    // TODO(clean-room): transcribed partially -- the IsKindOf class is
    // unknown, so the check is treated as passing.
    if (*reinterpret_cast<HWND*>(pOld + 0x40) == nullptr) return 0;

    // Move the old control's data to the replacement (retail GetControlData at
    // 0x180033d80; the map is unmodeled so this returns not-found).
    unsigned long cbData = 0;
    unsigned char* lpData = nullptr;
    const int bGot = impl__GetControlData_CMFCControlContainer__QEAAHGAEAKAEAPEAE_Z(
        pThis, nID, cbData, lpData);

    // Drop the old control from the array.
    if (idx >= s->m_nControls) return 0;
    s->m_pControls[idx] = nullptr;

    // TODO(clean-room): transcribed partially -- retail runs PreUnsubclass
    // Control, CWnd::UnsubclassWindow and the scalar-deleting destructor
    // (vslot 0x8) on the old control; the deleting destructor dispatch is not
    // modeled, so the object is left alive.

    if (!impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(&pControl, hWnd)) {
        return 0;
    }
    if (bGot != 0 && cbData != 0 && pControl.m_hWnd != nullptr) {
        // Retail relays the old control's data to the replacement with the
        // MFC message 0x37c (wParam = byte count, lParam = data pointer).
        ::SendMessageW(pControl.m_hWnd, 0x37c,
                       static_cast<WPARAM>(cbData),
                       reinterpret_cast<LPARAM>(lpData));
    }
    return 1;
}
// SetControlData(WORD nID, DWORD cbData, BYTE* lpData): stores a control-data
// block under nID.  Transcribed partially from retail RVA 0x33c90 -- the
// retail allocates a 0x28-byte CByteArray record (vtable 0x180321100),
// SetSize(cbData, -1), copies lpData into it and inserts it into the
// CMapWordToOb at +0x38 via operator[] (0x1802342b0).  The map is unmodeled
// (GetControlData returns not-found), so nothing can be inserted.
// Symbol: ?SetControlData@CMFCControlContainer@@QEAAXGKPEAE@Z
extern "C" void MS_ABI impl__SetControlData_CMFCControlContainer__QEAAXGKPEAE_Z(
    void* pThis, unsigned short /*nID*/, unsigned long /*cbData*/,
    unsigned char* /*lpData*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- the retail allocates a
    // 0x28-byte CByteArray, SetSize(cbData, -1) (0x1801d3850), memcpy's
    // lpData (or memsets when null) and stores the record in the map at +0x38
    // (CMapWordToOb::operator[] 0x1802342b0).  The unmodeled map cannot be
    // safely written, so nothing is stored (GetControlData stays not-found).
}
// SubclassDlgControls(): walks the parent's child windows, creates a control
// for each via CreateDlgControl and subclasses it.  Transcribed from retail
// RVA 0x33e60; CreateDlgControl is conservative (returns nullptr), so no
// controls are appended in practice, but the enumeration, the array append
// (CObArray::SetAtGrow 0x1801d4910) and the SubclassWindow call are modeled.
// Symbol: ?SubclassDlgControls@CMFCControlContainer@@QEAAHXZ
extern "C" int MS_ABI impl__SubclassDlgControls_CMFCControlContainer__QEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    CContainer* s = reinterpret_cast<CContainer*>(pThis);
    if (s->m_pWnd == nullptr) return 0;
    CWnd* pParent = reinterpret_cast<CWnd*>(s->m_pWnd);
    if (pParent->m_hWnd == nullptr) return 0;

    HWND hChild = ::GetWindow(pParent->m_hWnd, GW_CHILD);   // 0x1802c6c10
    while (hChild != nullptr) {
        CWnd* pFrom = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hChild);
        if (pFrom != nullptr && pFrom->m_hWnd != nullptr) {
            wchar_t buf[0x400];
            ::GetWindowTextW(pFrom->m_hWnd, buf, 0x400);    // 0x1802c7280
            CWnd* pControl = impl__CreateDlgControl_CMFCControlContainer__IEAAPEAVCWnd__PEB_W_Z(
                pThis, buf);                                 // 0x180033800
            if (pControl != nullptr) {
                ContainerArrayAppend(s, pControl);
                impl__SubclassWindow_CWnd__QEAAHPEAUHWND_____Z(pControl, pFrom->m_hWnd);
            }
        }
        hChild = ::GetWindow(hChild, GW_HWNDNEXT);           // 0x1802c6c10
    }
    return 1;
}
// UTF8ToString(const char* pszUTF8, CString& strResult, int cbUTF8): converts
// a UTF-8 byte range into strResult (Unicode) and returns the required UTF-16
// code-unit count.  Transcribed from retail RVA 0x34080 (MultiByteToWideChar
// with CP_UTF8, a malloc'd (cb+1) wchar_t buffer, CString::SetString via
// 0x180002e30, then free).
// Symbol: ?UTF8ToString@CMFCControlContainer@@SAHPEBDAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" int MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const char* pszUTF8, CString* pStrResult, int cbUTF8)
{
    if (pszUTF8 == nullptr || pStrResult == nullptr) return 0;
    const int cchWide =
        ::MultiByteToWideChar(CP_UTF8, 0, pszUTF8, cbUTF8, nullptr, 0);
    if (cchWide <= 0) return 0;
    wchar_t* buf = static_cast<wchar_t*>(
        std::calloc(static_cast<size_t>(cchWide) + 1, sizeof(wchar_t)));
    if (buf == nullptr) return 0;
    ::MultiByteToWideChar(CP_UTF8, 0, pszUTF8, cbUTF8, buf, cchWide);
    *pStrResult = buf;
    std::free(buf);
    return cchWide;
}
// Symbol: ??0CMFCControlContainer@@QEAA@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCControlContainer__QEAA_PEAVCWnd___Z(void* pThis, void* pWnd) {
    (void)pWnd;
    return pThis;
}
