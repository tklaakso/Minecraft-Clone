#pragma once

#include "Block.h"

class BlockStone : public Block
{
public:
	BlockStone(int x, int y, int z);
	bool shouldRenderType() override;
	~BlockStone();
private:
	static const bool renderType = true;
};

