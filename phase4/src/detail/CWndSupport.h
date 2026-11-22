#pragma once
// CWnd core-window exports (wave 1: core-windows)
//
// Implements the remaining CWnd exports in the Windows/MSVC x64 ABI. Every
// function is a plain extern "C" ms_abi function with `this` passed as an
// explicit first parameter. Semantics are transcribed from the retail
// mfc140u.dll disassembly (see /tmp/openmfc_remaining).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <map>
#include <mutex>
#include <utility>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Exported stubs used internally (avoid re-implementing)
// ---------------------------------------------------------------------------
extern "C" int MS_ABI impl__ModifyStyle_CWnd__SAHPEAUHWND____KKI_Z(HWND__* p0, unsigned long p1, unsigned long p2, unsigned int p3);
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__SAHPEAUHWND____KKI_Z(HWND__* p0, unsigned long p1, unsigned long p2, unsigned int p3);






























