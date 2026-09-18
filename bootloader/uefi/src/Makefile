ifndef EFICFLAGS
$(error EFICFLAGS not set)
endif

ifndef CXXFLAGS
$(error CXXFLAGS not set)
endif

LD := lld-link

BUILD ?= ../../../build/uefi
OUT ?= ../../../build/AngOS.efi

OBJECTS := \
	$(BUILD)/main.o \
	$(BUILD)/tosaithe-proto.o \
	$(BUILD)/tosaithe-util.o \
	$(BUILD)/cxxsupport.o

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

$(BUILD)/main.o: main.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/tosaithe-proto.o: tosaithe-proto.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/tosaithe-util.o: tosaithe-util.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/cxxsupport.o: cxxsupport.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXPPFLAGS) $(EFICFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(OUT)
