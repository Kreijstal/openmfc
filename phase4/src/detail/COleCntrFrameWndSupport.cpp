#include "COleCntrFrameWndSupport.h"

namespace openmfc::detail::colecntrframewnd {
namespace {
thread_local InPlaceBarCreation* currentCreation = nullptr;
}

InPlaceBarCreation::InPlaceBarCreation(COleCntrFrameWnd* main, COleCntrFrameWnd* document)
    : previous_(currentCreation), main_(main), document_(document) {
    currentCreation = this;
}

InPlaceBarCreation::~InPlaceBarCreation() {
    currentCreation = previous_;
}

COleCntrFrameWnd* InPlaceBarCreation::FindFrame(HWND window) {
    for (auto* scope = currentCreation; scope; scope = scope->previous_) {
        if (scope->main_ && scope->main_->m_hWnd == window) return scope->main_;
        if (scope->document_ && scope->document_->m_hWnd == window) return scope->document_;
    }
    return nullptr;
}
}
