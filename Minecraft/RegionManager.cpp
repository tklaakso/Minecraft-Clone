#include "RegionManager.h"
#include "Util.h"

RegionManager::RegionManager()
{
}

std::vector<Block*>* RegionManager::getChunkBlocks(int cx, int cz) {
	RegionCoords region = chunkToRegionCoords(cx, cz);
	for (int i = 0; i < regions.size(); i++) {
		if (regions[i]->getX() == region.x && regions[i]->getZ() == region.z) {
			return regions[i]->getChunkBlocks(cx, cz);
		}
	}
	return NULL;
}

void RegionManager::addRegion(Region* region) {
	regions.push_back(region);
}

RegionManager::~RegionManager()
{
}
