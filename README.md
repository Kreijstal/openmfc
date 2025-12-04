# OpenMFC

OpenMFC is a clean-room, binary-compatible reimplementation of the
Microsoft Foundation Class (MFC) runtime (`mfc140u.dll`).

> **Note:** ATL became header-only starting with VS2015 (v140), so there is no
> `atl140.dll` to reimplement.

This repository is structured in phases. **Phase 0A** is about ABI discovery:
we build a tiny MSVC DLL in CI and dump its exports and symbols to learn how
MSVC lays out a simple C++ class.

See `docs/abi_reference.md` for ongoing notes.
