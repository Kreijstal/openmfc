/* x64 assembly with MSVC-mangled symbols */
/* We manually create the MSVC-mangled names as assembly labels */

.globl ?AfxThrowMemoryException@@YAXXZ
.globl ?test_function@@YAXXZ

.text

?AfxThrowMemoryException@@YAXXZ:
    /* Stub implementation */
    ret

?test_function@@YAXXZ:
    /* Another test function */
    ret