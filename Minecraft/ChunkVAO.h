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
	void bind();
	unsigned int VBO, EBO, VAO, uvVBO, textureVBO, instanceVBO;
};

