#include "Block.h"
#include "Util.h"

unsigned int Block::texture = 0;

void Block::initialize(Shader* shader) {

	unsigned char* textureData = (unsigned char*)malloc(sizeof(unsigned char) * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * NUM_TEXTURES * 3);

	stbi_set_flip_vertically_on_load(true);

	for (int i = 0; i < NUM_TEXTURES; i++) {

		int width, height, nChannels;

		string strIndex = toString(i);

		string path = std::string("Resources/") + strIndex + std::string(".png");

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
		for (int x = i * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3; x < (i + 1) * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3; x++) {
			textureData[x] = data[x - i * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3];
		}

		stbi_image_free(data);

	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB, BLOCK_TEXTURE_WIDTH, BLOCK_TEXTURE_HEIGHT, NUM_TEXTURES);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, BLOCK_TEXTURE_WIDTH, BLOCK_TEXTURE_HEIGHT, NUM_TEXTURES, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	free(textureData);

}

void Block::deInitialize() {

}

void Block::updateNeighbors() {
	for (int i = 0; i < 6; i++) {
		if (neighbors[i] == NULL) {
			shouldRender = true;
			return;
		}
	}
	shouldRender = false;
}

Block::Block(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	shouldRender = true;
	neighbors = (Block**)malloc(sizeof(Block*) * 6);
	for (int i = 0; i < 6; i++) {
		neighbors[i] = NULL;
	}
}

Block::~Block()
{
	free(neighbors);
}
