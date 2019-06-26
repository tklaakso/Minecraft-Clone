#include "BlockDirt.h"

BlockDirt::BlockDirt(int x, int y, int z) : Block(BLOCK_DIRT, x, y, z)
{
}

bool BlockDirt::shouldRenderType() {
	return renderType;
}

Block* BlockDirt::clone() {
	return new BlockDirt(x, y, z);
}

BlockDirt::~BlockDirt()
{
}
