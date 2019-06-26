#include "InvisibleBlock.h"

InvisibleBlock::InvisibleBlock(int id, int x, int y, int z) : Block(id, x, y, z)
{

}

bool InvisibleBlock::shouldRenderType() {
	return false;
}

InvisibleBlock::~InvisibleBlock()
{
}
