#include "StructureSandHut.h"
#include "WorldGenerator.h"

namespace Structure {

	StructureSandHut::StructureSandHut(std::string filePath) : Structure(filePath)
	{
		setSpawnInterval(512);
		setAttemptsPerInterval(1);
	}

	bool StructureSandHut::canSpawn(int x, int z) {
		return WorldGenerator::getBiome(x, z) == BIOME_DESERT;
	}

	StructureSandHut::~StructureSandHut()
	{
	}

};