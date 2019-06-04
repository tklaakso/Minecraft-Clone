#include "World.h"
#include "Constants.h"

World::World()
{
	cm = new ChunkManager();
	pool = new ChunkThreadPool(cm, 1);
	renderDistance = 8;
}

Block* World::getBlock(int x, int y, int z, Chunk* suspect) {
	if (suspect != NULL && suspect->blockInChunk(x, y, z)) {
		return suspect->getBlock(x, y, z);
	}
	ChunkCoords coords = blockToChunkCoords(x, z);
	Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
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
		c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	}
	if (c != NULL) {
		c->textures[block->translationIndex] = block->shouldRender ? block->type : -1;
	}
}

void World::setBlock(int x, int y, int z, Block* block, bool update, bool reorderNeighbors) {
	ChunkCoords coords = blockToChunkCoords(x, z);
	Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
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
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->reorderBlocks();
	}
}

void World::reorderBlock(Block* block, Chunk* suspect) {
	int x = block->x, y = block->y, z = block->z;
	if (suspect->blockInChunk(x, y, z)) {
		suspect->reorderBlock(block);
		suspect->updateVAO();
	}
	else {
		ChunkCoords coords = blockToChunkCoords(x, z);
		Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
		c->reorderBlock(block);
		c->updateVAO();
	}
}

void World::updateChunkVAOs() {
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->updateVAO();
	}
}

bool World:: chunkExists(int x, int y) {
	ChunkCoords coords(x, y, 0, 0);
	return findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1) != NULL;
}

void World::updateLoadedChunks() {
	glm::vec2 playerChunkPos((float)playerChunkX, float(playerChunkY));
	for (int i = cm->chunks->size() - 1; i >= 0; i--) {
		if (glm::length(glm::vec2((float)(*(cm->chunks))[i]->x, (float)(*(cm->chunks))[i]->y) - playerChunkPos) > renderDistance) {
			deleteChunk((*(cm->chunks))[i]->x, (*(cm->chunks))[i]->y);
		}
	}
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

void World::updatePlayerChunkPosition(int chunkX, int chunkY) {
	bool update = false;
	if (chunkX != playerChunkX || chunkY != playerChunkY) {
		update = true;
	}
	playerChunkX = chunkX;
	playerChunkY = chunkY;
	if (update) {
		updateLoadedChunks();
	}
}

void World::render() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, Block::texture);
	std::cout << Chunk::chunksInPlay << std::endl;
	for (int i = 0; i < cm->chunks->size(); i++) {
		if (!(*(cm->chunks))[i]->generating && !(*(cm->chunks))[i]->deleting) {
			(*(cm->chunks))[i]->render();
		}
	}
}

