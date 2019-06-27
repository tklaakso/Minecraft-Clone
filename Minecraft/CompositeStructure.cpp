#include "CompositeStructure.h"
#include "Util.h"

namespace Structure {

	CompositeStructure::CompositeStructure(std::vector<std::string> filePaths) : Structure()
	{
		for (int f = 0; f < filePaths.size(); f++) {
			std::vector<Block*>* currentBlocks = new std::vector<Block*>();
			std::ifstream file;
			std::string filePath = filePaths[f];
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
				if (i == 1 && f == 0) {
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
				Block* b = makeBlock(id, x, y, z, NULL);
				currentBlocks->push_back(b);
			}
			file.close();
			blocks.push_back(currentBlocks);
		}
	}

	std::vector<Block*> CompositeStructure::generate(int x, int y, int z) {
		float noise = (WorldGenerator::noise->GetWhiteNoise(x, z) + 1.0f) / 2.0f;
		for (int i = 0; i < blocks.size(); i++) {
			if (noise <= (float)(i + 1) / blocks.size() || i == blocks.size() - 1) {
				std::vector<Block*>* b = blocks[i];
				std::vector<Block*> generated;
				for (int i = 0; i < b->size(); i++) {
					Block* block = b->at(i);
					Block* gb = makeBlock(block->getId(), block->getX() + x, block->getY() + y, block->getZ() + z, NULL);
					generated.push_back(gb);
				}
				return generated;
			}
		}
	}

	CompositeStructure::~CompositeStructure()
	{
		for (int i = 0; i < blocks.size(); i++) {
			for (int j = 0; j < blocks[i]->size(); j++) {
				delete blocks[i]->at(j);
			}
			delete blocks[i];
		}
	}

}