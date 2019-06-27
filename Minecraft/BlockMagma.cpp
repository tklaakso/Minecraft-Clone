#include "BlockMagma.h"

BlockMagma::BlockMagma(int x, int y, int z) : Block(BLOCK_MAGMA, x, y, z)
{
}

bool BlockMagma::shouldRenderType() {
	return renderType;
}

BlockMagma::~BlockMagma()
{
}
