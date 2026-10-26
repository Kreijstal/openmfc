#pragma once
// Exception factories are peripheral to the TLS algorithms under test.
extern "C" void AFXAPI impl__AfxThrowMemoryException__YAXXZ() { throw 91; }
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ() { throw 19; }
extern "C" void AFXAPI impl__AfxThrowOleException__YAXJ_Z(long value) { throw value; }
