#include "Chunk.h"
#include "Constants.h"
#include "Util.h"

Chunk::Chunk(int x, int y, ChunkVAO* vao, int vaoIndex)
{
	this->x = x;
	this->y = y;
	this->vao = vao;
	this->vaoIndex = vaoIndex;
	insideViewFrustum = true;
	numBlocks = numBlocksRendered = 0;
	translations = (glm::vec3*)malloc(sizeof(glm::vec3) * BLOCKS_PER_CHUNK);
	translationBlocks = (Block**)malloc(sizeof(Block*) * BLOCKS_PER_CHUNK);
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		translationBlocks[i] = NULL;
	}
	textures = (int*)malloc(sizeof(int) * BLOCKS_PER_CHUNK);
}

void Chunk::swapBlockIndices(Block* b1, int index1, Block* b2, int index2) {
	glm::vec3 tempTranslation = translations[index1];
	int tempTexture = textures[index1];
	translations[index1] = translations[index2];
	textures[index1] = textures[index2];
	translations[index2] = tempTranslation;
	textures[index2] = tempTexture;
	translationBlocks[index1] = b2;
	translationBlocks[index2] = b1;
	if (b1 != NULL) {
		b1->translationIndex = index2;
	}
	if (b2 != NULL) {
		b2->translationIndex = index1;
	}
}

Block* Chunk::blockWithTranslationIndex(int translationIndex) {
	return translationBlocks[translationIndex];
}

void Chunk::initWithoutGeneration() {
	blocks = (Block**)malloc(sizeof(Block*) * BLOCKS_PER_CHUNK);
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		blocks[i] = NULL;
	}
}

void Chunk::generate(WorldGenerator* gen, Chunk** neighbors) {
	blocks = gen->getChunkBlocks(x, y);
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		int bx, by, bz, gx, gy, gz;
		if (blocks[i] != NULL) {
			bx = blocks[i]->x - x * CHUNK_WIDTH;
			by = blocks[i]->y;
			bz = blocks[i]->z - y * CHUNK_WIDTH;
			gx = blocks[i]->x;
			gy = blocks[i]->y;
			gz = blocks[i]->z;
		}
		else {
			BlockCoords coords = indexToBlockCoords(i);
			bx = coords.x;
			by = coords.y;
			bz = coords.z;
			gx = coords.x + x * CHUNK_WIDTH;
			gy = coords.y;
			gz = coords.z + y * CHUNK_WIDTH;
		}
		if (blocks[i] != NULL) {
			blocks[i]->neighbors[BLOCK_NEIGHBOR_UP] = getBlock(gx, gy + 1, gz);
			blocks[i]->neighbors[BLOCK_NEIGHBOR_DOWN] = getBlock(gx, gy - 1, gz);
		}
		if (bx == 0) {
			if (neighbors[CHUNK_NEIGHBOR_LEFT] != NULL) {
				Block* b = neighbors[CHUNK_NEIGHBOR_LEFT]->getBlock(gx - 1, gy, gz);
				if (b != NULL) {
					b->neighbors[BLOCK_NEIGHBOR_RIGHT] = blocks[i];
				}
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_LEFT] = b;
				}
				if (b != NULL) {
					b->updateNeighbors();
				}
			}
			else {
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_LEFT] = NULL;
				}
			}
		}
		else {
			if (blocks[i] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_LEFT] = getBlock(gx - 1, gy, gz);
			}
		}
		if (bx == CHUNK_WIDTH - 1) {
			if (neighbors[CHUNK_NEIGHBOR_RIGHT] != NULL) {
				Block* b = neighbors[CHUNK_NEIGHBOR_RIGHT]->getBlock(gx + 1, gy, gz);
				if (b != NULL) {
					b->neighbors[BLOCK_NEIGHBOR_LEFT] = blocks[i];
				}
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_RIGHT] = b;
				}
				if (b != NULL) {
					b->updateNeighbors();
				}
			}
			else {
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_RIGHT] = NULL;
				}
			}
		}
		else {
			if (blocks[i] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_RIGHT] = getBlock(gx + 1, gy, gz);
			}
		}
		if (bz == 0) {
			if (neighbors[CHUNK_NEIGHBOR_FRONT] != NULL) {
				Block* b = neighbors[CHUNK_NEIGHBOR_FRONT]->getBlock(gx, gy, gz - 1);
				if (b != NULL) {
					b->neighbors[BLOCK_NEIGHBOR_BACK] = blocks[i];
				}
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_FRONT] = b;
				}
				if (b != NULL) {
					b->updateNeighbors();
				}
			}
			else {
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_FRONT] = NULL;
				}
			}
		}
		else {
			if (blocks[i] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_FRONT] = getBlock(gx, gy, gz - 1);
			}
		}
		if (bz == CHUNK_WIDTH - 1) {
			if (neighbors[CHUNK_NEIGHBOR_BACK] != NULL) {
				Block* b = neighbors[CHUNK_NEIGHBOR_BACK]->getBlock(gx, gy, gz + 1);
				if (b != NULL) {
					b->neighbors[BLOCK_NEIGHBOR_FRONT] = blocks[i];
				}
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_BACK] = b;
				}
				if (b != NULL) {
					b->updateNeighbors();
				}
			}
			else {
				if (blocks[i] != NULL) {
					blocks[i]->neighbors[BLOCK_NEIGHBOR_BACK] = NULL;
				}
			}
		}
		else {
			if (blocks[i] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_BACK] = getBlock(gx, gy, gz + 1);
			}
		}
		if (blocks[i] != NULL) {
			blocks[i]->updateNeighbors();
			translations[numBlocks] = glm::vec3(gx, gy, gz);
			translationBlocks[numBlocks] = blocks[i];
			textures[numBlocks] = blocks[i]->type;
			numBlocks++;
			numBlocksRendered++;
		}
	}
	reorderBlocks();
	updateVAO();
	if (neighbors[CHUNK_NEIGHBOR_LEFT] != NULL) {
		neighbors[CHUNK_NEIGHBOR_LEFT]->reorderBlocks();
		neighbors[CHUNK_NEIGHBOR_LEFT]->updateVAO();
	}
	if (neighbors[CHUNK_NEIGHBOR_RIGHT] != NULL) {
		neighbors[CHUNK_NEIGHBOR_RIGHT]->reorderBlocks();
		neighbors[CHUNK_NEIGHBOR_RIGHT]->updateVAO();
	}
	if (neighbors[CHUNK_NEIGHBOR_FRONT] != NULL) {
		neighbors[CHUNK_NEIGHBOR_FRONT]->reorderBlocks();
		neighbors[CHUNK_NEIGHBOR_FRONT]->updateVAO();
	}
	if (neighbors[CHUNK_NEIGHBOR_BACK] != NULL) {
		neighbors[CHUNK_NEIGHBOR_BACK]->reorderBlocks();
		neighbors[CHUNK_NEIGHBOR_BACK]->updateVAO();
	}
}

