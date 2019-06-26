#pragma once

#include "InvisibleBlock.h"

class BlockAir : public InvisibleBlock
{
public:
	BlockAir(int x, int y, int z);
	Block* clone() override;
	~BlockAir();
};