void World::makeChunk(int x, int y) {
	/*ChunkVAO* vao;
	int vaoIndex;
	if (!cm->freeVAOs.empty()) {
		vaoIndex = cm->freeVAOs.front();
		vao = cm->VAOs[vaoIndex];
		cm->freeVAOs.pop();
	}
	else {
		vaoIndex = cm->numUsedVAOs;
		vao = cm->VAOs[vaoIndex];
		cm->numUsedVAOs++;
	}
	Chunk* c = new Chunk(x, y, vao, vaoIndex);
	Chunk** neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);
	ChunkCoords left(c->x - 1, c->y, 0, 0);
	ChunkCoords right(c->x + 1, c->y, 0, 0);
	ChunkCoords front(c->x, c->y - 1, 0, 0);
	ChunkCoords back(c->x, c->y + 1, 0, 0);
	neighbors[CHUNK_NEIGHBOR_LEFT] = findChunkWithCoords(cm->chunks, &left, 0, cm->chunks.size() - 1);
	neighbors[CHUNK_NEIGHBOR_RIGHT] = findChunkWithCoords(cm->chunks, &right, 0, cm->chunks.size() - 1);
	neighbors[CHUNK_NEIGHBOR_FRONT] = findChunkWithCoords(cm->chunks, &front, 0, cm->chunks.size() - 1);
	neighbors[CHUNK_NEIGHBOR_BACK] = findChunkWithCoords(cm->chunks, &back, 0, cm->chunks.size() - 1);
	c->generate(cm->generator, neighbors);
	free(neighbors);
	if (cm->chunks.size() == 0) {
		cm->chunks.push_back(c);
		return;
	}
	for (int i = 0; i < cm->chunks.size(); i++) {
		int cmp = cm->chunks[i]->compare(c);
		if (cmp == 0) {
			std::cout << "Error: tried to create existing chunk" << std::endl;
			return;
		}
		if (cmp > 0) {
			cm->chunks.insert(cm->chunks.begin() + i, c);
			return;
		}
	}
	cm->chunks.push_back(c);*/
	if (!chunkExists(x, y)) {
		Chunk* c = new Chunk(x, y);
		c->generating = true;
		bool addedChunk = false;
		for (int i = 0; i < cm->chunks->size(); i++) {
			int cmp = (*(cm->chunks))[i]->compare(c);
			if (cmp == 0) {
				std::cout << "Error: tried to create existing chunk" << std::endl;
				addedChunk = true;
				break;
			}
			if (cmp > 0) {
				cm->chunks->insert(cm->chunks->begin() + i, c);
				addedChunk = true;
				break;
			}
		}
		if (!addedChunk) {
			cm->chunks->push_back(c);
		}
		pool->createChunk(c);
		pool->finalizeChunk(c);
	}
}

void World::updateViewFrustum(ViewFrustum* frustum) {
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->updateViewFrustum(frustum);
	}
}

void World::deleteChunk(int x, int y) {
  /*ChunkCoords coords(x, y, 0, 0);
	Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks.size() - 1);
	if (chunk != NULL) {
		for (int i = 0; i < cm->chunks.size(); i++) {
			if (cm->chunks[i] == chunk) {
				cm->chunks.erase(cm->chunks.begin() + i);
				break;
			}
		}
		cm->freeVAOs.push(chunk->vaoIndex);
		delete chunk;
		ChunkCoords leftCoords(x - 1, y, 0, 0);
		ChunkCoords rightCoords(x + 1, y, 0, 0);
		ChunkCoords frontCoords(x, y - 1, 0, 0);
		ChunkCoords backCoords(x, y + 1, 0, 0);
		Chunk* left = findChunkWithCoords(cm->chunks, &leftCoords, 0, cm->chunks.size() - 1);
		Chunk* right = findChunkWithCoords(cm->chunks, &rightCoords, 0, cm->chunks.size() - 1);
		Chunk* front = findChunkWithCoords(cm->chunks, &frontCoords, 0, cm->chunks.size() - 1);
		Chunk* back = findChunkWithCoords(cm->chunks, &backCoords, 0, cm->chunks.size() - 1);
		if (left != NULL) {
			left->reorderBlocks();
			left->updateVAO();
		}
		if (right != NULL) {
			right->reorderBlocks();
			right->updateVAO();
		}
		if (front != NULL) {
			front->reorderBlocks();
			front->updateVAO();
		}
		if (back != NULL) {
			back->reorderBlocks();
			back->updateVAO();
		}
	}
	else {
		std::cout << "Error: tried to delete nonexistent chunk" << std::endl;
	}*/
	ChunkCoords coords(x, y, 0, 0);
	Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	if (c != NULL) {
		for (int i = 0; i < cm->chunks->size(); i++) {
			if ((*(cm->chunks))[i] == c) {
				cm->chunks->erase(cm->chunks->begin() + i);
				break;
			}
		}
		c->deleting = true;
		pool->deleteChunk(c);
	}
}

ChunkCoords World::blockToChunkCoords(int bx, int bz) {
	int chunkX = bx / CHUNK_WIDTH;
	int chunkZ = bz / CHUNK_WIDTH;
	int localX = bx - chunkX * CHUNK_WIDTH;
	int localZ = bz - chunkZ * CHUNK_WIDTH;
	return ChunkCoords(chunkX, chunkZ, localX, localZ);
}

World::~World()
{
	delete cm;
	delete pool;
}
