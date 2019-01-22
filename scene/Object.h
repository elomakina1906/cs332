#pragma once

#include "structures.h"
#include "glsl.h"

using namespace cwc;

class Object {
	Vector3D position, rotation, model_rotation;
	GLfloat size;
	Material material;
	vector<Vertex> vertexes;
	GLuint texture;

public:
	Object() {
		texture = 0;
		size = 1.0f;
	}

	void loadModel(const char* fileName) {
		ifstream file;
		file.open(fileName);
		if (file.is_open()) {
			char line[64];
			vector<Vector3D> v_list;
			vector<Vector2D> vt_list;
			vector<Vector3D> vn_list;
			while (file) {
				file.getline(line, 64);
				if (strlen(line)) {
					stringstream ss(line);
					string line_code;
					ss >> line_code;
					if (line_code == "v") {
						Vector3D v;
						GLfloat coord;
						ss >> coord;
						v.x = coord;
						ss >> coord;
						v.y = coord;
						ss >> coord;
						v.z = coord;
						v_list.push_back(v);
					}
					if (line_code == "vt") {
						Vector2D vt;
						GLfloat coord;
						ss >> coord;
						vt.x = coord;
						ss >> coord;
						vt.y = coord;
						vt_list.push_back(vt);
					} 
					if (line_code == "vn") {
						Vector3D vn;
						GLfloat coord;
						ss >> coord;
						vn.x = coord;
						ss >> coord;
						vn.y = coord;
						ss >> coord;
						vn.z = coord;
						vn_list.push_back(vn);
					}
					if (line_code == "f") {
						GLuint ind;
						for (int i = 0; i < 3; i++) {
							Vertex _vertex;
							string s;
							ss >> s;
							replace(s.begin(), s.end(), '/', ' ');
							stringstream ss(s);
							ss >> ind;
							_vertex.v = v_list[ind - 1];
							ss >> ind;
							_vertex.vt = vt_list[ind - 1];
							ss >> ind;
							_vertex.vn = vn_list[ind - 1];
							vertexes.push_back(_vertex);
						}
					}
				}
			}
			file.close();
		}

	}

	void loadTexture(const char* fileName) {
		texture = SOIL_load_OGL_texture(fileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture == 0) {
			return;
		}
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void draw(glShader *shader) {
		glPushMatrix();
		glRotatef(rotation.x, 1, 0, 0);
		glRotatef(rotation.y, 0, 1, 0);
		glRotatef(rotation.z, 0, 0, 1);
		glTranslatef(position.x, position.y, position.z);
		glRotatef(model_rotation.x, 1, 0, 0);
		glRotatef(model_rotation.y, 0, 1, 0);
		glRotatef(model_rotation.z, 0, 0, 1);
		glScalef(size, size, size);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		shader->setUniform1i("texture_image", 0);
		shader->setUniform3f("material_ambient", material.ambient.x, material.ambient.y, material.ambient.z);
		shader->setUniform3f("material_diffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
		shader->setUniform3f("material_specular", material.specular.x, material.specular.y, material.specular.z);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < vertexes.size(); i++) {
			glNormal3f(vertexes[i].vn.x, vertexes[i].vn.y, vertexes[i].vn.z);
			glTexCoord2f(vertexes[i].vt.x, vertexes[i].vt.y);
			glVertex3f(vertexes[i].v.x, vertexes[i].v.y, vertexes[i].v.z);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	void setMaterial(Material material) {
		this->material = material;
	}

	void setPosition(Vector3D position) {
		this->position = position;
	}

	void setRotation(Vector3D rotation) {
		this->rotation = rotation;
	}

	void setModelRotation(Vector3D model_rotation) {
		this->model_rotation = model_rotation;
	}

	void setSize(GLfloat size) {
		this->size = size;
	}

	Vector3D getPosition() {
		return position;
	}

	Vector3D getRotation() {
		return rotation;
	}

	Vector3D getModelRotation() {
		return model_rotation;
	}

	GLfloat getSize() {
		return size;
	}
};
