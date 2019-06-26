#include "InvisibleBlock.h"

InvisibleBlock::InvisibleBlock(int id, int x, int y, int z) : Block(id, x, y, z)
{
	setTransparency(0.0f);
}

bool InvisibleBlock::shouldRenderType() {
	return false;
}

InvisibleBlock::~InvisibleBlock()
{
}
