#pragma once

#include <vector>
#include "Region.h"

class RegionManager
{
public:
	RegionManager();
	std::vector<Block*>* getChunkBlocks(int cx, int cz);
	void addRegion(Region* region);
	~RegionManager();
private:
	std::vector<Region*> regions;
};

