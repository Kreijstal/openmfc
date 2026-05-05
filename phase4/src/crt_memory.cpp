// Minimal CRT memory/string routines to avoid importing api-ms-win-crt-private.
// llvm-mingw may otherwise resolve these primitives through the private UCRT
// facade, which is not loadable on the Windows CI runners.

#include <stddef.h>
#include <wchar.h>

extern "C" {

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    return dest;
}

void* memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);

    if (d == s || n == 0) {
        return dest;
    }

    if (d < s || d >= s + n) {
        for (size_t i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

int memcmp(const void* lhs, const void* rhs, size_t n) {
    const unsigned char* a = static_cast<const unsigned char*>(lhs);
    const unsigned char* b = static_cast<const unsigned char*>(rhs);
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return static_cast<int>(a[i]) - static_cast<int>(b[i]);
        }
    }
    return 0;
}

void* memchr(const void* ptr, int ch, size_t n) {
    const unsigned char* p = static_cast<const unsigned char*>(ptr);
    const unsigned char c = static_cast<unsigned char>(ch);
    for (size_t i = 0; i < n; ++i) {
        if (p[i] == c) {
            return const_cast<unsigned char*>(p + i);
        }
    }
    return nullptr;
}

char* strchr(const char* str, int ch) {
    const char c = static_cast<char>(ch);
    for (;; ++str) {
        if (*str == c) {
            return const_cast<char*>(str);
        }
        if (*str == '\0') {
            return nullptr;
        }
    }
}

int __intrinsic_setjmpex(void*) {
    return 0;
}

[[noreturn]] void longjmp(void*, int) {
    __builtin_trap();
}

void (*__imp_longjmp)(void*, int) = longjmp;
int (*__imp___intrinsic_setjmpex)(void*) = __intrinsic_setjmpex;

wchar_t* wcschr(const wchar_t* str, wchar_t ch) {
    for (;; ++str) {
        if (*str == ch) {
            return const_cast<wchar_t*>(str);
        }
        if (*str == L'\0') {
            return nullptr;
        }
    }
}

wchar_t* wcsrchr(const wchar_t* str, wchar_t ch) {
    const wchar_t* found = nullptr;
    for (;; ++str) {
        if (*str == ch) {
            found = str;
        }
        if (*str == L'\0') {
            break;
        }
    }
    return const_cast<wchar_t*>(found);
}

}
