CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
LIBFILES = $(wildcard uspi/lib/*.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCPATH = ~/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin

all: clean kernel8.img

boot.o: boot.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c boot.S -o boot.o

%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES) $(LIBFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib boot.o $(OFILES) $(LIBFILES) uspi/lib/libuspi.a -T link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true

include uspi/Rules.mk