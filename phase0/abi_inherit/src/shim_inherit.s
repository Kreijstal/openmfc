.intel_syntax noprefix

.text

.extern GetProcessHeap
.extern HeapAlloc
.extern HeapFree

.section .rdata,"dr"
.align 8
.global "??_7Base@@6B@"
"??_7Base@@6B@":
    .quad "??_GBase@@QEAA@PEAXI@Z"
    .quad "?Get@Base@@QEBAHXZ"
    .quad "?Set@Base@@QEAAXH@Z"

.align 8
.global "??_7Derived@@6B@"
"??_7Derived@@6B@":
    .quad "??_GDerived@@QEAA@PEAXI@Z"
    .quad "?Get@Derived@@QEBAHXZ"
    .quad "?Set@Derived@@QEAAXH@Z"

.align 8
.global "??_7Base1@@6B@"
"??_7Base1@@6B@":
    .quad "??_GBase1@@QEAA@PEAXI@Z"
    .quad "?Get1@Base1@@QEBAHXZ"
    .quad "?Set1@Base1@@QEAAXH@Z"

.align 8
.global "??_7Base2@@6B@"
"??_7Base2@@6B@":
    .quad "??_GBase2@@QEAA@PEAXI@Z"
    .quad "?Get2@Base2@@QEBAHXZ"
    .quad "?Set2@Base2@@QEAAXH@Z"

.align 8
.global "??_7DerivedMI@@6B@"
"??_7DerivedMI@@6B@":
    .quad "??_GDerivedMI@@QEAA@PEAXI@Z"
    .quad "?Get1@DerivedMI@@QEBAHXZ"
    .quad "?Set1@DerivedMI@@QEAAXH@Z"

.align 8
.global "??_7DerivedMI@@6B@Base2@"
"??_7DerivedMI@@6B@Base2@":
    .quad "??_EDerivedMI@@QEAA@PEAXI@Z"   # adjustor delete? reuse same dtor
    .quad "?Get2@DerivedMI@@QEBAHXZ"
    .quad "?Set2@DerivedMI@@QEAAXH@Z"

.text

# Helpers
.macro heap_alloc size:req
    sub rsp, 0x20
    call GetProcessHeap
    test rax, rax
    jz 1f
    mov rcx, rax
    xor edx, edx
    mov r8, \size
    call HeapAlloc
1:
    add rsp, 0x20
.endm

.macro heap_free ptr:req
    sub rsp, 0x20
    call GetProcessHeap
    test rax, rax
    jz 1f
    mov rcx, rax
    xor edx, edx
    mov r8, \ptr
    call HeapFree
1:
    add rsp, 0x20
.endm

# Base ctor: RCX=this
.global "??0Base@@QEAA@XZ"
"??0Base@@QEAA@XZ":
    lea rax, ["??_7Base@@6B@" + rip]
    mov [rcx], rax
    mov dword ptr [rcx + 8], 1
    mov rax, rcx
    ret

# Base dtor
.global "??1Base@@QEAA@XZ"
"??1Base@@QEAA@XZ":
    mov rax, rcx
    ret

.global "??_GBase@@QEAA@PEAXI@Z"
"??_GBase@@QEAA@PEAXI@Z":
    push rbx
    sub rsp, 0x20
    mov rbx, rcx
    call "??1Base@@QEAA@XZ"
    test dl, 1
    jz 1f
    heap_free rbx
1:
    mov rax, rbx
    add rsp, 0x20
    pop rbx
    ret

.global "?Get@Base@@QEBAHXZ"
"?Get@Base@@QEBAHXZ":
    mov eax, dword ptr [rcx + 8]
    ret

.global "?Set@Base@@QEAAXH@Z"
"?Set@Base@@QEAAXH@Z":
    mov dword ptr [rcx + 8], edx
    ret

# Derived ctor
.global "??0Derived@@QEAA@XZ"
"??0Derived@@QEAA@XZ":
    lea rax, ["??_7Derived@@6B@" + rip]
    mov [rcx], rax
    mov dword ptr [rcx + 8], 2
    mov rax, rcx
    ret

.global "??1Derived@@QEAA@XZ"
"??1Derived@@QEAA@XZ":
    mov rax, rcx
    ret

.global "??_GDerived@@QEAA@PEAXI@Z"
"??_GDerived@@QEAA@PEAXI@Z":
    push rbx
    sub rsp, 0x20
    mov rbx, rcx
    call "??1Derived@@QEAA@XZ"
    test dl, 1
    jz 1f
    heap_free rbx
1:
    mov rax, rbx
    add rsp, 0x20
    pop rbx
    ret

.global "?Get@Derived@@QEBAHXZ"
"?Get@Derived@@QEBAHXZ":
    mov eax, dword ptr [rcx + 8]
    add eax, 10
    ret

.global "?Set@Derived@@QEAAXH@Z"
"?Set@Derived@@QEAAXH@Z":
    sub edx, 10
    mov dword ptr [rcx + 8], edx
    ret

# Base1
.global "??0Base1@@QEAA@XZ"
"??0Base1@@QEAA@XZ":
    lea rax, ["??_7Base1@@6B@" + rip]
    mov [rcx], rax
    mov dword ptr [rcx + 8], 11
    mov rax, rcx
    ret

