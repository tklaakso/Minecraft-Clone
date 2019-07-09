#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include "Chunk.h"
#include "ChunkVAO.h"
#include "ChunkCoords.h"
#include "ChunkManager.h"
#include "Util.h"
#include "ChunkThreadPool.h"
#include "RegionManager.h"
#include <GLFW/glfw3.h>
#include "BlockCastInfo.h"

class Player;

class World
{
public:
	World(GLFWwindow* window);
	~World();
	void render();
	void exit();
	void setBlock(int x, int y, int z, Block* block, bool update, bool reorderNeighbors);
	void updateRendering(Block* block, Chunk* suspect);
	Block* getBlock(int x, int y, int z, Chunk* suspect);
	void reorderBlocks();
	void reorderBlock(Block* block, Chunk* suspect);
	void updateChunkVAOs();
	void makeChunk(int x, int y);
	void makeRegion(int x, int y);
	void deleteChunk(int x, int y);
	void deleteRegion(int x, int y);
	void updatePlayerChunkPosition(int chunkX, int chunkY);
	bool chunkExists(int x, int y);
	bool regionExists(int x, int y);
	void updateLoadedChunks();
	void updateLoadedRegions();
	BlockCastInfo blockCast(glm::vec3 origin, glm::vec3 dir);
	int renderDistance;
	Player* getPlayer();
	static RegionManager* rm;
private:
	Player* player;
	Block* selectedBlock = NULL;
	int playerChunkX, playerChunkY;
	ChunkThreadPool* pool;
	ChunkManager* cm;
};

