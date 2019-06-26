#pragma once

#include "Block.h"

class BlockSandstone : public Block
{
public:
	BlockSandstone(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockSandstone();
private:
	static const bool renderType = true;
};

