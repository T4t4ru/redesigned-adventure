#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/ValueTracking.h> 
#include <llvm/Support/RandomNumberGenerator.h> 

using namespace llvm;

namespace {
    struct MorphingPass : public FunctionPass {
        static char ID;
        MorphingPass() : FunctionPass(ID) {}

   
        MorphingPass(int ConstantToAdd, bool MorphOnlySomeInstructions, int MorphEveryNth)
            : FunctionPass(ID), ConstantToAdd(ConstantToAdd),
              MorphOnlySomeInstructions(MorphOnlySomeInstructions), MorphEveryNth(MorphEveryNth) {}

  
        bool shouldMorphInstruction(Instruction &I) {
            if (!MorphOnlySomeInstructions) {
                return true;
            }
            if (MorphCounter % MorphEveryNth == 0) {
                MorphCounter++;
                return true;
            }
            MorphCounter++;
            return false;
        }

        bool runOnFunction(Function &F) override {
            LLVMContext &Context = F.getContext();
            IRBuilder<> Builder(Context);

            for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                    
                    if (shouldMorphInstruction(I)) {
                        if (auto *BinaryOp = dyn_cast<BinaryOperator>(&I)) {
                         
                            if (isUsedByOthers(I)) {
                                Value *OriginalResult = &I;
                                Value *ConstantToAddValue = ConstantInt::get(Type::getInt32Ty(Context), ConstantToAdd);
                                Value *NewResult = Builder.CreateAdd(OriginalResult, ConstantToAddValue);
                                I.replaceAllUsesWith(NewResult);
                                I.eraseFromParent();
                            }
                        }
                    }
                }
            }

            return true;
        }

 
        bool isUsedByOthers(Instruction &I) {
            return !I.user_empty();
        }

    private:
        int ConstantToAdd;
        bool MorphOnlySomeInstructions;
        int MorphEveryNth;
        int MorphCounter = 1; // Счетчик для определения, следует ли морфить инструкцию
    };
}

char MorphingPass::ID = 0;
static RegisterPass<MorphingPass> X("morphing-pass", "LLVM Pass for Code Morphing", false, false);

static void registerMorphingPass(const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
  
    int ConstantToAdd = 42; 
    bool MorphOnlySomeInstructions = true; 
    int MorphEveryNth = 5;
    PM.add(new MorphingPass(ConstantToAdd, MorphOnlySomeInstructions, MorphEveryNth));
}

static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible, registerMorphingPass);
