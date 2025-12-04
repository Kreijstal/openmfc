#define ABI_INHERIT_EXPORTS
#include "abi_inherit.h"

Base::Base() : m_val(1) {}
Base::~Base() = default;
int Base::Get() const { return m_val; }
void Base::Set(int v) { m_val = v; }

Derived::Derived() { m_val = 2; }
Derived::~Derived() = default;
int Derived::Get() const { return m_val + 10; }
void Derived::Set(int v) { m_val = v - 10; }

Base1::Base1() : m_v1(11) {}
Base1::~Base1() = default;
int Base1::Get1() const { return m_v1; }
void Base1::Set1(int v) { m_v1 = v; }

Base2::Base2() : m_v2(22) {}
Base2::~Base2() = default;
int Base2::Get2() const { return m_v2; }
void Base2::Set2(int v) { m_v2 = v; }

DerivedMI::DerivedMI() { m_v1 = 111; m_v2 = 222; }
DerivedMI::~DerivedMI() = default;
int DerivedMI::Get1() const { return m_v1 + 1; }
void DerivedMI::Set1(int v) { m_v1 = v - 1; }
int DerivedMI::Get2() const { return m_v2 + 2; }
void DerivedMI::Set2(int v) { m_v2 = v - 2; }

extern "C" Base* CreateBase() { return new Base(); }
extern "C" Derived* CreateDerived() { return new Derived(); }
extern "C" DerivedMI* CreateDerivedMI() { return new DerivedMI(); }
extern "C" void DestroyBase(Base* p) { delete p; }
extern "C" void DestroyDerived(Derived* p) { delete p; }
extern "C" void DestroyDerivedMI(DerivedMI* p) { delete p; }
