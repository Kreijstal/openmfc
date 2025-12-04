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
.global OpenMFC_type_info_vftable
OpenMFC_type_info_vftable:
    .quad 0
    .quad 0

.align 8
.global CObject_type_descriptor
CObject_type_descriptor:
    .quad OpenMFC_type_info_vftable
    .quad 0
    .asciz ".?AVCObject@@"
    .align 4

.align 4
.global CObject_base_class_descriptor
CObject_base_class_descriptor:
    .long CObject_type_descriptor@IMGREL
    .long 1                      # numContainedBases (self only)
    .long 0                      # PMD.mdisp
    .long 0                      # PMD.pdisp (no vbtable)
    .long 0                      # PMD.vdisp
    .long 1                      # attributes: has CHD
    .long CObject_class_hierarchy@IMGREL

.align 4
CObject_base_class_array:
    .long CObject_base_class_descriptor@IMGREL

.align 4
.global CObject_class_hierarchy
CObject_class_hierarchy:
    .long 0                      # signature
    .long 0                      # attributes (single inheritance)
    .long 1                      # numBaseClasses
    .long CObject_base_class_array@IMGREL

.align 4
.global CObject_complete_locator
CObject_complete_locator:
    .long 1                      # signature
    .long 0                      # offset
    .long 0                      # cdOffset
    .long CObject_type_descriptor@IMGREL
    .long CObject_class_hierarchy@IMGREL
    .long CObject_complete_locator@IMGREL

.global CObject_rtti_ptr
CObject_rtti_ptr:
    .quad CObject_complete_locator  # vptr[-1] points at COL
.global CObject_vftable

CObject_vftable:
    .quad   CObject_scalar_dtor       # slot 0: scalar deleting destructor
    .quad   CObject_GetRuntimeClass   # slot 1: GetRuntimeClass (no extra dtor slot)
    .quad   CObject_Serialize         # slot 2: Serialize stub

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
    .seh_proc CObject_scalar_dtor
    push rbx
    .seh_pushreg rbx
    push rdi
    .seh_pushreg rdi
    sub rsp, 0x28          # shadow space + alignment
    .seh_stackalloc 0x28
    .seh_endprologue

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
    .seh_endproc

# Vector deleting destructor (treat same as scalar; RDX flags)
.align 16
.global CObject_vector_dtor
CObject_vector_dtor:
    .seh_proc CObject_vector_dtor
    push rsi
    .seh_pushreg rsi
    push rbx
    .seh_pushreg rbx
    sub rsp, 0x20                  # shadow space, keep 16-byte alignment
    .seh_stackalloc 0x20
    .seh_endprologue

    mov rsi, rcx                   # element pointer (user-visible)
    mov rbx, rsi                   # walking pointer
    mov r10d, edx                  # preserve delete flags

    mov r8, [rsi - 8]              # array count cookie
    mov r9, 8                      # cookie size
    test r8, r8
    jnz .Lloop_setup
    mov r8, 1                      # fallback: treat as single element
.Lloop_setup:
    xor edx, edx                   # scalar flags = 0
.Lloop:
    mov rcx, rbx
    call CObject_scalar_dtor
    add rbx, 8                     # sizeof(CObject)
    dec r8
    jnz .Lloop

    test r10b, 1
    jz .Ldone
    mov rcx, rsi
    sub rcx, r9                    # adjust to allocation start (cookie)
    call Impl_CObject_Delete
.Ldone:
    mov rax, rsi

    add rsp, 0x20
    pop rsi
    pop rbx
    ret
    .seh_endproc

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

# Serialize thunk
.align 16
.global CObject_Serialize
CObject_Serialize:
    jmp Impl_CObject_Serialize

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
