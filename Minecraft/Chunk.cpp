#include "Chunk.h"
#include "Constants.h"
#include "Util.h"
#include "ChunkThreadPool.h"

int Chunk::chunksActive = 0;

Chunk::Chunk(int x, int y, ChunkVAO* vao, int vaoIndex, ChunkThreadPool* pool)
{

	this->pool = pool;

	this->x = x;

	this->y = y;

	this->vao = vao;

	this->vaoIndex = vaoIndex;

	insideViewFrustum = true;

	numBlocks = numBlocksRendered = 0;

	// Chunk starts out by waiting for generation
	state = WAITING_FOR_GENERATE;

	neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);

	// Initialize neighbors to NULL
	for (int i = 0; i < 4; i++) {

		neighbors[i] = NULL;

	}

}

Chunk::Chunk(int x, int y, ChunkThreadPool* pool) {

	this->pool = pool;

	this->x = x;

	this->y = y;

	insideViewFrustum = true;

	numBlocks = numBlocksRendered = 0;

	// Chunk starts out by waiting for generation
	state = WAITING_FOR_GENERATE;

	neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);

	// Initialize neighbors to NULL
	for (int i = 0; i < 4; i++) {

		neighbors[i] = NULL;

	}

}

void Chunk::setVAO(ChunkVAO* vao, int vaoIndex) {

	this->vao = vao;

	this->vaoIndex = vaoIndex;

}

void Chunk::swapBlockIndices(Block* b1, int index1, Block* b2, int index2) {

	glm::vec3 tempTranslation = translations[index1];

	int tempTexture = textures[index1];

	// Swap lightmap indices for each face of blocks
	for (int i = 0; i < 6; i++) {

		int tempLight = lightMap[i][index1];

		lightMap[i][index1] = lightMap[i][index2];

		lightMap[i][index2] = tempLight;

	}

	translations[index1] = translations[index2];

	textures[index1] = textures[index2];

	translations[index2] = tempTranslation;

	textures[index2] = tempTexture;

	translationBlocks[index1] = b2;

	translationBlocks[index2] = b1;

	if (b1 != NULL) {

		b1->setTranslationIndex(index2);

	}

	if (b2 != NULL) {

		b2->setTranslationIndex(index1);

	}

}

Block* Chunk::blockWithTranslationIndex(int translationIndex) {

	return translationBlocks[translationIndex];

}

void Chunk::generate(WorldGenerator* gen) {

	// Chunk is considered to be active following generation
	chunksActive++;

	translations = (glm::vec3*)malloc(sizeof(glm::vec3) * BLOCKS_PER_CHUNK);

	translationBlocks = (Block**)malloc(sizeof(Block*) * BLOCKS_PER_CHUNK);

	textures = (int*)malloc(sizeof(int) * BLOCKS_PER_CHUNK);

	// This is where the "generation" takes place
	blocks = gen->getChunkBlocks(x, y);

	lightMap = (int**)malloc(sizeof(int*) * 6);

	for (int i = 0; i < 6; i++) {

		lightMap[i] = (int*)malloc(sizeof(int) * BLOCKS_PER_CHUNK);

	}

	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {

		translationBlocks[i] = NULL;

		// Initialize lightmap values to 0 by default
		for (int j = 0; j < 6; j++) {

			lightMap[j][i] = 0;

		}

		// Bind blocks to their chunk
		if (blocks[i] != NULL) {

			blocks[i]->setParent(this);

		}

	}

}

void Chunk::updateBlockNeighbors() {

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

			// Create a translation index for this block
			blocks[i]->setTranslationIndex(numBlocks);

			// Make mapping from translation index to translation vector for use in chunk VAO
			translations[numBlocks] = glm::vec3(gx, gy, gz);

			// Make mapping from translation index to block for efficient indexing of blocks based on translation indices
			translationBlocks[numBlocks] = blocks[i];

			// Add texture id to textures array, used by chunk VAO to determine block texture
			textures[numBlocks] = blocks[i]->getId();

			// Initialize lightmap for this block to 0 by default
			for (int j = 0; j < 6; j++) {

				lightMap[j][numBlocks] = 0;

			}

			// A new block has been created so numBlocks and numBlocksRendered must be incremented
			numBlocks++;

			numBlocksRendered++;

		}

	}

}

void Chunk::calculateLighting() {

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

	// Create the physical lightmap for use in chunk VAO
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {

		if (blocks[i] != NULL) {

			for (int j = 0; j < 6; j++) {

				lightMap[j][blocks[i]->getTranslationIndex()] = blocks[i]->getLightValue(j);

			}

		}

	}

}

void Chunk::updateLightingOnRender() {

	this->updateLighting = true;

}

