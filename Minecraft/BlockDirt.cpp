#include "BlockDirt.h"

BlockDirt::BlockDirt(int x, int y, int z) : Block(BLOCK_DIRT, x, y, z)
{
}

bool BlockDirt::shouldRenderType() {
	return renderType;
}

BlockDirt::~BlockDirt()
{
}
