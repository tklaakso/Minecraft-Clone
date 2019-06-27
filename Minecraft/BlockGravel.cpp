#include "BlockGravel.h"

BlockGravel::BlockGravel(int x, int y, int z) : Block(BLOCK_GRAVEL, x, y, z)
{
}

bool BlockGravel::shouldRenderType() {
	return renderType;
}

BlockGravel::~BlockGravel()
{
}