.global "??1Base1@@QEAA@XZ"
"??1Base1@@QEAA@XZ":
    mov rax, rcx
    ret

.global "??_GBase1@@QEAA@PEAXI@Z"
"??_GBase1@@QEAA@PEAXI@Z":
    push rbx
    sub rsp, 0x20
    mov rbx, rcx
    call "??1Base1@@QEAA@XZ"
    test dl, 1
    jz 1f
    heap_free rbx
1:
    mov rax, rbx
    add rsp, 0x20
    pop rbx
    ret

.global "?Get1@Base1@@QEBAHXZ"
"?Get1@Base1@@QEBAHXZ":
    mov eax, dword ptr [rcx + 8]
    ret

.global "?Set1@Base1@@QEAAXH@Z"
"?Set1@Base1@@QEAAXH@Z":
    mov dword ptr [rcx + 8], edx
    ret

# Base2
.global "??0Base2@@QEAA@XZ"
"??0Base2@@QEAA@XZ":
    lea rax, ["??_7Base2@@6B@" + rip]
    mov [rcx], rax
    mov dword ptr [rcx + 8], 22
    mov rax, rcx
    ret

.global "??1Base2@@QEAA@XZ"
"??1Base2@@QEAA@XZ":
    mov rax, rcx
    ret

.global "??_GBase2@@QEAA@PEAXI@Z"
"??_GBase2@@QEAA@PEAXI@Z":
    push rbx
    sub rsp, 0x20
    mov rbx, rcx
    call "??1Base2@@QEAA@XZ"
    test dl, 1
    jz 1f
    heap_free rbx
1:
    mov rax, rbx
    add rsp, 0x20
    pop rbx
    ret

.global "?Get2@Base2@@QEBAHXZ"
"?Get2@Base2@@QEBAHXZ":
    mov eax, dword ptr [rcx + 8]
    ret

.global "?Set2@Base2@@QEAAXH@Z"
"?Set2@Base2@@QEAAXH@Z":
    mov dword ptr [rcx + 8], edx
    ret

# DerivedMI layout: Base1 at 0, Base2 at +16
.global "??0DerivedMI@@QEAA@XZ"
"??0DerivedMI@@QEAA@XZ":
    lea rax, ["??_7DerivedMI@@6B@" + rip]
    mov [rcx], rax
    mov dword ptr [rcx + 8], 111
    lea rax, ["??_7DerivedMI@@6B@Base2@" + rip]
    mov [rcx + 16], rax
    mov dword ptr [rcx + 24], 222
    mov rax, rcx
    ret

.global "??1DerivedMI@@QEAA@XZ"
"??1DerivedMI@@QEAA@XZ":
    mov rax, rcx
    ret

.global "??_GDerivedMI@@QEAA@PEAXI@Z"
"??_GDerivedMI@@QEAA@PEAXI@Z":
    push rbx
    sub rsp, 0x20
    mov rbx, rcx
    call "??1DerivedMI@@QEAA@XZ"
    test dl, 1
    jz 1f
    heap_free rbx
1:
    mov rax, rbx
    add rsp, 0x20
    pop rbx
    ret

# Adjustor for second vptr delete
.global "??_EDerivedMI@@QEAA@PEAXI@Z"
"??_EDerivedMI@@QEAA@PEAXI@Z":
    sub rcx, 16
    jmp "??_GDerivedMI@@QEAA@PEAXI@Z"

.global "?Get1@DerivedMI@@QEBAHXZ"
"?Get1@DerivedMI@@QEBAHXZ":
    mov eax, dword ptr [rcx + 8]
    add eax, 1
    ret

.global "?Set1@DerivedMI@@QEAAXH@Z"
"?Set1@DerivedMI@@QEAAXH@Z":
    sub edx, 1
    mov dword ptr [rcx + 8], edx
    ret

.global "?Get2@DerivedMI@@QEBAHXZ"
"?Get2@DerivedMI@@QEBAHXZ":
    mov eax, dword ptr [rcx + 8]
    add eax, 2
    ret

.global "?Set2@DerivedMI@@QEAAXH@Z"
"?Set2@DerivedMI@@QEAAXH@Z":
    sub edx, 2
    mov dword ptr [rcx + 8], edx
    ret

# Factories
.global CreateBase
CreateBase:
    heap_alloc 16
    test rax, rax
    jz 1f
    mov rcx, rax
    call "??0Base@@QEAA@XZ"
1:  ret

.global CreateDerived
CreateDerived:
    heap_alloc 16
    test rax, rax
    jz 1f
    mov rcx, rax
    call "??0Derived@@QEAA@XZ"
1:  ret

.global CreateDerivedMI
CreateDerivedMI:
    heap_alloc 32
    test rax, rax
    jz 1f
    mov rcx, rax
    call "??0DerivedMI@@QEAA@XZ"
1:  ret

.global DestroyBase
DestroyBase:
    mov edx, 1
    jmp "??_GBase@@QEAA@PEAXI@Z"

.global DestroyDerived
DestroyDerived:
    mov edx, 1
    jmp "??_GDerived@@QEAA@PEAXI@Z"

.global DestroyDerivedMI
DestroyDerivedMI:
    mov edx, 1
    jmp "??_GDerivedMI@@QEAA@PEAXI@Z"
