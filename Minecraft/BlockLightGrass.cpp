#include "BlockLightGrass.h"

BlockLightGrass::BlockLightGrass(int x, int y, int z) : Block(BLOCK_LIGHT_GRASS, x, y, z)
{
}

bool BlockLightGrass::shouldRenderType() {
	return renderType;
}

BlockLightGrass::~BlockLightGrass()
{
}
