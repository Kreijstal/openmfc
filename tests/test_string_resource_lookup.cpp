// Integrate the real module finder and loader: an existing RT_STRING block is
// insufficient when the requested slot is empty, absent or outside the block.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <array>
#include <cstdio>
#include <cstring>
extern "C" int impl__AfxLoadString__YAHIPEA_WI_Z(UINT,wchar_t*,UINT);
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
HINSTANCE resources=reinterpret_cast<HINSTANCE>(0x44),instance=reinterpret_cast<HINSTANCE>(0x44);
HINSTANCE AFXAPI AfxGetResourceHandle(){return resources;}
HINSTANCE AFXAPI AfxGetInstanceHandle(){return instance;}
void AFXAPI AfxThrowInvalidArgException(){throw 7;}
std::array<WORD,32> first{};DWORD firstSize=64;int failures;
void check(bool good,const char* message){if(!good){++failures;printf("FAIL: %s\n",message);}}
HRSRC WINAPI find(HMODULE module,LPCWSTR name,LPCWSTR type){check(UINT_PTR(name)==2&&UINT_PTR(type)==6,"resource lookup receives the requested STRING block");return module==resources||module==instance?reinterpret_cast<HRSRC>(module):nullptr;}
HGLOBAL WINAPI load(HMODULE module,HRSRC found){check(module==reinterpret_cast<HMODULE>(found),"resource block belongs to selected module");return reinterpret_cast<HGLOBAL>(module);}
void* WINAPI lock(HGLOBAL handle){return first.data();}
DWORD WINAPI size(HMODULE module,HRSRC){return firstSize;}
void** imported(const char* wanted){auto* b=reinterpret_cast<BYTE*>(GetModuleHandleW(nullptr));auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);for(;d->Name;++d){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);auto* p=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);for(;n->u1.AddressOfData;++n,++p)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&p->u1.Function);}}return nullptr;}
struct Patch{void** p;void* old;DWORD protection;Patch(const char* name,void* replacement){p=imported(name);if(!p)abort();old=*p;VirtualProtect(p,8,PAGE_READWRITE,&protection);*p=replacement;}~Patch(){*p=old;DWORD ignored;VirtualProtect(p,8,protection,&ignored);}};
int main(){setvbuf(stdout,nullptr,_IONBF,0);Patch a("FindResourceW",reinterpret_cast<void*>(&find)),b("LoadResource",reinterpret_cast<void*>(&load)),c("LockResource",reinterpret_cast<void*>(&lock)),d("SizeofResource",reinterpret_cast<void*>(&size));
    // String 17 is index 1 of block 2 in the current module.
    first[1]=3;first[2]='o';first[3]='k';first[4]='!';wchar_t out[16];
    check(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(17)==resources,"non-empty slot selects its actual resource module");
    check(impl__AfxLoadString__YAHIPEA_WI_Z(17,out,16)==3&&!wcscmp(out,L"ok!"),"real finder/loader integration selects and copies the non-empty string");
    first.fill(0);out[0]='x';check(!impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(17)&&!impl__AfxLoadString__YAHIPEA_WI_Z(17,out,16)&&!out[0],"missing string in existing blocks returns empty output without dereferencing a null image");
    first[0]=30;firstSize=8;out[0]='x';check(!impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(17)&&!impl__AfxLoadString__YAHIPEA_WI_Z(17,out,16)&&!out[0],"entry beyond resource bounds is rejected before copying");
    printf("String resource lookup: %d failures\n",failures);return failures?1:0;
}
