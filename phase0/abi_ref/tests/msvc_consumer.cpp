#include "abi_ref_class.h"
#include <iostream>

int main() {
    std::cout << "[Host] creating via factory..." << std::endl;
    CReferenceTest* obj = CreateRef();
    if (!obj) {
        std::cerr << "Factory returned null" << std::endl;
        return 1;
    }

    std::cout << "[Host] checking default value..." << std::endl;
    if (obj->GetValue() != 42) {
        std::cerr << "Default GetValue != 42: " << obj->GetValue() << std::endl;
        DestroyRef(obj);
        return 1;
    }

    std::cout << "[Host] setting value to 123..." << std::endl;
    obj->SetValue(123);
    if (obj->GetValue() != 123) {
        std::cerr << "GetValue after SetValue != 123: " << obj->GetValue() << std::endl;
        DestroyRef(obj);
        return 1;
    }

    DestroyRef(obj);
    std::cout << "PASS" << std::endl;
    return 0;
}
