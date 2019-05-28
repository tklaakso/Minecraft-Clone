#include "Math.h"

Plane::Plane(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d), normal(glm::vec3(_a, _b, _c)) {
	float x, y, z;
	if (glm::abs(_c) >= glm::max(glm::abs(_a), glm::abs(_b))) {
		if (_c == 0) {
			z = 0.0f;
		}
		else {
			z = -_d / _c;
		}
		x = y = 0.0f;
	}
	else if (glm::abs(_b) >= glm::max(glm::abs(_a), glm::abs(_c))) {
		if (_b == 0) {
			y = 0.0f;
		}
		else {
			y = -_d / _b;
		}
		x = z = 0.0f;
	}
	else {
		if (_a == 0) {
			x = 0.0f;
		}
		else {
			x = -_d / _a;
		}
		y = z = 0.0f;
	}
	pointOnPlane = glm::vec3(x, y, z);
}
Plane::Plane() { }
void Plane::update(float a, float b, float c, float d) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	float x, y, z;
	if (glm::abs(c) >= glm::max(glm::abs(a), glm::abs(b))) {
		if (c == 0) {
			z = 0.0f;
		}
		else {
			z = -d / c;
		}
		x = y = 0.0f;
	}
	else if (glm::abs(b) >= glm::max(glm::abs(a), glm::abs(c))) {
		if (b == 0) {
			y = 0.0f;
		}
		else {
			y = -d / b;
		}
		x = z = 0.0f;
	}
	else {
		if (a == 0) {
			x = 0.0f;
		}
		else {
			x = -d / a;
		}
		y = z = 0.0f;
	}
	pointOnPlane = glm::vec3(x, y, z);
	normal = glm::vec3(a, b, c);
}
bool Plane::inFront(glm::vec3 point) {
	glm::vec3 diff = point - pointOnPlane;
	return glm::dot(diff, normal) > 0;
}

ViewFrustum::ViewFrustum() {
	top = Plane();
	bottom = Plane();
	left = Plane();
	right = Plane();
}
void ViewFrustum::update(glm::mat4 trans) {
	bottom.update(trans[0][3] + trans[0][1], trans[1][3] + trans[1][1], trans[2][3] + trans[2][1], trans[3][3] + trans[3][1]);
	top.update(trans[0][3] - trans[0][1], trans[1][3] - trans[1][1], trans[2][3] - trans[2][1], trans[3][3] - trans[3][1]);
	left.update(trans[0][3] + trans[0][0], trans[1][3] + trans[1][0], trans[2][3] + trans[2][0], trans[3][3] + trans[3][0]);
	right.update(trans[0][3] - trans[0][0], trans[1][3] - trans[1][0], trans[2][3] - trans[2][0], trans[3][3] - trans[3][0]);
}
bool ViewFrustum::inside(glm::vec3 point) {
	return !top.inFront(point) && bottom.inFront(point) && left.inFront(point) && right.inFront(point);
}
bool ViewFrustum::insideVertical(glm::vec3 point) {
	return left.inFront(point) && right.inFront(point);
}
bool ViewFrustum::chunkInView(int x, int y) {
	glm::vec3 p1 = glm::vec3(x * CHUNK_WIDTH, 0, y * CHUNK_WIDTH);
	glm::vec3 p2 = glm::vec3((x + 1) * CHUNK_WIDTH, 0, y * CHUNK_WIDTH);
	glm::vec3 p3 = glm::vec3((x + 1) * CHUNK_WIDTH, 0, (y + 1) * CHUNK_WIDTH);
	glm::vec3 p4 = glm::vec3(x * CHUNK_WIDTH, 0, (y + 1) * CHUNK_WIDTH);
	glm::vec3 p5 = glm::vec3(x * CHUNK_WIDTH, CHUNK_HEIGHT, y * CHUNK_WIDTH);
	glm::vec3 p6 = glm::vec3((x + 1) * CHUNK_WIDTH, CHUNK_HEIGHT, y * CHUNK_WIDTH);
	glm::vec3 p7 = glm::vec3((x + 1) * CHUNK_WIDTH, CHUNK_HEIGHT, (y + 1) * CHUNK_WIDTH);
	glm::vec3 p8 = glm::vec3(x * CHUNK_WIDTH, CHUNK_HEIGHT, (y + 1) * CHUNK_WIDTH);
	bool lifP1 = left.inFront(p1);
	bool rifP1 = right.inFront(p1);
	bool lifP2 = left.inFront(p2);
	bool rifP2 = right.inFront(p2);
	bool lifP3 = left.inFront(p3);
	bool rifP3 = right.inFront(p3);
	bool lifP4 = left.inFront(p4);
	bool rifP4 = right.inFront(p4);
	bool lifP5 = left.inFront(p5);
	bool rifP5 = right.inFront(p5);
	bool lifP6 = left.inFront(p6);
	bool rifP6 = right.inFront(p6);
	bool lifP7 = left.inFront(p7);
	bool rifP7 = right.inFront(p7);
	bool lifP8 = left.inFront(p8);
	bool rifP8 = right.inFront(p8);
	if ((lifP1 && rifP1) || (lifP2 && rifP2) || (lifP3 && rifP3) || (lifP4 && rifP4) || (lifP5 && rifP5) || (lifP6 && rifP6) || (lifP7 && rifP7) || (lifP8 && rifP8)) {
		return true;
	}
	int sum1 = glm::abs(lifP1 + lifP2 + lifP3 + lifP4) % 4;
	int sum2 = glm::abs(rifP1 + rifP2 + rifP3 + rifP4) % 4;
	int sum3 = glm::abs(lifP5 + lifP6 + lifP7 + lifP8) % 4;
	int sum4 = glm::abs(rifP5 + rifP6 + rifP7 + rifP8) % 4;
	if ((sum1 > 0 && sum2 > 0) || (sum3 > 0 && sum4 > 0)) {
		return true;
	}
	return false;
}
void ViewFrustum::log() {
	std::cout << "-------------------" << std::endl;
	std::cout << "Top plane: " << top.a << ", " << top.b << ", " << top.c << ", " << top.d << std::endl;
	std::cout << "Bottom plane: " << bottom.a << ", " << bottom.b << ", " << bottom.c << ", " << bottom.d << std::endl;
	std::cout << "Left plane: " << left.a << ", " << left.b << ", " << left.c << ", " << left.d << std::endl;
	std::cout << "Right plane: " << right.a << ", " << right.b << ", " << right.c << ", " << right.d << std::endl;
	std::cout << "-------------------" << std::endl;
}

float clamp(float f, float low, float high) {
	if (f < low) {
		f = low;
	}
	if (f > high) {
		f = high;
	}
	return f;
}
	