// Focused coverage for COleDocument item tracking and COleClientItem state.
#include "openmfc/afxole.h"

#include <cstdio>

int main() {
    COleDocument doc;
    if (doc.GetItemCount() != 0 || doc.GetStartPosition() != nullptr) {
        std::printf("FAIL: new COleDocument should have no items\n");
        return 1;
    }

    COleClientItem first(&doc);
    COleClientItem second(&doc);

    if (doc.GetItemCount() != 2) {
        std::printf("FAIL: constructor did not register client items\n");
        return 1;
    }
    if (!doc.HasBlankItems()) {
        std::printf("FAIL: empty client items should count as blank\n");
        return 1;
    }
    if (doc.IsInPlaceActive()) {
        std::printf("FAIL: empty document should not be in-place active\n");
        return 1;
    }

    POSITION pos = reinterpret_cast<POSITION>(doc.GetStartPosition());
    COleClientItem* gotFirst = doc.GetNextClientItem(pos);
    COleClientItem* gotSecond = doc.GetNextClientItem(pos);
    COleClientItem* gotEnd = doc.GetNextClientItem(pos);
    if (gotFirst != &first || gotSecond != &second || gotEnd != nullptr || pos != nullptr) {
        std::printf("FAIL: COleDocument enumeration order/state mismatch\n");
        return 1;
    }

    if (doc.GetPrimarySelectedItem() != &first) {
        std::printf("FAIL: primary selected item should default to first tracked item\n");
        return 1;
    }

    first.SetItemState(OLE_LOADED);
    second.SetItemState(OLE_LOADED);
    if (doc.HasBlankItems()) {
        std::printf("FAIL: loaded client items should not count as blank\n");
        return 1;
    }

    first.SetActiveVerb(OLEIVERB_SHOW);
    first.SetModifiedFlag(TRUE);
    if (first.GetActiveVerb() != OLEIVERB_SHOW || !first.IsModified()) {
        std::printf("FAIL: COleClientItem state table did not retain verb/modified state\n");
        return 1;
    }

    doc.RemoveItem(&first);
    if (doc.GetItemCount() != 1 || doc.GetPrimarySelectedItem() != &second) {
        std::printf("FAIL: RemoveItem did not update tracked item list\n");
        return 1;
    }
    doc.AddItem(&first);
    if (doc.GetItemCount() != 2 || first.m_pContainerDoc != &doc || first.m_pDocument != &doc) {
        std::printf("FAIL: AddItem did not restore document ownership\n");
        return 1;
    }

    std::printf("OK: COleDocument/COleClientItem tests passed\n");
    return 0;
}
