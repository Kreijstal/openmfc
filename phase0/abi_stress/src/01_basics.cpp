#include "01_basics.h"

CStage1_Simple::CStage1_Simple() : m_value(42) {}
CStage1_Simple::~CStage1_Simple() = default;

int CStage1_Simple::GetValue() const {
    return m_value;
}

void CStage1_Simple::SetValue(int v) {
    m_value = v;
}
