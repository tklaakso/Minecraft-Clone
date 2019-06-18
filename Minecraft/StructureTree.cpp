#include "StructureTree.h"



StructureTree::StructureTree(std::string filePath) : Structure(filePath)
{
	setSpawnInterval(16);
	setAttemptsPerInterval(1);
}

bool StructureTree::canSpawn(int x, int z) {
	return true;
}

StructureTree::~StructureTree()
{
}
