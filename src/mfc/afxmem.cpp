#include <new>
#include <cstdlib>

// Global new/delete overrides to use UCRT directly
// This ensures that memory allocated by OpenMFC can be freed by the host app and vice-versa.

void* operator new(std::size_t count) {
    void* p = std::malloc(count);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete(void* ptr) noexcept {
    std::free(ptr);
}

void* operator new[](std::size_t count) {
    void* p = std::malloc(count);
    if (!p) throw std::bad_alloc();
    return p;
}

void operator delete[](void* ptr) noexcept {
    std::free(ptr);
}

// Minimal class definitions for exception helpers
// We need these to instantiate the objects.
// In a real build, we would include the actual MFC headers.
// For now, we define minimal versions matching the layouts.json mock.

struct CObject {
    virtual ~CObject() {}
};

struct CException : public CObject {
    int m_bAutoDelete;
    CException() : m_bAutoDelete(1) {}
};

struct CMemoryException : public CException {
    CMemoryException() {}
};

struct CFileException : public CException {
    int m_cause;
    int m_lOsError;
    // string m_strFileName; // omitted for simplicity in mock
    CFileException(int cause, int lOsError) : m_cause(cause), m_lOsError(lOsError) {}
};

extern "C" {

void* OpenMFC_New_CMemoryException() {
    return new CMemoryException();
}

void* OpenMFC_New_CFileException(int cause, int lOsError, const char* pFileName) {
    // Ignoring pFileName for now as we didn't mock CString
    return new CFileException(cause, lOsError);
}

}
