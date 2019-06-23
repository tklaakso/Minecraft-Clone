#pragma once

#include <vector>
#include "Block.h"
#include "StructureData.h"

namespace Structure {
	class Structure
	{
	public:
		Structure();
		static void initStructures();
		static std::vector<Structure*>* getStructureList();
		virtual bool canSpawn(int x, int z) = 0;
		virtual std::vector<Block*> generate(int x, int y, int z) = 0;
		int getMinX();
		int getMaxX();
		int getMinZ();
		int getMaxZ();
		int getMinY();
		int getMaxY();
		int getSpawnInterval();
		int getSpawnAttemptsPerInterval();
		StructureData getData(int x, int y, int z);
		~Structure();
	protected:
		void setSpawnInterval(int spawnInterval);
		void setAttemptsPerInterval(int attemptsPerInterval);
		int minx, maxx, minz, maxz, miny, maxy;
	private:
		static std::vector<Structure*>* structures;
		int spawnInterval = REGION_WIDTH;
		int attemptsPerInterval = 1;
	};
};
