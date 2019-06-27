#pragma once

#include "Block.h"

class BlockSand : public Block
{
public:
	BlockSand(int x, int y, int z);
	bool shouldRenderType() override;
	~BlockSand();
private:
	static const bool renderType = true;
};

