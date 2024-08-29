# Cross compilation

Host: the machine you are working on.
Target: the machine you are compiling for.
(Cross-compilation since target and host are different)

If we are cross-compiling for like an embedded system everything will be static so there will be no problems.
If instead we are cross-compiling for Linux with shared-objects we need to be sure the pats of those objects are the same.

| Component      | LLVM component             |
| -------------- | -------------------------- |
| C/C++ compiler | clang                      |
| Assembler      | clang integrated assembler |
| Linker         | ld.lld                     |
| Runtime        | compiler-rt                |
| C++ library    | libc++, libc++abi          |

If you are compiling for example on Linux, by default everything will use the gcc equivalent.

## Cross-compiling with clang

### Target triple

Format: `<Arch><Sub-arch>-<Vendor>-<Os>-<Enviroment>`
- `Arch`: the target architecture (aarch64)
- `Sub-arch`: refinement specific to an architecture (e.g: armv7a, but if we are compiling for linux we don't need this)
- `Vendor`: Apple, IBM, ..
- `Os`: Darwin, Linux, ..
- `Enviroment`: the ABI and object file format (android, elf, ..)

> If you don't provide information, the triple will assume a default value

Command example: `clang --target=aarch64-linux-gnu func1.s hello.c -o hello`

### Cose che ho provato
Comando per copiare file veso la vm:
-  scp -P 2222 /Users/lorenzo/test.txt root@localhost:/root
export LLVM_DIR=/Users/lorenzo/Developer/cyfes-project-cfi/llvm-project-ptrauth/build

exec qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -smp 1 -kernel Image -append "rootwait root=/dev/vda console=ttyAMA0" -netdev user,id=eth0,hostfwd=tcp::2222-:22 -device virtio-net-device,netdev=eth0 -drive file=rootfs.ext4,if=none,format=raw,id=hd0 -device virtio-blk-device,drive=hd0  ${EXTRA_ARGS} "$@"


$LLVM_DIR/bin/clang --target=aarch64-linux-gnueabihf --sysroot=/Users/lorenzo/sysroot/impazzisco/host/aarch64-buildroot-linux-gnu/sysroot -L /Users/lorenzo/sysroot/impazzisco/host -L /Users/lorenzo/sysroot/impazzisco/host/opt/ext-toolchain/lib/gcc/aarch64-none-linux-gnu/13.2.1/ -L /Users/lorenzo/sysroot/impazzisco/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib/libc.so -L /Users/lorenzo/sysroot/impazzisco/host/opt/ext-toolchain/lib/gcc/aarch64-none-linux-gnu/13.2.1/crtendS.o -O0 -fuse-ld=lld -v ../test_programs/test1.c -o test_program

-fuse-ld=$LLVM_DIR/bin/ld.lld

$LLVM_DIR/bin/clang --target=aarch64-linux-gnueabihf --sysroot=/Users/lorenzo/sysroot/test/aarch64-buildroot-linux-gnu/sysroot -L /Users/lorenzo/sysroot/test/aarch64-buildroot-linux-gnu/lib -I /Users/lorenzo/sysroot/test/aarch64-buildroot-linux-gnu/sysroot/usr/include/ -L /Users/lorenzo/sysroot/test/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ -O0 -fuse-ld=lld -v ../test_programs/test1.c -o test

- ho copiato i file crtbeginS.o e crtendS.o da ln -s ./opt/ext-toolchain/lib/gcc/aarch64-none-linux-gnu/13.2.1/crtendS.o ./aarch64-buildroot-linux-gnu/sysroot/usr/lib/crtendS.o


$LLVM_DIR/bin/clang --target=aarch64-linux-gnueabihf --sysroot=/Users/lorenzo/sysroot/last-try/aarch64-buildroot-linux-gnu/sysroot -L /Users/lorenzo/sysroot/impazzisco/host -L /Users/lorenzo/sysroot/impazzisco/host/opt/ext-toolchain/lib/gcc/aarch64-none-linux-gnu/13.2.1/ -L /Users/lorenzo/sysroot/impazzisco/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib/libc.so -O0 -fuse-ld=lld -v ../test_programs/test1.c -o test

$LLVM_DIR/bin/clang \
--target=aarch64-linux-gnueabihf \
--gcc-toolchain=/Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
--sysroot=/Users/lorenzo/sysroot/last-try/aarch64-buildroot-linux-gnu/sysroot \
-L /Users/lorenzo/sysroot/last-try/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ \
-L /Users/lorenzo/sysroot/last-try/ \
-O0 \
-fuse-ld=lld \
-v \
../test_programs/test1.c \
-o test

-L /Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ \

$LLVM_DIR/bin/clang \
--target=aarch64-linux-gnu \
--gcc-toolchain=/Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
--sysroot=/Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/ \
-L /Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ \
-l /Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0/crtbeginS.o \
-l /Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0/crtendS.o \
-O0 \
-fuse-ld=lld \
-v \
../test_programs/test1.c \
-o test

$LLVM_DIR/bin/clang \
--target=aarch64-linux-gnu \
--gcc-toolchain=/Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
--sysroot=/Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/ \
-L /Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ \
-L /Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
-O0 \
-fuse-ld=lld \
-v \
../test_programs/test1.c \
-o test



$LLVM_DIR/bin/clang \
--target=aarch64-linux-gnu \
--gcc-toolchain=/Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
--sysroot=/Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/ \
-L /Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ \
-L /Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
-O0 \
-fuse-ld=lld \
-v \
-B /Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
../test_programs/test1.c \
-o test_program


$LLVM_DIR/bin/clang \
--target=aarch64-none-linux-gnu \
--gcc-toolchain=/Users/lorenzo/sysroot/last_try/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
--sysroot=/Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/ \
-L /Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ \
-L /Users/lorenzo/sysroot/last_try/aarch64-buildroot-linux-gnu/sysroot/lib/ \
-O0 \
-fuse-ld=lld \
-v \
../test_programs/test1.c \
-o test_program

$LLVM_DIR/bin/clang \
--target=aarch64-none-linux-gnu \
--gcc-toolchain=/Users/lorenzo/sysroot/aarch64-buildroot-linux-gnu/sysroot/usr/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
--sysroot=/Users/lorenzo/sysroot/aarch64-buildroot-linux-gnu/sysroot/ \
-L /Users/lorenzo/sysroot/aarch64-buildroot-linux-gnu/sysroot/usr/lib/ \
-L /Users/lorenzo/sysroot/aarch64-buildroot-linux-gnu/sysroot/lib/ \
-O0 \
-fuse-ld=lld \
-v \
-B /Users/lorenzo/sysroot/lib/gcc/aarch64-buildroot-linux-gnu/13.3.0 \
../test_programs/test1.c \
-o test_program
