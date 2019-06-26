#include "WorldGenerator.h"
#include "Util.h"
#include "World.h"
#include "BlockAir.h"
#include "BlockDirt.h"
#include "BlockGrass.h"
#include "BlockLeaf.h"
#include "BlockLightGrass.h"
#include "BlockGravel.h"
#include "BlockMagma.h"
#include "BlockWater.h"
#include "BlockGoldOre.h"
#include "BlockIronOre.h"
#include "BlockDiamondOre.h"
#include "BlockCoalOre.h"
#include "BlockLog.h"
#include "BlockSand.h"
#include "BlockCactus.h"
#include "BlockSandstone.h"
#include "BlockLeafOrange.h"
#include "BlockLeafRed.h"

FastNoise* WorldGenerator::noise = new FastNoise();

BiomeHeightmapOverworld* WorldGenerator::heightMap = new BiomeHeightmapOverworld();

BiomeMap* WorldGenerator::biomeMap = new BiomeMap(heightMap);

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
				assert(b->getX() >= cx * CHUNK_WIDTH && b->getX() < (cx + 1) * CHUNK_WIDTH);
				assert(b->getZ() >= cy * CHUNK_WIDTH && b->getZ() < (cy + 1) * CHUNK_WIDTH);
				int index = blockCoordsToIndex(b->getX() - cx * CHUNK_WIDTH, b->getY(), b->getZ() - cy * CHUNK_WIDTH);
				b->setTranslationIndex(index);
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
						Block* b = NULL;
						int biome = getBiome(x + cx * CHUNK_WIDTH, z + cy * CHUNK_WIDTH);
						if (biome == BIOME_DESERT) {
							b = new BlockSand(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
						}
						else if (biome == BIOME_FOREST) {
							if (y < height - 1) {
								b = new BlockDirt(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
							}
							else {
								b = new BlockGrass(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
							}
						}
						else if (biome == BIOME_JUNGLE) {
							if (y < height - 1) {
								b = new BlockDirt(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
							}
							else {
								b = new BlockLightGrass(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
							}
						}
						else if (biome == BIOME_PLAINS) {
							if (y < height - 1) {
								b = new BlockDirt(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
							}
							else {
								b = new BlockGrass(x + cx * CHUNK_WIDTH, y, z + cy * CHUNK_WIDTH);
							}
						}
						b->setTranslationIndex(index);
						blocks[index] = b;
					}
				}
			}
		}
	}
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		BlockCoords coords = indexToBlockCoords(i);
		if (blocks[i] == NULL) {
			Block* air = new BlockAir(coords.x + cx * CHUNK_WIDTH, coords.y, coords.z + cy * CHUNK_WIDTH);
			air->setTranslationIndex(i);
			blocks[i] = air;
		}
	}
	return blocks;
}

int WorldGenerator::getHeight(int x, int z) {
	return biomeMap->getHeight(x, z);
}

int WorldGenerator::getBiome(int x, int z) {
	return biomeMap->getBiome(x, z);
}

WorldGenerator::~WorldGenerator()
{
}
