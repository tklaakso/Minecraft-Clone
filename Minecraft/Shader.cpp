#pragma once
#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	string vertexCode;
	string fragmentCode;
	ifstream vertexFile;
	ifstream fragmentFile;
	vertexFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		stringstream vertexStream, fragmentStream;
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();
		vertexFile.close();
		fragmentFile.close();
		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}
	const char* cVertexCode = vertexCode.c_str();
	const char* cFragmentCode = fragmentCode.c_str();
	GLuint vertex, fragment;
	int success;
	char infoLog[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &cVertexCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Vertex shader compilation failed: " << infoLog << endl;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &cFragmentCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "Fragment shader compilation failed: " << infoLog << endl;
	}
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		cout << "Shader program linking failed: " << infoLog << endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::setBool(const string &name, bool value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat4(const string &name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const string &name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

Shader::~Shader() {

}