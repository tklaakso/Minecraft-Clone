#include "WorldGenerator.h"
#include "Util.h"
#include "World.h"

FastNoise* WorldGenerator::noise = new FastNoise();;

WorldGenerator::WorldGenerator()
{
}

Block** WorldGenerator::getChunkBlocks(int cx, int cy) {
	Block** blocks = (Block**)malloc(sizeof(Block*) * BLOCKS_PER_CHUNK);
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		blocks[i] = NULL;
	}
	std::vector<Block*>* structureBlocks = World::rm->getChunkBlocks(cx, cy);
	if (structureBlocks != NULL) {
		if (structureBlocks->size() > 0) {
			Block* b = (*structureBlocks)[0];
			for (int i = 0; i < structureBlocks->size(); i++) {
				Block* b = (*structureBlocks)[i];
				assert(b->x >= cx * CHUNK_WIDTH && b->x < (cx + 1) * CHUNK_WIDTH);
				assert(b->z >= cy * CHUNK_WIDTH && b->z < (cy + 1) * CHUNK_WIDTH);
				int index = blockCoordsToIndex(b->x - cx * CHUNK_WIDTH, b->y, b->z - cy * CHUNK_WIDTH);
				b->translationIndex = index;
				blocks[index] = b;
			}
		}
		delete structureBlocks;
	}
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_WIDTH; z++) {
			int height = getHeight(x + cx * CHUNK_WIDTH, z + cy * CHUNK_WIDTH);
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				int index = blockCoordsToIndex(x, y, z);
				if (blocks[index] == NULL) {
					if (y < height) {
						Block* b = new Block(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
						int biome = getBiome(x + cx * CHUNK_WIDTH, z + cy * CHUNK_WIDTH);
						if (biome == BIOME_DESERT) {
							b->type = BLOCK_SAND;
						}
						else if (biome == BIOME_PLAINS) {
							if (y < height - 1) {
								b->type = BLOCK_DIRT;
							}
							else {
								b->type = BLOCK_GRASS;
							}
						}
						b->translationIndex = index;
						blocks[index] = b;
					}
				}
			}
		}
	}
	return blocks;
}

int WorldGenerator::getHeight(int x, int z) {
	return 80 + noise->GetNoise(x * 2, z * 2) * 2;
}

int WorldGenerator::getBiome(int x, int z) {
	if (temperatureMap(x, z) < 0) {
		return BIOME_DESERT;
	}
	else {
		return BIOME_PLAINS;
	}
}

float WorldGenerator::temperatureMap(int x, int z) {
	return noise->GetNoise(x, z, 100);
}

WorldGenerator::~WorldGenerator()
{
}
