#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include "Chunk.h"
#include "ChunkVAO.h"
#include "ChunkCoords.h"

class World
{
public:
	World();
	~World();
	void render();
	void setBlock(int x, int y, int z, Block* block, bool update, bool reorderNeighbors);
	void updateViewFrustum(ViewFrustum* frustum);
	void updateRendering(Block* block, Chunk* suspect);
	Block* getBlock(int x, int y, int z, Chunk* suspect);
	void reorderBlocks();
	void reorderBlock(Block* block, Chunk* suspect);
	void updateChunkVAOs();
	void makeChunk(int x, int y);
	void deleteChunk(int x, int y);
	void updatePlayerChunkPosition(int chunkX, int chunkY);
	bool chunkExists(int x, int y);
	void updateLoadedChunks();
	int renderDistance;
private:
	int playerChunkX, playerChunkY;
	Chunk* findChunkWithCoords(ChunkCoords* coords, int l, int r);
	WorldGenerator* generator;
	static ChunkCoords blockToChunkCoords(int bx, int bz);
	std::vector<Chunk*> chunks;
	ChunkVAO** VAOs;
	std::queue<int> freeVAOs;
	int numUsedVAOs;
};
