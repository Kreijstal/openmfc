#include "openmfc/eh_rtti.h"
#include <stdlib.h>

extern "C" __declspec(dllimport) void _CxxThrowException(void* pExceptionObject, const struct ThrowInfo* pThrowInfo);

struct CMemoryException { void* vtable; int dummy; };
struct CFileException { void* vtable; int dummy; };

extern const struct ThrowInfo* const TI_CMemoryException;
extern const struct ThrowInfo* const TI_CFileException;

static void* ucrt_malloc(size_t sz) { return malloc(sz); }
static void ucrt_free(void* p) { free(p); }

void AfxThrowMemoryException(void) {
    struct CMemoryException* ex = (struct CMemoryException*)ucrt_malloc(sizeof(struct CMemoryException));
    if (!ex) return;
    ex->vtable = NULL;
    ex->dummy = 0;
    _CxxThrowException(ex, TI_CMemoryException);
}

void AfxThrowFileException(void) {
    struct CFileException* ex = (struct CFileException*)ucrt_malloc(sizeof(struct CFileException));
    if (!ex) return;
    ex->vtable = NULL;
    ex->dummy = 0;
    _CxxThrowException(ex, TI_CFileException);
}

void OpenMFC_DeleteException(void* p) { ucrt_free(p); }
