#include "Chunk.h"
#include "Constants.h"
#include "Util.h"
#include "ChunkThreadPool.h"
#include "ChunkManager.h"

int Chunk::chunksActive = 0;

int Chunk::chunksExisting = 0;

Chunk::Chunk(int x, int y, ChunkVAO* vao, int vaoIndex, ChunkThreadPool* pool, ChunkManager* manager)
{

	chunksExisting++;

	this->pool = pool;

	this->x = x;

	this->y = y;

	this->manager = manager;

	// Chunk starts out by waiting for generation
	state = WAITING_FOR_GENERATE;

	neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);

	// Initialize neighbors to NULL
	for (int i = 0; i < 4; i++) {

		neighbors[i] = NULL;

	}

}

Chunk::Chunk(int x, int y, ChunkThreadPool* pool, ChunkManager* manager) {

	chunksExisting++;

	this->pool = pool;

	this->x = x;

	this->y = y;
	
	this->manager = manager;

	// Chunk starts out by waiting for generation
	state = WAITING_FOR_GENERATE;

	neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);

	// Initialize neighbors to NULL
	for (int i = 0; i < 4; i++) {

		neighbors[i] = NULL;

	}

}

void Chunk::generate(WorldGenerator* gen) {

	id = chunksActive;

	// Chunk is considered to be active following generation
	chunksActive++;

	// This is where the "generation" takes place
	Block** blocks = gen->getChunkBlocks(x, y);

	manager->setChunkBlocks(blocks, id);

	manager->bindChunkBlocks(this, id);

	delete blocks;
	
}

void Chunk::updateBlockNeighbors() {

	Block** blocks = manager->getBlocks(id);

	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {

		// Local (bx, by, bz) and global (gx, gy, gz) block coordinates for current block
		int bx, by, bz, gx, gy, gz;

		if (blocks[i] != NULL) {

			// If block is not NULL we can just ask for its coordinates
			bx = blocks[i]->getX() - x * CHUNK_WIDTH;
			by = blocks[i]->getY();
			bz = blocks[i]->getZ() - y * CHUNK_WIDTH;
			gx = blocks[i]->getX();
			gy = blocks[i]->getY();
			gz = blocks[i]->getZ();

		}
		else {

			// Otherwise we calculate the block coordinates
			BlockCoords coords = indexToBlockCoords(i);

			bx = coords.x;
			by = coords.y;
			bz = coords.z;
			gx = coords.x + x * CHUNK_WIDTH;
			gy = coords.y;
			gz = coords.z + y * CHUNK_WIDTH;

		}

		// Set block neighbors on top and bottom of block
		if (blocks[i] != NULL) {

			blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_UP] = getBlock(gx, gy + 1, gz);

			blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_DOWN] = getBlock(gx, gy - 1, gz);

		}

		// Perform check for neighboring chunks if blocks lie on outer edge of chunk, otherwise just assign neighbors within this chunk
		if (bx == 0) {

			if (neighbors[CHUNK_NEIGHBOR_LEFT] != NULL) {

				Block* b = neighbors[CHUNK_NEIGHBOR_LEFT]->getBlock(gx - 1, gy, gz);

				if (b != NULL) {

					b->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] = blocks[i];

				}

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_LEFT] = b;

				}

				if (b != NULL) {

					b->determineRendering();

				}

			}
			else {

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_LEFT] = NULL;

				}

			}

		}
		else {

			if (blocks[i] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_LEFT] = getBlock(gx - 1, gy, gz);

			}

		}

		if (bx == CHUNK_WIDTH - 1) {

			if (neighbors[CHUNK_NEIGHBOR_RIGHT] != NULL) {

				Block* b = neighbors[CHUNK_NEIGHBOR_RIGHT]->getBlock(gx + 1, gy, gz);

				if (b != NULL) {

					b->getNeighbors()[BLOCK_NEIGHBOR_LEFT] = blocks[i];

				}

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] = b;

				}

				if (b != NULL) {

					b->determineRendering();

				}

			}
			else {

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] = NULL;

				}

			}

		}
		else {

			if (blocks[i] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] = getBlock(gx + 1, gy, gz);

			}

		}

		if (bz == 0) {

			if (neighbors[CHUNK_NEIGHBOR_FRONT] != NULL) {

				Block* b = neighbors[CHUNK_NEIGHBOR_FRONT]->getBlock(gx, gy, gz - 1);

				if (b != NULL) {

					b->getNeighbors()[BLOCK_NEIGHBOR_BACK] = blocks[i];

				}

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_FRONT] = b;

				}

				if (b != NULL) {

					b->determineRendering();

				}

			}
			else {

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_FRONT] = NULL;

				}

			}

		}
		else {

			if (blocks[i] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_FRONT] = getBlock(gx, gy, gz - 1);

			}

		}

		if (bz == CHUNK_WIDTH - 1) {

			if (neighbors[CHUNK_NEIGHBOR_BACK] != NULL) {

				Block* b = neighbors[CHUNK_NEIGHBOR_BACK]->getBlock(gx, gy, gz + 1);

				if (b != NULL) {

					b->getNeighbors()[BLOCK_NEIGHBOR_FRONT] = blocks[i];

				}

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_BACK] = b;

				}

				if (b != NULL) {

					b->determineRendering();

				}

			}
			else {

				if (blocks[i] != NULL) {

					blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_BACK] = NULL;

				}

			}

		}
		else {

			if (blocks[i] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_BACK] = getBlock(gx, gy, gz + 1);

			}

		}

		if (blocks[i] != NULL) {

			// Update block rendering based on neighboring blocks
			blocks[i]->determineRendering();

			manager->initBlock(blocks[i]);

		}

	}

}

