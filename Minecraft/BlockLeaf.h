#pragma once

#include "Block.h"

class BlockLeaf : public Block
{
public:
	BlockLeaf(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockLeaf();
private:
	static const bool renderType = true;
};

