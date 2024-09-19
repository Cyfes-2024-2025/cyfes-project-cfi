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
