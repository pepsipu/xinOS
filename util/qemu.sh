# qemu options:
# -net nic,model=rtl8139
make && qemu-system-i386 -cdrom ./build/xinOS.iso -s -machine q35 $@
