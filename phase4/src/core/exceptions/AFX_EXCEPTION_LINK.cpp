// Native exception-handler stack and THROW_LAST ownership transfer.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstat_.h"
namespace {
_AFX_THREAD_STATE* ExceptionThreadState() {
    DWORD error=::GetLastError();
    auto* state=AfxGetThreadState();
    if(!state)AfxThrowInvalidArgException();
    ::SetLastError(error);
    return state;
}
}
AFX_EXCEPTION_LINK::AFX_EXCEPTION_LINK():m_pException(nullptr) {
    auto* state=ExceptionThreadState();
    m_pLinkPrev=state->m_exceptionContext.m_pLinkTop;
    state->m_exceptionContext.m_pLinkTop=this;
}
void AFXAPI AfxTryCleanup() {
    auto* state=ExceptionThreadState();
    if(auto* link=state->m_exceptionContext.m_pLinkTop) {
        if(link->m_pException)link->m_pException->Delete();
        state->m_exceptionContext.m_pLinkTop=link->m_pLinkPrev;
    }
}
void AFXAPI AfxThrowLastCleanup() {
    auto* state=ExceptionThreadState();
    if(auto* link=state->m_exceptionContext.m_pLinkTop)link->m_pException=nullptr;
}
// Symbol: ??0AFX_EXCEPTION_LINK@@QEAA@XZ
extern "C" AFX_EXCEPTION_LINK* impl___0AFX_EXCEPTION_LINK__QEAA_XZ(AFX_EXCEPTION_LINK* self) {return ::new(self) AFX_EXCEPTION_LINK;}
// Symbol: ?AfxTryCleanup@@YAXXZ
extern "C" void impl__AfxTryCleanup__YAXXZ() {AfxTryCleanup();}
// Symbol: ?AfxThrowLastCleanup@@YAXXZ
extern "C" void impl__AfxThrowLastCleanup__YAXXZ() {AfxThrowLastCleanup();}
