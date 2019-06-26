#pragma once

#include "Block.h"

class BlockLightGrass : public Block
{
public:
	BlockLightGrass(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockLightGrass();
private:
	static const bool renderType = true;
};

