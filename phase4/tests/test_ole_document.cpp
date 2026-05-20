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

    COleServerDoc serverDoc;
    COleClientItem embeddedClient(&serverDoc);
    if (serverDoc.GetEmbeddedItem() != &embeddedClient) {
        std::printf("FAIL: COleServerDoc embedded client lookup mismatch\n");
        return 1;
    }
    if (serverDoc.GetEmbeddedServerItem() != nullptr || serverDoc.GetLinkedServerItem(nullptr) != nullptr) {
        std::printf("FAIL: new COleServerDoc should not have server items\n");
        return 1;
    }

    {
        COleServerItem serverItem(&serverDoc);
        if (serverItem.GetDocument() != &serverDoc || serverItem.m_pDocument != &serverDoc) {
            std::printf("FAIL: COleServerItem constructor did not attach to server doc\n");
            return 1;
        }
        if (serverDoc.GetEmbeddedServerItem() != &serverItem ||
            serverDoc.GetLinkedServerItem(nullptr) != &serverItem ||
            serverDoc.GetLinkedServerItem(L"AnyName") != &serverItem) {
            std::printf("FAIL: COleServerDoc server item lookup mismatch\n");
            return 1;
        }
    }

    if (serverDoc.GetEmbeddedServerItem() != nullptr || serverDoc.GetLinkedServerItem(nullptr) != nullptr) {
        std::printf("FAIL: COleServerItem destructor did not detach from server doc\n");
        return 1;
    }
    if (serverDoc.IsModified()) {
        std::printf("FAIL: new COleServerDoc should start unmodified\n");
        return 1;
    }
    serverDoc.NotifyChanged();
    if (!serverDoc.IsModified()) {
        std::printf("FAIL: COleServerDoc::NotifyChanged should set modified\n");
        return 1;
    }
    serverDoc.NotifySaved();
    if (serverDoc.IsModified()) {
        std::printf("FAIL: COleServerDoc::NotifySaved should clear modified\n");
        return 1;
    }
    serverDoc.NotifyRename(nullptr);
    if (serverDoc.GetTitle()[0] != L'\0') {
        std::printf("FAIL: COleServerDoc::NotifyRename(nullptr) should clear title\n");
        return 1;
    }

    {
        COleServerItem serverItemA(&serverDoc);
        COleServerItem serverItemB(&serverDoc);
        if (serverDoc.GetLinkedServerItem(L"1") != &serverItemA ||
            serverDoc.GetLinkedServerItem(L"2") != &serverItemB ||
            serverDoc.GetLinkedServerItem(L"3") != nullptr ||
            serverDoc.GetLinkedServerItem(L"AnyName") != nullptr) {
            std::printf("FAIL: COleServerDoc linked server item index lookup mismatch\n");
            return 1;
        }
    }

    std::printf("OK: COleDocument/COleClientItem tests passed\n");
    return 0;
}
