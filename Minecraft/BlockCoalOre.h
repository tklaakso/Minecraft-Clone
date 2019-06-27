#pragma once

#include "Block.h"

class BlockCoalOre : public Block
{
public:
	BlockCoalOre(int x, int y, int z);
	bool shouldRenderType() override;
	~BlockCoalOre();
private:
	static const bool renderType = true;
};

