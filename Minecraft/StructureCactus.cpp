#include "StructureCactus.h"
#include "WorldGenerator.h"

namespace Structure {

	StructureCactus::StructureCactus() : SingularStructure("Structures/cactus.struct")
	{
		setSpawnInterval(32);
		setAttemptsPerInterval(2);
	}

	bool StructureCactus::canSpawn(int x, int z) {
		return WorldGenerator::getBiome(x, z) == BIOME_DESERT;
	}

	StructureCactus::~StructureCactus()
	{
	}

};