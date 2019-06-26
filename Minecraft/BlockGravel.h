#pragma once

#include "Block.h"

class BlockGravel : public Block
{
public:
	BlockGravel(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockGravel();
private:
	static const bool renderType = true;
};