void Chunk::setBlock(int globalX, int globalY, int globalZ, Block* block, bool update) {
	int localX = globalX - x * CHUNK_WIDTH;
	int localZ = globalZ - y * CHUNK_WIDTH;
	if (localX >= 0 && localX < CHUNK_WIDTH && globalY >= 0 && globalY < CHUNK_HEIGHT && localZ >= 0 && localZ < CHUNK_WIDTH) {
		int index = blockCoordsToIndex(localX, globalY, localZ);
		if (blocks[index] == NULL) {
			if (block != NULL) {
				if (freeBlocks.empty()) {
					block->translationIndex = numBlocks;
					blocks[index] = block;
					translations[numBlocks] = glm::vec3(globalX, globalY, globalZ);
					translationBlocks[numBlocks] = block;
					textures[numBlocks] = block->type;
					if (numBlocks != numBlocksRendered) {
						swapBlockIndices(block, numBlocks, blockWithTranslationIndex(numBlocksRendered), numBlocksRendered);
					}
					numBlocks++;
					numBlocksRendered++;
					if (update) {
						updateVAO();
					}
				}
				else {
					int freeTranslationIndex = freeBlocks.front();
					freeBlocks.pop();
					block->translationIndex = freeTranslationIndex;
					blocks[index] = block;
					translations[freeTranslationIndex] = glm::vec3(globalX, globalY, globalZ);
					translationBlocks[freeTranslationIndex] = block;
					textures[freeTranslationIndex] = block->type;
					if (freeTranslationIndex >= numBlocksRendered) {
						swapBlockIndices(block, freeTranslationIndex, blockWithTranslationIndex(numBlocksRendered), numBlocksRendered);
						numBlocksRendered++;
					}
					if (update) {
						updateVAO();
					}
				}
			}
		}
		else {
			if (block != NULL) {
				int translationIndex = blocks[index]->translationIndex;
				block->translationIndex = translationIndex;
				delete blocks[index];
				blocks[index] = block;
				translations[translationIndex] = glm::vec3(globalX, globalY, globalZ);
				translationBlocks[translationIndex] = block;
				textures[translationIndex] = block->type;
				if (translationIndex >= numBlocksRendered) {
					swapBlockIndices(block, translationIndex, blockWithTranslationIndex(numBlocksRendered), numBlocksRendered);
					numBlocksRendered++;
				}
				if (update) {
					updateVAO();
				}
			}
			else {
				int translationIndex = blocks[index]->translationIndex;
				freeBlocks.push(translationIndex);
				delete blocks[index];
				blocks[index] = NULL;
				textures[translationIndex] = -1;
				if (update) {
					vao->updateTextureData(textures, numBlocks);
				}
			}
		}
	}
	else {
		std::cout << "Error: block not within chunk range" << std::endl;
	}
}

