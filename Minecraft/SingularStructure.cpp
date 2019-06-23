#include "SingularStructure.h"
#include "Util.h"

namespace Structure {

	SingularStructure::SingularStructure(std::string filePath) : Structure()
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

	std::vector<Block*> SingularStructure::generate(int x, int y, int z) {
		std::vector<Block*> generated;
		for (int i = 0; i < blocks.size(); i++) {
			Block* b = blocks[i];
			Block* gb = new Block(b->x + x, b->y + y, b->z + z);
			gb->type = b->type;
			generated.push_back(gb);
		}
		return generated;
	}

	SingularStructure::~SingularStructure()
	{
		for (int i = 0; i < blocks.size(); i++) {
			delete blocks[i];
		}
	}

};