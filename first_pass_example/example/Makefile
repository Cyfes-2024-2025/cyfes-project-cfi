SOURCES        := test.c
SOURCES_PLUGIN := ../skeleton/Skeleton.cpp
OUT_BIN        := test.out
OUT_BIN_ARM    := test.arm.out

UNAME := $(shell uname)
.PHONY: $(OUT_BIN) arm

ifeq ($(UNAME), Linux)
PLUGIN      := SkeletonPass.so
else
# NOTE: This assumes that if you're not using linux, you're on a Mac
PLUGIN      := SkeletonPass.dylib
endif


arm: $(SOURCES) $(PLUGIN)
	clang -fpass-plugin=./$(PLUGIN) \
        --target=aarch64-linux-gnu \
        --sysroot=/usr/aarch64-linux-gnu \
        -march=armv8.3a \
        $(SOURCES) -o $(OUT_BIN_ARM)

$(OUT_BIN): $(SOURCES) $(PLUGIN)
	clang -fpass-plugin=$(shell ls ./SkeletonPass.*) $(SOURCES) -o $(OUT_BIN)

$(PLUGIN): ../build/Makefile $(SOURCES_PLUGIN)
	$(MAKE) -C ../build
	cp ../build/skeleton/$(PLUGIN) ./

# Somewhat janky but it'll work 
../build/Makefile:
	cd ..; mkdir build; cd build; cmake ..
	

