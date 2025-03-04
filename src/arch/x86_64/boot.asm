global start  ; Make a label public (start is the entry point from LD)

extern long_mode_start

section .text
bits 32  ; Load in protected mode -> 32-bit instructions
start:
    mov esp, stack_top                  ; Set stack pointer (esp register) to the top of the stack

    call check_multiboot            ; Check if the kernel is loaded by a Multiboot compliant bootloader
    call check_cpuid                ; Check if CPUID instruction is supported
    call check_long_mode            ; Check if long mode is supported
    call set_up_page_tables         ; Set up page tables
    call enable_paging              ; Enable paging

    ; Here, we cannot use the 64-bit instructions because we are still in 32-bit mode.
    ; We need to set up the new Global Descriptor Table (http://pages.cs.wisc.edu/~remzi/OSTEP/vm-segmentation.pdf)
    lgdt [gdt64.pointer]
    jmp gdt64.code:long_mode_start

; Error procedure if the CPU does not support any of the needed features.
error:
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    hlt

; Ensure that kernel is loaded by a Multiboot compliant bootloader by checking eax register's magic number
check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    jmp error

; Check if CPUID is supported by attempting to flip the ID bit (21) in FLAGS register. Is so, CPUID is available
; CPUID is an instruction that returns processor identification and feature information to the CPU.
check_cpuid:
    ; We cannot operate with FLAGS register directly, so we need need to load its value into general-purpose such as EAX
    ; Copy FLAGS in to EAX via stack
    pushfd              ; Push FLAGS register to stack
    pop eax             ; Set EAX register to stack value (FLAGS)
    mov ecx, eax        ; Copy to ECX as well for comparing later on
    xor eax, 1 << 21    ; Flip ID bit
    push eax            ; Write stack with EAX value
    popfd               ; Pop the stack and store values into FLAGS register
    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax
    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd
    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error

set_up_page_tables:
    mov eax, p3_table   ; map first P4 entry to P3 table
    or eax, 0b11        ; present + writable
    mov [p4_table], eax
    mov eax, p2_table   ; map first P3 entry to P2 table
    or eax, 0b11        ; present + writable
    mov [p3_table], eax
    mov ecx, 0         ; counter variable. Map each P2 entry to a huge 2MiB page

.map_p2_table:
    ; map ecx-th P2 entry to a huge page that starts at address 2MiB*ecx
    mov eax, 0x200000  ; 2MiB
    mul ecx            ; start address of ecx-th page
    or eax, 0b10000011 ; present + writable + huge
    mov [p2_table + ecx * 8], eax ; map ecx-th entry
    inc ecx            ; increase counter
    cmp ecx, 512       ; if counter == 512, the whole P2 table is mapped
    jne .map_p2_table  ; else map the next entry
    ret

enable_paging:
    ; load P4 to cr3 register (cpu uses this to access the P4 table)
    mov eax, p4_table
    mov cr3, eax
    ; enable PAE-flag in cr4 (Physical Address Extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    ; set the long mode bit in the EFER MSR (model specific register)
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    ret

; Create paging and stack memory region.
; Can go to bss since does not need ot be initialised, so the GRUB will initialize them to 0
section .bss
align 4096  ; ensure alignment of 512*8 = 4096, align to 4096 bytes (page size)
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096
stack_bottom:
    resb 64  ; resb (REServe Byte) reserves 64 bytes of memory
stack_top:

section .rodata
gdt64:
    dq 0 ; 64-bit constant (Define Quad)
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1  ; $ will be replaced with the current address
    dq gdt64