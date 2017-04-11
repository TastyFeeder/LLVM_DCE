#define DEBUG_TYPE "hello"

#include "llvm/Pass.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/Local.h" // llvm::isInstructionTriviallyDead
#include <llvm/Analysis/TargetLibraryInfo.h>
using namespace llvm;


typedef SmallSetVector<Instruction *, 256> InstSet;
typedef SmallSetVector<BasicBlock *, 16> BlockSet;
//std::vector<Instruction> worklist;

namespace
{
  // Hello - The first implementation, without getAnalysisUsage.
	struct Hello : public FunctionPass
	{
		static char ID; // Pass identification, replacement for typeid
		Hello() : FunctionPass(ID) {}
		bool MyDCEInstruction(Instruction *inst,
                              InstSet &work_list,
                              const TargetLibraryInfo *tli) {
    	if (isInstructionTriviallyDead(inst, tli))
        	return false;

    	for (unsigned i=0; i<inst->getNumOperands(); i++) {
        	Value *op = inst->getOperand(i);
			if (op->use_empty())
				errs() <<"--------------------------"<< *op<<"\n";
        	if (Instruction *opi = dyn_cast<Instruction>(op))
            	MyDCEInstruction(opi, work_list, tli);
    	}

    	work_list.remove(inst);
    	return true;
	}
		virtual bool runOnFunction(Function &F)
		{
			InstSet worklist;
			BlockSet useblock;
			bool change = false;
			SmallVector<Instruction *, 16> dead;
			auto *tlip = getAnalysisIfAvailable<TargetLibraryInfoWrapperPass>(); 
    		TargetLibraryInfo *tli = tlip?&tlip->getTLI():nullptr;
			// find main function
			errs() << "Find Function : " <<F.getName() << '\n';
			//only change return value of main funcrion
			//if(F.getName().equals("main") == false) return change;
			for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i)
			{//printi block name and find return block
			
				errs() << "Find Basic Block : " << i->getName() << '\n';
				errs() << "\t" <<" has "<< i->size() << " instructions.\n";
				
				//Find return instruction
				bool usedblock = false;
				for (BasicBlock::iterator ii = i->begin(), ee = i->end(); ii != ee; ++ii)
				{
					
					errs() << *ii ;
					errs() << "  sideeffect: "<< ii->mayHaveSideEffects() ;
					errs() << "  maywriteto memory: "<< ii->mayWriteToMemory();
					errs() << "  maythrow: "<< ii->mayThrow();
					errs() << "\n";
					
					if(!ii->mayHaveSideEffects() && !dyn_cast<ReturnInst>(&*ii) && !dyn_cast<BranchInst>(&*ii) && ii->use_empty() )
						dead.push_back(&*ii);
						//worklist.insert(&*ii);
					//	usedblock = true;
				}
				if(usedblock)
					useblock.insert(&*i);
			}
			/*
			errs()<<"----------may have side effect or it's a return----------\n";
			for(int i = 0; i<worklist.size();i++)
				errs()<< *worklist[i] <<"\n";
			errs()<<"---------------------------------------------------------\n";
			*/
			/*
			while (!worklist.empty()) {
        		Instruction *inst = worklist[0]; 
        		worklist.remove(inst); 
        		if (!MyDCEInstruction(inst, worklist, tli)) 
				{
					dead.push_back(inst); 
				} 
	    	}
			*/
			errs()<<"\n";
			for(int i = 0;i < dead.size();i++){
					errs()<<"all------>"<<dead[i]<<"\n";
				}
			while (!dead.empty()) { 
				Instruction *tmp;
				tmp = dead.front();
				
				errs()<<"dead :"<<*tmp<<"\n";
				dead.erase(dead.begin());
        		tmp->eraseFromParent(); 
				change = true; 
    		}
			errs()<<change<<"\n";
			return change;
   		}
  	};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass");
