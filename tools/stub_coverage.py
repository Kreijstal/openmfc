#!/usr/bin/env python3
"""Generate and validate OpenMFC remaining-stub coverage shards.

The manifest is the source of truth. Shards are only a work queue: each current
stub export must appear in exactly one shard so agent work can be parallelized
without losing coverage.
"""

import argparse
import json
import re
import subprocess
from collections import Counter, defaultdict
from pathlib import Path

import gen_manifest


ROOT = Path(__file__).resolve().parent.parent
DEFAULT_OUT = ROOT / "docs" / "stub_coverage"
DEFAULT_MAX_SYMBOLS = 250


FEATURE_PACK_CLASSES = {
    "CAnimationController",
    "CBasePane",
    "CBaseTabbedPane",
    "CDockSite",
    "CDockablePane",
    "CDockingManager",
    "CDockingPanesRow",
    "CFrameImpl",
    "CFrameWndEx",
    "CGlobalUtils",
    "CKeyboardManager",
    "CMDIChildWndEx",
    "CMDIClientAreaWnd",
    "CMDIFrameWndEx",
    "CMFCCaptionBar",
    "CMFCBaseTabCtrl",
    "CMFCButton",
    "CMFCColorBar",
    "CMFCColorPickerCtrl",
    "CMFCMenuBar",
    "CMFCOutlookBarPane",
    "CMFCOutlookBarTabCtrl",
    "CMFCPopupMenu",
    "CMFCPopupMenuBar",
    "CMFCPropertyGridCtrl",
    "CMFCPropertyGridProperty",
    "CMFCPropertyPage",
    "CMFCPropertySheet",
    "CMFCRibbonBar",
    "CMFCRibbonBaseElement",
    "CMFCRibbonButton",
    "CMFCRibbonButtonsGroup",
    "CMFCRibbonCategory",
    "CMFCRibbonGallery",
    "CMFCRibbonPanel",
    "CMFCRibbonPanelMenuBar",
    "CMFCRibbonStatusBar",
    "CMFCShellListCtrl",
    "CMFCShellTreeCtrl",
    "CMFCStatusBar",
    "CMFCTabCtrl",
    "CMFCTasksPane",
    "CMFCToolBar",
    "CMFCToolBarButton",
    "CMFCToolBarComboBoxButton",
    "CMFCToolBarDateTimeCtrl",
    "CMFCToolBarImages",
    "CMFCToolBarsCustomizeDialog",
    "CMFCVisualManager",
    "CMFCVisualManagerOffice2003",
    "CMFCVisualManagerOffice2007",
    "CMFCVisualManagerOfficeXP",
    "CMFCVisualManagerWindows",
    "CMFCVisualManagerWindows7",
    "CMultiPaneFrameWnd",
    "CPane",
    "CPaneContainer",
    "CPaneContainerManager",
    "CPaneDivider",
    "CPaneFrameWnd",
    "CShellManager",
    "CTabbedPane",
    "CTaskDialog",
    "CUserToolsManager",
}

OLE_CLASSES = {
    "CCmdTarget",
    "CDocObjectServer",
    "CEnumFormatEtc",
    "COleBusyDialog",
    "COleChangeIconDialog",
    "COleChangeSourceDialog",
    "COleClientItem",
    "COleControl",
    "COleControlSite",
    "COleDataObject",
    "COleDataSource",
    "COleDispatchDriver",
    "COleDispatchException",
    "COleDocIPFrameWndEx",
    "COleDocObjectItem",
    "COleDocument",
    "COleDropSource",
    "COleDropTarget",
    "COleException",
    "COleFrameHook",
    "COleIPFrameWnd",
    "COleIPFrameWndEx",
    "COleInsertDialog",
    "COleLinkingDoc",
    "COleLinksDialog",
    "COleMessageFilter",
    "COleObjectFactory",
    "COlePasteSpecialDialog",
    "COlePropertiesDialog",
    "COlePropertyPage",
    "COleResizeBar",
    "COleServerDoc",
    "COleServerItem",
    "COleStreamFile",
    "COleTemplateServer",
}

DATABASE_CLASSES = {
    "CDaoDatabase",
    "CDaoException",
    "CDaoFieldExchange",
    "CDaoQueryDef",
    "CDaoRecordset",
    "CDaoRecordView",
    "CDaoTableDef",
    "CDaoWorkspace",
    "CDBException",
    "CDatabase",
    "CFieldExchange",
    "CRecordset",
    "CRecordView",
}

