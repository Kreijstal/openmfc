// CBlobProperty — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CBlobProperty is MFC's private IPersistStream wrapper around an HGLOBAL blob.
// In mfc140u its only vftable references outside the ctor are two inlined
// constructions inside CPropbagPropExchange::ExchangeBlobProp (RVA 0x1e7f10, mfc140u),
// which hands the object to IPropertyBag::Read/Write. No header in the on-host MSVC
// tree declares the class and include/openmfc does not either, so the layout is
// pinned in-file below from the retail mfc140u disassembly:
//   ctor   ??0CBlobProperty@@QEAA@PEAX@Z   RVA 0x1e7840 (mfc140u):
//            vfptr = &vftable (0x180324880 in mfc140u); +0x08 = 1; +0x10 = hBlob
//   Release (vtable slot 2, RVA 0x1e7860 (mfc140u), not exported by this image):
//            calls the unexported 0x2b77b0 (mfc140u) with edx = 0x18 -- read as a
//            sized operator delete of 0x18 bytes; it jumps to 0x27c0 (mfc140u), which
//            tail-jumps through the CRT `free` import. No destructor call.
// Vtable at 0x180324880 (mfc140u), read slot by slot:
//   0 QueryInterface 0x1e7890   1 AddRef 0x96a50       2 Release 0x1e7860
//   3 GetClassID 0x1e7920       4 IsDirty 0x71e0       5 Load 0x1e7950
//   6 Save 0x1e7a50             7 GetSizeMax 0x1e7ae0            (all RVAs mfc140u)
// The virtual bodies below are transcribed from those entries; the ctor and GetBlob
// (RVA 0x96a40, mfc140u) from their own exported entries.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();   // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                 // detail/MemcoreSupport.cpp (operator delete)

extern "C" long MS_ABI impl__QueryInterface_CBlobProperty__UEAAJAEBU_GUID__PEAPEAX_Z(void* pThis, const GUID* riid, void** ppvObj);
extern "C" unsigned long MS_ABI impl__AddRef_CBlobProperty__UEAAKXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__Release_CBlobProperty__UEAAKXZ(void* pThis);
extern "C" long MS_ABI impl__GetClassID_CBlobProperty__UEAAJPEAU_GUID___Z(void* pThis, GUID* pClassID);
extern "C" long MS_ABI impl__IsDirty_CBlobProperty__UEAAJXZ(void* pThis);
extern "C" long MS_ABI impl__Load_CBlobProperty__UEAAJPEAUIStream___Z(void* pThis, IStream* pStm);
extern "C" long MS_ABI impl__Save_CBlobProperty__UEAAJPEAUIStream__H_Z(void* pThis, IStream* pStm, int fClearDirty);
extern "C" long MS_ABI impl__GetSizeMax_CBlobProperty__UEAAJPEAT_ULARGE_INTEGER___Z(void* pThis, ULARGE_INTEGER* pcbSize);

