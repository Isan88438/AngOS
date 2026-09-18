ifndef EFICFLAGS
$(error EFICFLAGS not set)
endif

ifndef CXXFLAGS
$(error CXXFLAGS not set)
endif

LD := lld-link

OBJECTS := main.o tosaithe-proto.o tosaithe-util.o cxxsupport.o

OUT ?= ../../../../build/AngOS.efi

.PHONY: all clean

all: $(OUT)

$(OUT): $(OBJECTS)
	mkdir -p $(dir $@)
	MSYS2_ARG_CONV_EXCL="*" $(LD) \
	  /machine:x64 \
	  /subsystem:efi_application \
	  /entry:EfiMain \
	  /nodefaultlib \
	  /base:0 \
	  /fixed:no \
	  /out:$(OUT) \
	  $(OBJECTS)

main.o: main.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

tosaithe-proto.o: tosaithe-proto.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

tosaithe-util.o: tosaithe-util.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

cxxsupport.o: cxxsupport.cc
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
