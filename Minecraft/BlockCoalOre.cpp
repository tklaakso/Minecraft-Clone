#include "BlockCoalOre.h"

BlockCoalOre::BlockCoalOre(int x, int y, int z) : Block(BLOCK_COAL_ORE, x, y, z)
{
}

bool BlockCoalOre::shouldRenderType() {
	return renderType;
}

Block* BlockCoalOre::clone() {
	return new BlockCoalOre(x, y, z);
}

BlockCoalOre::~BlockCoalOre()
{
}
