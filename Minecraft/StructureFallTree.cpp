#include "StructureFallTree.h"
#include "WorldGenerator.h"

namespace Structure {

	StructureFallTree::StructureFallTree() : CompositeStructure(std::vector<std::string> { "Structures/tree.struct", "Structures/tree_orange.struct", "Structures/tree_yellow.struct" })
	{
		setSpawnInterval(16);
		setAttemptsPerInterval(1);
	}

	bool StructureFallTree::canSpawn(int x, int z) {
		return WorldGenerator::getBiome(x, z) == BIOME_JUNGLE;
	}

	StructureFallTree::~StructureFallTree()
	{
	}

}