#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/IR/Instructions.h>

using namespace llvm;

namespace {

struct SkeletonPass : public PassInfoMixin<SkeletonPass> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        for (auto &function : M.functions()) {
            for (auto &block : function) {
                errs() << "I saw a block called \"" << block.getName() << "\"!\n";
                for (auto &instruction : block) {
                    errs() << "I saw an instruction with opcode \"" << instruction.getOpcodeName() << "\"!\n";
                    if (auto *storeOp = dyn_cast<StoreInst>(&instruction)) {
                        errs() << "  Got a store instruction" << "\n";
                        // auto ptrOperand = storeOp.get



                    }

                }
            }

        }
        return PreservedAnalyses::all();
    };
};

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "Skeleton pass",
        .PluginVersion = "v0.1",
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                    MPM.addPass(SkeletonPass());
                });
        }
    };
}
