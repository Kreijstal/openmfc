// 01_basics.h
// Minimal MSVC ABI probe class to reveal:
// 1. Vptr position and vtable layout
// 2. MSVC name mangling for virtual methods
// 3. Class size and padding

#ifndef ABI_STRESS_01_BASICS_H
#define ABI_STRESS_01_BASICS_H

class CStage1_Basics {
public:
    virtual int GetInt();
    virtual void SetInt(int value);
private:
    int m_data;
};

#endif // ABI_STRESS_01_BASICS_H
