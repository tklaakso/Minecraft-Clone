#include "BlockGoldOre.h"

BlockGoldOre::BlockGoldOre(int x, int y, int z) : Block(BLOCK_GOLD_ORE, x, y, z)
{
}

bool BlockGoldOre::shouldRenderType() {
	return renderType;
}

Block* BlockGoldOre::clone() {
	return new BlockGoldOre(x, y, z);
}

BlockGoldOre::~BlockGoldOre()
{
}
