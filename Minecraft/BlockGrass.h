#pragma once

#include "Block.h"

class BlockGrass : public Block
{
public:
	BlockGrass(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockGrass();
private:
	static const bool renderType = true;
};

