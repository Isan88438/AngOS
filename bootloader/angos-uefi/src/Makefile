# These should be set from parent directory makefile:
#EFICFLAGS =
#CXXFLAGS =
ifndef EFICFLAGS
$(error EFICFLAGS not set)
endif
ifndef CXXFLAGS
$(error CXXFLAGS not set)
endif

# (Not needed if using primary link method below)
ELF2EFI64 ?= elf2efi64

LD := lld-link
STRIP ?= strip

all: tosaithe.efi

tosaithe.efi: main.o tosaithe-proto.o tosaithe-util.o cxxsupport.o
	MSYS2_ARG_CONV_EXCL="*" $(LD) /machine:x64 /subsystem:efi_application \
	  /entry:EfiMain /nodefaultlib /base:0 /fixed:no \
	  /out:tosaithe.efi \
	  main.o tosaithe-proto.o tosaithe-util.o cxxsupport.o

main.o: main.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c main.cc

tosaithe-proto.o: tosaithe-proto.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c tosaithe-proto.cc

tosaithe-util.o: tosaithe-util.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c tosaithe-util.cc

cxxsupport.o: cxxsupport.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c cxxsupport.cc

config.o: config.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c config.cc

# Everything below here is concerned with building disk images and running in QEMU

disk.img: tosaithe.efi
	rm -f disk.img
	@echo "==="
	@echo "Please create disk image (disk.img). On linux use 'make disk.img-linux'"
	@echo "==="
	@exit 1

# Create "image", a directory used to create a virtual FAT image
image-dir: tosaithe.efi
	mkdir -p image/EFI/boot
	cp tosaithe.efi image/EFI/boot/bootx64.efi
	cp ../doc/tosaithe.conf.example image/tosaithe.conf

# Create a disk image with the tosaithe.efi executable on it (Linux).
# requires mtools and dosfstools. This is an example as alternative to the above
# if you want a "real" disk image.
disk.img-linux: tosaithe.efi
	dd if=/dev/zero of=disk.img bs=512 count=93750 # 48Mb
	/sbin/sfdisk disk.img < sfdisk.input # create partition table ("label") and EFI system partition
	# start=2048 end=93716 sectors=91669
	/usr/sbin/mkfs.fat -F 32 --offset=2048 -S 512 disk.img $$((91669 * 512 / 1024))
	# offset 2048 sectors = 1M
	mmd -i disk.img@@1M ::/EFI
	mmd -i disk.img@@1M ::/EFI/BOOT	
	mcopy -i disk.img@@1M tosaithe.efi ::/EFI/BOOT/bootx64.efi
	mcopy -i disk.img@@1M ../doc/tosaithe.conf.example ::/tosaithe.conf

firmware-check:
	@if [ ! -e OVMF_CODE-pure-efi.fd -o ! -e OVMF_VARS-pure-efi.fd ]; then \
	    echo "!!! OVMF firmware images are missing; please download them to this directory."; \
	    exit 1; \
	fi

run-in-qemu: firmware-check image-dir
	qemu-system-x86_64 -s \
	  -drive if=pflash,format=raw,unit=0,file=OVMF_CODE-pure-efi.fd,readonly=on \
	  -drive if=pflash,format=raw,unit=1,file=OVMF_VARS-pure-efi.fd \
	  -net none  \
	  -drive file=fat:rw:image,media=disk,format=raw \
	  -debugcon file:ovmf-debug.log -global isa-debugcon.iobase=0x402

run-disk-img-qemu: firmware-check disk.img
	qemu-system-x86_64 -s \
	  -drive if=pflash,format=raw,unit=0,file=OVMF_CODE-pure-efi.fd,readonly=on \
	  -drive if=pflash,format=raw,unit=1,file=OVMF_VARS-pure-efi.fd \
	  -net none  \
	  -drive file=disk.img \
	  -debugcon file:ovmf-debug.log -global isa-debugcon.iobase=0x402
	
clean:
	rm -f main.o tosaithe-proto.o tosaithe-util.o config.o tosaithe.efi.so tosaithe.efi disk.img
	rm -rf image
