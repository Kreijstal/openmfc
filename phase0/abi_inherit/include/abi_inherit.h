#pragma once

#ifdef ABI_INHERIT_EXPORTS
#    define ABI_INHERIT_API __declspec(dllexport)
#else
#    define ABI_INHERIT_API __declspec(dllimport)
#endif

// Single inheritance
class ABI_INHERIT_API Base {
public:
    Base();
    virtual ~Base();
    virtual int Get() const;
    virtual void Set(int v);
protected:
    int m_val;
};

class ABI_INHERIT_API Derived : public Base {
public:
    Derived();
    virtual ~Derived();
    int Get() const override;
    void Set(int v) override;
};

// Simple multiple inheritance (no diamonds)
class ABI_INHERIT_API Base1 {
public:
    Base1();
    virtual ~Base1();
    virtual int Get1() const;
    virtual void Set1(int v);
protected:
    int m_v1;
};

class ABI_INHERIT_API Base2 {
public:
    Base2();
    virtual ~Base2();
    virtual int Get2() const;
    virtual void Set2(int v);
protected:
    int m_v2;
};

class ABI_INHERIT_API DerivedMI : public Base1, public Base2 {
public:
    DerivedMI();
    virtual ~DerivedMI();
    int Get1() const override;
    void Set1(int v) override;
    int Get2() const override;
    void Set2(int v) override;
};

extern "C" {
    ABI_INHERIT_API Base* CreateBase();
    ABI_INHERIT_API Derived* CreateDerived();
    ABI_INHERIT_API DerivedMI* CreateDerivedMI();
    ABI_INHERIT_API void DestroyBase(Base* p);
    ABI_INHERIT_API void DestroyDerived(Derived* p);
    ABI_INHERIT_API void DestroyDerivedMI(DerivedMI* p);
}
