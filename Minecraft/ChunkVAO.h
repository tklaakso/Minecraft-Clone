#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <iostream>

class ChunkVAO
{
public:
	ChunkVAO();
	~ChunkVAO();
	void updateTranslationData(glm::vec3* translations, int len);
	void updateTextureData(int* textures, int len);
	void updateLightmapData(int* lightMap, int len);
	void bind(int face);
	unsigned int* vertexVBO;
	unsigned int* EBO;
	unsigned int* VAO;
	unsigned int* uvVBO;
	unsigned int* textureVBO;
	unsigned int* instanceVBO;
	unsigned int* lightmapVBO;
};

