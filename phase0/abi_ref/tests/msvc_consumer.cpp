#include "abi_ref_class.h"
#include <iostream>

int main() {
    std::cout << "[Host] creating via factory..." << std::endl;
    CReferenceTest* obj = CreateRef();
    if (!obj) {
        std::cerr << "Factory returned null" << std::endl;
        return 1;
    }

    std::cout << "[Host] creating a second instance..." << std::endl;
    CReferenceTest* obj2 = CreateRef();
    if (!obj2) {
        std::cerr << "Second factory returned null" << std::endl;
        DestroyRef(obj);
        return 1;
    }

    std::cout << "[Host] checking default value..." << std::endl;
    if (obj->GetValue() != 42) {
        std::cerr << "Default GetValue != 42: " << obj->GetValue() << std::endl;
        DestroyRef(obj);
        DestroyRef(obj2);
        return 1;
    }

    std::cout << "[Host] setting value to 123..." << std::endl;
    obj->SetValue(123);
    if (obj->GetValue() != 123) {
        std::cerr << "GetValue after SetValue != 123: " << obj->GetValue() << std::endl;
        DestroyRef(obj);
        DestroyRef(obj2);
        return 1;
    }

    std::cout << "[Host] setting second instance to 7..." << std::endl;
    obj2->SetValue(7);
    if (obj2->GetValue() != 7) {
        std::cerr << "Second instance GetValue != 7: " << obj2->GetValue() << std::endl;
        DestroyRef(obj);
        DestroyRef(obj2);
        return 1;
    }
    if (obj->GetValue() != 123) {
        std::cerr << "First instance changed unexpectedly: " << obj->GetValue() << std::endl;
        DestroyRef(obj);
        DestroyRef(obj2);
        return 1;
    }

    DestroyRef(obj);
    DestroyRef(obj2);
    std::cout << "PASS" << std::endl;
    return 0;
}
