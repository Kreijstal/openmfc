; The event sink passes this leaf through DispCallFunc. Replace only RCX
; (the thunk) with the handler object, then tail-call its event function.
; Native mfc140u helper: RVA 0x2b82c0.
PUBLIC openmfc_DHCS_StdCallThunkHelper
.code
openmfc_DHCS_StdCallThunkHelper PROC
    mov rax, rcx
    mov rcx, QWORD PTR [rcx+8]
    jmp QWORD PTR [rax+10h]
openmfc_DHCS_StdCallThunkHelper ENDP
END
