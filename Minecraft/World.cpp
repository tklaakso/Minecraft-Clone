#include "World.h"
#include "Constants.h"

World::World()
{
	VAOs = (ChunkVAO**)malloc(sizeof(ChunkVAO*) * NUM_CHUNK_VAOS);
	for (int i = 0; i < NUM_CHUNK_VAOS; i++) {
		ChunkVAO* vao = new ChunkVAO();
		VAOs[i] = vao;
	}
	generator = new WorldGenerator();
}

Chunk* World::findChunkWithCoords(ChunkCoords* coords, int l, int r) {
	if (r >= l) {
		int mid = l + (r - l) / 2;
		int cmp = chunks[mid]->compare(coords);
		if (cmp == 0) {
			return chunks[mid];
		}
		if (cmp > 0) {
			return findChunkWithCoords(coords, l, mid - 1);
		}
		return findChunkWithCoords(coords, mid + 1, r);
	}
	return NULL;
}

Block* World::getBlock(int x, int y, int z, Chunk* suspect) {
	if (suspect != NULL && suspect->blockInChunk(x, y, z)) {
		return suspect->getBlock(x, y, z);
	}
	ChunkCoords coords = blockToChunkCoords(x, z);
	Chunk* chunk = findChunkWithCoords(&coords, 0, chunks.size() - 1);
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
		c = findChunkWithCoords(&coords, 0, chunks.size() - 1);
	}
	if (c != NULL) {
		c->textures[block->translationIndex] = block->shouldRender ? block->type : -1;
	}
}

void World::setBlock(int x, int y, int z, Block* block, bool update, bool reorderNeighbors) {
	ChunkCoords coords = blockToChunkCoords(x, z);
	Chunk* chunk = findChunkWithCoords(&coords, 0, chunks.size() - 1);
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

void World::reorderBlocks() {
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i]->reorderBlocks();
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
		Chunk* c = findChunkWithCoords(&coords, 0, chunks.size() - 1);
		c->reorderBlock(block);
		c->updateVAO();
	}
}

void World::updateChunkVAOs() {
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i]->updateVAO();
	}
}

void World::render() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, Block::texture);
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i]->render();
	}
}

void World::makeChunk(int x, int y) {
	ChunkVAO* vao;
	int vaoIndex;
	if (!freeVAOs.empty()) {
		vaoIndex = freeVAOs.front();
		vao = VAOs[vaoIndex];
		freeVAOs.pop();
	}
	else {
		vaoIndex = numUsedVAOs;
		vao = VAOs[vaoIndex];
		numUsedVAOs++;
	}
	Chunk* c = new Chunk(x, y, vao, vaoIndex);
	Chunk** neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);
	ChunkCoords left(c->x - 1, c->y, 0, 0);
	ChunkCoords right(c->x + 1, c->y, 0, 0);
	ChunkCoords front(c->x, c->y - 1, 0, 0);
	ChunkCoords back(c->x, c->y + 1, 0, 0);
	neighbors[CHUNK_NEIGHBOR_LEFT] = findChunkWithCoords(&left, 0, chunks.size() - 1);
	neighbors[CHUNK_NEIGHBOR_RIGHT] = findChunkWithCoords(&right, 0, chunks.size() - 1);
	neighbors[CHUNK_NEIGHBOR_FRONT] = findChunkWithCoords(&front, 0, chunks.size() - 1);
	neighbors[CHUNK_NEIGHBOR_BACK] = findChunkWithCoords(&back, 0, chunks.size() - 1);
	c->generate(generator, neighbors);
	free(neighbors);
	if (chunks.size() == 0) {
		chunks.push_back(c);
		return;
	}
	for (int i = 0; i < chunks.size(); i++) {
		int cmp = chunks[i]->compare(c);
		if (cmp == 0) {
			std::cout << "Error: tried to create existing chunk" << std::endl;
			return;
		}
		if (cmp > 0) {
			chunks.insert(chunks.begin() + i, c);
			return;
		}
	}
	chunks.push_back(c);
}

void World::updateViewFrustum(ViewFrustum* frustum) {
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i]->updateViewFrustum(frustum);
	}
}

void World::deleteChunk(int x, int y) {
	ChunkCoords coords(x, y, 0, 0);
	Chunk* chunk = findChunkWithCoords(&coords, 0, chunks.size() - 1);
	if (chunk != NULL) {
		std::remove(chunks.begin(), chunks.end(), chunk);
		freeVAOs.push(chunk->vaoIndex);
		delete chunk;
		ChunkCoords leftCoords(x - 1, y, 0, 0);
		ChunkCoords rightCoords(x + 1, y, 0, 0);
		ChunkCoords frontCoords(x, y - 1, 0, 0);
		ChunkCoords backCoords(x, y + 1, 0, 0);
		Chunk* left = findChunkWithCoords(&leftCoords, 0, chunks.size() - 1);
		Chunk* right = findChunkWithCoords(&rightCoords, 0, chunks.size() - 1);
		Chunk* front = findChunkWithCoords(&frontCoords, 0, chunks.size() - 1);
		Chunk* back = findChunkWithCoords(&backCoords, 0, chunks.size() - 1);
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
	for (int i = 0; i < NUM_CHUNK_VAOS; i++) {
		free(VAOs[i]);
	}
	free(VAOs);
	delete generator;
}
