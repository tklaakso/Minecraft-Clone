#include "BiomeHeightmapOverworld.h"
#include "Constants.h"
#include "WorldGenerator.h"

BiomeHeightmapOverworld::BiomeHeightmapOverworld()
{

	// Set number of biomes in overworld
	setNumBiomes(4);

}


int BiomeHeightmapOverworld::getHeight(int biome, int x, int z) {

	// Check which overworld biome is being accessed and return the desired height for block x and z
	if (biome == BIOME_FOREST) {

		return 85 + WorldGenerator::noise->GetNoise(x * 2, z * 2) * 10;

	}
	else if (biome == BIOME_DESERT) {

		return 80 + WorldGenerator::noise->GetNoise(x * 2, z * 2) * 2;

	}
	else if (biome == BIOME_JUNGLE) {

		return 90 + WorldGenerator::noise->GetNoise(x * 2, z * 2) * 20;

	}
	else if (biome == BIOME_PLAINS) {

		return 80 + WorldGenerator::noise->GetNoise(x * 2, z * 2) * 2;

	}

}

BiomeHeightmapOverworld::~BiomeHeightmapOverworld()
{
}
