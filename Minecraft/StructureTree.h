#pragma once

#include "SingularStructure.h"

namespace Structure {

	class StructureTree : public SingularStructure
	{
	public:
		StructureTree();
		bool canSpawn(int x, int z) override;
		~StructureTree();
	};

};