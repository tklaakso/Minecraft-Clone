#pragma once

#include "Block.h"

class BlockDiamondOre : public Block
{
public:
	BlockDiamondOre(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockDiamondOre();
private:
	static const bool renderType = true;
};

