#include "BiomeMap.h"
#include "WorldGenerator.h"
#include "Math.h"

BiomeMap::BiomeMap(BiomeHeightmap* heightMap) : heightMap(heightMap)
{
	numNoiseFunctions = (int)ceil(log2(heightMap->getNumBiomes()));
}

int BiomeMap::getBiome(int x, int z) {
	int biome = 0;
	for (int i = 0; i < numNoiseFunctions; i++) {
		if (WorldGenerator::noise->GetNoise(x / BIOME_SPARSITY, z / BIOME_SPARSITY, i * 100) > 0) {
			biome += pow(2, i);
		}
	}
	biome %= heightMap->getNumBiomes();
	return biome;
}

int BiomeMap::getHeight(int x, int z) {
	int biome = getBiome(x, z);
	float averageHeight = heightMap->getHeight(biome, x, z) + 0.5f;
	float cumulativeWeight = 1.0f;
	for (int i = 0; i < numNoiseFunctions; i++) {
		float noise = WorldGenerator::noise->GetNoise(x / BIOME_SPARSITY, z / BIOME_SPARSITY, i * 100);
		if (abs(noise) <= BIOME_HEIGHT_BLEND) {
			int adjustedBiome;
			if (noise > 0) {
				adjustedBiome = fullMod(biome - pow(2, i), heightMap->getNumBiomes());
			}
			else {
				adjustedBiome = fullMod(biome + pow(2, i), heightMap->getNumBiomes());
			}
			float weight = (BIOME_HEIGHT_BLEND - abs(noise)) / BIOME_HEIGHT_BLEND;
			averageHeight += weight * (heightMap->getHeight(adjustedBiome, x, z) + 0.5f);
			cumulativeWeight += weight;
		}
	}
	averageHeight /= cumulativeWeight;
	return (int)floor(averageHeight);
}

BiomeMap::~BiomeMap()
{
}