INTERNET_CLASSES = {
    "CFileFind",
    "CFtpConnection",
    "CFtpFileFind",
    "CGopherConnection",
    "CGopherFile",
    "CGopherFileFind",
    "CHttpConnection",
    "CHttpFile",
    "CInternetConnection",
    "CInternetException",
    "CInternetFile",
    "CInternetSession",
}

SOCKET_CLASSES = {"CAsyncSocket", "CSocket", "CSocketFile"}

DOC_FILE_CLASSES = {
    "CArchive",
    "CArchiveException",
    "CDocTemplate",
    "CDocument",
    "CEditView",
    "CFile",
    "CFormView",
    "CListView",
    "CMemFile",
    "CMultiDocTemplate",
    "CScrollView",
    "CSingleDocTemplate",
    "CStdioFile",
    "CTreeView",
    "CView",
}

COLLECTION_STRING_CLASSES = {
    "CByteArray",
    "CDWordArray",
    "CMapPtrToPtr",
    "CMapPtrToWord",
    "CMapStringToOb",
    "CMapStringToPtr",
    "CMapStringToString",
    "CMapWordToOb",
    "CMapWordToPtr",
    "CObArray",
    "CObList",
    "CPtrArray",
    "CStringArray",
    "CStringList",
    "CUIntArray",
}

CORE_WINDOW_CLASSES = {
    "CButton",
    "CCheckListBox",
    "CColorDialog",
    "CComboBox",
    "CDialog",
    "CDialogEx",
    "CFileDialog",
    "CFindReplaceDialog",
    "CFontDialog",
    "CFrameWnd",
    "CListBox",
    "CListCtrl",
    "CMDIChildWnd",
    "CMDIFrameWnd",
    "CMenu",
    "CPageSetupDialog",
    "CPrintDialog",
    "CProgressCtrl",
    "CPropertyPage",
    "CPropertySheet",
    "CScrollBar",
    "CSliderCtrl",
    "CSpinButtonCtrl",
    "CStatic",
    "CTabCtrl",
    "CToolTipCtrl",
    "CTreeCtrl",
    "CWnd",
}

GDI_CLASSES = {
    "CBitmap",
    "CBrush",
    "CClientDC",
    "CDC",
    "CFont",
    "CGdiObject",
    "CImageList",
    "CPaintDC",
    "CPalette",
    "CPen",
    "CPreviewDC",
    "CPreviewView",
    "CRect",
    "CRgn",
    "CRichEditView",
    "CWindowDC",
}

APP_RUNTIME_CLASSES = {
    "CCommandLineInfo",
    "CCriticalSection",
    "CEvent",
    "CException",
    "CFileException",
    "CMutex",
    "CRecentFileList",
    "CRuntimeClass",
    "CSemaphore",
    "CSyncObject",
    "CWinApp",
    "CWinAppEx",
    "CWinThread",
}


PRIMARY_FILES = {
    "feature_pack_ui": ["include/openmfc/afxmfc.h", "phase4/src/cbarcore.cpp", "phase4/src/mfccore.cpp"],
    "ole_com_activex": ["include/openmfc/afxole.h", "phase4/src/olecore.cpp"],
    "global_residuals": ["phase4/src/manual_thunks.cpp", "phase4/src/thunks.cpp", "phase4/scripts/build_phase4.sh"],
    "other_mfc": ["include/openmfc/afxwin.h", "phase4/src/manual_thunks.cpp"],
    "gdi_drawing": ["include/openmfc/afxwin.h", "phase4/src/gdicore.cpp"],
    "core_windows_common_controls": ["include/openmfc/afxwin.h", "phase4/src/wincore.cpp", "phase4/src/dlgcore.cpp", "phase4/src/ctrlcore.cpp"],
    "collections_strings": ["include/openmfc/afx.h", "phase4/src/strcore.cpp", "phase4/src/filecore.cpp"],
    "app_runtime_globals": ["include/openmfc/afxwin.h", "phase4/src/appcore.cpp", "phase4/src/mfc_exceptions.cpp"],
    "docview_files_archive": ["include/openmfc/afxwin.h", "phase4/src/docview.cpp", "phase4/src/filecore.cpp"],
    "database_dao_odbc": ["include/openmfc/afxdb.h", "include/openmfc/afxdao.h", "phase4/src/dbcore.cpp", "phase4/src/daocore.cpp"],
    "internet_wininet": ["include/openmfc/afxinet.h", "phase4/src/inetcore.cpp"],
    "sockets": ["include/openmfc/afxsock.h", "phase4/src/sockcore.cpp"],
}


