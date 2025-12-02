#include "08_covariant.h"

CCovariantBase* CCovariantBase::Clone() { return new CCovariantBase(); }

CCovariantDerived* CCovariantDerived::Clone() { return new CCovariantDerived(); }
