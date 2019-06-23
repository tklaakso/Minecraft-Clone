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
	void updateBlockNeighbors();
	void calculateLighting();
	void bake();
	void bakeNeighbors();
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
	int state;
	Chunk** neighbors;
	static int chunksInPlay;
	static const int WAITING_FOR_GENERATE = 0;
	static const int GENERATING = 1;
	static const int WAITING_FOR_FINALIZE = 2;
	static const int FINALIZING = 3;
	static const int RENDERING = 4;
	static const int DELETING = 5;
	~Chunk();
private:
	bool insideViewFrustum;
	bool shouldUpdateVAO;
	void swapBlockIndices(Block* b1, int index1, Block* b2, int index2);
	Block* blockWithTranslationIndex(int translationIndex);
	int numBlocks;
	int numBlocksRendered;
	int blockChanges;
	Block** prevBlocks;
	Block** blocks;
	Block** translationBlocks;
	int* lightMap;
	queue<int> freeBlocks;
};

