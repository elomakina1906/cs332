#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Vector2D {
	GLfloat x;
	GLfloat y;

	Vector2D() {
		x = y = 0;
	}

	Vector2D(GLfloat x, GLfloat y) {
		this->x = x;
		this->y = y;
	}
};

struct Vector3D {
	GLfloat x;
	GLfloat y;
	GLfloat z;

	Vector3D() {
		x = y = z = 0;
	}

	Vector3D(GLfloat x, GLfloat y, GLfloat z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct PointLight {
	bool active;
	Vector3D position;
	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;
};

struct Material {
	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;
};

struct Vertex {
	Vector3D v;
	Vector2D vt;
	Vector3D vn;
};
