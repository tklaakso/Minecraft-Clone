#include "BlockLeaf.h"

BlockLeaf::BlockLeaf(int x, int y, int z) : Block(BLOCK_LEAF, x, y, z)
{
}

bool BlockLeaf::shouldRenderType() {
	return renderType;
}

BlockLeaf::~BlockLeaf()
{
}
