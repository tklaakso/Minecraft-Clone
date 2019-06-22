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

std::vector<Region*> RegionManager::getRegions() {
	return regions;
}

Region* RegionManager::getRegion(int x, int z) {
	for (int i = 0; i < regions.size(); i++) {
		if (regions[i]->getX() == x && regions[i]->getZ() == z) {
			return regions[i];
		}
	}
	return NULL;
}

void RegionManager::addRegion(Region* region) {
	regions.push_back(region);
}

Region* RegionManager::removeRegion(int x, int z) {
	Region* r = NULL;
	for (int i = 0; i < regions.size(); i++) {
		if (regions[i]->getX() == x && regions[i]->getZ() == z) {
			r = regions[i];
			regions.erase(regions.begin() + i);
		}
	}
	return r;
}

RegionManager::~RegionManager()
{
}
