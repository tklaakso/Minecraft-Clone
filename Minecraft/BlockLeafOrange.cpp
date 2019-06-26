#include "BlockLeafOrange.h"

BlockLeafOrange::BlockLeafOrange(int x, int y, int z) : Block(BLOCK_LEAF_ORANGE, x, y, z)
{
}

bool BlockLeafOrange::shouldRenderType() {
	return renderType;
}

Block* BlockLeafOrange::clone() {
	return new BlockLeafOrange(x, y, z);
}

BlockLeafOrange::~BlockLeafOrange()
{
}
