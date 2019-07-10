#pragma once
#include "Block.h"
#include "ChunkVAO.h"
#include "ChunkCoords.h"
#include "WorldGenerator.h"
#include "BlockCoords.h"
#include <queue>
#include <GLFW/glfw3.h>

class ChunkThreadPool;

class ChunkManager;

class Chunk
{
public:

	/*
	Create chunk with VAO parameter
	*/
	Chunk(int x, int y, ChunkVAO* vao, int vaoIndex, ChunkThreadPool* pool, ChunkManager* manager);

	/*
	Create chunk without VAO parameter
	*/
	Chunk(int x, int y, ChunkThreadPool* pool, ChunkManager* manager);

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
	Render this chunk
	*/
	void render();

	ChunkManager* getManager();

	int getId();

public:

	/*
	Coordinates of this chunk (in chunk coordinates)
	*/
	int x, y;

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

	static int chunksExisting;

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
	Reference to the chunk thread pool handling threaded chunk creation, finalization and deletion
	*/
	ChunkThreadPool* pool;

	ChunkManager* manager;

	int id;

};

