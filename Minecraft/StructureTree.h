#pragma once

#include "Structure.h"

class StructureTree : public Structure
{
public:
	StructureTree(std::string filePath);
	bool canSpawn(int x, int z) override;
	~StructureTree();
};

