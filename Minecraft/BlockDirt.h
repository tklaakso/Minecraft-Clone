#pragma once

#include "Block.h"

class BlockDirt : public Block
{
public:
	BlockDirt(int x, int y, int z);
	bool shouldRenderType() override;
	~BlockDirt();
private:
	static const bool renderType = true;
};

