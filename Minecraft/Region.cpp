#include "Region.h"

Region::Region(int x, int z)
{
	this->x = x;
	this->z = z;
	blocks = (std::vector<Block*>**)malloc(sizeof(std::vector<Block*>*) * REGION_WIDTH_CHUNKS);
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		blocks[i] = (std::vector<Block*>*)malloc(sizeof(std::vector<Block*>) * REGION_WIDTH_CHUNKS);
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
					float noiseValX = (noise->GetWhiteNoise(i + x * REGION_WIDTH, j + z * REGION_WIDTH, 0) + 1.0f) / 2.0f;
					float noiseValZ = (noise->GetWhiteNoise(i + x * REGION_WIDTH, j + z * REGION_WIDTH, 1) + 1.0f) / 2.0f;
					int noiseBlockX = (int)floor(noiseValX * interval) + i + x * REGION_WIDTH;
					int noiseBlockZ = (int)floor(noiseValZ * interval) + j + z * REGION_WIDTH;
					if (str->canSpawn(noiseBlockX, noiseBlockZ)) {

					}
				}
			}
		}
	}
}

Region::~Region()
{
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		free(blocks[i]);
	}
	free(blocks);
}
