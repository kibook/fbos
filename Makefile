CC=i686-elf-gcc
CFLAGS=-c
AS=i686-elf-as
all: build

build:
	$(AS) boot.s -o boot.o
	$(CC) $(CFLAGS) fbos.c -o fbos.o
	$(CC) -T linker.ld -o fbos.obj -ffreestanding -O2 -nostdlib boot.o fbos.o -lgcc

clean:
	rm -f *.o *.obj
	rm -rf iso *.iso

iso: build
	mkdir -p iso/boot/grub
	cp fbos.obj iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o fbos.iso iso