def git_head():
    return subprocess.check_output(["git", "rev-parse", "--short=12", "HEAD"], cwd=ROOT, text=True).strip()


def git_head_full():
    return subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=ROOT, text=True).strip()


def load_current_manifest():
    exports = gen_manifest.load_exports(ROOT / "mfc_complete_ordinal_mapping.json")
    skips = gen_manifest.load_skip_list(ROOT / "tools" / "thunks_skip.txt")
    comments = gen_manifest.collect_symbol_comments(ROOT / "phase4" / "src")
    manifest = gen_manifest.generate_manifest(exports, comments, skips)
    return manifest, len(exports)


def is_global(entry):
    return not entry.get("class")


def category(entry):
    sym = entry["symbol"]
    cls = entry.get("class") or ""

    if is_global(entry):
        return "global_residuals"
    if cls in FEATURE_PACK_CLASSES or cls.startswith("CMFC") or "AFX_WM_" in sym:
        return "feature_pack_ui"
    if cls in OLE_CLASSES or "Ole" in sym or "OLE" in sym or "XDispatch" in sym or "XOle" in sym:
        return "ole_com_activex"
    if cls in DATABASE_CLASSES or sym.startswith("?RFX_") or sym.startswith("?DFX_"):
        return "database_dao_odbc"
    if cls in INTERNET_CLASSES or any(x in sym for x in ("Internet", "Http", "Ftp", "Gopher")):
        return "internet_wininet"
    if cls in SOCKET_CLASSES or "Socket" in sym:
        return "sockets"
    if cls in DOC_FILE_CLASSES:
        return "docview_files_archive"
    if cls in COLLECTION_STRING_CLASSES or "CStringT" in sym:
        return "collections_strings"
    if cls in CORE_WINDOW_CLASSES:
        return "core_windows_common_controls"
    if cls in GDI_CLASSES or any(x in sym for x in ("Bitmap", "Brush", "Pen", "Font", "Draw", "Paint")):
        return "gdi_drawing"
    if cls in APP_RUNTIME_CLASSES or sym.startswith("?Afx"):
        return "app_runtime_globals"
    return "other_mfc"


def slug(text):
    text = re.sub(r"[^a-z0-9]+", "-", text.lower())
    return text.strip("-") or "misc"


def shard_entries(entries, max_symbols):
    entries = sorted(entries, key=lambda e: (e.get("class") or "", e["ordinal"], e["symbol"]))
    shards = []
    current = []

    for entry in entries:
        if current and len(current) >= max_symbols:
            shards.append(current)
            current = []
        current.append(entry)

    if current:
        shards.append(current)
    return shards


def shard_title(cat, entries, index, total):
    classes = Counter(e.get("class") or "global" for e in entries)
    top = [name for name, _ in classes.most_common(3)]
    if len(top) == 1:
        focus = top[0]
    else:
        focus = ", ".join(top)
    return f"{cat} shard {index:02d}/{total:02d}: {focus}"


def build_coverage(max_symbols):
    manifest, total_exports = load_current_manifest()
    stubs = manifest["stub"]
    by_category = defaultdict(list)
    for entry in stubs:
        by_category[category(entry)].append(entry)

    shards = []
    for cat in sorted(by_category):
        parts = shard_entries(by_category[cat], max_symbols)
        total = len(parts)
        for i, part in enumerate(parts, 1):
            shard_id = f"{slug(cat)}-{i:02d}"
            shards.append({
                "id": shard_id,
                "category": cat,
                "title": shard_title(cat, part, i, total),
                "symbol_count": len(part),
                "primary_files": PRIMARY_FILES.get(cat, []),
                "symbols": [
                    {
                        "symbol": e["symbol"],
                        "class": e.get("class"),
                        "ordinal": e["ordinal"],
                    }
                    for e in part
                ],
            })

    summary = {
        "base_commit": git_head_full(),
        "base_commit_short": git_head(),
        "total_exports": total_exports,
        "real_count": len(manifest["real"]),
        "skip_count": len(manifest["skip"]),
        "stub_count": len(manifest["stub"]),
        "shard_count": len(shards),
        "max_symbols_per_shard": max_symbols,
        "category_counts": dict(sorted(Counter(s["category"] for s in shards).items())),
        "category_symbol_counts": dict(sorted(Counter(category(e) for e in manifest["stub"]).items())),
    }
    return manifest, summary, shards


