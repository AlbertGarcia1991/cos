section .text
global outb

; void outb(unsigned short port, unsigned char val)
outb:
    ; Arguments:
    ; port - [esp + 4]
    ; val  - [esp + 8]

    mov edx, [esp + 4]  ; Load port into edx
    mov al, [esp + 8]   ; Load value into al
    out dx, al          ; Output value to port
    ret                 ; Return from function