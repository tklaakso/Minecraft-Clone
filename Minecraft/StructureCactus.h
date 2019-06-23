#pragma once

#include "SingularStructure.h"

namespace Structure {

	class StructureCactus : public SingularStructure
	{
	public:
		StructureCactus();
		bool canSpawn(int x, int z) override;
		~StructureCactus();
	};

};