void Chunk::bakeNeighbors() {

	// Update rendered blocks in neighboring chunks (used to hide blocks that are no longer exposed due to creation of this chunk)
	// TODO: Optimize, avoid reordering all blocks in neighboring chunks and instead only ones on exposed edge
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

	// Don't modify this chunk's blocks if it's currently generating, about to be generated, or currently deleting
	if (state == Chunk::WAITING_FOR_GENERATE || state == Chunk::GENERATING || state == Chunk::DELETING) {

		return;

	}
	
	int localX = globalX - x * CHUNK_WIDTH;

	int localZ = globalZ - y * CHUNK_WIDTH;

	// Check to make sure block is inside chunk bounds
	if (localX >= 0 && localX < CHUNK_WIDTH && globalY >= 0 && globalY < CHUNK_HEIGHT && localZ >= 0 && localZ < CHUNK_WIDTH) {

		int index = blockCoordsToIndex(localX, globalY, localZ);

		if (blocks[index] == NULL) {

			if (block != NULL) {

				// If block is not NULL and the block at its position does not exist, we'll insert it into the chunk
				if (freeBlocks.empty()) {

					// If freeBlocks is empty, we'll make a new translation index for this block
					block->setTranslationIndex(numBlocks);

					blocks[index] = block;

					translations[numBlocks] = glm::vec3(globalX, globalY, globalZ);

					translationBlocks[numBlocks] = block;

					textures[numBlocks] = block->getId();

					// Make sure this block will be rendered
					if (numBlocks != numBlocksRendered) {

						swapBlockIndices(block, numBlocks, blockWithTranslationIndex(numBlocksRendered), numBlocksRendered);

					}

					numBlocksRendered++;

					numBlocks++;

					// Optionally, update VAO to render new block
					if (update) {

						updateVAO();

					}

				}
				else {

					// Otherwise retrieve free translation index and use it
					int freeTranslationIndex = freeBlocks.front();

					freeBlocks.pop();

					block->setTranslationIndex(freeTranslationIndex);

					blocks[index] = block;

					translations[freeTranslationIndex] = glm::vec3(globalX, globalY, globalZ);

					translationBlocks[freeTranslationIndex] = block;

					textures[freeTranslationIndex] = block->getId();

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

				// If block already exists in the target position, we'll have to delete it and replace it with this block
				int translationIndex = blocks[index]->getTranslationIndex();

				block->setTranslationIndex(translationIndex);

				delete blocks[index];

				blocks[index] = block;

				translations[translationIndex] = glm::vec3(globalX, globalY, globalZ);

				translationBlocks[translationIndex] = block;

				textures[translationIndex] = block->getId();

				if (block->shouldRenderType()) {

					if (translationIndex >= numBlocksRendered) {

						swapBlockIndices(block, translationIndex, blockWithTranslationIndex(numBlocksRendered), numBlocksRendered);

						numBlocksRendered++;

					}

				}
				else {

					if (translationIndex < numBlocksRendered) {

						swapBlockIndices(block, translationIndex, blockWithTranslationIndex(numBlocksRendered - 1), numBlocksRendered - 1);

						numBlocksRendered--;

					}

				}

				if (update) {

					updateVAO();

				}

			}
			else {
				
				// Finally if block already exists but we're setting it to NULL, we'll simply delete the block in the target position
				int translationIndex = blocks[index]->getTranslationIndex();

				// This is how free translation indices get created (they're used to avoid having to shift the translation index array, which is an O(n) operation)
				freeBlocks.push(translationIndex);

				delete blocks[index];

				blocks[index] = NULL;

				// Setting texture to -1 indicates to fragment shader that it should be discarded
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

	// Go through all blocks and bring renderable block translation indices to the portion of the array that is being rendered
	for (int i = 0; i < numBlocks; i++) {

		Block* b = blockWithTranslationIndex(i);

		if (b != NULL && b->shouldRender()) {

			swapBlockIndices(b, i, blockWithTranslationIndex(iter), iter);

			iter++;

		}

	}

	numBlocksRendered = iter;

}

void Chunk::reorderBlock(Block* block) {

	// Reorder an individual block by moving its translation index to the portion of the array being rendered
	if (block->getTranslationIndex() >= numBlocksRendered) {

		swapBlockIndices(block, block->getTranslationIndex(), blockWithTranslationIndex(numBlocksRendered), numBlocksRendered);

		numBlocksRendered++;

	}

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

	int localX = globalX - x * CHUNK_WIDTH;

	int localZ = globalZ - y * CHUNK_WIDTH;

	// Make sure block index is within chunk bounds
	if (localX >= 0 && localX < CHUNK_WIDTH && globalY >= 0 && globalY < CHUNK_HEIGHT && localZ >= 0 && localZ < CHUNK_WIDTH) {

		return blocks[blockCoordsToIndex(localX, globalY, localZ)];

	}

	return NULL;

}

void Chunk::updateVAO() {

	shouldUpdateVAO = true;

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

void Chunk::updateViewFrustum(ViewFrustum* frustum) {

	insideViewFrustum = frustum->chunkInView(x, y);

}

void Chunk::render() {

	// If the updateLighting flag was set, generate physical lightmap and update VAO to render new lightmap values
	if (updateLighting) {

		makeLightmap();

		updateVAO();

		updateLighting = false;

	}

	// If updating the VAO, set current translation, texture and lightmap data
	if (shouldUpdateVAO) {

		vao->updateTranslationData(translations, numBlocksRendered);

		vao->updateTextureData(textures, numBlocksRendered);

		vao->updateLightmapData(lightMap, numBlocksRendered);

		shouldUpdateVAO = false;

	}

	// Only render if inside view frustum
	if (insideViewFrustum) {

		for (int i = 0; i < 6; i++) {

			vao->bind(i);

			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, numBlocksRendered);

		}

	}

}

Chunk::~Chunk()
{

	// Chunk is no longer active once it is deleted
	chunksActive--;

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

			neighbors[i]->updateVAO();

		}

	}

	free(blocks);

	free(translations);

	free(translationBlocks);

	free(textures);

	for (int i = 0; i < 6; i++) {

		free(lightMap[i]);

	}

	free(lightMap);

	free(neighbors);

}
