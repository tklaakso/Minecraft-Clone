#pragma once

#include "Structure.h"

namespace Structure {

	class CompositeStructure : public Structure
	{
	public:
		CompositeStructure(std::vector<std::string> filePaths);
		virtual bool canSpawn(int x, int z) = 0;
		std::vector<Block*> generate(int x, int y, int z);
		~CompositeStructure();
	private:
		std::vector<std::vector<Block*>*> blocks;
	};

}