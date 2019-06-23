#include "World.h"
#include "Constants.h"

RegionManager* World::rm = new RegionManager();

World::World()
{
	cm = new ChunkManager();
	pool = new ChunkThreadPool(cm, 1);
	renderDistance = 8;
	updateLoadedRegions();
	updateLoadedChunks();
}

Block* World::getBlock(int x, int y, int z, Chunk* suspect) {
	if (suspect != NULL && suspect->blockInChunk(x, y, z)) {
		return suspect->getBlock(x, y, z);
	}
	ChunkCoords coords = blockToChunkCoords(x, z);
	pool->chunkManagerMutex.lock();
	Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	pool->chunkManagerMutex.unlock();
	if (chunk == NULL) {
		return NULL;
	}
	return chunk->getBlock(x, y, z);
}

void World::updateRendering(Block* block, Chunk* suspect) {
	Chunk* c = NULL;
	int x = block->x, y = block->y, z = block->z;
	if (suspect != NULL && suspect->blockInChunk(x, y, z)) {
		c = suspect;
	}
	else {
		ChunkCoords coords = blockToChunkCoords(x, z);
		pool->chunkManagerMutex.lock();
		c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
		pool->chunkManagerMutex.unlock();
	}
	if (c != NULL) {
		c->textures[block->translationIndex] = block->shouldRender ? block->type : -1;
	}
}

void World::setBlock(int x, int y, int z, Block* block, bool update, bool reorderNeighbors) {
	ChunkCoords coords = blockToChunkCoords(x, z);
	pool->chunkManagerMutex.lock();
	Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	pool->chunkManagerMutex.unlock();
	if (chunk != NULL) {
		chunk->setBlock(x, y, z, block, update);
		Block* left = getBlock(x - 1, y, z, chunk);
		Block* right = getBlock(x + 1, y, z, chunk);
		Block* up = getBlock(x, y + 1, z, chunk);
		Block* down = getBlock(x, y - 1, z, chunk);
		Block* front = getBlock(x, y, z - 1, chunk);
		Block* back = getBlock(x, y, z + 1, chunk);
		if (left != NULL) {
			if (block != NULL) {
				block->neighbors[BLOCK_NEIGHBOR_LEFT] = left;
			}
			left->neighbors[BLOCK_NEIGHBOR_RIGHT] = block;
		}
		if (right != NULL) {
			if (block != NULL) {
				block->neighbors[BLOCK_NEIGHBOR_RIGHT] = right;
			}
			right->neighbors[BLOCK_NEIGHBOR_LEFT] = block;
		}
		if (up != NULL) {
			if (block != NULL) {
				block->neighbors[BLOCK_NEIGHBOR_UP] = up;
			}
			up->neighbors[BLOCK_NEIGHBOR_DOWN] = block;
		}
		if (down != NULL) {
			if (block != NULL) {
				block->neighbors[BLOCK_NEIGHBOR_DOWN] = down;
			}
			down->neighbors[BLOCK_NEIGHBOR_UP] = block;
		}
		if (front != NULL) {
			if (block != NULL) {
				block->neighbors[BLOCK_NEIGHBOR_FRONT] = front;
			}
			front->neighbors[BLOCK_NEIGHBOR_BACK] = block;
		}
		if (back != NULL) {
			if (block != NULL) {
				block->neighbors[BLOCK_NEIGHBOR_BACK] = back;
			}
			back->neighbors[BLOCK_NEIGHBOR_FRONT] = block;
		}
		if (left != NULL) {
			left->updateNeighbors();
			if (reorderNeighbors) {
				reorderBlock(left, chunk);
			}
		}
		if (right != NULL) {
			right->updateNeighbors();
			if (reorderNeighbors) {
				reorderBlock(right, chunk);
			}
		}
		if (up != NULL) {
			up->updateNeighbors();
			if (reorderNeighbors) {
				reorderBlock(up, chunk);
			}
		}
		if (down != NULL) {
			down->updateNeighbors();
			if (reorderNeighbors) {
				reorderBlock(down, chunk);
			}
		}
		if (front != NULL) {
			front->updateNeighbors();
			if (reorderNeighbors) {
				reorderBlock(front, chunk);
			}
		}
		if (back != NULL) {
			back->updateNeighbors();
			if (reorderNeighbors) {
				reorderBlock(back, chunk);
			}
		}
	}
	else {
		std::cout << "Error: tried to add block in nonexistent chunk" << std::endl;
	}
}

void World::exit() {
	pool->exit();
}

void World::reorderBlocks() {
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->reorderBlocks();
	}
	pool->chunkManagerMutex.unlock();
}

void World::reorderBlock(Block* block, Chunk* suspect) {
	int x = block->x, y = block->y, z = block->z;
	if (suspect->blockInChunk(x, y, z)) {
		suspect->reorderBlock(block);
		suspect->updateVAO();
	}
	else {
		ChunkCoords coords = blockToChunkCoords(x, z);
		pool->chunkManagerMutex.lock();
		Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
		pool->chunkManagerMutex.unlock();
		c->reorderBlock(block);
		c->updateVAO();
	}
}

