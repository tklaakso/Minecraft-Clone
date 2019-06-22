#pragma once
class StructureData
{
public:
	StructureData(int x, int y, int z, int w, int h, int l);
	~StructureData();
	int getX() const;
	int getY() const;
	int getZ() const;
	int getWidth() const;
	int getHeight() const;
	int getLength() const;
	bool intersects(const StructureData &other);
private:
	int x, y, z, w, h, l;
};

