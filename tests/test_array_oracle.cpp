#include <windows.h>
#include "array_probe.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>

int main() {
    setvbuf(stdout,nullptr,_IONBF,0);
    auto source=LoadLibraryW(L"array_source.dll"),sdk=LoadLibraryW(L"array_sdk.dll"),native=LoadLibraryW(L"array_native.dll");
    if(!source||!sdk||!native){printf("Array oracle modules failed to load: %lu\n",GetLastError());return 1;}
    auto run=reinterpret_cast<RunArrayProbe>(GetProcAddress(source,"run_array_probe"));
    auto oracle=reinterpret_cast<RunArrayProbe>(GetProcAddress(native,"run_array_probe"));
    auto sdkRun=reinterpret_cast<RunArrayProbe>(GetProcAddress(sdk,"run_array_probe"));
    if(!run||!oracle||!sdkRun)return 1;
    auto left=std::make_unique<ArrayReport>(),right=std::make_unique<ArrayReport>();
    int cases=0,failures=0;
    for(int client=0;client<2;++client)for(int kind=0;kind<8;++kind)for(int scenario=0;scenario<(kind==7?39:kind==6?30:kind==4?2:kind==3?6:16);++scenario)for(int failure=0;failure<(kind==7?10:kind==6?10:kind==5?4:kind>=3?1:10);++failure) {
        (client?sdkRun:run)(kind,scenario,failure,left.get());oracle(kind,scenario,failure,right.get());++cases;
        if(left->overflow||right->overflow||left->count!=right->count||memcmp(left->events,right->events,left->count*sizeof(ArrayEvent))) {
            ++failures;printf("Mismatch client=%d kind=%d scenario=%d failure=%d events=%u/%u overflow=%u/%u\n",client,kind,scenario,failure,left->count,right->count,left->overflow,right->overflow);
            for(unsigned i=0;i<left->count&&i<right->count;++i)if(memcmp(&left->events[i],&right->events[i],sizeof(ArrayEvent))) {
                const auto& a=left->events[i];const auto& b=right->events[i];
                printf("event %u Source: %lld %lld %lld %lld | %lld %lld %lld %lld\n",i,a.kind,a.a,a.b,a.c,a.size,a.capacity,a.grow,a.data);
                printf("event %u Native: %lld %lld %lld %lld | %lld %lld %lld %lld\n",i,b.kind,b.a,b.b,b.c,b.size,b.capacity,b.grow,b.data);break;
            }
        }
    }
    printf("Native SDK array oracle: %d scenarios, %d failures\n",cases,failures);return failures?1:0;
}
