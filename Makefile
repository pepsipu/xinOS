BUILD=build
CC=gcc
CFLAGS=-I src -std=gnu99 -ffreestanding -Wall -Wextra -m32 -fno-pic -g -mgeneral-regs-only -mno-red-zone -fno-stack-protector -masm=intel
AS=nasm
LD=gcc
LDFLAGS=-T link.ld -ffreestanding -nostdlib -m32

CSOURCES=$(shell find . -type f -iname '*.c')
COBJECTS=$(foreach x, $(basename $(CSOURCES)), $(x).o)
ASMSOURCES=$(shell find . -type f -iname '*.asm')
ASMOBJECTS=$(foreach x, $(basename $(ASMSOURCES)), $(x).asm.o)
NAME=xinOS
BIN=$(NAME).bin
ISONAME=$(NAME).iso

OBJS=$(COBJECTS) $(ASMOBJECTS)

.PHONY: clean mkdirs $(BIN) iso

all: mkdirs $(BIN) iso

mkdirs:
	echo $(ASMOBJECTS)
	echo $(ASMSOURCES)
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/iso/boot/grub

iso:
	cp grub.cfg $(BUILD)/iso/boot/grub
	cp $(BUILD)/$(BIN) $(BUILD)/iso/boot
	grub-mkrescue -o $(BUILD)/$(ISONAME) $(BUILD)/iso

$(BIN): $(COBJECTS) $(ASMOBJECTS) 
	$(LD) -o $(BUILD)/$@ $(OBJS) $(LDFLAGS)

clean:
	rm $(OBJS)
	rm -r $(BUILD)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.asm.o: %.asm
	$(AS) $< -I . -f elf32 -g -o $@