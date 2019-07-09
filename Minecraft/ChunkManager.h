#pragma once
#include <vector>
#include <queue>
#include "Chunk.h"
#include "ChunkVAO.h"
class ChunkManager
{
public:
	ChunkManager();
	std::vector<Chunk*>* chunks;
	WorldGenerator* generator;
	~ChunkManager();

	/*
	VAO for this chunk
	*/
	ChunkVAO* vao;

	/*
	Translations of blocks within this chunk, indexed by their translation indices
	*/
	glm::vec3* translations;

	/*
	Textures of blocks within this chunk, indexed by their translation indices
	*/
	int* textures;

	/*
	Flag controlling whether this chunk should update its VAO before the next render call
	*/
	bool shouldUpdateVAO;

	int numBlocks;

	/*
	Number of blocks currently rendered in this chunk
	*/
	int numBlocksRendered;

	/*
	Stores blocks in this chunk with indices defined by their spatial coordinates
	*/
	Block** blocks;

	/*
	Mapping of translation indices to blocks for fast retrieval
	*/
	Block** translationBlocks;

	/*
	Stores light values for each face of each block in this chunk
	*/
	int** lightMap;

	/*
	Queue of indices of blocks that were deleted and may be freely replaced with new blocks
	*/
	queue<int> freeBlocks;

	void swapBlockIndices(Block* b1, int index1, Block* b2, int index2);

	void setChunkBlocks(Block** blocks, int chunkId);

	void makeLightmap(int chunkId);

	void deleteChunkBlocks(int chunkId);

	void bindChunkBlocks(Chunk* chunk, int chunkId);

	Block** getBlocks(int chunkId);

	void initBlock(Block* b);

	void setBlock(Chunk* c, int globalX, int globalY, int globalZ, Block* block, bool update);

	void reorderBlock(Block* block);

	void reorderBlocks(int chunkId);

	void updateVAO();

	void render();

};

