#include "BlockWater.h"

BlockWater::BlockWater(int x, int y, int z) : Block(BLOCK_WATER, x, y, z)
{
}

bool BlockWater::shouldRenderType() {
	return renderType;
}

BlockWater::~BlockWater()
{
}
