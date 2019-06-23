#pragma once
#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Constants.h"

class Plane {
public:
	Plane(float _a, float _b, float _c, float _d);
	Plane();
	void update(float a, float b, float c, float d);
	bool inFront(glm::vec3 point);
	float a;
	float b;
	float c;
	float d;
	glm::vec3 normal;
	glm::vec3 pointOnPlane;
};

class ViewFrustum {
public:
	ViewFrustum();
	void update(glm::mat4 trans);
	bool inside(glm::vec3 point);
	bool insideVertical(glm::vec3 point);
	bool chunkInView(int x, int y);
	void log();
	Plane top, bottom, left, right;
};

float clamp(float f, float low, float high);

float lerp(float a, float b, float t);

float fullMod(int a, int b);

#endif