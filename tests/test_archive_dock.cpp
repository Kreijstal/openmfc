#define _AFX_NOFORCE_LIBS
#define _ATL_NO_DEFAULT_LIBS
#include <afxwin.h>
#include <afxext.h>
#include <afxpriv.h>
#include <afxadv.h>
#include <afxstat_.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

namespace {
int failures;
void Check(bool condition, const char* message) {
    if (!condition) {
        fprintf(stderr, "%s\n", message);
        ++failures;
    }
}

void Fill(CControlBarInfo& bar) {
    bar.m_nBarID = 0xe810;
    bar.m_bVisible = FALSE;
    bar.m_bFloating = TRUE;
    bar.m_bHorz = FALSE;
    bar.m_bDockBar = TRUE;
    bar.m_pointPos = CPoint(200, 100);
    bar.m_nMRUWidth = 300;
    bar.m_bDocking = TRUE;
    bar.m_uMRUDockID = 0xe81b;
    bar.m_rectMRUDockPos = CRect(10, 20, 100, 120);
    bar.m_dwMRUFloatStyle = 0x80000001;
    bar.m_ptMRUFloatPos = CPoint(40, 50);
    bar.m_arrBarID.Add(0);
    bar.m_arrBarID.Add(0x1e812);
    bar.m_arrBarID.Add(0xe813);
}

void Report(const CControlBarInfo& bar) {
    printf("%u %d %d %d %d %ld %ld %u %d %u %ld %ld %ld %ld %lu %ld %ld %lld",
           bar.m_nBarID, bar.m_bVisible, bar.m_bFloating, bar.m_bHorz, bar.m_bDockBar,
           bar.m_pointPos.x, bar.m_pointPos.y, bar.m_nMRUWidth, bar.m_bDocking, bar.m_uMRUDockID,
           bar.m_rectMRUDockPos.left, bar.m_rectMRUDockPos.top, bar.m_rectMRUDockPos.right,
           bar.m_rectMRUDockPos.bottom, bar.m_dwMRUFloatStyle, bar.m_ptMRUFloatPos.x,
           bar.m_ptMRUFloatPos.y, static_cast<long long>(bar.m_arrBarID.GetCount()));
    for (INT_PTR i = 0; i < bar.m_arrBarID.GetCount(); ++i) printf(" %u", bar.m_arrBarID.GetAt(i));
    printf("\n");
}

void DumpBytes(CMemFile& file) {
    file.SeekToBegin();
    BYTE value;
    while (file.Read(&value, 1)) printf("%02x", unsigned(value));
    printf("\n");
}

struct DockStateProbe : CDockState {
    using CDockState::m_dwVersion;
};

void TestRecords() {
    CControlBarInfo bar;
    Report(bar);
    Fill(bar);
    DockStateProbe state;
    CSize logical = state.GetScreenSize();
    logical.cx *= 2;
    logical.cy *= 2;
    state.SetScreenSize(logical);
    for (DWORD version : {1ul, 2ul}) {
        state.m_dwVersion = version;
        CMemFile file;
        {
            CArchive store(&file, CArchive::store, 128);
            bar.Serialize(store, &state);
            store.Close();
        }
        Check(file.GetLength() == (version == 1 ? 38 : 78), "standalone record size");
        DumpBytes(file);
        const auto length = file.GetLength();
        for (ULONGLONG cut = length; cut > 0; --cut) {
            file.SetLength(cut);
            file.SeekToBegin();
            CControlBarInfo restored;
            restored.m_arrBarID.Add(0x70070070);
            {
                CArchive load(&file, CArchive::load, 128);
                try {
                    restored.Serialize(load, &state);
                    Check(cut == length, "truncated record read succeeded");
                } catch (CArchiveException* error) {
                    Check(cut < length && error->m_cause == CArchiveException::endOfFile,
                          "unexpected archive exception");
                    error->Delete();
                }
            }
            Report(restored);
        }
    }

    Check(CRuntimeClass::FromName("CDockState") == RUNTIME_CLASS(CDockState),
          "dock class registration");
    CDockState saved;
    auto* contained = new CControlBarInfo;
    Fill(*contained);
    saved.m_arrBarInfo.Add(contained);
    CMemFile graph;
    {
        CArchive store(&graph, CArchive::store, 128);
        store << static_cast<CObject*>(&saved) << static_cast<CObject*>(&saved);
        store.Close();
    }
    DumpBytes(graph);
    graph.SeekToBegin();
    {
        CArchive load(&graph, CArchive::load, 128);
        CDockState* restored = nullptr;
        CDockState* repeated = nullptr;
        load >> restored >> repeated;
        Check(restored && restored == repeated && restored->m_arrBarInfo.GetCount() == 1,
              "dock object archive identity");
        if (restored && restored->m_arrBarInfo.GetCount() == 1) {
            auto* record = static_cast<CControlBarInfo*>(restored->m_arrBarInfo[0]);
            Check(record->m_nBarID == contained->m_nBarID && record->m_arrBarID.GetCount() == 3,
                  "dock object archive records");
            Report(*record);
        }
        delete restored;
    }
}

void TestProfile(CWinApp& app) {
    static const wchar_t section[] = L"OpenMFC-Bar-4";
    app.CWinApp::WriteProfileInt(section, L"Stale", 12);
    CControlBarInfo bar;
    Fill(bar);
    Check(bar.SaveState(L"OpenMFC", -4), "save populated profile");
    Check(app.CWinApp::GetProfileInt(section, L"Stale", 99) == 99, "old profile section cleared");
    Check(app.CWinApp::GetProfileInt(section, L"Bar#1", 0) == 0x1e812, "contained bar profile key");
    CDockState state;
    CControlBarInfo restored;
    restored.m_arrBarID.Add(0x12345678);
    Check(restored.LoadState(L"OpenMFC", -4, &state), "load populated profile");
    Check(restored.m_arrBarID.GetCount() == 4 && restored.m_arrBarID[0] == 0x12345678,
          "profile appends contained bar IDs");
    Report(restored);

    bar.m_arrBarID.SetSize(1);
    bar.m_bVisible = TRUE;
    bar.m_bFloating = FALSE;
    bar.m_pointPos = CPoint(-1, -1);
    Check(!bar.SaveState(L"OpenMFC", -4), "skip default dock bar");
    Check(app.CWinApp::GetProfileInt(section, L"BarID", 0) == 0, "skipped record clears old section");
    Check(!restored.LoadState(L"OpenMFC", -4, &state), "load absent bar returns false");
    Report(restored);

    CDockState saved;
    auto* skip = new CControlBarInfo;
    skip->m_bDockBar = TRUE;
    skip->m_bVisible = TRUE;
    saved.m_arrBarInfo.Add(skip);
    auto* keep = new CControlBarInfo;
    Fill(*keep);
    keep->m_bFloating = FALSE;
    saved.m_arrBarInfo.Add(keep);
    saved.SaveState(L"Complete");
    Check(app.CWinApp::GetProfileInt(L"Complete-Summary", L"Bars", 0) == 1,
          "dock summary skips the default record");
    state.LoadState(L"Complete");
    Check(state.m_arrBarInfo.GetCount() == 1, "dock profile loaded one record");
    state.LoadState(L"Complete");
    Check(state.m_arrBarInfo.GetCount() == 1, "dock profile replaces old records");
    if (state.m_arrBarInfo.GetCount() == 1) Report(*static_cast<CControlBarInfo*>(state.m_arrBarInfo[0]));
}
}

int main() {
    static_assert(sizeof(CControlBarInfo) == 0x78 && sizeof(CDockState) == 0x60);
    TestRecords();
#ifdef OPENMFC_NATIVE_DOCK
    wchar_t directory[MAX_PATH], path[MAX_PATH];
    if (!GetTempPathW(_countof(directory), directory) || !GetTempFileNameW(directory, L"OMF", 0, path))
        return 1;
    {
        CWinApp app(L"OpenMFC dock fixture");
        if (app.m_pszProfileName) free(const_cast<wchar_t*>(app.m_pszProfileName));
        app.m_pszProfileName = _wcsdup(path);
        TestProfile(app);
    }
    DeleteFileW(path);
#else
    // The DLL's process-local profile store uses the current app as its key.
    auto* module = AfxGetModuleState();
    auto* previous = module->m_pCurrentWinApp;
    alignas(CWinApp) unsigned char appIdentity[sizeof(CWinApp)]{};
    auto* app = reinterpret_cast<CWinApp*>(appIdentity);
    module->m_pCurrentWinApp = app;
    TestProfile(*app);
    module->m_pCurrentWinApp = previous;
#endif
    printf("Dock archive and profile: %d failures\n", failures);
    return failures ? 1 : 0;
}
