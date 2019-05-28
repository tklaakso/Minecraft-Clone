#include "WorldGenerator.h"
#include "Util.h"

WorldGenerator::WorldGenerator()
{
	noise = new FastNoise();
}

Block** WorldGenerator::getChunkBlocks(int cx, int cy) {
	Block** blocks = (Block**)malloc(sizeof(Block*) * BLOCKS_PER_CHUNK);
	for (int x = 0; x < CHUNK_WIDTH; x++) {
		for (int z = 0; z < CHUNK_WIDTH; z++) {
			int height = 80 + noise->GetNoise((x + cx * CHUNK_WIDTH) * 2, (z + cy * CHUNK_WIDTH) * 2) * 10;
			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				int index = blockCoordsToIndex(x, y, z);
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
				else {
					blocks[index] = NULL;
				}
			}
		}
	}
	return blocks;
}

WorldGenerator::~WorldGenerator()
{
}
