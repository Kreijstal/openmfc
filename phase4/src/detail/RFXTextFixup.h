#pragma once
#include <mbstring.h>
namespace openmfc { namespace detail {
inline char* NextTextCharacter(char* text) {
    return reinterpret_cast<char*>(_mbsinc(reinterpret_cast<unsigned char*>(text)));
}
inline wchar_t* NextTextCharacter(wchar_t* text) {return text+1;}
template<class CH> void StripTrailingTextSpaces(CH* text) {
    CH* spaces=nullptr;
    for(CH* cursor=text;*cursor;cursor=NextTextCharacter(cursor)) {
        if(*cursor==CH(' ')) {if(!spaces)spaces=cursor;}
        else spaces=nullptr;
    }
    if(spaces)*spaces=0;
}
} }
