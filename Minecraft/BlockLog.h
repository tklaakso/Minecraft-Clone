#pragma once

#include "Block.h"

class BlockLog : public Block
{
public:
	BlockLog(int x, int y, int z);
	bool shouldRenderType() override;
	~BlockLog();
private:
	static const bool renderType = true;
};

