#pragma once
#include "Block.h"
#include "FastNoise.h"
#include "RegionManager.h"

class WorldGenerator
{
public:
	WorldGenerator();
	static Block** getChunkBlocks(int cx, int cy);
	static int getHeight(int x, int z);
	static FastNoise* noise;
	~WorldGenerator();
};

