#include "02_signatures.h"

void CStage2_Signatures::Func(int) {}
void CStage2_Signatures::Func(int*) {}
void CStage2_Signatures::Func(const int*) {}
void CStage2_Signatures::Func(int&) {}
void CStage2_Signatures::Func(const int&) {}
void CStage2_Signatures::Func(volatile int*) {}
