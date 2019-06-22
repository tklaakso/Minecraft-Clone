#pragma once

#include "Structure.h"

namespace Structure {

	class StructureCactus : public Structure
	{
	public:
		StructureCactus(std::string filePath);
		bool canSpawn(int x, int z) override;
		~StructureCactus();
	};

};