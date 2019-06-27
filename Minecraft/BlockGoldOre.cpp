#include "BlockGoldOre.h"

BlockGoldOre::BlockGoldOre(int x, int y, int z) : Block(BLOCK_GOLD_ORE, x, y, z)
{
}

bool BlockGoldOre::shouldRenderType() {
	return renderType;
}

BlockGoldOre::~BlockGoldOre()
{
}
