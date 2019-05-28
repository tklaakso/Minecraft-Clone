#pragma once
#include "Block.h"
#include "FastNoise.h"

class WorldGenerator
{
public:
	WorldGenerator();
	Block** getChunkBlocks(int cx, int cy);
	FastNoise* noise;
	~WorldGenerator();
};

