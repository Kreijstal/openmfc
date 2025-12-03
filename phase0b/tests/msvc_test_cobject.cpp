#include "../include/afx.h"
#include <iostream>

class CMyObj : public CObject {
public:
    CMyObj() = default;
    virtual ~CMyObj() = default;
};

class CMyDerived : public CObject {
public:
    static CRuntimeClass classMyDerived;
    CMyDerived() = default;
    virtual ~CMyDerived() = default;
    virtual CRuntimeClass* GetRuntimeClass() const override {
        return &classMyDerived;
    }
};

CRuntimeClass CMyDerived::classMyDerived = {
    "CMyDerived",
    static_cast<int>(sizeof(CMyDerived)),
    0x1234,
    nullptr,
    nullptr
};

int main() {
    std::cout << "[Host] allocating..." << std::endl;
    CMyObj* p = new CMyObj();
    if (!p) {
        std::cerr << "Alloc failed" << std::endl;
        return 1;
    }

    std::cout << "[Host] calling GetRuntimeClass..." << std::endl;
    CRuntimeClass* cls = p->GetRuntimeClass();
    if (!cls || std::string(cls->m_lpszClassName) != "CObject") {
        std::cerr << "Runtime class mismatch" << std::endl;
        delete p;
        return 1;
    }

    std::cout << "[Host] calling IsKindOf..." << std::endl;
    if (!p->IsKindOf(cls)) {
        std::cerr << "IsKindOf failed" << std::endl;
        delete p;
        return 1;
    }

    std::cout << "[Host] calling Serialize..." << std::endl;
    // Pass a null archive reference to prove symbol/vtable wiring works.
    p->Serialize(*reinterpret_cast<CArchive*>(nullptr));

    // Derived class should use its own GetRuntimeClass (virtual dispatch)
    CMyDerived* pDer = new CMyDerived();
    if (!pDer->IsKindOf(&CMyDerived::classMyDerived)) {
        std::cerr << "Derived IsKindOf failed" << std::endl;
        delete pDer;
        delete p;
        return 1;
    }
    delete pDer;

    std::cout << "[Host] deleting..." << std::endl;
    delete p;
    std::cout << "PASS" << std::endl;
    return 0;
}
