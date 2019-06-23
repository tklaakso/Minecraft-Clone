#include "StructureSandHut.h"
#include "WorldGenerator.h"

namespace Structure {

	StructureSandHut::StructureSandHut() : SingularStructure("Structures/sand_hut.struct")
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