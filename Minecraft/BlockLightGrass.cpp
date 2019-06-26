#include "BlockLightGrass.h"

BlockLightGrass::BlockLightGrass(int x, int y, int z) : Block(BLOCK_LIGHT_GRASS, x, y, z)
{
}

bool BlockLightGrass::shouldRenderType() {
	return renderType;
}

Block* BlockLightGrass::clone() {
	return new BlockLightGrass(x, y, z);
}

BlockLightGrass::~BlockLightGrass()
{
}
