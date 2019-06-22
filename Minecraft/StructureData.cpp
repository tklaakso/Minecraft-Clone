#include "StructureData.h"



StructureData::StructureData(int x, int y, int z, int w, int h, int l)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	this->h = h;
	this->l = l;
}

int StructureData::getX() const{
	return x;
}

int StructureData::getY() const{
	return y;
}

int StructureData::getZ() const{
	return z;
}

int StructureData::getWidth() const{
	return w;
}

int StructureData::getHeight() const{
	return h;
}

int StructureData::getLength() const{
	return l;
}

bool StructureData::intersects(const StructureData &other) {
	return x + w >= other.getX() && other.getX() + other.getWidth() >= x &&
				 y + h >= other.getY() && other.getY() + other.getHeight() >= y &&
				 z + l >= other.getZ() && other.getZ() + other.getLength() >= l;
}

StructureData::~StructureData()
{
}
