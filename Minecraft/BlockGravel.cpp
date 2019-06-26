#include "BlockGravel.h"

BlockGravel::BlockGravel(int x, int y, int z) : Block(BLOCK_GRAVEL, x, y, z)
{
}

bool BlockGravel::shouldRenderType() {
	return renderType;
}

Block* BlockGravel::clone() {
	return new BlockGravel(x, y, z);
}

BlockGravel::~BlockGravel()
{
}
