#include "Texture.h"



Texture::Texture(glm::vec2* uv)
{
	this->uv = uv;
}


Texture::~Texture()
{
	free(uv);
}
