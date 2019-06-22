#pragma once

#include <vector>
#include "Block.h"
#include "Structure.h"
#include "FastNoise.h"
#include "StructureData.h"

class Region
{
public:
	Region(int x, int z);
	void generate();
	std::vector<Block*>* getChunkBlocks(int cx, int cz);
	int getX();
	int getZ();
	~Region();
private:
	std::vector<Block*>* blocks[REGION_WIDTH_CHUNKS][REGION_WIDTH_CHUNKS];
	std::vector<StructureData>* structureMap[REGION_WIDTH_CHUNKS][REGION_WIDTH_CHUNKS];
	std::vector<std::vector<StructureData>*> getOverlapping(StructureData d);
	bool canCreate(StructureData d);
	int x, z;
};

