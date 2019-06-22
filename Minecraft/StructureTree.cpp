#include "StructureTree.h"

namespace Structure {

	StructureTree::StructureTree(std::string filePath) : Structure(filePath)
	{
		setSpawnInterval(16);
		setAttemptsPerInterval(4);
	}

	bool StructureTree::canSpawn(int x, int z) {
		return true;
	}

	StructureTree::~StructureTree()
	{
	}

};