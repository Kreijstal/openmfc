#!/usr/bin/env python3
"""Build and run CImageList handle-map runtime checks with an optional native oracle."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent
SOURCES = [
    "phase4/src/core/gdi/CImageList.cpp",
    "phase4/src/core/gdi/CImageListLifetime.cpp",
    "phase4/src/core/collections/CHandleMap.cpp",
    "phase4/src/detail/CHandleMapSupport.cpp",
    "phase4/src/detail/CFixedAllocSupport.cpp",
    "phase4/src/core/collections/CMapPtrToPtrStorage.cpp",
    "phase4/src/core/collections/CPlex.cpp",
    "tests/test_cimagelist_handle_map.cpp",
]


def versioned(paths):
    return sorted(paths, key=lambda p: [int(part) for part in p.name.split(".")])[-1]


def sdk_paths(sdk):
    vc = versioned((sdk / "VC/Tools/MSVC").iterdir())
    kit = versioned((sdk / "Windows Kits/10/Include").iterdir())
    return vc, kit


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="cl" if os.name == "nt" else "clang-cl")
    parser.add_argument("--linker", default="link" if os.name == "nt" else "lld-link")
    parser.add_argument("--sdk", type=Path, default=Path(os.environ.get("MSVC_DIR", "/home/kreijstal/msvc")))
    parser.add_argument("--build", type=Path,
                        default=Path(tempfile.gettempdir()) / "openmfc-cimagelist-handle-map")
    parser.add_argument("--native-dll", type=Path,
                        help="optional pinned mfc140u.dll oracle (timestamp/image are validated)")
    parser.add_argument("--optimize", action="store_true")
    args = parser.parse_args()

    out = args.build.resolve()
    out.mkdir(parents=True, exist_ok=True)
    flags = ["/nologo", "/EHsc", "/MD", "/std:c++17", "/Gy", "/Gw", "/W0",
             "/DUNICODE", "/D_UNICODE", "/DNOMINMAX", "/DOPENMFC_EXPORTS",
             "/D_WIN32_WINNT=0x0601", "/FIwinsock2.h", "/FIobjbase.h",
             "/FIcommdlg.h", "/FIshellapi.h", "/I" + str(ROOT / "include"),
             "/I" + str(ROOT / "phase4/src")]
    link_flags = ["/nologo", "/SUBSYSTEM:CONSOLE", "/OPT:REF",
                  "/OUT:" + str(out / "test_cimagelist_handle_map.exe")]
    if args.optimize:
        flags.append("/O2")
    vc = kit = None
    if args.sdk.is_dir():
        vc, kit = sdk_paths(args.sdk)
        include_dirs = [vc / "include"] + [kit / name for name in ("ucrt", "um", "shared")]
        if "clang-cl" in Path(args.compiler).name:
            for path in include_dirs:
                flags.extend(["-imsvc", str(path)])
        else:
            flags.extend("/I" + str(path) for path in include_dirs)
        link_flags.extend("/LIBPATH:" + str(path) for path in
                          [vc / "lib/x64"] + [args.sdk / "Windows Kits/10/Lib" / kit.name / name / "x64"
                                                for name in ("ucrt", "um")])
    elif os.name != "nt":
        parser.error("--sdk must point to an MSVC/Windows SDK when building off Windows")

    objects = []
    for relative in SOURCES:
        source = ROOT / relative
        obj = out / (relative.removesuffix(".cpp").replace("/", "_") + ".obj")
        subprocess.run([args.compiler, *flags, "/c", str(source), "/Fo" + str(obj)],
                       check=True, cwd=out)
        objects.append(str(obj))
    subprocess.run([args.linker, *link_flags, *objects, "kernel32.lib", "user32.lib", "comctl32.lib"],
                   check=True, cwd=out)

    env = dict(os.environ)
    if vc:
        for name in ("vcruntime140.dll", "vcruntime140_1.dll"):
            candidates = list((args.sdk / "VC/Redist/MSVC").glob("*/x64/Microsoft.VC*.CRT/" + name))
            if candidates:
                shutil.copy2(candidates[-1], out / name)
    native = args.native_dll
    if native is None and vc:
        pinned = args.sdk / "VC/Redist/MSVC/14.51.36231/x64/Microsoft.VC145.MFC/mfc140u.dll"
        native = pinned if pinned.is_file() else None
    if native and native.is_file():
        native_path = native.resolve()
        env["OPENMFC_NATIVE_IMAGELIST_DLL"] = (
            str(native_path) if os.name == "nt" else "Z:" + str(native_path).replace("/", "\\")
        )
    else:
        env.pop("OPENMFC_NATIVE_IMAGELIST_DLL", None)

    executable = out / "test_cimagelist_handle_map.exe"
    if os.name == "nt":
        command = [str(executable)]
    else:
        env.setdefault("WINEDEBUG", "-all")
        env.setdefault("WINEDLLOVERRIDES", "vcruntime140,vcruntime140_1=n")
        command = (["xvfb-run", "-a"] if not env.get("DISPLAY") and shutil.which("xvfb-run") else [])
        command += ["wine", str(executable)]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)


if __name__ == "__main__":
    main()
