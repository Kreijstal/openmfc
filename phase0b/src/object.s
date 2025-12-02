.intel_syntax noprefix

.text

# External C implementations (object_impl.cpp)
.global Impl_CObject_New
.global Impl_CObject_Delete
.global Impl_CObject_GetRuntimeClass
.global Impl_CObject_IsKindOf

# ------------------------------------------------------------------
# VTABLE DEFINITION (MSVC layout: slot0 scalar deleting dtor, slot1 GetRuntimeClass)
# Exported as ??_7CObject@@6B@ via DEF
# ------------------------------------------------------------------
.section .rdata,"dr"
.align 8
.global CObject_vftable

CObject_vftable:
    .quad   CObject_scalar_dtor       # slot 0: scalar deleting destructor
    .quad   CObject_GetRuntimeClass   # slot 1: GetRuntimeClass (no extra dtor slot)

# ------------------------------------------------------------------
# CODE SECTION
# ------------------------------------------------------------------
.text

# Constructor
.align 16
.global CObject_ctor
CObject_ctor:
    lea rax, [rip + CObject_vftable]
    mov [rcx], rax         # set vptr
    mov rax, rcx           # return this
    ret

# Destructor (non-deleting)
.align 16
.global CObject_dtor
CObject_dtor:
    ret

# Scalar deleting destructor: RCX=this, RDX=flags (bit0 => delete)
.align 16
.global CObject_scalar_dtor
CObject_scalar_dtor:
    push rbx
    push rdi
    sub rsp, 0x28          # shadow space + alignment

    mov rbx, rcx           # save this
    mov rdi, rdx           # save flags

    # call non-deleting dtor
    call CObject_dtor

    # delete if flag bit0 set
    test dil, 1
    jz .Lno_delete
    mov rcx, rbx
    call Impl_CObject_Delete
.Lno_delete:
    mov rax, rbx           # return this

    add rsp, 0x28
    pop rdi
    pop rbx
    ret

# Vector deleting destructor (treat same as scalar; RDX flags)
.align 16
.global CObject_vector_dtor
CObject_vector_dtor:
    # Not present in vtable; keep symbol exported to match MSVC expectations
    jmp CObject_scalar_dtor

# GetRuntimeClass thunk
.align 16
.global CObject_GetRuntimeClass
CObject_GetRuntimeClass:
    jmp Impl_CObject_GetRuntimeClass

# IsKindOf thunk
.align 16
.global CObject_IsKindOf
CObject_IsKindOf:
    jmp Impl_CObject_IsKindOf

# operator new
.align 16
.global CObject_new
CObject_new:
    jmp Impl_CObject_New

# operator delete
.align 16
.global CObject_delete
CObject_delete:
    jmp Impl_CObject_Delete
