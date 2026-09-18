TOPDIR := $(CURDIR)

EFICFLAGS := -ffreestanding -fbuiltin -fshort-wchar \
             -march=x86-64 -mno-red-zone \
             -fno-stack-protector -fno-stack-check -fpie

CFLAGS := -g -Os -Wall

CXXFLAGS := $(CFLAGS) \
            -fno-rtti \
            -fno-exceptions \
            -fno-unwind-tables \
            -fno-asynchronous-unwind-tables

CXX := g++
CC := gcc

VIS_HDR := -include $(TOPDIR)/include/visibility-header.h

CXXPPFLAGS := -nostdinc++ \
              -I $(TOPDIR)/include \
              $(VIS_HDR)

OUT ?= $(TOPDIR)/../../build/AngOS.efi

export EFICFLAGS CXXFLAGS CXXPPFLAGS CXX CC OUT

.PHONY: all clean

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean
