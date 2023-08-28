#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

namespace {
    struct MorphingPass : public FunctionPass {
        static char ID;
        MorphingPass() : FunctionPass(ID) {}
        bool runOnFunction(Function &F) override {
            LLVMContext &Context = F.getContext();
            IRBuilder<> Builder(Context);

            for (BasicBlock &BB : F) {

                for (Instruction &I : BB) {

                    Value *OriginalResult = &I;
                    Value *Constant42 = ConstantInt::get(Type::getInt32Ty(Context), 42);
                    Value *NewResult = Builder.CreateAdd(OriginalResult, Constant42);
                    I.replaceAllUsesWith(NewResult);
                    I.eraseFromParent();
                }
            }

            return true;
        }
    };
}

char MorphingPass::ID = 0;
static RegisterPass<MorphingPass> X("morphing-pass", "LLVM Pass for Code Morphing", false, false);
static void registerMorphingPass(const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
    PM.add(new MorphingPass());
}
static RegisterStandardPasses
        RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                       registerMorphingPass);
