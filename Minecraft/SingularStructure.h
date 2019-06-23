#pragma once

#include <vector>
#include "Block.h"
#include "Structure.h"

namespace Structure {
	class SingularStructure : public Structure
	{
	public:
		SingularStructure(std::string filePath);
		virtual bool canSpawn(int x, int z) = 0;
		std::vector<Block*> generate(int x, int y, int z);
		~SingularStructure();
	private:
		std::vector<Block*> blocks;
	};
};
