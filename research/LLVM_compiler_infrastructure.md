# LLVM compiler infrastructure

[Main documentation](https://llvm.org/doxygen/index.html)

Instead of having a monolith compiler that does all the optimization + translation LLVM is a **modular infrastructure**.
The main idea behind it is the usage of the **LLVM IR**.

The main flow is the following:
1. .c program
2. It's converted into an .ll file which is a LLVM IR file. At this point
2. .ll file (LLVM IR), **here all the optimizations (transformations) happen**. The LLVM compiler will try to apply some
transformations to the IR as many times as possible until it says "i'm done"
3. At this point the **LLVM backend** will translate the platform-generic IR to a platform-specific assembly code

This architecture enables the reuse of code between languages (for example, I can re-use optimizations written for the
C language also into another language).

## LLVM Codebase map

[Source](https://blog.regehr.org/archives/1453)

Inside the LLVM folder we can find:
- `bindings`: enable LLVM API's to be used from other languages than C++
- `cmake`: build stuff
- `docs`: documentation
- `examples`: source code for the tutorials
- `include`: `llvm-c` contains C bindings, `llvm` contains all the header files defining all the LLV API's
- `projects`: empty by default, it's where some external projects live
- `resources`: the version resource definition used for Windows binaries
- `runtimes`: another spot for external projects
- `test`: it contains tests (in the form of .ll files => you can check that you pass is behaving like expected)
- `tools`: all the other LLVM files are just libraries, in here you can find all the "main" programs for all the tools
- `unittests`: other unit tests
- `utils`: random stuff, probably we don't care about it

### lib folder

It's where most of the useful stuff lives:
- `Analysis`: lots of static analyses (alias analysis. global value numbering). Some analyses are structured as LLVM passes that must be run by the pass manager; others are structured as libraries that can be called directly. An odd member of the analysis family is InstructionSimplify.cpp, which is a transformation, not an analysis.
- `AsmParser`: parse textual IR into memory
- `Bitcode`: serialize IR into the compact format and read it back into RAM
- `CodeGen`: the LLVM target-independent code generator, a framework that LLVM backends fit into and also a bunch of library functions that backends can use. 
- `DebugInfo`: is a library for maintaining mappings between LLVM instructions and source code locations.
- `ExecutionEngine`: It's where the JIT interpreter lives
- `Fuzzer`: coverage-guided fuzzer similar to AFL. Uses LLVM functionality to perform fuzzing of programs that are compiled using LLVM.
- `IR`: big collection of IR-related code, with no obvious unifying theme (dumping IR to the textual format,  upgrading bitcode files created by earlier versions of LLVM, folding constants as IR nodes are created, etc)
- `IRReader, LibDriver, LineEditor`: nobody cares
- `Linker`: The LLVM linker combines multiple modules into a single, larger module.
- `LTO`: Link-time optimization permits compiler optimizations to see through boundaries created by separate compilation
- `MC`: The MC subsystem in LLVM generates machine code directly
- `Object`: Deals with details of object file formats such as ELF.
- `ObjectYAML`:  supports encoding object files as YAML
- `Option`: Command line parsing
- `Passes`: part of the pass manager, which schedules and sequences LLVM passes, taking their dependencies and invalidations into account.
- `ProfileData`: Read and write profile data to support profile-guided optimizations
- `Support`: Miscellaneous support code including APInts (arbitrary-precision integers that are used pervasively in LLVM) and much else.
- `TableGen`: tool that inputs .td files  containing structured data and uses a domain-specific backend to emit C++ code that gets compiled into LLVM. TableGen is used, for example, to take some of the tedium out of implementing assemblers and disassemblers.
- `Target`: processor-specific parts of the backends live here.
- `Transforms`: this is where the middle-end optimizations live. InstCombine is LLVM’s beast of a peephole optimizer. Instrumentation supports sanitizers. ObjCARC supports this. Scalar contains a pile of compiler-textbooky kinds of optimizers. Utils are helper code. Vectorize is LLVM’s auto-vectorizer. 

## LLVM IR

It uses **SSA** (single-static assignment, you can see it as a sort of "infinite number of registers"). 
To observe it in Godbolt, you can use the `-emit-llvm` flag.
Remember that the LLVM IR you read is the result of the blind translation of the C code, so it's not optimized.

LLVM IR in its textual form it's never generated: the IR always exists in RAM and it's dumpable in a textual form just for debugging purposes.
A frontend builds IR by calling into the LLVM APIs, then it runs some optimization passes, and finally it invokes a backend to generate assembly or machine code

TODO

## LLVM Passes

Passes perform the transformations and optimizations that make up the compiler, they build the analysis results that are 
used by these transformations, and they are, above all, a structuring technique for compiler code.

Before starting the development of a pass you need to choose if you want to use the **new pass manager** or the **legacy pass manager**.

A pass is simply something that operates on a **unit of IR** (e.g. Module or Function).
If we are working with the new pass manager, the passes are divided into **Transformation Passes** and **Analysis Passes**:
- **Transformation Passes** are the ones that modify the IR => they will be harder since you modify the IR and the output needs to be equivalent
- **Analysis Passes** are the ones that analyze the IR and emit some high-level information for the unit of IR

When experimenting, the way you convert a C-file into a LLVM IR file is the following command `clang -emit-llvm -S -O0 file_name.c -o file.ll`.
The whole content of the file is what it's called the **IR module**.

For example, given this c-code:
```c
int foo(int a, int b) {
    return a + b;
}
```
The result you obtain is (with `-Ofast`):
```llvm
; ModuleID = 'sum.c'
source_filename = "sum.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx14.0.0"

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone ssp willreturn uwtable
define i32 @foo(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = add nsw i32 %1, %0
  ret i32 %3
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind readnone ssp willreturn uwtable "approx-func-fp-math"="true" "darwin-stkchk-strong-link" "frame-pointer"="all" "min-legal-vector-width"="0" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "probe-stack"="___chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+cx8,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "tune-cpu"="generic" "unsafe-fp-math"="true" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 14, i32 4]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Apple clang version 15.0.0 (clang-1500.3.9.4)"}
```
The whole file it's an example of **IR module**.
This module contains only one function, **`foo`**.
This function is in turn **a unit of IR** and this function in particular is composed of a single **basic block** (because there are no jumps and no branches).
So to recap: modules => functions => basic blocks => instructions.

### OPT

To load the passes we write we'll use `opt`. It takes in input the `.ll` file plus some plugins (= shared objects) and will execute them on the code.
The output of `opt` depends on the type of pass that we are executing: if it's an optimization pass then the output will be inside a new `.ll` file, if instead it's an analysis pass the output will be printed on the terminal.
A cool flag to use is `-dot-cfg` to generate the CFG files.

`opt` is responsible for running the pass-pipeline (aka the list of passes you want to run)

## Relevant files you might be interested in

-  `llvm/include/llvm/IR/PassManager.h`
- `include/llvm/Passes/PassPlugin.h`
- `tools/opt/NewPMDriver.cpp`

## HelloWorld demo
Steps i followed:
1. `mkdir HelloWorld_build`
2. `cd HelloWorld_build`
3. `export LLVM_DIR=export LLVM_DIR=/Users/lorenzo/Developer/cyfes-project-cfi/llvm-project-ptrauth/build`
4. `cmake -G Ninja -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../HelloWorld`
5. `ninja`
6. `$LLVM_DIR/bin/clang -emit-llvm -S -O1 file.c`
7. `$LLVM_DIR/bin/opt -load-pass-plugin libHelloWorld.dylib -passes=hello-world,hello-world -disable-output file.ll`


## Transformation Pass

The idea of the example is to be able to go through the program, check every instruction to find an addition, check if the addition is
between two eight bit numbers and only then replace the addition with another strange (equivalent) formula.
We will be using **IRBuilder** and then once the instruction is built we'll use **ReplaceInstWithInst** to replace the old instruction with the new one. 

Steps to follow for this example (always from the `build` directory):
- `$LLVM_DIR/bin/clang -emit-llvm -S -O1 -isysroot `xcrun --show-sdk-path` ../inputs/input_for_mba.c`
- `$LLVM_DIR/bin/opt -load-pass-plugin lib/libMBAAdd.dylib -passes=mba-add input_for_mba.ll -S -o output_mba.ll`
- `$LLVM_DIR/bin/lli file_non_modificato.ll 1 2 3 4`
- `ehco $?`
- `$LLVM_DIR/bin/lli file_modificato.ll 1 2 3 4`
- `ehco $?`

## Analysis Pass

TODO

- IR files can be directly executed with `lli`
- `opt` can generate useful data like cfg files with `-dot-cfg` or debug pass pipelines with `debug-pass=Structure` and `debug-pass=Executions`