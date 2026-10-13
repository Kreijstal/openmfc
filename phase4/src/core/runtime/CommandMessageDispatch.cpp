// Shared native command/notification decoding for message-map dispatch.
#define OPENMFC_APPCORE_IMPL
#include "detail/CommandMessageDispatch.h"
#include <cstring>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace openmfc { namespace detail {
namespace {
struct Notify { LRESULT* result;NMHDR* header; };
template<class Return,class... Args> Return Call(void* function,CCmdTarget* target,Args... args) {
    return reinterpret_cast<Return (MS_ABI*)(CCmdTarget*,Args...)>(function)(target,args...);
}
}
BOOL DispatchCommandMessage(CCmdTarget* target,UINT id,void* function,void* extra,UINT_PTR signature,AFX_CMDHANDLERINFO* info) {
    if(info) { info->pTarget=target;std::memcpy(&info->pmf,&function,sizeof(function));return TRUE; }
    auto* notify=static_cast<Notify*>(extra);
    switch(signature) {
    case AfxSigCmd_v:Call<void>(function,target);return TRUE;
    case AfxSigCmd_b:return Call<BOOL>(function,target);
    case AfxSigCmd_RANGE:Call<void>(function,target,id);return TRUE;
    case AfxSigCmd_EX:return Call<BOOL>(function,target,id);
    case AfxSigNotify_v:case AfxSigNotify_b:case AfxSigNotify_RANGE:case AfxSigNotify_EX:
        if(!notify) { impl__AfxThrowInvalidArgException__YAXXZ();return FALSE; }
        if(signature==AfxSigNotify_v) { Call<void>(function,target,notify->header,notify->result);return TRUE; }
        if(signature==AfxSigNotify_b)return Call<BOOL>(function,target,notify->header,notify->result);
        if(signature==AfxSigNotify_RANGE) { Call<void>(function,target,id,notify->header,notify->result);return TRUE; }
        return Call<BOOL>(function,target,id,notify->header,notify->result);
    case AfxSigCmdUI:case AfxSigCmdUI_RANGE: {
        if(!extra) { impl__AfxThrowInvalidArgException__YAXXZ();return FALSE; }
        if(signature==AfxSigCmdUI)Call<void>(function,target,extra);else Call<void>(function,target,extra,id);
        auto* continued=reinterpret_cast<BOOL*>(static_cast<char*>(extra)+0x2c);
        BOOL handled=!*continued;*continued=FALSE;return handled;
    }
    case AfxSigCmd_v_pv:Call<void>(function,target,extra);return TRUE;
    case AfxSigCmd_b_pv:return Call<BOOL>(function,target,extra);
    default:return FALSE;
    }
}
} }
