#include "Structure.h"
#include "StructureTree.h"
#include "StructureCactus.h"
#include "StructureSandHut.h"
#include "StructureFallTree.h"
#include "CompositeStructure.h"
#include "Util.h"

namespace Structure {

	std::vector<Structure*>* Structure::structures = new std::vector<Structure*>();

	Structure::Structure()
	{
	}

	std::vector<Structure*>* Structure::getStructureList() {
		return structures;
	}

	void Structure::initStructures() {
		structures->push_back(new StructureSandHut());
		structures->push_back(new StructureCactus());
		structures->push_back(new StructureFallTree());
		structures->push_back(new StructureTree());
	}

	void Structure::setSpawnInterval(int spawnInterval) {
		assert(spawnInterval > 0 && spawnInterval <= REGION_WIDTH && REGION_WIDTH % spawnInterval == 0);
		this->spawnInterval = spawnInterval;
	}

	void Structure::setAttemptsPerInterval(int attemptsPerInterval) {
		assert(attemptsPerInterval > 0);
		this->attemptsPerInterval = attemptsPerInterval;
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
	}

};