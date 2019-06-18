#pragma once

#include <vector>
#include "Block.h"
#include "Util.h"

namespace Structure {
	class Structure
	{
	public:
		Structure(std::string filePath);
		static void initStructures();
		static std::vector<Structure*>* getStructureList();
		virtual bool canSpawn(int x, int z) = 0;
		std::vector<Block*> generate(int x, int y, int z);
		int getMinX();
		int getMaxX();
		int getMinZ();
		int getMaxZ();
		int getMinY();
		int getMaxY();
		int getSpawnInterval();
		int getSpawnAttemptsPerInterval();
		~Structure();
	protected:
		void setSpawnInterval(int spawnInterval);
		void setAttemptsPerInterval(int attemptsPerInterval);
	private:
		static std::vector<Structure*>* structures;
		int minx, maxx, minz, maxz, miny, maxy;
		int spawnInterval = REGION_WIDTH;
		int attemptsPerInterval = 1;
		std::vector<Block*> blocks;
	};
};
