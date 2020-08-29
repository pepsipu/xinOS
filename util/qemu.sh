make && qemu-system-i386 -cdrom ./build/xinOS.iso -s -machine q35 -net nic,model=rtl8139 -nic mac=02:ca:fe:f0:0d:01 -monitor stdio -d int -no-reboot $@
