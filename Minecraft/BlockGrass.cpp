#include "BlockGrass.h"

BlockGrass::BlockGrass(int x, int y, int z) : Block(BLOCK_GRASS, x, y, z)
{
}

bool BlockGrass::shouldRenderType() {
	return renderType;
}

Block* BlockGrass::clone() {
	return new BlockGrass(x, y, z);
}

BlockGrass::~BlockGrass()
{
}
