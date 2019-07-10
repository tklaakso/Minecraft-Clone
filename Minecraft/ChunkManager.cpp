#include "ChunkManager.h"
#include "Util.h"

ChunkManager::ChunkManager()
{

	vao = new ChunkVAO();

	generator = new WorldGenerator();

	chunks = new std::vector<Chunk*>();
	
	numBlocks = numBlocksRendered = 0;

	blocks = (Block**)malloc(sizeof(Block*) * BLOCKS_PER_CHUNK * CHUNK_CAP);

	translations = (glm::vec3*)malloc(sizeof(glm::vec3) * BLOCKS_PER_CHUNK * CHUNK_CAP);

	translationBlocks = (Block**)malloc(sizeof(Block*) * BLOCKS_PER_CHUNK * CHUNK_CAP);

	textures = (int*)malloc(sizeof(int) * BLOCKS_PER_CHUNK * CHUNK_CAP);

	lightMap = (int**)malloc(sizeof(int*) * 6);

	for (int i = 0; i < 6; i++) {

		lightMap[i] = (int*)malloc(sizeof(int) * BLOCKS_PER_CHUNK * CHUNK_CAP);

	}

	for (int i = 0; i < BLOCKS_PER_CHUNK * CHUNK_CAP; i++) {

		translationBlocks[i] = NULL;

		// Initialize lightmap values to 0 by default
		for (int j = 0; j < 6; j++) {

			lightMap[j][i] = 0;

		}
		
	}

}

void ChunkManager::swapBlockIndices(Block* b1, int index1, Block* b2, int index2) {

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

void ChunkManager::setChunkBlocks(Block** blocks, int chunkId) {

	for (int i = chunkId * BLOCKS_PER_CHUNK; i < (chunkId + 1) * BLOCKS_PER_CHUNK; i++) {

		this->blocks[i] = blocks[i - chunkId * BLOCKS_PER_CHUNK];

	}

}

void ChunkManager::makeLightmap(int chunkId) {

	// Create the physical lightmap for use in chunk VAO
	for (int i = chunkId * BLOCKS_PER_CHUNK; i < (chunkId + 1) * BLOCKS_PER_CHUNK; i++) {

		if (blocks[i] != NULL) {

			for (int j = 0; j < 6; j++) {

				lightMap[j][blocks[i]->getTranslationIndex()] = blocks[i]->getLightValue(j);

			}

		}

	}

}

void ChunkManager::deleteChunkBlocks(int chunkId) {

	for (int i = chunkId * BLOCKS_PER_CHUNK; i < (chunkId + 1) * BLOCKS_PER_CHUNK; i++) {

		if (blocks[i] != NULL) {

			delete blocks[i];

		}

	}

}

void ChunkManager::bindChunkBlocks(Chunk* chunk, int chunkId) {

	for (int i = chunkId * BLOCKS_PER_CHUNK; i < (chunkId + 1) * BLOCKS_PER_CHUNK; i++) {

		if (blocks[i] != NULL) {

			blocks[i]->setParent(chunk);

		}

	}

}

Block** ChunkManager::getBlocks(int chunkId) {
	
	return blocks + chunkId * BLOCKS_PER_CHUNK;

}

void ChunkManager::initBlock(Block* b) {

	int translationIndex = numBlocks;

	if (!freeBlocks.empty()) {

		translationIndex = freeBlocks.front();

		freeBlocks.pop();

	}
	
	// Create a translation index for this block
	b->setTranslationIndex(translationIndex);

	// Make mapping from translation index to translation vector for use in chunk VAO
	translations[translationIndex] = glm::vec3(b->getX(), b->getY(), b->getZ());

	// Make mapping from translation index to block for efficient indexing of blocks based on translation indices
	translationBlocks[translationIndex] = b;

	// Add texture id to textures array, used by chunk VAO to determine block texture
	textures[translationIndex] = b->getId();

	// Initialize lightmap for this block to 0 by default
	for (int j = 0; j < 6; j++) {

		lightMap[j][translationIndex] = 0;

	}
	
	// A new block has been created so numBlocks and numBlocksRendered must be incremented
	numBlocks++;

	numBlocksRendered++;

}

void ChunkManager::setBlock(Chunk* c, int globalX, int globalY, int globalZ, Block* block, bool update) {

	int localX = globalX - c->x * CHUNK_WIDTH;

	int localZ = globalZ - c->y * CHUNK_WIDTH;

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

						swapBlockIndices(block, numBlocks, translationBlocks[numBlocksRendered], numBlocksRendered);

					}

					numBlocksRendered++;

					numBlocks++;

					// Optionally, update VAO to render new block
					if (update) {

						shouldUpdateVAO = true;

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

						swapBlockIndices(block, freeTranslationIndex, translationBlocks[numBlocksRendered], numBlocksRendered);

						numBlocksRendered++;

					}

					if (update) {

						shouldUpdateVAO = true;

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

						swapBlockIndices(block, translationIndex, translationBlocks[numBlocksRendered], numBlocksRendered);

						numBlocksRendered++;

					}

				}
				else {

					if (translationIndex < numBlocksRendered) {

						swapBlockIndices(block, translationIndex, translationBlocks[numBlocksRendered - 1], numBlocksRendered - 1);

						numBlocksRendered--;

					}

				}

				if (update) {

					shouldUpdateVAO = true;

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

void ChunkManager::reorderBlock(Block* block) {

	// Reorder an individual block by moving its translation index to the portion of the array being rendered
	if (block->shouldRender() && block->getTranslationIndex() >= numBlocksRendered) {

		swapBlockIndices(block, block->getTranslationIndex(), translationBlocks[numBlocksRendered], numBlocksRendered);

		numBlocksRendered++;

	}
	else if (!block->shouldRender() && block->getTranslationIndex() < numBlocksRendered) {

		swapBlockIndices(block, block->getTranslationIndex(), translationBlocks[numBlocksRendered - 1], numBlocksRendered - 1);

		numBlocksRendered--;

	}

}

void ChunkManager::reorderBlocks(int chunkId) {

	// Go through all blocks and bring renderable block translation indices to the portion of the array that is being rendered
	for (int i = chunkId * BLOCKS_PER_CHUNK; i < (chunkId + 1) * BLOCKS_PER_CHUNK; i++) {

		Block* b = blocks[i];

		reorderBlock(b);

	}

}

void ChunkManager::updateVAO() {

	shouldUpdateVAO = true;

}

void ChunkManager::render() {

	// If updating the VAO, set current translation, texture and lightmap data
	if (shouldUpdateVAO) {

		vao->updateTranslationData(translations, numBlocksRendered);

		vao->updateTextureData(textures, numBlocksRendered);

		vao->updateLightmapData(lightMap, numBlocksRendered);

		shouldUpdateVAO = false;

	}

	for (int i = 0; i < 6; i++) {

		vao->bind(i);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, numBlocksRendered);

	}

}

ChunkManager::~ChunkManager()
{

	delete vao;

	delete generator;
	
	delete chunks;

	free(blocks);

	free(translations);

	free(translationBlocks);

	free(textures);

	for (int i = 0; i < 6; i++) {

		free(lightMap[i]);

	}

	free(lightMap);

}
