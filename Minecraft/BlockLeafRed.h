#pragma once

#include "Block.h"

class BlockLeafRed : public Block
{
public:
	BlockLeafRed(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockLeafRed();
private:
	static const bool renderType = true;
};

