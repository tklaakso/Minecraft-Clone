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
		Block* b = (*structureBlocks)[0];
		for (int i = 0; i < structureBlocks->size(); i++) {
			Block* b = (*structureBlocks)[i];
			assert(b->x >= cx * CHUNK_WIDTH && b->x < (cx + 1) * CHUNK_WIDTH);
			assert(b->z >= cy * CHUNK_WIDTH && b->z < (cy + 1) * CHUNK_WIDTH);
			int index = blockCoordsToIndex(b->x - cx * CHUNK_WIDTH, b->y, b->z - cy * CHUNK_WIDTH);
			b->translationIndex = index;
			blocks[index] = b;
		}
		delete structureBlocks;
	}
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_WIDTH; z++) {
			int height = 80 + noise->GetNoise((x + cx * CHUNK_WIDTH) * 2, (z + cy * CHUNK_WIDTH) * 2) * 10;
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				int index = blockCoordsToIndex(x, y, z);
				if (blocks[index] == NULL) {
					if (y < height) {
						Block* b = new Block(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
						if (y < height - 1) {
							b->type = 0;
						}
						else {
							b->type = 1;
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
	return 80 + noise->GetNoise(x * 2, z * 2) * 10;
}

WorldGenerator::~WorldGenerator()
{
}
