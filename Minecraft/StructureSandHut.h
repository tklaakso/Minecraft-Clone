#pragma once

#include "SingularStructure.h"

namespace Structure {

	class StructureSandHut : public SingularStructure
	{
	public:
		StructureSandHut();
		bool canSpawn(int x, int z) override;
		~StructureSandHut();
	};

};