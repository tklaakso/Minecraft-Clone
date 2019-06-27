#include "Region.h"
#include "Util.h"

Region::Region(int x, int z)
{
	this->x = x;
	this->z = z;
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		for (int j = 0; j < REGION_WIDTH_CHUNKS; j++) {
			blocks[i][j] = new std::vector<Block*>();
			structureMap[i][j] = new std::vector<StructureData>();
		}
	}
}

std::vector<std::vector<StructureData>*> Region::getOverlapping(StructureData d) {
	std::vector<std::vector<StructureData>*> overlapping;
	ChunkCoords min = blockToChunkCoords(d.getX(), d.getZ());
	ChunkCoords max = blockToChunkCoords(d.getX() + d.getWidth(), d.getZ() + d.getLength());
	int chunkXLocalMin = min.chunkX - x * REGION_WIDTH_CHUNKS;
	int chunkZLocalMin = min.chunkZ - z * REGION_WIDTH_CHUNKS;
	int chunkXLocalMax = max.chunkX - x * REGION_WIDTH_CHUNKS;
	int chunkZLocalMax = max.chunkZ - z * REGION_WIDTH_CHUNKS;
	assert(chunkXLocalMin >= 0 && chunkXLocalMin < REGION_WIDTH_CHUNKS);
	assert(chunkXLocalMax >= 0 && chunkXLocalMax < REGION_WIDTH_CHUNKS);
	assert(chunkZLocalMin >= 0 && chunkZLocalMin < REGION_WIDTH_CHUNKS);
	assert(chunkZLocalMax >= 0 && chunkZLocalMax < REGION_WIDTH_CHUNKS);
	for (int cx = chunkXLocalMin; cx <= chunkXLocalMax; cx++) {
		for (int cz = chunkZLocalMin; cz <= chunkZLocalMax; cz++) {
			overlapping.push_back(structureMap[cx][cz]);
		}
	}
	return overlapping;
}

bool Region::canCreate(StructureData d) {
	std::vector<std::vector<StructureData>*> overlapping = getOverlapping(d);
	for (int i = 0; i < overlapping.size(); i++) {
		std::vector<StructureData>* str = overlapping[i];
		for (int j = 0; j < str->size(); j++) {
			if (d.intersects((*str)[j])) {
				return false;
			}
		}
	}
	return true;
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
						StructureData data = str->getData(noiseBlockX, noiseBlockY, noiseBlockZ);
						if (canCreate(data)) {
							std::vector<Block*> bl = str->generate(noiseBlockX, noiseBlockY, noiseBlockZ);
							for (int l = 0; l < bl.size(); l++) {
								Block* b = bl[l];
								ChunkCoords c = blockToChunkCoords(b->getX(), b->getZ());
								int regionChunkX = c.chunkX - x * REGION_WIDTH_CHUNKS;
								int regionChunkZ = c.chunkZ - z * REGION_WIDTH_CHUNKS;
								assert(regionChunkX >= 0 && regionChunkX < REGION_WIDTH_CHUNKS);
								assert(regionChunkZ >= 0 && regionChunkZ < REGION_WIDTH_CHUNKS);
								blocks[regionChunkX][regionChunkZ]->push_back(b);
							}
							std::vector<std::vector<StructureData>*> overlapping = getOverlapping(data);
							for (int l = 0; l < overlapping.size(); l++) {
								overlapping[l]->push_back(data);
							}
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
	for (int i = 0; i < blocks[chunkX][chunkZ]->size(); i++) {
		b->push_back(clone(blocks[chunkX][chunkZ]->at(i)));
	}
	return b;
}

Region::~Region()
{
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		for (int j = 0; j < REGION_WIDTH_CHUNKS; j++) {
			for (int k = 0; k < blocks[i][j]->size(); k++) {
				delete blocks[i][j]->at(k);
			}
			delete blocks[i][j];
			delete structureMap[i][j];
		}
	}
}
