# OpenMFC

OpenMFC is a clean-room, binary-compatible reimplementation of the
Microsoft Foundation Class (MFC) runtime (e.g. `mfc140u.dll`, `atl140.dll`).

This repository is structured in phases. **Phase 0A** is about ABI discovery:
we build a tiny MSVC DLL in CI and dump its exports and symbols to learn how
MSVC lays out a simple C++ class.

See `docs/abi_reference.md` for ongoing notes.
