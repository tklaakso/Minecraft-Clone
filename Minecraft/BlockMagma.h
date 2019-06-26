#pragma once

#include "Block.h"

class BlockMagma : public Block
{
public:
	BlockMagma(int x, int y, int z);
	bool shouldRenderType() override;
	Block* clone() override;
	~BlockMagma();
private:
	static const bool renderType = true;
};

