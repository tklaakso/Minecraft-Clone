#include "Region.h"
#include "Util.h"

Region::Region(int x, int z)
{
	this->x = x;
	this->z = z;
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		for (int j = 0; j < REGION_WIDTH_CHUNKS; j++) {
			blocks[i][j] = std::vector<Block*>();
		}
	}
}

void Region::generate() {
	std::vector<Structure::Structure*>* structures = Structure::Structure::getStructureList();
	FastNoise* noise = new FastNoise();
	for (int s = 0; s < structures->size(); s++) {
		Structure::Structure* str = (*structures)[s];
		int interval = str->getSpawnInterval();
		int attempts = str->getSpawnAttemptsPerInterval();
		for (int i = 0; i < REGION_WIDTH; i += interval) {
			for (int j = 0; j < REGION_WIDTH; j += interval) {
				for (int k = 0; k < attempts; k++) {
					float noiseValX = (noise->GetWhiteNoise(i + x * REGION_WIDTH, j + z * REGION_WIDTH, k * 2) + 1.0f) / 2.0f;
					float noiseValZ = (noise->GetWhiteNoise(i + x * REGION_WIDTH, j + z * REGION_WIDTH, k * 2 + 1) + 1.0f) / 2.0f;
					int noiseBlockX = (int)floor(noiseValX * (interval + (str->getMinX() - str->getMaxX()))) + i - str->getMinX() + x * REGION_WIDTH;
					int noiseBlockZ = (int)floor(noiseValZ * (interval + (str->getMinZ() - str->getMaxZ()))) + j - str->getMinZ() + z * REGION_WIDTH;
					int noiseBlockY = WorldGenerator::getHeight(noiseBlockX, noiseBlockZ);
					if (str->canSpawn(noiseBlockX, noiseBlockZ)) {
						std::vector<Block*> bl = str->generate(noiseBlockX, noiseBlockY, noiseBlockZ);
						bool blockExists = false;
						for (int l = 0; l < bl.size(); l++) {
							Block* b = bl[l];
							ChunkCoords c = blockToChunkCoords(b->x, b->z);
							int regionChunkX = c.chunkX - x * REGION_WIDTH_CHUNKS;
							int regionChunkZ = c.chunkZ - z * REGION_WIDTH_CHUNKS;
							assert(regionChunkX >= 0 && regionChunkX < REGION_WIDTH_CHUNKS);
							assert(regionChunkZ >= 0 && regionChunkZ < REGION_WIDTH_CHUNKS);
							std::vector<Block*> existingBlocks = blocks[regionChunkX][regionChunkZ];
							for (int m = 0; m < existingBlocks.size(); m++) {
								if (existingBlocks[m]->x == b->x && existingBlocks[m]->y == b->y && existingBlocks[m]->z == b->z) {
									blockExists = true;
									break;
								}
							}
							if (blockExists) {
								break;
							}
						}
						if (blockExists) {
							continue;
						}
						for (int l = 0; l < bl.size(); l++) {
							Block* b = bl[l];
							ChunkCoords c = blockToChunkCoords(b->x, b->z);
							int regionChunkX = c.chunkX - x * REGION_WIDTH_CHUNKS;
							int regionChunkZ = c.chunkZ - z * REGION_WIDTH_CHUNKS;
							assert(regionChunkX >= 0 && regionChunkX < REGION_WIDTH_CHUNKS);
							assert(regionChunkZ >= 0 && regionChunkZ < REGION_WIDTH_CHUNKS);
							blocks[regionChunkX][regionChunkZ].push_back(b);
						}
					}
				}
			}
		}
	}
}

int Region::getX() {
	return x;
}

int Region::getZ() {
	return z;
}

std::vector<Block*>* Region::getChunkBlocks(int cx, int cz) {
	int chunkX = cx - x * REGION_WIDTH_CHUNKS;
	int chunkZ = cz - z * REGION_WIDTH_CHUNKS;
	assert(chunkX >= 0 && chunkX < REGION_WIDTH_CHUNKS);
	assert(chunkZ >= 0 && chunkZ < REGION_WIDTH_CHUNKS);
	std::vector<Block*>* b = new std::vector<Block*>();
	for (int i = 0; i < blocks[chunkX][chunkZ].size(); i++) {
		b->push_back(blocks[chunkX][chunkZ][i]->clone());
	}
	return b;
}

Region::~Region()
{
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		for (int j = 0; j < REGION_WIDTH_CHUNKS; j++) {
			for (int k = 0; k < blocks[i][j].size(); k++) {
				delete blocks[i][j][k];
			}
		}
	}
}
