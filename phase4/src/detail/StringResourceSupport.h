#pragma once
#include <windows.h>
namespace openmfc { namespace detail {
// Native ATL string image lookup: a block can exist without this particular
// non-empty entry. Both module selection and copying must check the same image.
inline const WORD* StringResourceImage(HINSTANCE module,UINT id) {
    HRSRC resource=FindResourceW(module,MAKEINTRESOURCEW(WORD((id>>4)+1)),RT_STRING);
    if (!resource) return nullptr;
    HGLOBAL loaded=LoadResource(module,resource);
    if (!loaded) return nullptr;
    auto* cursor=static_cast<const WORD*>(LockResource(loaded));
    if (!cursor) return nullptr;
    const BYTE* end=reinterpret_cast<const BYTE*>(cursor)+SizeofResource(module,resource);
    for (UINT index=id&15;index;--index) {
        if (reinterpret_cast<const BYTE*>(cursor)>=end) return nullptr;
        cursor+=1+*cursor;
    }
    if (reinterpret_cast<const BYTE*>(cursor)>=end || !*cursor) return nullptr;
    return cursor;
}
} }
