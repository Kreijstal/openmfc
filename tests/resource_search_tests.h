// Included in the real extension-library fixture, inside its private namespace.
namespace resource_test {
HINSTANCE handle(unsigned index){return reinterpret_cast<HINSTANCE>(UINT_PTR(0x1000+index));}
int index(HINSTANCE module){return int(UINT_PTR(module)-0x1000);}
std::vector<int> probes,allocations,freeDepths;
unsigned available;
bool strings;
int change,allocationFailure,allocationCall,resourceFrees;
CDynLinkLibrary *firstLibrary,*secondLibrary;
void* snapshot;
void* (__cdecl* realRecalloc)(void*,size_t,size_t);
void (__cdecl* realFree)(void*);
LPCWSTR requestedName,requestedType;
void mutate() {
    int action=change;change=0;
    if(action==1)modules[0].m_libraryList.Remove(firstLibrary);
    if(action==2)firstLibrary->m_bSystem=TRUE;
    if(action==3)firstLibrary->m_hResource=handle(7);
    if(action==4){modules[0].m_libraryList.Remove(firstLibrary);secondLibrary->m_hResource=handle(4);}
    if(action==5)select(1);
    if(action==6){select(1);modules[0].m_bSystem=TRUE;}
    if(action==7)modules[0].m_appLangDLL=handle(7);
    if(action==8)throw 51;
}
HRSRC WINAPI find(HMODULE module,LPCWSTR name,LPCWSTR type) {
    check(!resourceLockDepth,"resource API callbacks run outside the library lock");
    check(name==(strings ? MAKEINTRESOURCEW(2):requestedName) && type==(strings ? RT_STRING:requestedType),
        "resource finder preserves names/types and selects the requested string block");
    int which=index(module);probes.push_back(which);
    if(which==4 && change)mutate();
    return which>=0 && which<8 && (available&(1u<<which)) ? reinterpret_cast<HRSRC>(module):nullptr;
}
std::array<WORD,32> image;
DWORD imageSize;
HGLOBAL WINAPI load(HMODULE module,HRSRC found){check(module==reinterpret_cast<HMODULE>(found),"resource load belongs to selected module");return reinterpret_cast<HGLOBAL>(module);}
void* WINAPI lock(HGLOBAL){check(!resourceLockDepth,"string images are locked outside the library list lock");return image.data();}
DWORD WINAPI size(HMODULE,HRSRC){return imageSize;}
void* __cdecl recalloc(void* previous,size_t count,size_t bytes) {
    check(resourceLockDepth==1 && bytes==8,"resource snapshots grow under the real recursive library lock");
    allocations.push_back(int(count));++allocationCall;
    if(allocationCall==allocationFailure)return nullptr;
    if(change==9){change=0;firstLibrary->m_hResource=handle(7);}
    void* result=realRecalloc(previous,count,bytes);if(result)snapshot=result;return result;
}
void __cdecl release(void* memory) {
    if(memory && memory==snapshot){snapshot=nullptr;++resourceFrees;freeDepths.push_back(resourceLockDepth);}
    realFree(memory);
}
struct Result {
    int selected,caught,frees;std::vector<int> visits,growth,releaseDepths;
    bool operator==(const Result& other)const{return selected==other.selected && caught==other.caught && frees==other.frees && visits==other.visits && growth==other.growth && releaseDepths==other.releaseDepths;}
};
std::vector<Result> goldenResources;
Result sample(bool string,bool system,unsigned mask,int mutation,int fail) {
    clear();strings=string;available=mask;change=0;allocationFailure=0;
    modules[0].m_bSystem=system;modules[0].m_hCurrentResourceHandle=handle(0);
    modules[0].m_appLangDLL=handle(1);modules[1].m_hCurrentResourceHandle=handle(7);
    Object objects[6];
    // Registration is LIFO. Both categories include multiple actual libraries.
    const unsigned ids[]={2,3,4,5,6,7};
    for(unsigned i=0;i<6;++i){objects[i].reset();api->handles(objects[i].get(),nullptr,i==5 ? nullptr:handle(ids[i]));objects[i].get()->m_bSystem=i==3 ? -1:i==4 ? 17:FALSE;}
    firstLibrary=objects[2].get();secondLibrary=objects[1].get();
    change=mutation;allocationFailure=fail;allocationCall=resourceFrees=0;snapshot=nullptr;
    probes.clear();allocations.clear();freeDepths.clear();resourceLockDepth=0;resourceRecording=true;
    HINSTANCE selected=nullptr;int caught=0;
    try{selected=string ? static_cast<HINSTANCE>(api->string(17)):api->resource(requestedName,requestedType);}
    catch(int error){caught=error;}
    resourceRecording=false;
    check(!resourceLockDepth && !snapshot,"resource searches balance locks and release snapshots, including callback exceptions");
    Result result{selected ? index(selected):-1,caught,resourceFrees,probes,allocations,freeDepths};
    change=0;select(0);
    for(auto& object:objects)api->destroy(object.get());
    check(modules[0].m_libraryList.IsEmpty(),"resource callbacks leave no extension registrations after cleanup");
    return result;
}
}
void resourceSearch() {
    using namespace resource_test;
    auto imageModule=api->image ? api->image:GetModuleHandleW(nullptr);
    Patch a(imageModule,"FindResourceW",reinterpret_cast<void*>(&find));
    Patch b(imageModule,"LoadResource",reinterpret_cast<void*>(&load));
    Patch c(imageModule,"LockResource",reinterpret_cast<void*>(&lock));
    Patch d(imageModule,"SizeofResource",reinterpret_cast<void*>(&size));
    Patch e(imageModule,"_recalloc",reinterpret_cast<void*>(&recalloc));realRecalloc=reinterpret_cast<decltype(realRecalloc)>(e.previous);
    Patch f(imageModule,"free",reinterpret_cast<void*>(&release));realFree=reinterpret_cast<decltype(realFree)>(f.previous);
    probes.reserve(32);allocations.reserve(16);freeDepths.reserve(16);goldenResources.reserve(12000);
    image.fill(0);image[1]=3;image[2]='o';image[3]='k';image[4]='!';imageSize=DWORD(image.size()*2);
    requestedName=MAKEINTRESOURCEW(17);requestedType=RT_BITMAP;
    unsigned sampleIndex=0;
    for(bool string:{false,true})for(bool system:{false,true})for(unsigned mask=0;mask<256;++mask)for(int mutation:{0,1,2,3,4,5,6,7,8,9}) {
        auto result=sample(string,system,mask,mutation,0);
        if(api->image)check(sampleIndex<goldenResources.size() && result==goldenResources[sampleIndex],"native/source search order, list mutation, allocation and cleanup match");
        else goldenResources.push_back(result);
        ++sampleIndex;
    }
    auto miss=sample(false,false,0,0,0);
    check(miss.selected==0 && miss.visits==std::vector<int>({0,4,3,2,1,6,5}) && miss.growth==std::vector<int>({1,2,4,1,2}),
        "general resource misses return the current module after application, non-system, language and system searches");
    auto systemMiss=sample(true,true,0,0,0);
    check(systemMiss.selected==-1 && systemMiss.visits==std::vector<int>({4,3,2,1,0,6,5}),
        "system module resources follow language resources and missing strings return null");
    for(bool string:{false,true})for(int fail:{1,2,3,4,5}) {
        auto result=sample(string,false,0,0,fail);
        if(api->image)check(sampleIndex<goldenResources.size() && result==goldenResources[sampleIndex],"native/source partial snapshot retention and allocation retry match");
        else goldenResources.push_back(result);
        ++sampleIndex;
    }
    // Exercise the actual loader with the real finder, not an assumed module.
    clear();modules[0].m_hCurrentResourceHandle=handle(0);available=1;strings=true;change=0;resourceFrees=0;
    wchar_t out[16];
    check(api->load(17,out,16)==3 && !wcscmp(out,L"ok!"),"real finder/loader integration copies a non-empty individual string entry");
    image.fill(0);out[0]='x';check(!api->string(17) && !api->load(17,out,16) && !out[0],"empty string slot is rejected before the loader dereferences it");
    image[0]=30;imageSize=8;out[0]='x';check(!api->string(17) && !api->load(17,out,16) && !out[0],"out-of-bounds string entry is rejected before copying");
    // Null resource handles and null identifiers are forwarded to Win32 too.
    strings=false;requestedName=requestedType=nullptr;probes.clear();available=0;modules[0].m_hCurrentResourceHandle=nullptr;
    check(!api->resource(nullptr,nullptr) && !probes.empty(),"general finder forwards null names/types and current module handles to Win32");
    clear();std::printf("%s resource search: %u native comparison scenarios completed\n",api->image ? "Native":"Source",sampleIndex);
}
