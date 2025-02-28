ENTRY(start)    /* Entry point where bootloader will jump after loading the kernel */

SECTIONS {
    . = 2M;     /* Load address of the first section to 2Mb (convention for kernels) */

    .boot :
    {
        /* Ensure that the multiboot header is at the beginning of the file */
        KEEP(*(.multiboot_header))
    }

    .text :
    {
        *(.text)
    }
}

/*
To create the ELF file, we compile both asm with '-f elf64'.
Then, we create the ELF file using command 'ld -n -o kernel.bin -T linker.ld multiboot_header.o boot.o'.
Finally, we move the 'kernel.bin' to the 'boot' directory, and we run the cmd to create the ISO:
'grub-mkrescue -o os.iso isofiles'
*/