; When you turn on the computer, it loads the BIOS from some special flash memory.The BIOS runs self
; test and inialisation routines of the hardware, and then it looks for bootable devices. If it finds
; one, the control is transferred to its bootloader, which is a small portion of executable code 
; stored at the device's beginning. The bootloader has to determine the location of the kernel image 
; on the device and load it into memory. It also needs to switch the CPU to the so-called protected 
; mode because x86 CPUs start in the very limited real mode by default.

; Here, we will use GRUB as our bootloader. It is a popular bootloader for Linux and other Unix-like
; operating systems. It is capable of loading a kernel image into memory and passing some information
; to it. This information is stored in a special structure called the multiboot header. The multiboot
; header is a structure that is placed at the beginning of the kernel image. It contains information
; about the kernel image, such as its size, the memory map, the command line, and so on. The bootloader
; fills this structure and passes it to the kernel.

; To build assembly, use 'nasm' software.


; A program typically has 3 sections:
;   .data: Declare static memory regions with initialized values
;   .bss: Declare uninitialized memory regions
;   .text: Declare the code
; However, a bootloader program has an additional section:
;   .multiboot_header: Declare the multiboot header
section .multiboot_header
header_start:
    ;; 'dd' (Define Doubleword) instruction assembles int32 (4 bytes) constants
    dd 0xe85250d6                   ; Magic number (multiboot 2 specification https://nongnu.askapache.com/grub/phcoder/multiboot.pdf)
    dd 0x0                          ; Architecture 0 (protected mode i386)
    dd header_end - header_start    ; Header length
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start)) ; Checksum

    ;; 'dw' (Define quadWord) instruction assembled int64 (8 bytes) constants
    dw 0x0                          ; Type 0 (end of header)
    dw 0x0                          ; Flags
    dw 0x8                          ; Size
header_end: