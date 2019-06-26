#pragma once

#include "Block.h"

class BlockGoldOre : public Block
{
public:
	BlockGoldOre(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockGoldOre();
private:
	static const bool renderType = true;
};

