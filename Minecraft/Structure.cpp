#include "Structure.h"
#include "StructureTree.h"
#include "StructureCactus.h"
#include "StructureSandHut.h"
#include "Util.h"

namespace Structure {

	std::vector<Structure*>* Structure::structures = new std::vector<Structure*>();

	Structure::Structure(std::string filePath)
	{
		std::ifstream file;
		file.open(filePath);
		std::vector<std::string> lines;
		while (!file.eof()) {
			std::string temp;
			file >> temp;
			if (temp.length() > 0) {
				lines.push_back(temp);
			}
		}
		for (int i = 1; i < lines.size(); i++) {
			std::vector<std::string> params = split(lines[i], ",");
			int id = std::stoi(params[0]);
			int x = std::stoi(params[1]);
			int y = std::stoi(params[2]);
			int z = std::stoi(params[3]);
			if (i == 1) {
				minx = maxx = x;
				miny = maxy = y;
				minz = maxz = z;
			}
			else {
				if (x < minx) {
					minx = x;
				}
				if (x > maxx) {
					maxx = x;
				}
				if (y < miny) {
					miny = y;
				}
				if (y > maxy) {
					maxy = y;
				}
				if (z < minz) {
					minz = z;
				}
				if (z > maxz) {
					maxz = z;
				}
			}
			Block* b = new Block(x, y, z);
			b->type = id;
			blocks.push_back(b);
		}
		file.close();
	}

	std::vector<Structure*>* Structure::getStructureList() {
		return structures;
	}

	void Structure::initStructures() {
		structures->push_back(new StructureSandHut("Structures/sand_hut.struct"));
		structures->push_back(new StructureCactus("Structures/cactus.struct"));
		structures->push_back(new StructureTree("Structures/tree.struct"));
	}

	void Structure::setSpawnInterval(int spawnInterval) {
		assert(spawnInterval > 0 && spawnInterval <= REGION_WIDTH && REGION_WIDTH % spawnInterval == 0);
		this->spawnInterval = spawnInterval;
	}

	void Structure::setAttemptsPerInterval(int attemptsPerInterval) {
		assert(attemptsPerInterval > 0);
		this->attemptsPerInterval = attemptsPerInterval;
	}

	std::vector<Block*> Structure::generate(int x, int y, int z) {
		std::vector<Block*> generated;
		for (int i = 0; i < blocks.size(); i++) {
			Block* b = blocks[i];
			Block* gb = new Block(b->x + x, b->y + y, b->z + z);
			gb->type = b->type;
			generated.push_back(gb);
		}
		return generated;
	}

	int Structure::getSpawnInterval() {
		return spawnInterval;
	}

	int Structure::getSpawnAttemptsPerInterval() {
		return attemptsPerInterval;
	}

	int Structure::getMinX() {
		return minx;
	}
	int Structure::getMaxX() {
		return maxx;
	}
	int Structure::getMinZ() {
		return minz;
	}
	int Structure::getMaxZ() {
		return maxz;
	}
	int Structure::getMinY() {
		return miny;
	}
	int Structure::getMaxY() {
		return maxy;
	}

	StructureData Structure::getData(int x, int y, int z) {
		return StructureData(minx + x, miny + y, minz + z, maxx - minx, maxy - miny, maxz - minz);
	}

	Structure::~Structure()
	{
		for (int i = 0; i < blocks.size(); i++) {
			delete blocks[i];
		}
	}

};