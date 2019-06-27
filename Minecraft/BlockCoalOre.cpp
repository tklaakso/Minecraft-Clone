#include "BlockCoalOre.h"

BlockCoalOre::BlockCoalOre(int x, int y, int z) : Block(BLOCK_COAL_ORE, x, y, z)
{
}

bool BlockCoalOre::shouldRenderType() {
	return renderType;
}

BlockCoalOre::~BlockCoalOre()
{
}
