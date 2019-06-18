#pragma once

#include <vector>
#include "Region.h"

class RegionManager
{
public:
	RegionManager();
	~RegionManager();
private:
	std::vector<Region*> regions;
};

