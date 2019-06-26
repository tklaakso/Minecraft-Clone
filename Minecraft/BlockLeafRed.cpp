#include "BlockLeafRed.h"

BlockLeafRed::BlockLeafRed(int x, int y, int z) : Block(BLOCK_LEAF_RED, x, y, z)
{
}

bool BlockLeafRed::shouldRenderType() {
	return renderType;
}

Block* BlockLeafRed::clone() {
	return new BlockLeafRed(x, y, z);
}

BlockLeafRed::~BlockLeafRed()
{
}
