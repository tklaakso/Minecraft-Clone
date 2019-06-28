#include "BlockCastInfo.h"
#include "Block.h"

BlockCastInfo::BlockCastInfo(Block* block, int face)
{
	this->block = block;
	this->face = face;
}


BlockCastInfo::~BlockCastInfo()
{
}
