#include "06_modern.h"

CStage6_Modern::CStage6_Modern() {}
CStage6_Modern::CStage6_Modern(CStage6_Modern&&) noexcept {}
CStage6_Modern& CStage6_Modern::operator=(CStage6_Modern&&) noexcept { return *this; }
void CStage6_Modern::NoExceptFunc() noexcept {}
void CStage6_Modern::ConstFunc() const {}
