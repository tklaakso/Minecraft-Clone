#include "StructureTree.h"
#include "WorldGenerator.h"

namespace Structure {

	StructureTree::StructureTree() : SingularStructure("Structures/tree.struct")
	{
		setSpawnInterval(16);
		setAttemptsPerInterval(4);
	}

	bool StructureTree::canSpawn(int x, int z) {
		return WorldGenerator::getBiome(x, z) == BIOME_FOREST;
	}

	StructureTree::~StructureTree()
	{
	}

};