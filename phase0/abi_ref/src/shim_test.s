.intel_syntax noprefix

.text

.extern malloc
.extern free

# ------------------------------------------------------------------
# VTABLE (MSVC layout: slot0 scalar deleting dtor, slot1 GetValue, slot2 SetValue)
# ------------------------------------------------------------------
.section .rdata,"dr"
.align 8
.global "??_7CReferenceTest@@6B@"
"??_7CReferenceTest@@6B@":
    .quad "??_GCReferenceTest@@QEAA@PEAXI@Z"
    .quad "?GetValue@CReferenceTest@@QEBAHXZ"
    .quad "?SetValue@CReferenceTest@@QEAAXH@Z"

# ------------------------------------------------------------------
# CODE SECTION
# ------------------------------------------------------------------
.text

# Constructor: RCX = this
.align 16
.global "??0CReferenceTest@@QEAA@XZ"
"??0CReferenceTest@@QEAA@XZ":
    lea rax, ["??_7CReferenceTest@@6B@" + rip]
    mov [rcx], rax          # vptr
    mov dword ptr [rcx + 8], 42
    mov rax, rcx
    ret

# Non-deleting destructor: RCX = this
.align 16
.global "??1CReferenceTest@@QEAA@XZ"
"??1CReferenceTest@@QEAA@XZ":
    mov rax, rcx
    ret

# Scalar deleting destructor: RCX=this, RDX=flags (bit0 => delete)
.align 16
.global "??_GCReferenceTest@@QEAA@PEAXI@Z"
"??_GCReferenceTest@@QEAA@PEAXI@Z":
    .seh_proc "??_GCReferenceTest@@QEAA@PEAXI@Z"
    push rbx
    .seh_pushreg rbx
    sub rsp, 0x20
    .seh_stackalloc 0x20
    .seh_endprologue

    mov rbx, rcx
    # call non-deleting dtor (no body)
    call "??1CReferenceTest@@QEAA@XZ"

    test dl, 1
    jz .Lno_delete
    mov rcx, rbx
    call free
.Lno_delete:
    mov rax, rbx

    add rsp, 0x20
    pop rbx
    ret
    .seh_endproc

# GetValue: RCX=this -> EAX=value
.align 16
.global "?GetValue@CReferenceTest@@QEBAHXZ"
"?GetValue@CReferenceTest@@QEBAHXZ":
    mov eax, dword ptr [rcx + 8]
    ret

# SetValue: RCX=this, EDX=value
.align 16
.global "?SetValue@CReferenceTest@@QEAAXH@Z"
"?SetValue@CReferenceTest@@QEAAXH@Z":
    mov dword ptr [rcx + 8], edx
    ret

# Factory: returns new CReferenceTest*
.align 16
.global CreateRef
CreateRef:
    mov ecx, 16                     # sizeof(CReferenceTest)
    call malloc
    test rax, rax
    jz .Lcref_ret
    mov rcx, rax
    call "??0CReferenceTest@@QEAA@XZ"
.Lcref_ret:
    ret

# DestroyRef: RCX = ptr
.align 16
.global DestroyRef
DestroyRef:
    test rcx, rcx
    jz .Ldre_ret
    mov edx, 1
    call "??_GCReferenceTest@@QEAA@PEAXI@Z"
.Ldre_ret:
    ret
