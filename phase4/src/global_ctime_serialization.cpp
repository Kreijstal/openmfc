// ATL CTime and CTimeSpan archive serialization.

#include <cstdint>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

struct CArchive;

namespace {

// Both ATL value types contain a single __time64_t value in the MSVC ABI.
struct CArchiveLayout {
    void* vtable;
    void* file;
    std::uint32_t mode;
};

extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(
    CArchive* archive,
    void* buffer,
    unsigned int byteCount
);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(
    CArchive* archive,
    const void* buffer,
    unsigned int byteCount
);

CArchive* SerializeTime64(void* value, CArchive* archive) {
    if (!value || !archive) {
        return archive;
    }

    if ((reinterpret_cast<CArchiveLayout*>(archive)->mode & 1U) != 0) {
        impl__Read_CArchive__QEAAIPEAXI_Z(archive, value, sizeof(long long));
    } else {
        impl__Write_CArchive__QEAAXPEBXI_Z(archive, value, sizeof(long long));
    }
    return archive;
}

} // namespace

// Symbol: ?Serialize64@CTime@ATL@@QEAAAEAVCArchive@@AEAV3@@Z
extern "C" CArchive* MS_ABI impl__Serialize64_CTime_ATL__QEAAAEAVCArchive__AEAV3__Z(
    void* pThis,
    CArchive* archive
) {
    return SerializeTime64(pThis, archive);
}

// Symbol: ?Serialize64@CTimeSpan@ATL@@QEAAAEAVCArchive@@AEAV3@@Z
extern "C" CArchive* MS_ABI impl__Serialize64_CTimeSpan_ATL__QEAAAEAVCArchive__AEAV3__Z(
    void* pThis,
    CArchive* archive
) {
    return SerializeTime64(pThis, archive);
}