void World::updateChunkVAOs() {
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->updateVAO();
	}
	pool->chunkManagerMutex.unlock();
}

bool World::chunkExists(int x, int y) {
	ChunkCoords coords(x, y, 0, 0);
	pool->chunkManagerMutex.lock();
	Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	pool->chunkManagerMutex.unlock();
	return c != NULL;
}

bool World::regionExists(int x, int y) {
	return rm->getRegion(x, y) != NULL;
}

void World::updateLoadedChunks() {
	glm::vec2 playerChunkPos((float)playerChunkX, (float)playerChunkY);
	pool->chunkManagerMutex.lock();
	for (int i = cm->chunks->size() - 1; i >= 0; i--) {
		if (glm::length(glm::vec2((float)(*(cm->chunks))[i]->x, (float)(*(cm->chunks))[i]->y) - playerChunkPos) > renderDistance) {
			deleteChunk((*(cm->chunks))[i]->x, (*(cm->chunks))[i]->y);
		}
	}
	pool->chunkManagerMutex.unlock();
	for (int x = (int)playerChunkPos.x - renderDistance; x < (int)playerChunkPos.x + renderDistance; x++) {
		for (int y = (int)playerChunkPos.y - renderDistance; y < (int)playerChunkPos.y + renderDistance; y++) {
			int localX = x - (int)playerChunkPos.x;
			int localY = y - (int)playerChunkPos.y;
			if (glm::sqrt(localX * localX + localY * localY) <= renderDistance && !chunkExists(x, y)) {
				makeChunk(x, y);
			}
		}
	}
}

void World::updateLoadedRegions() {
	glm::vec2 playerRegionPos(floor((float)playerChunkX / REGION_WIDTH_CHUNKS), floor((float)playerChunkY / REGION_WIDTH_CHUNKS));
	std::vector<Region*> regions = rm->getRegions();
	for (int i = regions.size() - 1; i >= 0; i--) {
		if (glm::abs(regions[i]->getX() - playerRegionPos.x) > REGION_GENERATION_DISTANCE || glm::abs(regions[i]->getZ() - playerRegionPos.y) > REGION_GENERATION_DISTANCE) {
			deleteRegion(regions[i]->getX(), regions[i]->getZ());
		}
	}
	for (int x = (int)playerRegionPos.x - REGION_GENERATION_DISTANCE; x <= (int)playerRegionPos.x + REGION_GENERATION_DISTANCE; x++) {
		for (int y = (int)playerRegionPos.y - REGION_GENERATION_DISTANCE; y <= (int)playerRegionPos.y + REGION_GENERATION_DISTANCE; y++) {
			if (!regionExists(x, y)) {
				makeRegion(x, y);
			}
		}
	}
}

void World::updatePlayerChunkPosition(int chunkX, int chunkY) {
	bool updateChunk = false;
	bool updateRegion = false;
	int playerRegionX = (int)floor((float)playerChunkX / REGION_WIDTH_CHUNKS);
	int playerRegionY = (int)floor((float)playerChunkY / REGION_WIDTH_CHUNKS);
	int regionX = (int)floor((float)chunkX / REGION_WIDTH_CHUNKS);
	int regionY = (int)floor((float)chunkY / REGION_WIDTH_CHUNKS);
	if (chunkX != playerChunkX || chunkY != playerChunkY) {
		updateChunk = true;
	}
	if (regionX != playerRegionX || regionY != playerRegionY) {
		updateRegion = true;
	}
	playerChunkX = chunkX;
	playerChunkY = chunkY;
	if (updateChunk) {
		updateLoadedChunks();
	}
	if (updateRegion) {
		updateLoadedRegions();
	}
}

void World::render() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, Block::texture);
	//std::cout << Chunk::chunksInPlay << std::endl;
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		if ((*(cm->chunks))[i]->state == Chunk::RENDERING) {
			(*(cm->chunks))[i]->render();
		}
	}
	pool->chunkManagerMutex.unlock();
}

void World::makeChunk(int x, int y) {
	if (!chunkExists(x, y)) {
		Chunk* c = new Chunk(x, y);
		pool->createChunk(c);
	}
}

void World::makeRegion(int x, int y) {
	if (!regionExists(x, y)) {
		Region* r = new Region(x, y);
		r->generate();
		rm->addRegion(r);
	}
}

void World::updateViewFrustum(ViewFrustum* frustum) {
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->updateViewFrustum(frustum);
	}
	pool->chunkManagerMutex.unlock();
}

void World::deleteChunk(int x, int y) {
	ChunkCoords coords(x, y, 0, 0);
	Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	if (c != NULL) {
		pool->deleteChunk(c);
	}
}

void World::deleteRegion(int x, int y) {
	Region* r = rm->removeRegion(x, y);
	delete r;
}

World::~World()
{
	delete cm;
	delete pool;
}
