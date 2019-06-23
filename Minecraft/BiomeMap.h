#pragma once

#include "BiomeHeightmap.h"
#include <vector>
#include <cmath>

class BiomeMap
{
public:
	BiomeMap(BiomeHeightmap* heightMap);
	int getBiome(int x, int z);
	int getHeight(int x, int z);
	~BiomeMap();
private:
	BiomeHeightmap* heightMap;
	int numNoiseFunctions;
};

