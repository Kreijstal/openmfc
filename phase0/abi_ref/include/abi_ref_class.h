#pragma once

#ifdef ABI_REF_EXPORTS
    #define ABI_REF_API __declspec(dllexport)
#else
    #define ABI_REF_API __declspec(dllimport)
#endif

// Minimal reference class to study MSVC's ABI:
// - ctor, dtor
// - two virtual methods
// - one data member
class ABI_REF_API CReferenceTest {
public:
    CReferenceTest();
    virtual ~CReferenceTest();

    virtual int GetValue() const;
    virtual void SetValue(int v);

private:
    int m_value;
};

extern "C" {
    // Factory helpers for cross-DLL validation.
    ABI_REF_API CReferenceTest* CreateRef();
    ABI_REF_API void DestroyRef(CReferenceTest* p);
}
