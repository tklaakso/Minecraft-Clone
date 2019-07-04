#pragma once
#include "Block.h"
#include "ChunkVAO.h"
#include "ChunkCoords.h"
#include "WorldGenerator.h"
#include "BlockCoords.h"
#include <queue>
#include <GLFW/glfw3.h>

class ChunkThreadPool;

class Chunk
{
public:

	/*
	Create chunk with VAO parameter
	*/
	Chunk(int x, int y, ChunkVAO* vao, int vaoIndex, ChunkThreadPool* pool);

	/*
	Create chunk without VAO parameter
	*/
	Chunk(int x, int y, ChunkThreadPool* pool);

	/*
	Set chunk VAO separately
	*/
	void setVAO(ChunkVAO* vao, int vaoIndex);

	/*
	Set a block inside this chunk based on its global coordinates
	*/
	void setBlock(int globalX, int globalY, int globalZ, Block* block, bool update);

	/*
	Get a block block inside this chunk based on its global coordinates
	*/
	Block* getBlock(int globalX, int globalY, int globalZ);

	/*
	Check if a given global block position is inside this chunk
	*/
	bool blockInChunk(int globalX, int globalY, int globalZ);

	/*
	Use world generator to generate the blocks for this chunk
	*/
	void generate(WorldGenerator* gen);

	/*
	Assigns neighboring blocks to blocks within this chunk and subsequently determines their lighting
	*/
	void updateBlockNeighbors();

	/*
	Does the following in sequence:
		1. Calculates base lighting values by projecting downward from the top of the chunk through invisible blocks
		2. Allows blocks to recursively calculate lighting spread
		3. Calculates lighting values of blocks neighboring this chunk from other chunks in order to combine with newly created blocks in this chunk
	*/
	void calculateLighting();

	/*
	Sets a flag which forces this chunk to generate a lightmap and update its VAO before the next render call
	*/
	void updateLightingOnRender();

	/*
	Generates a lightmap for input to the chunk VAO
	*/
	void makeLightmap();

	/*
	Reorders blocks and updates VAOs of neighboring chunks, if they exist
	*/
	void bakeNeighbors();

	/*
	Sets a flag which updates VAO with translation, texture and lightmap data before the next render call (VAO updates must be done on main thread, which isn't necessarily the caller)
	*/
	void updateVAO();

	/*
	Compares this chunk with another with x being the most significant coordinate
	*/
	int compare(Chunk* other);

	/*
	Compares this chunk with a ChunkCoords object representing a chunk position
	*/
	int compare(ChunkCoords* other);

	/*
	Reorders block indices within translations, textures and lightmap array to determine which ones will be rendered
	*/
	void reorderBlocks();

	/*
	Reorders an individual block based on whether it should be rendered
	*/
	void reorderBlock(Block* block);

	/*
	Updates whether or not this chunk is contained in the view frustum
	*/
	void updateViewFrustum(ViewFrustum* frustum);

	/*
	Render this chunk
	*/
	void render();

public:

	/*
	VAO for this chunk
	*/
	ChunkVAO* vao;

	/*
	Index of this chunk's VAO within the reusable chunk VAO array
	*/
	int vaoIndex;

	/*
	Coordinates of this chunk (in chunk coordinates)
	*/
	int x, y;

	/*
	Translations of blocks within this chunk, indexed by their translation indices
	*/
	glm::vec3* translations;

	/*
	Textures of blocks within this chunk, indexed by their translation indices
	*/
	int* textures;

	/*
	Integer-coded state of this chunk
	*/
	int state;

	/*
	Array of pointers to neighboring chunks
	*/
	Chunk** neighbors;

	/*
	Records the number of chunks existing at any given moment
	*/
	static int chunksActive;

	/*
	Integer-coded states of chunk generation
	*/
	static const int WAITING_FOR_GENERATE = 0;
	static const int GENERATING = 1;
	static const int WAITING_FOR_FINALIZE = 2;
	static const int FINALIZING = 3;
	static const int RENDERING = 4;
	static const int DELETING = 5;

	~Chunk();

private:

	/*
	Flag controlling whether this chunk should update its lighting before the next render call
	*/
	bool updateLighting;

	/*
	Stores whether or not this chunk is inside the view frustum
	*/
	bool insideViewFrustum;

	/*
	Flag controlling whether this chunk should update its VAO before the next render call
	*/
	bool shouldUpdateVAO;

	/*
	Swaps indices of b1 and b2 in translation, texture and lighmap arrays
	*/
	void swapBlockIndices(Block* b1, int index1, Block* b2, int index2);

	/*
	Retrieves the block with a given translation index
	*/
	Block* blockWithTranslationIndex(int translationIndex);

	/*
	Number of blocks existing in this chunk
	*/
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

	/*
	Reference to the chunk thread pool handling threaded chunk creation, finalization and deletion
	*/
	ChunkThreadPool* pool;

};

