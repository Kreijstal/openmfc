# RTTI Notes (Phase 0A)

Current DLL-level `symbols.txt` does not include RTTI (`??_R*`) entries. With the added object-level dump (`symbols_objects.txt`), RTTI metadata can be inspected per class if needed.

Next actions if deeper RTTI evidence is required:
- Run `dumpbin /SYMBOLS` on object files (already emitted to `symbols_objects.txt` in artifacts).
- Search for patterns like `??_R4ClassName@@6B@` (Complete Object Locator) and type descriptors `??_R0`.
- If still absent, disable optimization on targeted classes or add references that force RTTI emission.