namespace {

// Retail layout (see header comment). sizeof == 0x18 is the size passed to the sized
// operator delete in Release (RVA 0x1e7860, mfc140u).
struct S_CBlobProperty {
    void* const* vfptr;   // +0x00
    long          m_dwRef; // +0x08  (lock xadd target in AddRef/Release)
    HGLOBAL       m_hBlob; // +0x10  (GetBlob, Load, Save, GetSizeMax)
};
static_assert(offsetof(S_CBlobProperty, vfptr) == 0x00, "CBlobProperty vfptr");
static_assert(offsetof(S_CBlobProperty, m_dwRef) == 0x08, "CBlobProperty::m_dwRef");
static_assert(offsetof(S_CBlobProperty, m_hBlob) == 0x10, "CBlobProperty::m_hBlob");
static_assert(sizeof(S_CBlobProperty) == 0x18, "CBlobProperty size");

inline S_CBlobProperty* Blob(void* pThis) { return static_cast<S_CBlobProperty*>(pThis); }

// IPersistStream vtable in retail slot order (0x180324880 in mfc140u).
void* const kBlobPropertyVtbl[8] = {
    reinterpret_cast<void*>(&impl__QueryInterface_CBlobProperty__UEAAJAEBU_GUID__PEAPEAX_Z),
    reinterpret_cast<void*>(&impl__AddRef_CBlobProperty__UEAAKXZ),
    reinterpret_cast<void*>(&impl__Release_CBlobProperty__UEAAKXZ),
    reinterpret_cast<void*>(&impl__GetClassID_CBlobProperty__UEAAJPEAU_GUID___Z),
    reinterpret_cast<void*>(&impl__IsDirty_CBlobProperty__UEAAJXZ),
    reinterpret_cast<void*>(&impl__Load_CBlobProperty__UEAAJPEAUIStream___Z),
    reinterpret_cast<void*>(&impl__Save_CBlobProperty__UEAAJPEAUIStream__H_Z),
    reinterpret_cast<void*>(&impl__GetSizeMax_CBlobProperty__UEAAJPEAT_ULARGE_INTEGER___Z),
};

// CLSID returned by GetClassID: the 16 bytes at 0x18034b8c8 (mfc140u), which retail
// copies with movups/movdqu at RVA 0x1e7920 (mfc140u).
const GUID kClsidBlobProperty =
    { 0xf6f07540, 0x42ec, 0x11ce, { 0x81, 0x35, 0x00, 0xaa, 0x00, 0x4b, 0xb8, 0x51 } };

// IIDs QueryInterface compares against (16-byte memcmp each), read from mfc140u
// at 0x1802d9a58 / 0x1802d9b08 / 0x1802d9b18.
const GUID kIID_IUnknown =
    { 0x00000000, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
const GUID kIID_IPersist =
    { 0x0000010c, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
const GUID kIID_IPersistStream =
    { 0x00000109, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

} // namespace

// Symbol: ??0CBlobProperty@@QEAA@PEAX@Z
// Transcribed from RVA 0x1e7840 (mfc140u): install the vftable, m_dwRef = 1,
// m_hBlob = the argument, return this.
extern "C" void* MS_ABI impl___0CBlobProperty__QEAA_PEAX_Z(void* pThis, void* pBlob) {
    S_CBlobProperty* self = Blob(pThis);
    self->vfptr = kBlobPropertyVtbl;
    self->m_dwRef = 1;
    self->m_hBlob = static_cast<HGLOBAL>(pBlob);
    return pThis;
}

// Symbol: ?AddRef@CBlobProperty@@UEAAKXZ
// Transcribed from RVA 0x96a50 (mfc140u): lock xadd 1 into +0x08, return the new count.
extern "C" unsigned long MS_ABI impl__AddRef_CBlobProperty__UEAAKXZ(void* pThis) {
    return static_cast<unsigned long>(InterlockedIncrement(&Blob(pThis)->m_dwRef));
}

// Symbol: ?GetBlob@CBlobProperty@@QEAAPEAXXZ
// Transcribed from RVA 0x96a40 (mfc140u): return the HGLOBAL at +0x10.
extern "C" void* MS_ABI impl__GetBlob_CBlobProperty__QEAAPEAXXZ(void* pThis) {
    return Blob(pThis)->m_hBlob;
}

// Symbol: ?GetClassID@CBlobProperty@@UEAAJPEAU_GUID@@@Z
// Transcribed from RVA 0x1e7920 (mfc140u): a NULL pClassID calls
// AfxThrowInvalidArgException; otherwise copy the CLSID and return S_OK.
extern "C" long MS_ABI impl__GetClassID_CBlobProperty__UEAAJPEAU_GUID___Z(void* pThis, GUID* pClassID) {
    (void)pThis;
    if (pClassID == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    *pClassID = kClsidBlobProperty;
    return S_OK;
}

// Symbol: ?GetSizeMax@CBlobProperty@@UEAAJPEAT_ULARGE_INTEGER@@@Z
// Transcribed from RVA 0x1e7ae0 (mfc140u): NULL pcbSize -> AfxThrowInvalidArgException.
// GlobalLock(m_hBlob); on NULL return E_OUTOFMEMORY (no unlock). Otherwise
// HighPart = 0, LowPart = (leading DWORD length) + 4, GlobalUnlock(m_hBlob), S_OK.
extern "C" long MS_ABI impl__GetSizeMax_CBlobProperty__UEAAJPEAT_ULARGE_INTEGER___Z(void* pThis, ULARGE_INTEGER* pcbSize) {
    S_CBlobProperty* self = Blob(pThis);
    if (pcbSize == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    const DWORD* p = static_cast<const DWORD*>(::GlobalLock(self->m_hBlob));
    if (p == nullptr) {
        return E_OUTOFMEMORY;
    }
    pcbSize->HighPart = 0;
    pcbSize->LowPart = *p + 4;
    ::GlobalUnlock(self->m_hBlob);
    return S_OK;
}

// Symbol: ?IsDirty@CBlobProperty@@UEAAJXZ
// Vtable slot 4 of 0x180324880 (mfc140u) is RVA 0x71e0 (mfc140u), a COMDAT-folded
// `xor eax,eax; ret` (the RVA map names it ?AddRef@COleUILinkInfo@@UEAAKXZ; the
// IsDirty export ordinal, 7875, also resolves to 0x71e0 in mfc140u). So retail
// IsDirty unconditionally returns S_OK, i.e. the blob always reports itself dirty.
extern "C" long MS_ABI impl__IsDirty_CBlobProperty__UEAAJXZ(void* pThis) {
    (void)pThis;
    return S_OK;
}

// Symbol: ?Load@CBlobProperty@@UEAAJPEAUIStream@@@Z
// Transcribed from RVA 0x1e7950 (mfc140u):
//   NULL pStm -> AfxThrowInvalidArgException.
//   hr = pStm->Read(&cb, 4, &cbRead) (vtable +0x18); FAILED -> return hr;
//   cbRead != 4 -> E_FAIL.
//   h = GlobalAlloc(GMEM_MOVEABLE, cb + 4) (64-bit add); NULL -> E_OUTOFMEMORY.
//   p = GlobalLock(h); NULL -> E_FAIL. Retail does NOT free h on this path (a leak);
//   transcribed as-is.
//   *(DWORD*)p = cb; hr = pStm->Read(p + 4, cb, &cbRead); GlobalUnlock(h).
//   FAILED(hr) -> GlobalFree(h), return hr. cbRead != cb -> GlobalFree(h), E_FAIL.
//   Otherwise GlobalFree(old m_hBlob) (called unconditionally, even if NULL),
//   m_hBlob = h, return S_OK.
extern "C" long MS_ABI impl__Load_CBlobProperty__UEAAJPEAUIStream___Z(void* pThis, IStream* pStm) {
    S_CBlobProperty* self = Blob(pThis);
    if (pStm == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    ULONG cb = 0;
    ULONG cbRead = 0;
    HRESULT hr = pStm->Read(&cb, sizeof(ULONG), &cbRead);
    if (FAILED(hr)) {
        return hr;
    }
    if (cbRead != sizeof(ULONG)) {
        return E_FAIL;
    }
    HGLOBAL hBlobNew = ::GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>(cb) + 4);
    if (hBlobNew == nullptr) {
        return E_OUTOFMEMORY;
    }
    BYTE* p = static_cast<BYTE*>(::GlobalLock(hBlobNew));
    if (p == nullptr) {
        return E_FAIL;   // retail leaks hBlobNew here
    }
    *reinterpret_cast<ULONG*>(p) = cb;
    hr = pStm->Read(p + 4, cb, &cbRead);
    ::GlobalUnlock(hBlobNew);
    if (SUCCEEDED(hr)) {
        if (cbRead == cb) {
            ::GlobalFree(self->m_hBlob);
            self->m_hBlob = hBlobNew;
            return S_OK;
        }
        hr = E_FAIL;
    }
    ::GlobalFree(hBlobNew);
    return hr;
}

// Symbol: ?QueryInterface@CBlobProperty@@UEAAJAEBU_GUID@@PEAPEAX@Z
// Not in the RVA map; the body is vtable slot 0 of 0x180324880 (mfc140u), entry
// RVA 0x1e7890 (mfc140u). Transcribed: if riid equals IID_IUnknown, IID_IPersist or
// IID_IPersistStream (16-byte memcmp each), call AddRef through vtable slot 1, store
// this in *ppvObj and return S_OK; otherwise *ppvObj = NULL, E_NOINTERFACE. Retail
// does not NULL-check ppvObj.
extern "C" long MS_ABI impl__QueryInterface_CBlobProperty__UEAAJAEBU_GUID__PEAPEAX_Z(void* pThis, const GUID* riid, void** ppvObj) {
    if (std::memcmp(riid, &kIID_IUnknown, sizeof(GUID)) != 0 &&
        std::memcmp(riid, &kIID_IPersist, sizeof(GUID)) != 0 &&
        std::memcmp(riid, &kIID_IPersistStream, sizeof(GUID)) != 0) {
        *ppvObj = nullptr;
        return E_NOINTERFACE;
    }
    using AddRefFn = unsigned long (MS_ABI*)(void*);
    reinterpret_cast<AddRefFn>(Blob(pThis)->vfptr[1])(pThis);
    *ppvObj = pThis;
    return S_OK;
}

// Symbol: ?Release@CBlobProperty@@UEAAKXZ
// Not in the RVA map; the body is vtable slot 2 of 0x180324880 (mfc140u), entry
// RVA 0x1e7860 (mfc140u). Transcribed: lock xadd -1 into m_dwRef; if the new count
// is > 0, return m_dwRef re-read from the object (not the xadd result); otherwise
// free the object via 0x2b77b0 (mfc140u) with edx = 0x18 (sized delete, ending in CRT
// free) -- no destructor runs and m_hBlob is NOT freed -- and return 0. OpenMFC's operator delete is the exported
// ??3@YAXPEAX@Z thunk (free), which pairs with the malloc-based ??2@YAPEAX_K@Z.
extern "C" unsigned long MS_ABI impl__Release_CBlobProperty__UEAAKXZ(void* pThis) {
    S_CBlobProperty* self = Blob(pThis);
    if (InterlockedDecrement(&self->m_dwRef) > 0) {
        return static_cast<unsigned long>(self->m_dwRef);
    }
    impl___3_YAXPEAX_Z(pThis);
    return 0;
}

// Symbol: ?Save@CBlobProperty@@UEAAJPEAUIStream@@H@Z
// Not in the RVA map; the body is vtable slot 6 of 0x180324880 (mfc140u), entry
// RVA 0x1e7a50 (mfc140u). Transcribed: NULL pStm -> AfxThrowInvalidArgException.
// p = GlobalLock(m_hBlob); NULL -> E_OUTOFMEMORY. cb = *(DWORD*)p + 4;
// hr = pStm->Write(p, cb, &cbWritten) (vtable +0x20); GlobalUnlock(m_hBlob).
// FAILED(hr) -> hr; else cbWritten != cb -> E_FAIL, else S_OK. fClearDirty is unused.
extern "C" long MS_ABI impl__Save_CBlobProperty__UEAAJPEAUIStream__H_Z(void* pThis, IStream* pStm, int fClearDirty) {
    (void)fClearDirty;
    S_CBlobProperty* self = Blob(pThis);
    if (pStm == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    const BYTE* p = static_cast<const BYTE*>(::GlobalLock(self->m_hBlob));
    if (p == nullptr) {
        return E_OUTOFMEMORY;
    }
    const ULONG cb = *reinterpret_cast<const ULONG*>(p) + 4;
    ULONG cbWritten = 0;
    const HRESULT hr = pStm->Write(p, cb, &cbWritten);
    ::GlobalUnlock(self->m_hBlob);
    if (FAILED(hr)) {
        return hr;
    }
    return (cb != cbWritten) ? E_FAIL : S_OK;
}
