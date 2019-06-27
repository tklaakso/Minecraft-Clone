#pragma once

#include "Block.h"

class BlockLeafOrange : public Block
{
public:
	BlockLeafOrange(int x, int y, int z);
	bool shouldRenderType() override;
	~BlockLeafOrange();
private:
	static const bool renderType = true;
};

