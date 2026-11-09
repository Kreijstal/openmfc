// Module-dependent EXCEPINFO conversion with actual module/TLS and exceptions.
namespace ole_process_test {
std::vector<std::wstring> allocations;
unsigned failuresMask;int change;EXCEPINFO* activeOutput;COleDispatchException* activeError;
decltype(&::SysAllocString) realAllocate;
BSTR WINAPI allocate(const wchar_t* text) {
    allocations.emplace_back(text?text:L"<null>");
    if(allocations.size()==1){
        if(change==1){activeError->m_strDescription=L"replacement";activeError->m_strSource=L"callback source";}
        if(change==2)select(1);
        if(change==3){activeOutput->wCode=222;activeOutput->dwHelpContext=0;}
    }
    return failuresMask&(1u<<(allocations.size()-1))?nullptr:realAllocate(text);
}
int loadMode;
int load(UINT resource,wchar_t* output,UINT capacity) {
    check(resource==0xf108&&capacity==256,"generic dispatch errors request the native framework string and buffer capacity");
    if(loadMode)throw 51;
    wcscpy_s(output,capacity,L"internal failure");return 16;
}
struct Hook {BYTE* p;BYTE old[12];DWORD protection;Hook(void* entry,void* target):p(static_cast<BYTE*>(entry)){memcpy(old,p,12);VirtualProtect(p,12,PAGE_EXECUTE_READWRITE,&protection);BYTE jump[12]={0x48,0xb8};memcpy(jump+2,&target,8);jump[10]=0xff;jump[11]=0xe0;memcpy(p,jump,12);FlushInstructionCache(GetCurrentProcess(),p,12);}~Hook(){memcpy(p,old,12);FlushInstructionCache(GetCurrentProcess(),p,12);DWORD ignored;VirtualProtect(p,12,protection,&ignored);}};
struct Result {
    std::array<BYTE,64> bytes;std::array<std::wstring,3> strings;std::array<bool,3> present;std::vector<std::wstring> calls;int caught;
    bool operator==(const Result& other)const{return bytes==other.bytes&&strings==other.strings&&present==other.present&&calls==other.calls&&caught==other.caught;}
};
std::vector<Result> conversionGolden;
Result sample(int kind,unsigned fail,int mutation) {
    clear();modules[0].m_lpszCurrentAppName=L"owner module";modules[1].m_lpszCurrentAppName=L"callback module";
    alignas(8) BYTE bytes[64];
    auto* error=api->dispatchCtor(reinterpret_cast<COleDispatchException*>(bytes),L"description",kind==2?7:0,37);
    error->m_scError=E_ABORT;
    if(kind==1||kind==2){error->m_strHelpFile=L"help.chm";error->m_strSource=L"error source";}
    if(kind==3){const wchar_t text[3]={L'h',0,L'H'};memcpy(error->m_strHelpFile.GetBuffer(3),text,6);error->m_strHelpFile.ReleaseBuffer(3);
        error->m_strSource=L"error source";const wchar_t description[3]={L'd',0,L'D'};memcpy(error->m_strDescription.GetBuffer(3),description,6);error->m_strDescription.ReleaseBuffer(3);}
    // Keep the captured description alive when a callback replaces its owner.
    CString retained(error->m_strDescription);
    TestFrameworkException generic(FALSE);
    struct Output {EXCEPINFO info;BYTE guard[8];} output;
    memset(&output,0xa5,sizeof(output));activeOutput=&output.info;activeError=error;
    allocations.clear();failuresMask=fail;change=mutation;loadMode=kind==5;
    int caught=0;try{api->dispatchProcess(&output.info,kind>=4?static_cast<CException*>(&generic):error);}catch(int value){caught=value;}
    Result result{};memcpy(result.bytes.data(),&output.info,64);memset(result.bytes.data()+8,0,24);
    BSTR strings[]={output.info.bstrSource,output.info.bstrDescription,output.info.bstrHelpFile};
    for(int i=0;i<3;++i){result.present[i]=strings[i]!=nullptr;if(strings[i])result.strings[i].assign(strings[i],SysStringLen(strings[i]));::SysFreeString(strings[i]);}
    result.calls=allocations;result.caught=caught;
    for(BYTE byte:output.guard)check(byte==0xa5,"dispatch EXCEPINFO conversion preserves trailing guards");
    api->dispatchDtor(error);
    return result;
}
}
void oleProcess() {
    using namespace ole_process_test;
    auto image=api->image?api->image:GetModuleHandleW(nullptr);
    Patch allocator(image,"SysAllocString",reinterpret_cast<void*>(&allocate));realAllocate=reinterpret_cast<decltype(realAllocate)>(allocator.previous);
    Hook loader(reinterpret_cast<void*>(api->load),reinterpret_cast<void*>(&load));
    allocations.reserve(8);conversionGolden.reserve(400);unsigned sampleIndex=0;
    for(int kind=0;kind<6;++kind)for(unsigned fail=0;fail<16;++fail)for(int mutation=0;mutation<4;++mutation) {
        // No application is installed: retain explicit help when it is needed.
        if(kind==2&&(fail&1))continue;
        auto result=sample(kind,fail,mutation);
        if(api->image)check(sampleIndex<conversionGolden.size()&&result==conversionGolden[sampleIndex],"native/source EXCEPINFO bytes, BSTR order, truncation, partial failures and callbacks match");
        else conversionGolden.push_back(result);
        ++sampleIndex;
    }
    auto result=sample(3,0,0);
    check(result.strings[1]==L"d"&&result.strings[2]==L"h"&&result.calls==std::vector<std::wstring>({L"h",L"error source",L"d"}),
        "dispatch conversion uses null-terminated help, source and description BSTRs in native order");
    auto failed=sample(1,7,0);
    check(!failed.present[1]&&failed.present[0]&&!failed.present[2]&&!failed.caught&&failed.strings[0]==L"owner module",
        "BSTR allocation failures preserve partial output and retry the current module's source name");
    clear();modules[0].m_lpszCurrentAppName=modules[1].m_lpszCurrentAppName=nullptr;
    printf("%s dispatch conversion: %u native comparisons completed\n",api->image?"Native":"Source",sampleIndex);
}
