; filepath: /home/agplaza/Desktop/cos/src/io.asm
global outb

section .text
outb:
    ; Function parameters:
    ;   - port: dx
    ;   - val: al
    mov dx, [esp + 4]  ; Load the port number into dx
    mov al, [esp + 8]  ; Load the value into al
    out dx, al         ; Output the value to the port
    ret