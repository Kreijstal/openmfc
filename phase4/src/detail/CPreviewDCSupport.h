#pragma once
// Shared internals of the former gdi_previewdc.cpp translation unit.
// Definitions live in detail/CPreviewDCSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cpreviewdc {} } }
using namespace openmfc::detail::cpreviewdc;
// CPreviewDC GDI drawing implementations.
//
// CPreviewDC is not exposed in the public compatibility headers yet. Its
// harvested layout has CDC as the base prefix, so these exports operate on that
// CDC portion and keep preview-only state externally.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <mutex>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC*, CFont*);
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(CDC*, int);

namespace openmfc { namespace detail { namespace cpreviewdc {
struct PreviewDCState {
    int scaleNum = 1;
    int scaleDen = 1;
    CSize topLeftOffset = CSize(0, 0);
};
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
extern std::mutex g_previewDcMutex;
} } }
namespace openmfc { namespace detail { namespace cpreviewdc {
extern std::unordered_map<void*, PreviewDCState> g_previewDcState;
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
CDC* PreviewAsCDC(void* pThis);
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
PreviewDCState GetPreviewState(void* pThis);
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
void SetPreviewScale(void* pThis, int num, int den);
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
void SetPreviewOffset(void* pThis, CSize offset);
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
int ScaleCoord(int value, const PreviewDCState& state);
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
RECT ScaleRect(const RECT* rect, const PreviewDCState& state);
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
HDC OutputDC(void* pThis);
} } }

namespace openmfc { namespace detail { namespace cpreviewdc {
HDC AttribDC(void* pThis);
} } }


































