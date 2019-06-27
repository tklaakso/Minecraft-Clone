#include "BlockIronOre.h"

BlockIronOre::BlockIronOre(int x, int y, int z) : Block(BLOCK_IRON_ORE, x, y, z)
{
}

bool BlockIronOre::shouldRenderType() {
	return renderType;
}

BlockIronOre::~BlockIronOre()
{
}