def write_json(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="ascii") as f:
        json.dump(value, f, indent=2, sort_keys=True)
        f.write("\n")


def write_readme(out_dir, summary, shards):
    lines = [
        "# OpenMFC Stub Coverage",
        "",
        "This directory is generated from `tools/stub_coverage.py`.",
        "It is the work ledger for manifest-scale stub reduction.",
        "",
        f"Base commit: `{summary['base_commit_short']}`",
        f"Total exports: `{summary['total_exports']}`",
        f"Real implementations: `{summary['real_count']}`",
        f"Skip-listed: `{summary['skip_count']}`",
        f"Remaining stubs assigned to shards: `{summary['stub_count']}`",
        f"Shard count: `{summary['shard_count']}`",
        "",
        "Coverage invariant:",
        "",
        "```",
        "union(shard symbols) == current manifest stubs",
        "intersection(any two shard symbol sets) == empty",
        "```",
        "",
        "Validate with:",
        "",
        "```sh",
        "python3 tools/stub_coverage.py validate",
        "```",
        "",
        "Track progress after shard merges with:",
        "",
        "```sh",
        "python3 tools/stub_coverage.py status --by-shard",
        "```",
        "",
        "Agent completion rule for a symbol:",
        "",
        "- `real`: exact `// Symbol:` comment plus ABI-safe implementation that passes the phase4 build.",
        "- `skip`: explicit skip-list entry with a reason for truly compiler-generated or impractical exports.",
        "- `stub`: not completed.",
        "",
        "Shards:",
        "",
        "| Shard | Category | Symbols | Focus |",
        "|---|---|---:|---|",
    ]
    for shard in shards:
        lines.append(
            f"| [`{shard['id']}`](shards/{shard['id']}.json) | "
            f"`{shard['category']}` | {shard['symbol_count']} | {shard['title']} |"
        )
    lines.append("")
    (out_dir / "README.md").write_text("\n".join(lines), encoding="ascii")


def generate(args):
    out_dir = Path(args.out)
    manifest, summary, shards = build_coverage(args.max_symbols)
    if out_dir.exists():
        for path in (out_dir / "shards").glob("*.json"):
            path.unlink()
    write_json(out_dir / f"manifest-{summary['base_commit_short']}.json", manifest)
    write_json(out_dir / "summary.json", summary)
    for shard in shards:
        write_json(out_dir / "shards" / f"{shard['id']}.json", shard)
    write_readme(out_dir, summary, shards)
    validate_dir(out_dir, quiet=False)


def validate_dir(out_dir, quiet):
    summary_path = out_dir / "summary.json"
    if not summary_path.exists():
        raise SystemExit(f"missing {summary_path}")
    summary = json.loads(summary_path.read_text(encoding="ascii"))
    manifest_path = out_dir / f"manifest-{summary['base_commit_short']}.json"
    if not manifest_path.exists():
        raise SystemExit(f"missing {manifest_path}")
    manifest = json.loads(manifest_path.read_text(encoding="ascii"))
    expected = {e["symbol"] for e in manifest["stub"]}

    seen = {}
    duplicates = defaultdict(list)
    shard_count = 0
    for path in sorted((out_dir / "shards").glob("*.json")):
        shard_count += 1
        shard = json.loads(path.read_text(encoding="ascii"))
        for item in shard["symbols"]:
            sym = item["symbol"]
            if sym in seen:
                duplicates[sym].extend([seen[sym], shard["id"]])
            else:
                seen[sym] = shard["id"]

    actual = set(seen)
    missing = sorted(expected - actual)
    extra = sorted(actual - expected)
    if missing or extra or duplicates:
        print(f"coverage validation failed for {out_dir}")
        print(f"expected stubs: {len(expected)}")
        print(f"assigned symbols: {len(actual)}")
        print(f"missing: {len(missing)}")
        print(f"extra: {len(extra)}")
        print(f"duplicates: {len(duplicates)}")
        for label, values in (("missing", missing), ("extra", extra)):
            for sym in values[:20]:
                print(f"{label}: {sym}")
        for sym, ids in list(duplicates.items())[:20]:
            print(f"duplicate: {sym} in {sorted(set(ids))}")
        raise SystemExit(1)

    if shard_count != summary["shard_count"]:
        raise SystemExit(f"summary shard_count={summary['shard_count']} actual={shard_count}")
    if len(expected) != summary["stub_count"]:
        raise SystemExit(f"summary stub_count={summary['stub_count']} actual={len(expected)}")

    if not quiet:
        print(f"coverage validation passed: {len(expected)} stubs assigned once across {shard_count} shards")


