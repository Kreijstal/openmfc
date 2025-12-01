// 01_basics.cpp
// Implementation of CStage1_Basics - trivial methods for ABI testing

#include "01_basics.h"

int CStage1_Basics::GetInt() {
    return m_data;
}

void CStage1_Basics::SetInt(int value) {
    m_data = value;
}
