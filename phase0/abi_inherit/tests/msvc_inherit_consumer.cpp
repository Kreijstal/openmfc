#include "abi_inherit.h"
#include <iostream>

int main() {
    std::cout << "[Host] Base path..." << std::endl;
    Base* b = CreateBase();
    if (!b || b->Get() != 1) { std::cerr << "Base default wrong\n"; return 1; }
    b->Set(5);
    if (b->Get() != 5) { std::cerr << "Base set/get wrong\n"; return 1; }
    DestroyBase(b);

    std::cout << "[Host] Derived single..." << std::endl;
    Derived* d = CreateDerived();
    if (!d || d->Get() != 12) { std::cerr << "Derived default wrong\n"; return 1; }
    d->Set(30);
    if (d->Get() != 20) { std::cerr << "Derived set/get wrong\n"; return 1; }
    Base* bd = d;
    if (bd->Get() != 20) { std::cerr << "Derived via Base wrong\n"; return 1; }
    DestroyDerived(d);

    std::cout << "[Host] Multiple inheritance..." << std::endl;
    DerivedMI* mi = CreateDerivedMI();
    Base1* b1 = mi;
    Base2* b2 = mi;
    if (!mi || b1->Get1() != 112 || b2->Get2() != 224) { std::cerr << "MI defaults wrong\n"; return 1; }
    b1->Set1(50);
    b2->Set2(60);
    if (b1->Get1() != 49 || b2->Get2() != 58) { std::cerr << "MI set/get wrong\n"; return 1; }
    DestroyDerivedMI(mi);

    std::cout << "PASS" << std::endl;
    return 0;
}
