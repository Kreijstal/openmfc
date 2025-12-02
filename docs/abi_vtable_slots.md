# Vtable Slot Reference (Phase 0A)

Derived from `layout.log`.

## CStage1_Simple
- 0: dtor
- 1: GetValue
- 2: SetValue

## CStage2_Signatures
- 0..5: Func overloads in declaration order (int, int*, const int*, int&, const int&, volatile int*)

## CStage4_Multi
- Vftable@CStage4_A: slot0 FuncA
- Vftable@CStage4_B: slot0 FuncB (this adjustor +16)

## CStage6_Modern
- 0: NoExceptFunc
- 1: ConstFunc

## CStage7_NoVtable
- 0: Method

## CCovariantBase
- 0: Clone
- 1: dtor

## CCovariantDerived
- 0: Clone
- 1: dtor

## IPure
- 0: PureMethod (purecall)
- 1: dtor
