make && qemu-system-i386 -cdrom ./build/xinOS.iso -s -machine q35 -net nic,model=rtl8139 -monitor stdio -d int -no-reboot $@
