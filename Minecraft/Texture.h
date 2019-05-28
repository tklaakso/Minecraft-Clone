#pragma once
#include <glm/glm.hpp>

class Texture
{
public:
	Texture(glm::vec2* uv);
	~Texture();
	glm::vec2* uv;
};

