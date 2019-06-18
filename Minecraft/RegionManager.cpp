#include "RegionManager.h"



RegionManager::RegionManager()
{
	Region* region = new Region(0, 0);
	region->generate();
	regions.push_back(region);
}


RegionManager::~RegionManager()
{
}
