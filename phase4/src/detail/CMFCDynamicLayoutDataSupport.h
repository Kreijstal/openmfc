#pragma once
// Shared internals of the former global_cmfcdynamiclayoutdata.cpp translation unit.
// Definitions live in detail/CMFCDynamicLayoutDataSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {} } }
using namespace openmfc::detail::cmfcdynamiclayoutdata;
// OpenMFC — CMFCDynamicLayoutData real implementation.
//
// Harvested layout: class CMFCDynamicLayoutData size(56), a single member
// m_listCtrls of type CList<Item, Item&>.  CList derives from CObject, so it
// carries a vfptr at offset 0; the whole object IS that CList.  The class
// itself is non-polymorphic (the vfptr belongs to the embedded CList/CObject).
//
//   struct Item { CMFCDynamicLayout::MoveSettings m_moveSettings;   // 2 * int
//                 CMFCDynamicLayout::SizeSettings m_sizeSettings; };// 2 * int
//
// CList<TYPE> member order (CObject base first):
//   0  vfptr (CObject)
//   8  CNode* m_pNodeHead
//  16  CNode* m_pNodeTail
//  24  INT_PTR m_nCount
//  32  CNode* m_pNodeFree
//  40  CPlex* m_pBlocks
//  48  INT_PTR m_nBlockSize
//  -> size 56.

#include <windows.h>
#include <cstdlib>
#include <cstddef>

#ifdef __GNUC__
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif


// CMFCDynamicLayout::MoveSettings / SizeSettings are each two int ratios.
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
struct MoveSettings { int m_nXRatio; int m_nYRatio; };
} } }
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
struct SizeSettings { int m_nXRatio; int m_nYRatio; };
} } }

// CList<Item, Item&>::Item
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
struct Item {
    MoveSettings m_moveSettings;
    SizeSettings m_sizeSettings;
};
} } }

// CList<Item, Item&>::CNode
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
struct CNode {
    CNode* pNext;
    CNode* pPrev;
    Item   data;
};
} } }

// MFC CPlex header: a next pointer followed by a raw byte payload.
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
struct CPlex {
    CPlex* pNext;
    // BYTE data[nMax * cbElement] follows immediately.
};
} } }

// Layout-faithful view of CMFCDynamicLayoutData (== its embedded CList).
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
struct S_Cmfcdynamiclayoutdata {
    void*   vfptr;          // 0  CObject vfptr of the embedded CList
    CNode*  m_pNodeHead;    // 8
    CNode*  m_pNodeTail;    // 16
    INT_PTR m_nCount;       // 24
    CNode*  m_pNodeFree;    // 32
    CPlex*  m_pBlocks;      // 40
    INT_PTR m_nBlockSize;   // 48
};
} } }


// --- CList internals ------------------------------------------------------

// Allocate a fresh CNode, carving a new CPlex block when the free list is dry.
namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
CNode* ListNewNode(S_Cmfcdynamiclayoutdata* s, CNode* pPrev, CNode* pNext);
} } }

namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
void ListAddTail(S_Cmfcdynamiclayoutdata* s, const Item& item);
} } }

namespace openmfc { namespace detail { namespace cmfcdynamiclayoutdata {
void ListRemoveAll(S_Cmfcdynamiclayoutdata* s);
} } }


// ==========================================================================
// Exports
// ==========================================================================



