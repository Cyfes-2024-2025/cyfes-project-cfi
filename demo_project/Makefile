# Disable defaults
.SUFFIXES:
MAKEFLAGS := -rR

.PHONY: clean

CROSS_CC := ../buildroot/output/host/bin/aarch64-buildroot-linux-gnu-gcc
CLANG := ../llvm-project-ptrauth/build/bin/clang

demo: demo.o lib.o
	$(CROSS_CC) demo.o lib.o -o demo

lib.o: lib.c
	$(CROSS_CC) ./lib.c -c

demo.o: demo.c
	$(CLANG) -O1 --target=arm64e

clean:
	rm -r *.o demo
