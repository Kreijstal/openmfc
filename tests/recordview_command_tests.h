#pragma once
extern "C" BOOL MS_ABI impl__IsOnFirstRecord_CRecordView__QEAAHXZ(CRecordView*);
extern "C" BOOL MS_ABI impl__IsOnLastRecord_CRecordView__QEAAHXZ(CRecordView*);
extern "C" BOOL MS_ABI impl__OnMove_CRecordView__UEAAHI_Z(CRecordView*,UINT);
extern "C" void MS_ABI impl__OnInitialUpdate_CRecordView__UEAAXXZ(CRecordView*);
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView*);
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CRecordView__KAPEBUAFX_MSGMAP__XZ();
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CRecordView__MEBAPEBUAFX_MSGMAP__XZ(const CRecordView*);
extern "C" void MS_ABI impl__OnUpdateRecordFirst_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView*,CCmdUI*);
extern "C" void MS_ABI impl__OnUpdateRecordPrev_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView*,CCmdUI*);
extern "C" void MS_ABI impl__OnUpdateRecordNext_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView*,CCmdUI*);
extern "C" void MS_ABI impl__OnUpdateRecordLast_CRecordView__IEAAXPEAVCCmdUI___Z(CRecordView*,CCmdUI*);
namespace recordview_command_test {
using Event=std::array<INT64,6>;
using Move=BOOL (MS_ABI*)(CRecordView*,UINT);
using Status=BOOL (MS_ABI*)(CRecordView*);
using Command=void (MS_ABI*)(CRecordView*,CCmdUI*);
struct API { Status first,last;Move move;void (MS_ABI* initial)(CRecordView*);Command commands[4];const AFX_MSGMAP* (MS_ABI* map)(); };
static std::vector<Event> events;
static void event(INT64 tag,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0) { events.push_back({tag,a,b,c,d,e}); }
static CRecordset* current;
static int failure,getCount;
static bool boundary,updateOK,changeSecond;
static void fail(int at) { if(failure==at)throw 83; }
static CRecordset* MS_ABI get(CRecordView*) {
    event(1,++getCount);fail(1);
    if(changeSecond && getCount==2) { current->m_lCurrentRecord=0;current->m_lRecordCount=1;current->m_bEOFSeen=69; }
    return current;
}
static void MS_ABI edit(CRecordset* set) { event(2);fail(2);set->m_nEditMode=1; }
static BOOL MS_ABI update(CRecordset* set) { event(3);fail(3);set->m_nEditMode=0;return updateOK?69:FALSE; }
static void MS_ABI move(CRecordset* set,long rows,WORD direction) {
    event(4,rows,direction);fail(4);
    if(direction==SQL_FETCH_NEXT)set->m_bEOF=boundary?69:FALSE;
    if(direction==SQL_FETCH_PRIOR)set->m_bBOF=boundary?69:FALSE;
    if(direction==SQL_FETCH_FIRST)set->m_bBOF=FALSE;
    if(direction==SQL_FETCH_LAST)set->m_bEOF=FALSE;
}
static void MS_ABI exchange(CRecordset*,CFieldExchange* fx) { event(5,fx->m_nOperation,fx->m_bField,fx->m_pvField!=nullptr);fail(5); }
static void MS_ABI ddx(CRecordView* view,CDataExchange* fx) {
    event(6,fx->m_bSaveAndValidate,view->m_bInitDlgCompleted,fx->m_pDlgWnd==view);fail(fx->m_bSaveAndValidate?6:7);
    if(failure==8 && fx->m_bSaveAndValidate)throw new CUserException;
}
static void MS_ABI initialUpdate(CRecordView*,CView* sender,LPARAM hint,CObject* object) { event(7,sender!=nullptr,hint,object!=nullptr);fail(9); }
static BOOL MS_ABI open(CRecordset*,UINT type,const wchar_t* sql,DWORD options) { event(8,type,sql!=nullptr,options);fail(10);return updateOK?69:FALSE; }
static void MS_ABI wait(void*,int code) { event(9,code);fail(code>0?11:12); }
static BOOL MS_ABI overrideMove(CRecordView*,UINT command) { event(10,command);return command==0xe902?FALSE:69; }
struct UI:CCmdUI { void Enable(BOOL value) override { event(11,value);fail(13); } };
struct Fixture {
    recordset_move_test::Fixture record;
    alignas(8) unsigned char bytes[sizeof(CRecordView)];void* table[114]{};
    void* appTable[49]{};void** app=appTable;void* module[2]{};
    Fixture() {
        memset(bytes,0xa5,sizeof(bytes));*reinterpret_cast<void***>(bytes)=table;
        table[66]=reinterpret_cast<void*>(&ddx);table[104]=reinterpret_cast<void*>(&initialUpdate);
        table[112]=reinterpret_cast<void*>(&get);table[113]=reinterpret_cast<void*>(&overrideMove);
        view()->m_hWnd=nullptr;view()->m_bInitDlgCompleted=FALSE;
        record.table[5]=reinterpret_cast<void*>(&open);record.table[7]=reinterpret_cast<void*>(&move);
        record.table[11]=reinterpret_cast<void*>(&edit);record.table[12]=reinterpret_cast<void*>(&update);
        record.table[19]=reinterpret_cast<void*>(&exchange);
        current=&record.rs;current->m_bUpdatable=FALSE;current->m_bDeleted=FALSE;
        failure=getCount=0;boundary=false;changeSecond=false;updateOK=true;events.clear();
        appTable[48]=reinterpret_cast<void*>(&wait);module[1]=&app;
    }
    CRecordView* view() {return reinterpret_cast<CRecordView*>(bytes);}
    void save(std::vector<Event>& output,int returned,int caught) {
        output.insert(output.end(),events.begin(),events.end());
        auto* v=view();auto& rs=record.rs;
        output.push_back({90,returned,caught,rs.m_nEditMode,rs.m_bBOF,rs.m_bEOF});
        output.push_back({91,v->m_bInitDlgCompleted,v->m_bOnFirstRecord,v->m_bOnLastRecord,getCount,0});
    }
};
template<class F> static int invoke(F action) {try{action();return 0;}catch(int n){return n;}catch(CException* e){e->Delete();return -1;}}
static std::vector<Event> status(const API& api) {
    std::vector<Event> output;
    for(long position:{LONG_MIN,-2L,-1L,0L,1L,4L,LONG_MAX})for(long count:{LONG_MIN,-1L,0L,1L,5L,LONG_MAX})for(BOOL seen:{FALSE,BOOL(-1),BOOL(69)}) {
        Fixture f;current->m_lCurrentRecord=position;current->m_lRecordCount=count;current->m_bEOFSeen=seen;
        BOOL first=api.first(f.view()),last=api.last(f.view());event(12,first,last,position,count,seen);f.save(output,0,0);
    }
    return output;
}
static std::vector<Event> commands(const API& api) {
    std::vector<Event> output;
    for(auto command:api.commands)for(bool opened:{false,true})for(BOOL scroll:{FALSE,BOOL(-1),BOOL(69)})
    for(BOOL bof:{FALSE,BOOL(69)})for(BOOL eof:{FALSE,BOOL(-1)})for(bool seen:{false,true})for(long position:{-1L,0L,4L,5L})for(bool change:{false,true}) {
        Fixture f;current->m_hstmt=opened?reinterpret_cast<HSTMT>(0xbeef):nullptr;
        current->m_bScrollable=scroll;current->m_bBOF=bof;current->m_bEOF=eof;current->m_bEOFSeen=seen;current->m_lCurrentRecord=position;
        changeSecond=change;UI ui;command(f.view(),&ui);f.save(output,0,0);
    }
    for(auto command:api.commands)for(int at:{1,13}) {
        Fixture f;failure=at;UI ui;int caught=invoke([&]{command(f.view(),&ui);});f.save(output,0,caught);
    }
    return output;
}
static std::vector<Event> navigation(const API& api) {
    std::vector<Event> output;
    for(UINT command:{0u,0xe8ffu,0xe900u,0xe901u,0xe902u,0xe903u,0xe904u,UINT(-1)})for(BOOL updatable:{FALSE,BOOL(69)})
    for(BOOL deleted:{FALSE,BOOL(-1)})for(bool scroll:{false,true})for(bool end:{false,true})for(bool result:{false,true}) {
        Fixture f;current->m_bUpdatable=updatable;current->m_bDeleted=deleted;current->m_bScrollable=scroll;boundary=end;updateOK=result;
        BOOL returned=api.move(f.view(),command);f.save(output,returned,0);
    }
    for(UINT command:{0xe900u,0xe901u,0xe902u,0xe903u})for(int at:{1,2,3,4,5,6,7,8}) {
        Fixture f;current->m_bUpdatable=69;current->m_bScrollable=FALSE;boundary=true;failure=at;
        BOOL returned=-9;int caught=invoke([&]{returned=api.move(f.view(),command);});f.save(output,returned,caught);
    }
    return output;
}
static std::vector<Event> initialization(const API& api,void** nativeModule=nullptr) {
    std::vector<Event> output;
    for(bool opened:{false,true})for(bool result:{false,true})for(int at:{0,1,7,9,10,11}) {
        Fixture f;current->m_hstmt=opened?reinterpret_cast<HSTMT>(0xbeef):nullptr;failure=at;updateOK=result;
        auto* previous=g_cmdTestCurrentModule;g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(f.module);
        void* oldApp=nullptr;if(nativeModule){oldApp=nativeModule[1];nativeModule[1]=&f.app;}
        int caught=invoke([&]{api.initial(f.view());});
        if(nativeModule)nativeModule[1]=oldApp;g_cmdTestCurrentModule=previous;f.save(output,0,caught);
    }
    return output;
}
static void maps(const API& api) {
    const auto* map=api.map();bool valid=map && map->pfnGetBaseMap && map->lpEntries;
    const UINT ids[]={0xe900,0xe903,0xe902,0xe901};
    if(valid)for(size_t i=0;i<8;++i) {
        const auto& entry=map->lpEntries[i];
        valid=valid && entry.nMessage==WM_COMMAND && entry.nCode==(i%2?UINT(-1):0) && entry.nID==ids[i/2] && entry.nLastID==ids[i/2] &&
            entry.nSig==UINT_PTR(i%2?AfxSigCmdUI:AfxSigCmd_EX) && entry.pfn;
    }
    check(valid && map->lpEntries[8].nSig==AfxSig_end,"record-view message map exposes native command and update-handler discovery");
    if(!valid)return;
    for(size_t i=0;i<8;i+=2) {
        Fixture f;void* function;memcpy(&function,&map->lpEntries[i].pfn,sizeof(function));
        BOOL returned=reinterpret_cast<Move>(function)(f.view(),ids[i/2]);
        check(events==std::vector<Event>{{10,ids[i/2],0,0,0,0}} && returned==(ids[i/2]==0xe902?FALSE:69),
            "record command entries dispatch the derived OnMove virtual and preserve its return value");
    }
}
static void compare(const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label) {
    if(actual!=expected) {
        std::printf("%s: native %zu local %zu events\n",label,actual.size(),expected.size());
        for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]) {
            std::printf("mismatch %zu local:",i);for(auto value:expected[i])std::printf(" %lld",value);
            std::printf(" native:");for(auto value:actual[i])std::printf(" %lld",value);puts("");break;
        }
    }
    check(actual==expected,label);
}
}
static void recordviewCommands() {
    using namespace recordview_command_test;
    static_assert(sizeof(CView)==248 && sizeof(CScrollView)==304 && sizeof(CFormView)==352 && sizeof(CRecordView)==360,"native view sizes");
    static_assert(offsetof(CView,m_bInitialRedraw)==240 && offsetof(CScrollView,m_nMapMode)==256 && offsetof(CScrollView,m_totalDev)==268 &&
        offsetof(CFormView,m_bInitDlgCompleted)==328 && offsetof(CRecordView,m_bOnFirstRecord)==352,"native view members");
    API local{&impl__IsOnFirstRecord_CRecordView__QEAAHXZ,&impl__IsOnLastRecord_CRecordView__QEAAHXZ,&impl__OnMove_CRecordView__UEAAHI_Z,
        &impl__OnInitialUpdate_CRecordView__UEAAXXZ,{&impl__OnUpdateRecordFirst_CRecordView__IEAAXPEAVCCmdUI___Z,&impl__OnUpdateRecordPrev_CRecordView__IEAAXPEAVCCmdUI___Z,
        &impl__OnUpdateRecordNext_CRecordView__IEAAXPEAVCCmdUI___Z,&impl__OnUpdateRecordLast_CRecordView__IEAAXPEAVCCmdUI___Z},&impl__GetThisMessageMap_CRecordView__KAPEBUAFX_MSGMAP__XZ};
    check(CRecordView::GetThisClass()->m_nObjectSize==360,"record-view runtime metadata reports the native object size");
    maps(local);auto positions=status(local),enabled=commands(local),moves=navigation(local),initial=initialization(local);
    {
        Fixture f;auto* map=impl__GetMessageMap_CRecordView__MEBAPEBUAFX_MSGMAP__XZ(f.view());
        check(map==local.map() && map->pfnGetBaseMap()==CFormView::GetThisMessageMap() &&
            map->pfnGetBaseMap()->pfnGetBaseMap()==CScrollView::GetThisMessageMap(),"record-view maps preserve the form/scroll/view base-map chain");
        check(local.first(f.view())==FALSE && local.last(f.view())==FALSE,"record position queries ignore the unused cached view flags");
        events.clear();getCount=0;current->m_bUpdatable=69;failure=8;
        check(local.move(f.view(),0xe902)==TRUE && events==std::vector<Event>{{1,1,0,0,0,0},{2,0,0,0,0,0},{6,1,0,1,0,0}},
            "failed save validation leaves editing active, suppresses navigation and reports the command handled");
    }
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDVIEW_DLL",path,1024)) {
        auto module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for record-view comparisons");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"record-view oracle uses the verified native MFC image build");
            if(pinned) {
                API native{reinterpret_cast<Status>(base+0x2045e0),reinterpret_cast<Status>(base+0x204610),reinterpret_cast<Move>(base+0x204650),
                    reinterpret_cast<decltype(local.initial)>(base+0x204560),{reinterpret_cast<Command>(base+0x204760),reinterpret_cast<Command>(base+0x2047e0),
                    reinterpret_cast<Command>(base+0x2047f0),reinterpret_cast<Command>(base+0x204860)},reinterpret_cast<decltype(local.map)>(base+0x204500)};
                maps(native);compare(status(native),positions,"record-view currency queries match native MFC, including 32-bit overflow");
                compare(commands(native),enabled,"record command enabling, scrolling capabilities and repeated recordset callbacks match native MFC");
                compare(navigation(native),moves,"record navigation, edit/save ordering, EOF/BOF recovery and exceptions match native MFC");
                auto getter=reinterpret_cast<void** (MS_ABI*)()>(base+0x133930);
                compare(initialization(native,getter()),initial,"record/form initialization, wait-cursor unwinding, validation and view updates match native MFC");
            }
            ::FreeLibrary(module);
        }
    }
}
