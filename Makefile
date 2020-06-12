BUILD=build
CFLAGS=-I src -std=gnu99 -ffreestanding -Wall -Wextra -m32 -fno-pic -g -mno-red-zone -fno-stack-protector
CC=i386-elf-gcc
AS=nasm
LD=i386-elf-gcc
LDFLAGS=-T link.ld -ffreestanding -nostdlib -lgcc -m32

CSOURCES=$(shell find . -type f -iname '*.c')
COBJECTS=$(foreach x, $(basename $(CSOURCES)), $(x).o)
ASMSOURCES=$(shell find . -type f -iname '*.asm')
ASMOBJECTS=$(foreach x, $(basename $(ASMSOURCES)), $(x).asm.o)
BIN=dbos.bin
ISONAME=dbos.iso

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
	rm $(BIN)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.asm.o: %.asm
	$(AS) $< -I . -f elf32 -g -o $@