#pragma once
// Shared internals of the former global_afx_messages.cpp translation unit.
// Definitions live in detail/AfxMessagesSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace afxmessages {} } }
using namespace openmfc::detail::afxmessages;
// Global residuals: registered AFX window-message exports.
//
// MFC exposes these as DATA symbols initialized from RegisterWindowMessage.
// The generated .def already aliases the MSVC DATA exports to the impl__ names.

#include <windows.h>


namespace openmfc { namespace detail { namespace afxmessages {
unsigned int RegisterAfxMessage(const wchar_t* name);
} } }











































