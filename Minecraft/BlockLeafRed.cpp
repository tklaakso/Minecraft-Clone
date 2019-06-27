#include "BlockLeafRed.h"

BlockLeafRed::BlockLeafRed(int x, int y, int z) : Block(BLOCK_LEAF_RED, x, y, z)
{
}

bool BlockLeafRed::shouldRenderType() {
	return renderType;
}

BlockLeafRed::~BlockLeafRed()
{
}
