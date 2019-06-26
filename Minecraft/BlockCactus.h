#pragma once

#include "Block.h"

class BlockCactus : public Block
{
public:
	BlockCactus(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockCactus();
private:
	static const bool renderType = true;
};

