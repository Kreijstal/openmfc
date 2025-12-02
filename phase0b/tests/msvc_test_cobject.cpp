#include "../include/afx.h"
#include <iostream>

class CMyObj : public CObject {
public:
    CMyObj() = default;
    virtual ~CMyObj() = default;
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

    std::cout << "[Host] deleting..." << std::endl;
    delete p;
    std::cout << "PASS" << std::endl;
    return 0;
}