def validate(args):
    validate_dir(Path(args.out), quiet=args.quiet)


def load_ledger(out_dir):
    summary_path = out_dir / "summary.json"
    if not summary_path.exists():
        raise SystemExit(f"missing {summary_path}")
    summary = json.loads(summary_path.read_text(encoding="ascii"))
    manifest_path = out_dir / f"manifest-{summary['base_commit_short']}.json"
    if not manifest_path.exists():
        raise SystemExit(f"missing {manifest_path}")
    manifest = json.loads(manifest_path.read_text(encoding="ascii"))
    assigned = {}
    for path in sorted((out_dir / "shards").glob("*.json")):
        shard = json.loads(path.read_text(encoding="ascii"))
        for item in shard["symbols"]:
            assigned[item["symbol"]] = shard["id"]
    return summary, manifest, assigned


def status(args):
    out_dir = Path(args.out)
    validate_dir(out_dir, quiet=True)
    summary, ledger_manifest, assigned = load_ledger(out_dir)
    current_manifest, _ = load_current_manifest()

    current_real = {e["symbol"] for e in current_manifest["real"]}
    current_skip = {e["symbol"] for e in current_manifest["skip"]}
    current_stub = {e["symbol"] for e in current_manifest["stub"]}
    ledger_stubs = {e["symbol"] for e in ledger_manifest["stub"]}

    completed_real = ledger_stubs & current_real
    completed_skip = ledger_stubs & current_skip
    still_stub = ledger_stubs & current_stub
    disappeared = ledger_stubs - current_real - current_skip - current_stub
    unassigned_current = current_stub - set(assigned)

    by_shard = defaultdict(lambda: Counter({"real": 0, "skip": 0, "stub": 0, "gone": 0}))
    for sym in ledger_stubs:
        shard_id = assigned[sym]
        if sym in current_real:
            by_shard[shard_id]["real"] += 1
        elif sym in current_skip:
            by_shard[shard_id]["skip"] += 1
        elif sym in current_stub:
            by_shard[shard_id]["stub"] += 1
        else:
            by_shard[shard_id]["gone"] += 1

    print("=== Stub Coverage Status ===")
    print(f"ledger base:         {summary['base_commit_short']}")
    print(f"ledger stubs:        {len(ledger_stubs)}")
    print(f"completed real:      {len(completed_real)}")
    print(f"completed skip:      {len(completed_skip)}")
    print(f"still stub:          {len(still_stub)}")
    print(f"disappeared:         {len(disappeared)}")
    print(f"unassigned current:  {len(unassigned_current)}")

    if args.by_shard:
        print("")
        print("=== By Shard ===")
        for shard_id in sorted(by_shard):
            c = by_shard[shard_id]
            done = c["real"] + c["skip"]
            print(
                f"{shard_id:34s} done={done:4d} real={c['real']:4d} "
                f"skip={c['skip']:3d} stub={c['stub']:4d} gone={c['gone']:3d}"
            )

    if unassigned_current:
        print("")
        print("Unassigned current stubs:")
        for sym in sorted(unassigned_current)[:50]:
            print(sym)
        raise SystemExit(1)


def main():
    parser = argparse.ArgumentParser(description="Generate/validate OpenMFC stub coverage shards")
    sub = parser.add_subparsers(dest="cmd", required=True)

    gen = sub.add_parser("generate")
    gen.add_argument("--out", default=str(DEFAULT_OUT))
    gen.add_argument("--max-symbols", type=int, default=DEFAULT_MAX_SYMBOLS)
    gen.set_defaults(func=generate)

    val = sub.add_parser("validate")
    val.add_argument("--out", default=str(DEFAULT_OUT))
    val.add_argument("--quiet", action="store_true")
    val.set_defaults(func=validate)

    stat = sub.add_parser("status")
    stat.add_argument("--out", default=str(DEFAULT_OUT))
    stat.add_argument("--by-shard", action="store_true")
    stat.set_defaults(func=status)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
