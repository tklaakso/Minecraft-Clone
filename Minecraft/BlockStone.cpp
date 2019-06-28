#include "BlockStone.h"

BlockStone::BlockStone(int x, int y, int z) : Block(BLOCK_STONE, x, y, z)
{
}

bool BlockStone::shouldRenderType() {
	return renderType;
}

BlockStone::~BlockStone()
{
}
