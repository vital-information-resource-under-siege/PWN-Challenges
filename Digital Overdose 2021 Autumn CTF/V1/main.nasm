section .text
global _start

section .data
sanity db 0

section .text

_start:
    call get_input
    call exit

get_input:
    sub rsp, 0x8
    mov rax, 0x0
    mov rdi, 0x0
    mov rsi, rsp
    mov rdx, 4096
    syscall
    mov eax, dword [sanity]
    test eax, eax
    jne exit  ; Gotta do this in one shot!
    mov dword [sanity], 1
    mov rax, 0
    mov rbx, 0
    mov rcx, 0
    mov rdx, 0
    mov rsi, 0
    mov r8, 0
    mov r9, 0
    mov r10, 0
    add rsp, 0x8
    ret

exit:
    mov rax, 0x3c
    mov rdi, 0x0
    syscall
    call exit  ; no gadget for you >:(

gadget_1:
    mov rax, 0x6
    ret

gadget_2:
    mov rax, 0x9
    ret

gadget_3:
    mov rdi, rax
    ret

gadget_4:
    mov rax, rdi
    ret

gadget_5:
    mov rsi, rax
    ret

gadget_6:
    mov rax, rsi
    ret

gadget_7:
    mov rdx, rax
    ret

gadget_8:
    mov rax, rdx
    ret

gadget_9:
    imul rdi
    ret

gadget_10:
    add rax, rdi
    ret

gadget_11:
    sub rax, rdi
    ret

gadget_12:
    div rdi
    ret

gadget_13:
    mov byte [rdi], al
    cmp rdi, sanity  ; Nope!
    je exit
    ret

