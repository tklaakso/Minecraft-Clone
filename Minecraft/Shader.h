#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Shader
{
public:
	GLuint id;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
	void setBool(const string &name, bool value);
	void setInt(const string &name, int value);
	void setFloat(const string &name, float value);
	void setMat4(const string &name, glm::mat4 value);
	void setVec3(const string &name, glm::vec3 value);
	~Shader();
};

#endif