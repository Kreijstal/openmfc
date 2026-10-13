#pragma once
#include "openmfc/afx.h"
namespace openmfc { namespace detail {
inline void WriteCollectionCount(CArchive& ar,UINT64 count) {
    if(count<0xffff)ar<<static_cast<WORD>(count);
    else {ar<<static_cast<WORD>(0xffff);if(count<0xffffffff)ar<<static_cast<DWORD>(count);
        else {ar<<static_cast<DWORD>(0xffffffff);ar.Write(&count,8);}}
}
inline UINT64 ReadCollectionCount(CArchive& ar) {
    WORD word;ar>>word;if(word!=0xffff)return word;
    DWORD dword;ar>>dword;if(dword!=0xffffffff)return dword;
    UINT64 count;ar.Read(&count,8);return count;
}
} }
