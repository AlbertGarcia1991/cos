arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso
target ?= $(arch)-cos
linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg

NASM_FLAGS = -felf64
C_FLAGS = -nostdlib -nostartfiles -nodefaultlibs -fno-builtin -m64 -ffreestanding -Iinclude -Wall -Wextra -Werror -std=gnu99 -O2
LINKER_FLAGS = -nostdlib

assembly_source_files := $(wildcard src/arch/$(arch)/*.asm src/asm/*.asm)
assembly_object_files := $(patsubst %.asm, build/%.o, $(assembly_source_files))

c_source_files := $(wildcard src/*.c)
c_object_files := $(patsubst %.c, build/%.o, $(c_source_files))

.PHONY: all run clean kernel iso build

all: $(kernel) run

$(kernel): $(assembly_object_files) $(c_object_files)
	ld -n -T $(linker_script) -o $@ $^ $(LINKER_FLAGS)

build/%.o: %.asm
	mkdir -p $(dir $@)
	nasm $(NASM_FLAGS) $< -o $@

build/%.o: %.c
	mkdir -p $(dir $@)
	gcc -c $< -o $@ $(C_FLAGS)

$(iso): $(kernel) $(grub_cfg)
	mkdir -p build/isofiles/boot/grub
	cp $(kernel) build/isofiles/boot/kernel.bin
	cp $(grub_cfg) build/isofiles/boot/grub
	grub-mkrescue -o $@ build/isofiles 2> /dev/null

run: $(iso)
	qemu-system-$(arch) -drive format=raw,file=$(iso)

clean:
	rm -rf build