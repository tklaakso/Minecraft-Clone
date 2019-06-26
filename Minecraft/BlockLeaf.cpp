#include "BlockLeaf.h"

BlockLeaf::BlockLeaf(int x, int y, int z) : Block(BLOCK_LEAF, x, y, z)
{
}

bool BlockLeaf::shouldRenderType() {
	return renderType;
}

Block* BlockLeaf::clone() {
	return new BlockLeaf(x, y, z);
}

BlockLeaf::~BlockLeaf()
{
}
