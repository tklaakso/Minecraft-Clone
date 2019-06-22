#pragma once

#include "Structure.h"

namespace Structure {

	class StructureSandHut : public Structure
	{
	public:
		StructureSandHut(std::string filePath);
		bool canSpawn(int x, int z) override;
		~StructureSandHut();
	};

};