#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <iostream>
#include <string>

using namespace llvm;
using namespace std;

namespace {
    // The struct i'm creating will hereditate from FunctionPass
    struct ExamplePass : public FunctionPass {
        static char ID;
        // We will never explicitly call the constructor, it will be done automatically by LLVM, no need to do anything
        ExamplePass() : FunctionPass(ID) {}

        /**
         * This function that we are overloading will be run on every function that is present in the code we are
         * compiling, that's why it's called runOnFunction
         * Clang while running will take every function in the LLVM IR and will pass it as a Function object to this
         * function
         */
        virtual bool runOnFunction(Function &F) {
            // Inside here there's the actual Pass

            // think of it as a reference to the core LLVM "engine" that you should pass to the various methods that require a LLVMContext.
            LLVMContext &context = F.getContext();
            // We will get the module of the function (which is the larger module this function is a part of)
            auto module = F.getParent();
            // We can now start initializing variables we'll need to use to print both the function name and the number
            // of times it was called
            /**
             * To do so we first need to get the signature of the printf function: in a C program you would simply include
             * the header file that contains the declaration of the printf function, signaling that somewhere there is the
             * definition of the function.
             * In LLVM IR we can't do that, so we need to create a FunctionType object that represents the signature of the
             * printf function, and then we can use the getOrInsertFunction method of the module object to get a reference
             * We are creating a function type for the printf function such that it returns a int and it takes a 8 bit integer
             * pointer type as argument (a char), the last argument is a boolean that signals if the function is variadic - takes a variable number of arguments -
             */
            FunctionType *printfType = FunctionType::get(Type::getInt32T(context), {Type::getInt8PtrTy(context)}, true);
            /**
             * Now we can create the signature with a name: we are telling the module to get or insert a function named printf with
             * printfType signature.
             * What this is going to do is if a signature like the one defined already exists in the module, we will be returned a
             * FunctionCallee object that will allow us to insert a call to this specific FunctionCallee object.
             * If instead it doesn't exist, it will be inserted into the module and passed back to us
             * Effectively this enables us to figure out what printf is and how to call it
             */
            FunctionCallee printfFunction = module->getOrInsertFunction("printf", printfType);

            /**
             * Now that we know what printf is, we can start the data collection to get the function name and the number of
             * times it was called.
             */

            // Getting the function name and casting it to string (getName() returns the name as llvm::StringRef)
            string functionName = F.getName().str();
            // Here we are creating the name for a global variable that will hold the number of times the function has been called
            string functionCallVarName = functionName + "_callCount";
            // Adding the global variable to the module
            GlobablVariable *functionCallCount = module->getGlobalVariable(functionCallVarName);
            if( !functionCallCount ) {
                // If it's a null pointer we need to effectively create the variable
                functionCallCount = new GlobalVariable(*module, Type::getInt32Ty(context), false, GlobalValue::CommonLinkage, 0, functionCallVarName);
                functionCallCount->setInitializer(ConstantInt::get(Type::getInt32Ty(context), 0));
            }

            // Now we can start inserting the code

            /**
             * We are taking a reference to the function we have been passed (F), gettin the first thing in the iterator it's returned (front())
             * and than getting the first thing from the second iterator it's returned (second front() call).
             * Why? Because LLVM code is built of basic blocks. It's constituted of functions which are an iterator of basic blocks and
             * each basic block is an iterator of instructions. We are getting the first basic block of the function and from that the first
             * instruction at the entry point of the function.
             */
            Instruction *firstInstruction = &F.front().front();
            // The IR Builder will allow us to insert instructions right before the first instruction
            IRBuilder<> builder(firstInstruction);

            /**
             * How can we accomplish the printing of the number of times the function was called?
             * We need to do a load-store operation: we get the value of the global variable -> + 1 -> store the new value
             */

            Value *loadedCallCount = builder.CreateLoad(functionCallCount);
            // LLVM-IR is in SSA, we can't simply say "take that register value and add 1"
            Value *addedCallCount = builder.CreateAdd(loadedCallCount, builder.getInt32(1));
            builder.CreateStore(addedCallCount, functionCallCount);

            // Self-explanatory
            string printLog = functionName + "%d\n";
            Value *funcNamePtr = buildeer.CreateGlobalStringPtr(printLog);
            builder.CreateCall(printfFunction, {funcNamePtr, addedCallCount});

            return true;
        }
    };
} // namespace


// Registering the pass with the function pass manager

char RuntimePrintFunctionCallCountPass::ID = 0;
// Remember that the first argument is the name of the flag you need to pass to clang in order for the plugin to be invoked
static RegisterPass<RuntimePrintFunctionCallCountPass> X("example", "Example Pass", false, false);
static void registerRuntimePrintFunctionCallCountPass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
    PM.add(new RuntimePrintFunctionCallCountPass());
}
static RegisterStandardPasses RegisterMyPassLast(PassManagerBuilder::EP_EarlyAsPossible, registerRuntimePrintFunctionCallCountPass);
static RegisterStandardPasses RegisterMyPassEND(PassManagerBuilder::EP_EnabledOnOptLevel0, registerRuntimePrintFunctionCallCountPass);
