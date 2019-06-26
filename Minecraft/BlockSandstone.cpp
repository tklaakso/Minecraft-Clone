#include "BlockSandstone.h"

BlockSandstone::BlockSandstone(int x, int y, int z) : Block(BLOCK_SANDSTONE, x, y, z)
{
}

bool BlockSandstone::shouldRenderType() {
	return renderType;
}

Block* BlockSandstone::clone() {
	return new BlockSandstone(x, y, z);
}

BlockSandstone::~BlockSandstone()
{
}
