#pragma once

#include "BiomeHeightmap.h"

/*
Represents a biome heightmap for biome generation in the overworld
*/
class BiomeHeightmapOverworld : public BiomeHeightmap
{
public:

	BiomeHeightmapOverworld();

	int getHeight(int biome, int x, int z) override;

	~BiomeHeightmapOverworld();

};

