#pragma once
// Native CDocManager process storage consumed by application teardown.
// Retail mfc140u keeps these independent pointers at RVAs 0x3be3f0/0x3be3f8.
namespace openmfc { namespace detail { namespace docmanager {
inline void* staticDocumentManager=nullptr;
inline void* staticTemplateList=nullptr;
} } }
