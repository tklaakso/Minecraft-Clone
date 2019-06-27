#include "BlockDiamondOre.h"

BlockDiamondOre::BlockDiamondOre(int x, int y, int z) : Block(BLOCK_DIAMOND_ORE, x, y, z)
{
}

bool BlockDiamondOre::shouldRenderType() {
	return renderType;
}

BlockDiamondOre::~BlockDiamondOre()
{
}
