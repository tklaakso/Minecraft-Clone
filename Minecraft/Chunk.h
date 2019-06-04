#pragma once
#include "Block.h"
#include "ChunkVAO.h"
#include "ChunkCoords.h"
#include "WorldGenerator.h"
#include "BlockCoords.h"
#include <queue>
#include <GLFW/glfw3.h>

class Chunk
{
public:
	Chunk(int x, int y, ChunkVAO* vao, int vaoIndex);
	Chunk(int x, int y);
	void setVAO(ChunkVAO* vao, int vaoIndex);
	void setBlock(int globalX, int globalY, int globalZ, Block* block, bool update);
	Block* getBlock(int globalX, int globalY, int globalZ);
	bool blockInChunk(int globalX, int globalY, int globalZ);
	void setBlockLocal(int localX, int localY, int localZ, Block* block, bool update);
	void initWithoutGeneration();
	void generate(WorldGenerator* gen);
	void updateNeighbors(Chunk** neighbors);
	void updateVAO();
	int compare(Chunk* other);
	int compare(ChunkCoords* other);
	void reorderBlocks();
	void reorderBlock(Block* block);
	void updateViewFrustum(ViewFrustum* frustum);
	void render();
	ChunkVAO* vao;
	int vaoIndex;
	int x, y;
	glm::vec3* translations;
	int* textures;
	bool generating;
	bool finalizing;
	bool deleting;
	static int chunksInPlay;
	~Chunk();
private:
	bool insideViewFrustum;
	bool shouldUpdateVAO;
	void swapBlockIndices(Block* b1, int index1, Block* b2, int index2);
	Block* blockWithTranslationIndex(int translationIndex);
	int numBlocks;
	int numBlocksRendered;
	Block** blocks;
	Block** translationBlocks;
	queue<int> freeBlocks;
};

