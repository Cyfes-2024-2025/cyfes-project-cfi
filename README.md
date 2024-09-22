# CyfES Pointer Authentication Project

# How to build

```sh

# Build LLVM
cd llvm-project-ptrauth
mkdir build
cmake -S llvm -B build Ninja \
    -DLLVM_ENABLE_PROJECTS='clang;lld' \
    -DCMAKE_BUILD_TYPE=Debug \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DLLVM_USE_LINKER=lld \
    -DBUILD_SHARED_LIBS=ON
cd build
ninja

cd ../..

# Build QEMU
cd qemu
mkdir build
cd build
../configure --target-list=aarch64-softmmu
make

cd ../..

# Build Buildroot
cd buildroot
make ptrauthproject_defconfig
make

```

## Modified files

- QEMU custom periperhal: [qemu/hw/misc/qarma.c](https://github.com/Cyfes-2024-2025/qemu/blob/main/hw/misc/qarma.c)
- Kernel module: [buildroot/package/ptrauth](https://github.com/Cyfes-2024-2025/buildroot/tree/master/package/ptrauth)
- LLVM Module Pass: [llvm-project-ptrauth/llvm/lib/Transform/Utils/AddPtrauthGlobal.cpp](https://github.com/Cyfes-2024-2025/llvm-project-ptrauth/blob/main/llvm/lib/Transforms/Utils/AddPtrauthGlobal.cpp)
- LLVM Frame Lowering: [llvm-project-ptrauth/llvm/lib/Target/AArch64/AArch64FrameLowering.cpp](https://github.com/Cyfes-2024-2025/llvm-project-ptrauth/blob/main/llvm/lib/Target/AArch64/AArch64FrameLowering.cpp) (Changes start on line 4614)

