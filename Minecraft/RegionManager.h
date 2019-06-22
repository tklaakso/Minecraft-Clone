#pragma once

#include <vector>
#include "Region.h"

class RegionManager
{
public:
	RegionManager();
	std::vector<Block*>* getChunkBlocks(int cx, int cz);
	void addRegion(Region* region);
	Region* removeRegion(int x, int z);
	std::vector<Region*> getRegions();
	Region* getRegion(int x, int z);
	~RegionManager();
private:
	std::vector<Region*> regions;
};

