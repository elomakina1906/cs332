#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

//��������� ������
struct Vector2D {
	Vector2D() {
		x = y = 0;
	}

	Vector2D(GLfloat x, GLfloat y) {
		this->x = x;
		this->y = y;
	}
	GLfloat x;
	GLfloat y;
};

//���������� ������
struct Vector3D {
	Vector3D() {
		x = y = z = 0;
	}

	Vector3D(GLfloat x, GLfloat y, GLfloat z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	GLfloat x;
	GLfloat y;
	GLfloat z;
};

//�������� �������� �����
struct PointLight {
	bool active;
	Vector3D position;

	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;
} point_light;

//���������
struct SpotLight {
	bool active;
	Vector3D position;
	Vector3D direction;

	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;

	GLfloat cutoff;
} spot_light;

//������������
struct DirectionalLight {
	bool active;
	Vector3D direction;

	Vector3D ambient;
	Vector3D diffuse;
	Vector3D specular;
} directional_light;

//��������
struct Material {
	Vector3D ambient, diffuse, specular;
};

//������� ������
struct Vertex {
	Vector3D v;
	Vector2D vt;
	Vector3D vn;
};
