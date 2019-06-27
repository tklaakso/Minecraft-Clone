#include "BlockGrass.h"

BlockGrass::BlockGrass(int x, int y, int z) : Block(BLOCK_GRASS, x, y, z)
{
}

bool BlockGrass::shouldRenderType() {
	return renderType;
}

BlockGrass::~BlockGrass()
{
}
