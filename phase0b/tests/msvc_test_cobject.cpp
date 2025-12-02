#include "../include/afx.h"
#include <iostream>

class CMyObj : public CObject {
public:
    CMyObj() = default;
    virtual ~CMyObj() = default;
};

int main() {
    CMyObj* p = new CMyObj();
    if (!p) {
        std::cerr << "Alloc failed" << std::endl;
        return 1;
    }

    CRuntimeClass* cls = p->GetRuntimeClass();
    if (!cls || std::string(cls->m_lpszClassName) != "CObject") {
        std::cerr << "Runtime class mismatch" << std::endl;
        delete p;
        return 1;
    }

    if (!p->IsKindOf(cls)) {
        std::cerr << "IsKindOf failed" << std::endl;
        delete p;
        return 1;
    }

    delete p;
    std::cout << "PASS" << std::endl;
    return 0;
}

