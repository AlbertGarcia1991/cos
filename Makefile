arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso
target ?= $(arch)-cos

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/%.asm, build/arch/$(arch)/%.o, $(assembly_source_files))
c_source_files := $(wildcard src/*.c)

.PHONY: all run

all:
	unset GTK_PATH
	mkdir -p build/

	nasm -felf64 src/arch/x86_64/boot.asm -o build/arch/x86_64/boot.o
	nasm -felf64 src/arch/x86_64/multiboot_header.asm -o build/arch/x86_64/multiboot_header.o
	nasm -felf64 src/arch/x86_64/long_mode_init.asm -o build/arch/x86_64/long_mode_init.o
	nasm -felf64 src/asm/io.asm -o build/arch/x86_64/io.o

	gcc -c src/main.c -o build/main.o -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -m64 -ffreestanding -Iinclude -Wextra -Werror -std=gnu99 -O2
	gcc -c src/vga.c -o build/vga.o -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -m64 -ffreestanding -Iinclude -Wextra -Werror -std=gnu99 -O2
	
	ld -n -T src/arch/x86_64/linker.ld -o build/kernel.bin build/arch/x86_64/boot.o build/arch/x86_64/multiboot_header.o build/arch/x86_64/long_mode_init.o build/main.o build/vga.o build/arch/x86_64/io.o -nostdlib

	mkdir -p build/isofiles/boot/grub
	cp build/kernel.bin build/isofiles/boot/kernel.bin
	cp src/arch/x86_64/grub.cfg build/isofiles/boot/grub
	grub-mkrescue -o build/os-x86_64.iso build/isofiles 2> /dev/null

	qemu-system-x86_64 -drive format=raw,file=$(iso)