void Chunk::reorderBlocks() {
	int iter = 0;
	for (int i = 0; i < numBlocks; i++) {
		Block* b = blockWithTranslationIndex(i);
		if (b != NULL && b->shouldRender) {
			swapBlockIndices(b, i, blockWithTranslationIndex(iter), iter);
			iter++;
		}
	}
	numBlocksRendered = iter;
}

void Chunk::reorderBlock(Block* block) {
	if (block->translationIndex >= numBlocksRendered) {
		swapBlockIndices(block, block->translationIndex, blockWithTranslationIndex(numBlocksRendered), numBlocksRendered);
		numBlocksRendered++;
	}
}

bool Chunk::blockInChunk(int globalX, int globalY, int globalZ) {
	int localX = globalX - x * CHUNK_WIDTH;
	int localZ = globalZ - y * CHUNK_WIDTH;
	return localX >= 0 && localX < CHUNK_WIDTH && globalY >= 0 && globalY < CHUNK_HEIGHT && localZ >= 0 && localZ < CHUNK_WIDTH;
}

Block* Chunk::getBlock(int globalX, int globalY, int globalZ) {
	int localX = globalX - x * CHUNK_WIDTH;
	int localZ = globalZ - y * CHUNK_WIDTH;
	if (localX >= 0 && localX < CHUNK_WIDTH && globalY >= 0 && globalY < CHUNK_HEIGHT && localZ >= 0 && localZ < CHUNK_WIDTH) {
		return blocks[blockCoordsToIndex(localX, globalY, localZ)];
	}
	return NULL;
}

void Chunk::updateVAO() {
	vao->updateTranslationData(translations, numBlocksRendered);
	vao->updateTextureData(textures, numBlocksRendered);
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

void Chunk::setBlockLocal(int localX, int localY, int localZ, Block* block, bool update) {
	setBlock(localX + x * CHUNK_WIDTH, localY, localZ + y * CHUNK_WIDTH, block, update);
}

void Chunk::updateViewFrustum(ViewFrustum* frustum) {
	insideViewFrustum = frustum->chunkInView(x, y);
}

void Chunk::render() {
	if (insideViewFrustum) {
		vao->bind();
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, numBlocksRendered);
	}
}

Chunk::~Chunk()
{
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
  	if (blocks[i] != NULL) {
			if (blocks[i]->neighbors[BLOCK_NEIGHBOR_LEFT] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_LEFT]->neighbors[BLOCK_NEIGHBOR_RIGHT] = NULL;
				blocks[i]->neighbors[BLOCK_NEIGHBOR_LEFT]->updateNeighbors();
			}
			if (blocks[i]->neighbors[BLOCK_NEIGHBOR_RIGHT] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_RIGHT]->neighbors[BLOCK_NEIGHBOR_LEFT] = NULL;
				blocks[i]->neighbors[BLOCK_NEIGHBOR_RIGHT]->updateNeighbors();
			}
			if (blocks[i]->neighbors[BLOCK_NEIGHBOR_FRONT] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_FRONT]->neighbors[BLOCK_NEIGHBOR_BACK] = NULL;
				blocks[i]->neighbors[BLOCK_NEIGHBOR_FRONT]->updateNeighbors();
			}
			if (blocks[i]->neighbors[BLOCK_NEIGHBOR_BACK] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_BACK]->neighbors[BLOCK_NEIGHBOR_FRONT] = NULL;
				blocks[i]->neighbors[BLOCK_NEIGHBOR_BACK]->updateNeighbors();
			}
			if (blocks[i]->neighbors[BLOCK_NEIGHBOR_UP] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_UP]->neighbors[BLOCK_NEIGHBOR_DOWN] = NULL;
				blocks[i]->neighbors[BLOCK_NEIGHBOR_UP]->updateNeighbors();
			}
			if (blocks[i]->neighbors[BLOCK_NEIGHBOR_DOWN] != NULL) {
				blocks[i]->neighbors[BLOCK_NEIGHBOR_DOWN]->neighbors[BLOCK_NEIGHBOR_UP] = NULL;
				blocks[i]->neighbors[BLOCK_NEIGHBOR_DOWN]->updateNeighbors();
			}
			delete blocks[i];
		}
	}
	free(blocks);
	free(translations);
	free(translationBlocks);
	free(textures);
}