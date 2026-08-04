#pragma once
// Shared internals of the former global_ccontrolbarinfo.cpp translation unit.
// Definitions live in detail/CControlBarInfoSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ccontrolbarinfo {} } }
using namespace openmfc::detail::ccontrolbarinfo;
// OpenMFC real implementation of CControlBarInfo.
// Layout-faithful, MS_ABI thunks aliased from the .def via impl_ symbols.
//
// CControlBarInfo is the per-bar record used by CDockState to persist the
// docking layout of a frame window's control bars. It is a plain (non
// polymorphic) helper: no vtable, just attributes plus an embedded
// CUIntArray of contained bar IDs and a transient CControlBar back-pointer.
#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// class CControlBarInfo size(120):
//   0  | UINT   m_nBarID
//   4  | BOOL   m_bVisible
//   8  | BOOL   m_bFloating
//  12  | BOOL   m_bHorz
//  16  | BOOL   m_bDockBar
//  20  | CPoint m_pointPos           (LONG x, LONG y)
//  28  | UINT   m_nMRUWidth
//  32  | BOOL   m_bDocking
//  36  | UINT   m_uMRUDockID
//  40  | CRect  m_rectMRUDockPos     (LONG left/top/right/bottom)
//  56  | DWORD  m_dwMRUFloatStyle
//  60  | CPoint m_ptMRUFloatPos      (LONG x, LONG y)
//  68  | <4 bytes padding to 8-align the embedded CUIntArray>
//  72  | CUIntArray m_arrBarID       (vfptr + pData + nSize + nMaxSize + nGrowBy = 40 bytes)
// 112  | CControlBar* m_pBar         (transient)
namespace openmfc { namespace detail { namespace ccontrolbarinfo {
struct S_Ccontrolbarinfo {
    unsigned int  m_nBarID;                 // 0
    int           m_bVisible;               // 4
    int           m_bFloating;              // 8
    int           m_bHorz;                  // 12
    int           m_bDockBar;               // 16
    long          m_pointPos_x;             // 20
    long          m_pointPos_y;             // 24
    unsigned int  m_nMRUWidth;              // 28
    int           m_bDocking;               // 32
    unsigned int  m_uMRUDockID;             // 36
    long          m_rectMRUDockPos_left;    // 40
    long          m_rectMRUDockPos_top;     // 44
    long          m_rectMRUDockPos_right;   // 48
    long          m_rectMRUDockPos_bottom;  // 52
    unsigned long m_dwMRUFloatStyle;        // 56 (DWORD)
    long          m_ptMRUFloatPos_x;        // 60
    long          m_ptMRUFloatPos_y;        // 64
    char          _pad0[4];                 // 68 -> align embedded CUIntArray to 8
    // embedded CUIntArray m_arrBarID (offset 72)
    void*         m_arrBarID_vfptr;         // 72
    void*         m_arrBarID_pData;         // 80
    intptr_t      m_arrBarID_nSize;         // 88
    intptr_t      m_arrBarID_nMaxSize;      // 96
    intptr_t      m_arrBarID_nGrowBy;       // 104
    void*         m_pBar;                   // 112 (CControlBar*)
};
} } }


// MFC default for an "unset" MRU floating width.
namespace openmfc { namespace detail { namespace ccontrolbarinfo {
const unsigned int kDefaultMRUWidth = 32767;
} } }



// Build the MFC section name "<profile>-Bar<index>" used to key this bar's
// persisted blob. Returns the number of characters written (excluding NUL).
namespace openmfc { namespace detail { namespace ccontrolbarinfo {
int MakeBarSection(const wchar_t* lpszProfileName, int nIndex,
                          wchar_t* buf, size_t cch);
} } }