int Chunk::getId() {

	return id;

}

void Chunk::calculateLighting() {

	Block** blocks = manager->getBlocks(id);

	for (int x = 0; x < CHUNK_WIDTH; x++) {

		for (int z = 0; z < CHUNK_WIDTH; z++) {

			// Starting from top of chunk, give blocks maximum light value
			int lightValue = 255;

			for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {

				// Calculate block array index based on local chunk coordinates
				int index = blockCoordsToIndex(x, y, z);

				if (blocks[index] != NULL) {

					// By default, let block's faces all have light value of 0
					for (int i = 0; i < 6; i++) {

						blocks[index]->setLightValue(0, i);

					}

					// Set the top light value to lightValue (the sun is shining downward onto the top of blocks)
					blocks[index]->setLightValue(lightValue, BLOCK_NEIGHBOR_UP);

					// If we've reached a solid block, block the light ray from reaching lower blocks
					if (blocks[index]->shouldRenderType()) {

						lightValue = 0;

					}

				}

			}

		}

	}

	// Calculate lighting spread for each block
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {

		if (blocks[i] != NULL) {

			blocks[i]->determineLighting();

		}

	}

	// If block lies on edge of chunk, call determineNeighborLighting in order to enable lighting from existing chunks to spread to this one
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {

		if (blocks[i] != NULL) {

			int localX = blocks[i]->getX() - x * CHUNK_WIDTH;

			int localZ = blocks[i]->getZ() - y * CHUNK_WIDTH;

			if (localX == 0 || localX == CHUNK_WIDTH - 1 || localZ == 0 || localZ == CHUNK_WIDTH - 1) {

				blocks[i]->determineNeighborLighting();

			}

		}

	}

}

void Chunk::makeLightmap() {

	manager->makeLightmap(id);

}

void Chunk::updateLightingOnRender() {

	this->updateLighting = true;

}

void Chunk::bakeNeighbors() {

	// Update rendered blocks in neighboring chunks (used to hide blocks that are no longer exposed due to creation of this chunk)
	// TODO: Optimize, avoid reordering all blocks in neighboring chunks and instead only ones on exposed edge
	if (neighbors[CHUNK_NEIGHBOR_LEFT] != NULL) {

		neighbors[CHUNK_NEIGHBOR_LEFT]->reorderBlocks();

		manager->updateVAO();

	}

	if (neighbors[CHUNK_NEIGHBOR_RIGHT] != NULL) {

		neighbors[CHUNK_NEIGHBOR_RIGHT]->reorderBlocks();

		manager->updateVAO();

	}

	if (neighbors[CHUNK_NEIGHBOR_FRONT] != NULL) {

		neighbors[CHUNK_NEIGHBOR_FRONT]->reorderBlocks();

		manager->updateVAO();

	}

	if (neighbors[CHUNK_NEIGHBOR_BACK] != NULL) {

		neighbors[CHUNK_NEIGHBOR_BACK]->reorderBlocks();

		manager->updateVAO();

	}

}

void Chunk::setBlock(int globalX, int globalY, int globalZ, Block* block, bool update) {

	// Don't modify this chunk's blocks if it's currently generating, about to be generated, or currently deleting
	if (state == Chunk::WAITING_FOR_GENERATE || state == Chunk::GENERATING || state == Chunk::DELETING) {

		return;

	}
	
	manager->setBlock(this, globalX, globalY, globalZ, block, update);

}

void Chunk::reorderBlocks() {

	manager->reorderBlocks(id);

}

void Chunk::reorderBlock(Block* block) {

	manager->reorderBlock(block);

}

