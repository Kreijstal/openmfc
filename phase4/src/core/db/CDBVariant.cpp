// Native release x64 variant construction (0x2043e0), Clear (0x204440)
// and destruction (0x2044f0). The value union remains untouched after Clear.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxdao.h"
#include <cstddef>
#include <new>
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif

CDBVariant::CDBVariant():m_dwType(DBVT_NULL) {}
CDBVariant::~CDBVariant() {Clear();}
void CDBVariant::Clear() {
    switch(m_dwType) {
    case DBVT_DATE:
        ::operator delete(m_pdate);break;
    case DBVT_STRING:case DBVT_ASTRING:case DBVT_WSTRING: {
        // ANSI and Unicode CString objects have the same owning pointer and
        // CStringData prefix. Release through that allocation's own manager.
        void* string=m_pstring;
        if(string) {
            auto* data=reinterpret_cast<CStringData*>(*static_cast<void**>(string))-1;
            if(::InterlockedDecrement(&data->nRefs)<=0)data->pStringMgr->Free(data);
            ::operator delete(string);
        }
        break;
    }
    case DBVT_BINARY:delete m_pbinary;break;
    default:break;
    }
    m_dwType=DBVT_NULL;
}
static_assert(offsetof(CDBVariant,m_dwType)==8 && offsetof(CDBVariant,m_pbinary)==16,"native variant field offsets");
// Symbol: ??0CDBVariant@@QEAA@XZ
extern "C" void* MS_ABI impl___0CDBVariant__QEAA_XZ(void* storage) {return new(storage) CDBVariant;}
// Symbol: ??1CDBVariant@@UEAA@XZ
extern "C" void MS_ABI impl___1CDBVariant__UEAA_XZ(CDBVariant* value) {value->CDBVariant::~CDBVariant();}
// Symbol: ?Clear@CDBVariant@@QEAAXXZ
extern "C" void MS_ABI impl__Clear_CDBVariant__QEAAXXZ(CDBVariant* value) {value->Clear();}
