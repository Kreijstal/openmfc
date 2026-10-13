#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
CImageList::CImageList():m_hImageList(nullptr){}
CImageList::~CImageList(){if(m_hImageList)::ImageList_Destroy(m_hImageList);m_hImageList=nullptr;}
// Symbol: ??0CImageList@@QEAA@XZ
extern "C" CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList* self){return new(self) CImageList;}
// Symbol: ??1CImageList@@UEAA@XZ
extern "C" void MS_ABI impl___1CImageList__UEAA_XZ(CImageList* self){self->~CImageList();}
static_assert(sizeof(CImageList)==16,"native image list");
