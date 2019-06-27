#pragma once

#include "Block.h"

class BlockIronOre : public Block
{
public:
	BlockIronOre(int x, int y, int z);
	bool shouldRenderType() override;
	~BlockIronOre();
private:
	static const bool renderType = true;
};

