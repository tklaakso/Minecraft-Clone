#pragma once

#include <vector>
#include "Block.h"
#include "Structure.h"

class Region
{
public:
	Region(int x, int z);
	void generate();
	~Region();
private:
	std::vector<Block*>** blocks;
	int x, z;
};

