#pragma once
#include "detail/ModuleThreadStateSupport.h"

namespace openmfc { namespace detail { namespace frame {
// These embedded objects occupy the retail frame's reserved storage. Their
// construction and destruction are explicit so the surrounding bytes retain
// their native placement-construction values.
inline OpenMFC_CPtrList& ControlBars(CFrameWnd* self) {
    return *reinterpret_cast<OpenMFC_CPtrList*>(self->m_retailMiddle.m_reserved_128);
}
inline const OpenMFC_CPtrList& ControlBars(const CFrameWnd* self) {
    return *reinterpret_cast<const OpenMFC_CPtrList*>(self->m_retailMiddle.m_reserved_128);
}
inline void RegisterControlBar(CFrameWnd* frame, CControlBar* bar) {
    auto& bars = ControlBars(frame);
    if (!bars.Find(bar, nullptr)) bars.AddTail(bar);
}
inline void UnregisterControlBar(CFrameWnd* frame, CControlBar* bar) {
    auto& bars = ControlBars(frame);
    if (auto position = bars.Find(bar, nullptr)) bars.RemoveAt(position);
}
inline CString& Title(CFrameWnd* self) {
    return *reinterpret_cast<CString*>(self->m_retailTail.m_titleStorage);
}
}}}
