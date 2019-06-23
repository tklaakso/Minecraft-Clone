#pragma once

#include "CompositeStructure.h"

namespace Structure {

	class StructureFallTree : public CompositeStructure
	{
	public:
		StructureFallTree();
		bool canSpawn(int x, int z) override;
		~StructureFallTree();
	};

}