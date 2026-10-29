// CCachedDataPathProperty — OpenMFC implementation.
// The class is not declared in include/openmfc, so exported methods use void*
// and the retail object is viewed through the local shadow layout below.

#include <cstddef>

#ifndef MS_ABI
  #ifdef __GNUC__
    #define MS_ABI __attribute__((ms_abi))
  #else
    #define MS_ABI
  #endif
#endif

namespace {

// afxctl.h:517,527 declares CCachedDataPathProperty : CDataPathProperty
// with CMemFile m_Cache as its first derived member.  CDataPathProperty is
// 0x70 bytes (the shadow assertions in CDataPathProperty.cpp), CMemFile is
// 0x58 bytes (its retail CRuntimeClass descriptor), and the CCached descriptor
// in core/ole/RuntimeClasses.cpp records 200 bytes total.
struct CachedDataPathPropertyLayout {
    unsigned char dataPathBase[0x70];
    struct CacheLayout {
        void** vptr;
        unsigned char remainder[0x50];
    } m_Cache;
};
static_assert(offsetof(CachedDataPathPropertyLayout, m_Cache) == 0x70,
              "CCachedDataPathProperty::m_Cache offset");
static_assert(sizeof(CachedDataPathPropertyLayout::CacheLayout) == 0x58,
              "retail CMemFile size");
static_assert(sizeof(CachedDataPathPropertyLayout) == 0xc8,
              "retail CCachedDataPathProperty size");

template <class Fn>
inline Fn vtableSlot(void* pObject, unsigned int slot) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(pObject))[slot]);
}

// The offsets are the retail call-site displacements divided by eight:
// CMemFile GetPosition=5, Seek=13, SetLength=14, Write=17; Read=16 on the
// CAsyncMonikerFile/CFile base.  Dispatch through the MSVC vtables, rather
// than C++ virtual syntax, to preserve that ABI and any derived Read override.
using GetPositionFn = unsigned long long (MS_ABI*)(void*);
using SeekFn = unsigned long long (MS_ABI*)(void*, long long, unsigned int);
using SetLengthFn = void (MS_ABI*)(void*, unsigned long long);
using ReadFn = unsigned int (MS_ABI*)(void*, void*, unsigned int);
using WriteFn = void (MS_ABI*)(void*, const void*, unsigned int);

inline void* cacheOf(void* pThis) {
    return &static_cast<CachedDataPathPropertyLayout*>(pThis)->m_Cache;
}

inline void resetCache(void* pThis) {
    void* cache = cacheOf(pThis);
    vtableSlot<SetLengthFn>(cache, 14)(cache, 0);
}

}  // namespace

// Symbol: ?OnDataAvailable@CCachedDataPathProperty@@UEAAXKK@Z
// Retail RVA 0x1ef550 (mfc140u): save m_Cache's position, seek it to end,
// repeatedly Read 0x400 bytes from this through virtual slot 16 and Write each
// block to m_Cache through slot 17 (including the final zero-byte write), then
// restore the saved cache position.  The DWORD arguments are not read.
extern "C" void MS_ABI impl__OnDataAvailable_CCachedDataPathProperty__UEAAXKK_Z(
    void* pThis, unsigned long flags, unsigned long status) {
    (void)flags;
    (void)status;
    void* cache = cacheOf(pThis);
    const unsigned long long savedPosition = vtableSlot<GetPositionFn>(cache, 5)(cache);
    vtableSlot<SeekFn>(cache, 13)(cache, 0, 2);

    unsigned char buffer[0x400];
    unsigned int bytesRead;
    do {
        bytesRead = vtableSlot<ReadFn>(pThis, 16)(pThis, buffer, sizeof(buffer));
        vtableSlot<WriteFn>(cache, 17)(cache, buffer, bytesRead);
    } while (bytesRead != 0);

    vtableSlot<SeekFn>(cache, 13)(cache, static_cast<long long>(savedPosition), 0);
}

// Symbol: ?ResetData@CCachedDataPathProperty@@UEAAXXZ
// Retail RVA 0x1ef650 (mfc140u; export ordinal 12502): slot 14 on m_Cache,
// CMemFile::SetLength(0).  The symbol is folded and absent from the RVA name
// map, so this entry was disassembled directly with disas.py --u --at.
extern "C" void MS_ABI impl__ResetData_CCachedDataPathProperty__UEAAXXZ(void* pThis) {
    resetCache(pThis);
}

// Symbol: ?Close@CCachedDataPathProperty@@UEAAXXZ
// Retail RVA 0x1ef620 (mfc140u) sets m_Cache length to zero, then tail-jumps
// to CAsyncMonikerFile::Close (RVA 0x243240, mfc140u).  Do not chain to the
// existing OpenMFC base thunk: its CMonikerFile close path reaches a local
// COleStreamFile::Close view that reads m_lpStream at +0x18, while retail
// COleStreamFile stores it at +0x28 (see CAsyncMonikerFile.cpp's layout note).
// That can interpret the retail CString at +0x18 as an IStream*.  A faithful
// complete close needs a compatible base path; leave this export stubbed rather
// than risk corrupting a client object.
extern "C" void MS_ABI impl__Close_CCachedDataPathProperty__UEAAXXZ(void* pThis) {
    (void)pThis;
}
