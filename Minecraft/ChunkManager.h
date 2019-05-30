#pragma once
#include <vector>
#include <queue>
#include "Chunk.h"
#include "ChunkVAO.h"
class ChunkManager
{
public:
	ChunkManager();
	std::vector<Chunk*> chunks;
	ChunkVAO** VAOs;
	WorldGenerator* generator;
	std::queue<int> freeVAOs;
	int numUsedVAOs;
	~ChunkManager();
};

