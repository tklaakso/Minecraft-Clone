#include "BlockSand.h"

BlockSand::BlockSand(int x, int y, int z) : Block(BLOCK_SAND, x, y, z)
{
}

bool BlockSand::shouldRenderType() {
	return renderType;
}

Block* BlockSand::clone() {
	return new BlockSand(x, y, z);
}

BlockSand::~BlockSand()
{
}
