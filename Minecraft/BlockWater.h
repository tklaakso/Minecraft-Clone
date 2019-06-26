#pragma once

#include "Block.h"

class BlockWater : public Block
{
public:
	BlockWater(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockWater();
private:
	static const bool renderType = true;
};

