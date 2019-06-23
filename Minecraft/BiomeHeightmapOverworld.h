#pragma once

#include "BiomeHeightmap.h"

class BiomeHeightmapOverworld : public BiomeHeightmap
{
public:
	BiomeHeightmapOverworld();
	int getHeight(int biome, int x, int z) override;
	~BiomeHeightmapOverworld();
};

