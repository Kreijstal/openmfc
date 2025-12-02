# SEH / EH Notes (Phase 0A)

* MSVC build (abi_stress.dll) functions are trivial; disassembly shows no SEH prologues/epilogues in current probes.
* MinGW capability test: `Shim_SafeException` throws/catches internally and returns 42; host observes success, implying no SEH frame corruption when exceptions stay inside the DLL.
* To probe cross-frame EH/SEH behavior, add a function that throws across the boundary and expect failure; currently out of scope.
