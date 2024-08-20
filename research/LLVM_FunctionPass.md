# LLVM FunctionPass

There are multiple **passes** that will run on the LLVM IR during compilation.
For example, when compiling C code with Clang you use the -Ofast flag, it will cause the Pass Manager to run a series of
Function/Module/Loop (etc) passes to optimize the code. Some examples of the result of these passes could be for example
function inlining, loop unrolling, loop fusion (for example to merge two loops into one maybe reducing the branching)
etcetera. 

Not all passes have to be optimization passes, some of them can be analysis passes, analytics passes, etcetera.
There are also **instrumentation passes** that can be used to add some code to the program to measure the performance of
the program.

## How to compile a pass as a .so file

```bash
clang++ namefile.cpp -fPIC -shared -o name_library.so -fno-rtti
```