#ifndef SSAPHINODEGENERATOR_H
#define SSAPHINODEGENERATOR_H

#include "SSATransformer.h"
#include "General.h"
#include "Architecture.h"

namespace holodec {

	struct SSARegDef{
		HId ssaId;
		HId regId, parentId;
		uint64_t offset, size;
	};
	struct SSAMemDef{
		HId ssaId;
		HId memId;
	};
	
	struct BasicBlockWrapper{
		SSABB* ssaBB;
		
		HList<SSARegDef> outputs;
		
		void print(Architecture* arch);
	};
	
	struct SSAPhiNodeGenerator : public SSATransformer {
		Binary* binary;
		Function* function;
		
		HList<BasicBlockWrapper> bbwrappers;
		
		virtual bool doTransformation (Binary* binary, Function* function);
		
		SSARegDef findRegDef();

		void resolveRegs();

		void addRegDef(HId id, Register* reg, HList<SSARegDef>* list, bool replace);

		SSAArgument getSSAId(BasicBlockWrapper* wrapper, Register* reg);
		SSAArgument getSSAId(BasicBlockWrapper* wrapper, HList<SSARegDef>& defs, Register* reg);
		
		bool handleBBs(BasicBlockWrapper* wrapper, Register* reg, std::vector<std::pair<HId, HId>>& gatheredIds, std::vector<HId>& visitedBlocks);
		bool handleBBs(BasicBlockWrapper* wrapper, Memory* mem, std::vector<std::pair<HId, HId>>& gatheredIds, std::vector<HId>& visitedBlocks);
		
		BasicBlockWrapper* getWrapper(HId id){
			for(BasicBlockWrapper& wrapper : bbwrappers){
				if(wrapper.ssaBB->id == id)
					return &wrapper;
			}
			return nullptr;
		}
		
		void getDominator();
	};

}

#endif // SSAPHINODEGENERATOR_H
