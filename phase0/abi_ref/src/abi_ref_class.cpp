#define ABI_REF_EXPORTS
#include "abi_ref_class.h"

CReferenceTest::CReferenceTest()
    : m_value(42)
{
}

CReferenceTest::~CReferenceTest() = default;

int CReferenceTest::GetValue() const
{
    return m_value;
}

void CReferenceTest::SetValue(int v)
{
    m_value = v;
}

extern "C" CReferenceTest* CreateRef()
{
    return new CReferenceTest();
}

extern "C" void DestroyRef(CReferenceTest* p)
{
    delete p;
}
