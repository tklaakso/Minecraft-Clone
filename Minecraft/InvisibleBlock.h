#pragma once

#include "Block.h"

class InvisibleBlock : public Block
{
public:
	InvisibleBlock(int id, int x, int y, int z);
	bool shouldRenderType() override;
	~InvisibleBlock();
};

