# These are set by the parent Makefile.
ifndef EFICFLAGS
$(error EFICFLAGS not set)
endif

ifndef CXXFLAGS
$(error CXXFLAGS not set)
endif

LD := lld-link

SOURCES := main.cc tosaithe-proto.cc tosaithe-util.cc cxxsupport.cc
OBJECTS := main.o tosaithe-proto.o tosaithe-util.o cxxsupport.o

.PHONY: all clean

all: tosaithe.efi

tosaithe.efi: $(OBJECTS)
	MSYS2_ARG_CONV_EXCL="*" $(LD) /machine:x64 /subsystem:efi_application \
	  /entry:EfiMain /nodefaultlib /base:0 /fixed:no \
	  /out:tosaithe.efi \
	  $(OBJECTS)

main.o: main.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c main.cc

tosaithe-proto.o: tosaithe-proto.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c tosaithe-proto.cc

tosaithe-util.o: tosaithe-util.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c tosaithe-util.cc

cxxsupport.o: cxxsupport.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c cxxsupport.cc

clean:
	rm -f $(OBJECTS) tosaithe.efi