bool Chunk::blockInChunk(int globalX, int globalY, int globalZ) {

	int localX = globalX - x * CHUNK_WIDTH;

	int localZ = globalZ - y * CHUNK_WIDTH;

	return localX >= 0 && localX < CHUNK_WIDTH && globalY >= 0 && globalY < CHUNK_HEIGHT && localZ >= 0 && localZ < CHUNK_WIDTH;

}

Block* Chunk::getBlock(int globalX, int globalY, int globalZ) {

	// Don't bother retrieving block if chunk is currently in a state which potentially modifies blocks
	if (state == Chunk::WAITING_FOR_GENERATE || state == Chunk::GENERATING || state == Chunk::DELETING) {

		return NULL;

	}

	Block** blocks = manager->getBlocks(id);

	int localX = globalX - x * CHUNK_WIDTH;

	int localZ = globalZ - y * CHUNK_WIDTH;

	// Make sure block index is within chunk bounds
	if (localX >= 0 && localX < CHUNK_WIDTH && globalY >= 0 && globalY < CHUNK_HEIGHT && localZ >= 0 && localZ < CHUNK_WIDTH) {

		return blocks[blockCoordsToIndex(localX, globalY, localZ)];

	}

	return NULL;

}

int Chunk::compare(Chunk* other) {

	if (x < other->x) {

		return -1;

	}

	if (x > other->x) {

		return 1;

	}

	if (y < other->y) {

		return -1;

	}

	if (y > other->y) {

		return 1;

	}

	return 0;

}

int Chunk::compare(ChunkCoords* other) {

	if (x < other->chunkX) {

		return -1;

	}

	if (x > other->chunkX) {

		return 1;

	}

	if (y < other->chunkZ) {

		return -1;

	}

	if (y > other->chunkZ) {

		return 1;

	}

	return 0;

}

void Chunk::render() {

	// If the updateLighting flag was set, generate physical lightmap and update VAO to render new lightmap values
	if (updateLighting) {

		makeLightmap();

		manager->updateVAO();

		updateLighting = false;

	}

}

ChunkManager* Chunk::getManager() {

	return manager;

}

Chunk::~Chunk()
{

	// Erase this chunk's existence from its neighboring chunks
	if (neighbors[CHUNK_NEIGHBOR_LEFT] != NULL) {

		neighbors[CHUNK_NEIGHBOR_LEFT]->neighbors[CHUNK_NEIGHBOR_RIGHT] = NULL;

	}

	if (neighbors[CHUNK_NEIGHBOR_RIGHT] != NULL) {

		neighbors[CHUNK_NEIGHBOR_RIGHT]->neighbors[CHUNK_NEIGHBOR_LEFT] = NULL;

	}

	if (neighbors[CHUNK_NEIGHBOR_FRONT] != NULL) {

		neighbors[CHUNK_NEIGHBOR_FRONT]->neighbors[CHUNK_NEIGHBOR_BACK] = NULL;

	}

	if (neighbors[CHUNK_NEIGHBOR_BACK] != NULL) {

		neighbors[CHUNK_NEIGHBOR_BACK]->neighbors[CHUNK_NEIGHBOR_FRONT] = NULL;

	}

	// Erase the existence of any blocks within this chunk from neighboring blocks in adjacent chunks
	Block** blocks = manager->getBlocks(id);

	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {

  	if (blocks[i] != NULL) {

			if (blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_LEFT] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_LEFT]->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] = NULL;

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_LEFT]->determineRendering();

			}

			if (blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_RIGHT]->getNeighbors()[BLOCK_NEIGHBOR_LEFT] = NULL;

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_RIGHT]->determineRendering();

			}

			if (blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_FRONT] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_FRONT]->getNeighbors()[BLOCK_NEIGHBOR_BACK] = NULL;

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_FRONT]->determineRendering();

			}

			if (blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_BACK] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_BACK]->getNeighbors()[BLOCK_NEIGHBOR_FRONT] = NULL;

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_BACK]->determineRendering();

			}

			if (blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_UP] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_UP]->getNeighbors()[BLOCK_NEIGHBOR_DOWN] = NULL;

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_UP]->determineRendering();

			}

			if (blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_DOWN] != NULL) {

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_DOWN]->getNeighbors()[BLOCK_NEIGHBOR_UP] = NULL;

				blocks[i]->getNeighbors()[BLOCK_NEIGHBOR_DOWN]->determineRendering();

			}

			delete blocks[i];

		}

	}

	// We'll want to re-render neighboring chunks to re-expose blocks that were previously hidden
	for (int i = 0; i < 4; i++) {

		if (neighbors[i] != NULL) {

			neighbors[i]->reorderBlocks();

			manager->updateVAO();

		}

	}

	free(neighbors);

	// Chunk is no longer active once it is deleted
	chunksActive--;

	chunksExisting--;

}
