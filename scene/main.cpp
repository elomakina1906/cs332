#include <Windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <conio.h>
#include <math.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

const double PI = acos(-1);

int width = 0, height = 0;
GLuint textureTable, textureApple;
GLuint shaderProgramTable, shaderProgramApple;
GLuint VAOTable, VAOApple;

mat4 modelPos, viewProjection;
mat3 normaltr;

float viewPosition[]{ 0, 10, -20 };
float light_angle = 0, light_pos = 0, light_rad = 50;
float light[]{ 0, 5, 0 };

vector<unsigned short> indicesTable;
GLuint vertexbufferTable;
GLuint uvbufferTable;
GLuint normalbufferTable;
GLuint elementbufferTable;
vector<unsigned short> indicesApple;
GLuint vertexbufferApple;
GLuint uvbufferApple;
GLuint normalbufferApple;
GLuint elementbufferApple;

string objectAppleName = "apple.obj";
string objectAppleTextureName = "apple.jpg";
double scaleObjectApple = 0.1;
string objectTableName = "table.obj";
string objectTableTextureName = "wood.jpg";
double scaleObjectTable = 4;
float rotateX = 0, rotateY = 0, rotateZ = 0;

void loadTexture()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	textureTable = SOIL_load_OGL_texture
	(
		objectTableTextureName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	textureApple = SOIL_load_OGL_texture
	(
		objectAppleTextureName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

string readShader(const char* path)
{
	string res = "";
	ifstream file(path);
	string line;
	getline(file, res, '\0');
	while (getline(file, line))
		res += "\n " + line;
	return res;
}

void initShaderTable()
{
	string readed = readShader("tablev.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("tablef.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramTable = glCreateProgram();
	glAttachShader(shaderProgramTable, vShader);
	glAttachShader(shaderProgramTable, fShader);
	glLinkProgram(shaderProgramTable);

	int link_ok;
	glGetProgramiv(shaderProgramTable, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(table)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramTable, 512, &size, infoLog);
		cout << infoLog;
		return;
	}
}

void initShaderApple()
{
	string readed = readShader("applev.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readShader("applef.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	shaderProgramApple = glCreateProgram();
	glAttachShader(shaderProgramApple, vShader);
	glAttachShader(shaderProgramApple, fShader);
	glLinkProgram(shaderProgramApple);

	int link_ok;
	glGetProgramiv(shaderProgramApple, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		cout << "(apple)error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(shaderProgramApple, 512, &size, infoLog);
		cout << infoLog;
		return;
	}
}

void initShaders() {
	initShaderTable();
	initShaderApple();
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
}

void loadOBJ(const string & path, vector<vec3> & out_vertices, vector<vec2> & out_uvs, vector<vec3> & out_normals)
{
	vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	vector<vec3> temp_vertices;
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;

	ifstream infile(path);
	string line;
	while (getline(infile, line))
	{
		stringstream ss(line);
		string lineHeader;
		getline(ss, lineHeader, ' ');
		if (lineHeader == "v")
		{
			vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;

			vertex.x *= scaleObjectTable;
			vertex.y *= scaleObjectTable;
			vertex.z *= scaleObjectTable;
			temp_vertices.push_back(vertex);
		}
		else if (lineHeader == "vt")
		{
			vec2 uv;
			ss >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == "vn")
		{
			vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == "f")
		{
			unsigned int vertex_index, uv_index, normal_index;
			char slash;
			while (ss >> vertex_index >> slash >> uv_index >> slash >> normal_index)
			{
				vertex_indices.push_back(vertex_index);
				uv_indices.push_back(uv_index);
				normal_indices.push_back(normal_index);
			}
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertex_indices.size(); i++)
	{
		unsigned int vertexIndex = vertex_indices[i];
		vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvIndex = uv_indices[i];
		vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);

		unsigned int normalIndex = normal_indices[i];
		vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
}

// https://github.com/opengl-tutorials/ogl/blob/master/common/vboindexer.cpp
struct PackedVertex
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	bool operator<(const PackedVertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

bool getSimilarVertexIndex_fast(PackedVertex & packed, map<PackedVertex, unsigned short> & VertexToOutIndex, unsigned short & result)
{
	map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end())
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void indexVBO(vector<vec3> & in_vertices, vector<vec2> & in_uvs, vector<vec3> & in_normals,
	vector<unsigned short> & out_indices, vector<vec3> & out_vertices, vector<vec2> & out_uvs, vector<vec3> & out_normals)
{
	map<PackedVertex, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i < in_vertices.size(); i++)
	{
		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };

		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found)
		{ // A similar vertex is already in the VBO, use it instead!
			out_indices.push_back(index);
		}
		else
		{ // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void initBuffersTable()
{
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;
	vector<vec3> indexed_vertices;
	vector<vec2> indexed_uvs;
	vector<vec3> indexed_normals;
	loadOBJ(objectTableName.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indicesTable, indexed_vertices, indexed_uvs, indexed_normals);

	glGenBuffers(1, &vertexbufferTable);
	glGenBuffers(1, &uvbufferTable);
	glGenBuffers(1, &normalbufferTable);
	glGenBuffers(1, &elementbufferTable);
	glGenVertexArrays(1, &VAOTable);

	glBindVertexArray(VAOTable);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTable);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbufferTable);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbufferTable);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferTable);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesTable.size() * sizeof(unsigned short), &indicesTable[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTable);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbufferTable);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbufferTable);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void initBuffersApple()
{
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;
	vector<vec3> indexed_vertices;
	vector<vec2> indexed_uvs;
	vector<vec3> indexed_normals;
	loadOBJ(objectAppleName.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indicesApple, indexed_vertices, indexed_uvs, indexed_normals);

	glGenBuffers(1, &vertexbufferApple);
	glGenBuffers(1, &uvbufferApple);
	glGenBuffers(1, &normalbufferApple);
	glGenBuffers(1, &elementbufferApple);
	glGenVertexArrays(1, &VAOApple);

	glBindVertexArray(VAOApple);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferApple);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbufferApple);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbufferApple);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferApple);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesApple.size() * sizeof(unsigned short), &indicesApple[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferApple);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbufferApple);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(4);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbufferApple);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(5);

	glBindVertexArray(1);
}

void updateLight()
{
	light[0] = light_rad * cos(light_angle / 180 * PI);
	light[1] = light_pos;
	light[2] = light_rad * sin(light_angle / 180 * PI);
}

void setTransformTable()
{
	GLint s_model = glGetUniformLocation(shaderProgramTable, "transform.model");
	GLint s_proj = glGetUniformLocation(shaderProgramTable, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(shaderProgramTable, "transform.normal");
	GLint s_view = glGetUniformLocation(shaderProgramTable, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &modelPos[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);
}

void setTransformApple()
{
	GLint s_model = glGetUniformLocation(shaderProgramApple, "transform.model");
	GLint s_proj = glGetUniformLocation(shaderProgramApple, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(shaderProgramApple, "transform.normal");
	GLint s_view = glGetUniformLocation(shaderProgramApple, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &modelPos[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);
}

void setPointLightTable()
{
	GLint s_position = glGetUniformLocation(shaderProgramTable, "light.position");
	GLint s_ambient = glGetUniformLocation(shaderProgramTable, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(shaderProgramTable, "light.diffuse");
	GLint s_specular = glGetUniformLocation(shaderProgramTable, "light.specular");
	GLint s_attenuation = glGetUniformLocation(shaderProgramTable, "light.attenuation");

	float ambient[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float attenuation[]{ 1.0f, 0.0f, 0.0f };

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void setPointLightApple()
{
	GLint s_position = glGetUniformLocation(shaderProgramApple, "light.position");
	GLint s_ambient = glGetUniformLocation(shaderProgramApple, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(shaderProgramApple, "light.diffuse");
	GLint s_specular = glGetUniformLocation(shaderProgramApple, "light.specular");
	GLint s_attenuation = glGetUniformLocation(shaderProgramApple, "light.attenuation");

	float ambient[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float attenuation[]{ 1.0f, 0.0f, 0.0f };

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void setMaterialTable(float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness)
{
	GLint s_ambient = glGetUniformLocation(shaderProgramTable, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(shaderProgramTable, "material.diffuse");
	GLint s_specular = glGetUniformLocation(shaderProgramTable, "material.specular");
	GLint s_emission = glGetUniformLocation(shaderProgramTable, "material.emission");
	GLint s_shiness = glGetUniformLocation(shaderProgramTable, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void setMaterialApple(float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness)
{
	GLint s_ambient = glGetUniformLocation(shaderProgramApple, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(shaderProgramApple, "material.diffuse");
	GLint s_specular = glGetUniformLocation(shaderProgramApple, "material.specular");
	GLint s_emission = glGetUniformLocation(shaderProgramApple, "material.emission");
	GLint s_shiness = glGetUniformLocation(shaderProgramApple, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void show(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	modelPos = mat4(1.0f);
	modelPos = rotate(modelPos, radians(rotateX), vec3(1, 0, 0));
	modelPos = rotate(modelPos, radians(rotateY), vec3(0, 1, 0));
	modelPos = rotate(modelPos, radians(rotateZ), vec3(0, 0, 1));

	viewProjection = perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	viewProjection *= lookAt(
		vec3(viewPosition[0], viewPosition[1], viewPosition[2]),
		vec3(0, 0, 0),
		vec3(0, 1, 0)
	);
	normaltr = transpose(inverse(modelPos));

	GLint Unif_tex = glGetUniformLocation(shaderProgramTable, "myTexture");
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureTable);

	glUseProgram(shaderProgramTable);

	setTransformTable();
	setPointLightTable();
	float m_ambient[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float m_diffuse[]{ 1.0f, 1.0f, 1.0f, 1.0f };
	float m_specular[]{ 0.2f, 0.2f, 0.2f, 1.0f };
	float m_emission[]{ 0.0f, 0.0f, 0.0f, 1.0f };
	float m_shiness = 0;
	setMaterialTable(m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAOTable);
	glDrawElements(GL_QUADS, indicesTable.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	GLint Unif_tex_apple = glGetUniformLocation(shaderProgramApple, "myTexture");
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureApple);

	glUseProgram(shaderProgramApple);

	setTransformApple();
	setPointLightApple();
	setMaterialApple(m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAOApple);
	glDrawElements(GL_QUADS, indicesApple.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(1);

	glUseProgram(0);
	if (glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void lightManager(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		light_pos += 0.5;
		break;
	case GLUT_KEY_DOWN:
		light_pos -= 0.5;
		break;
	case GLUT_KEY_RIGHT:
		light_angle -= 3;
		break;
	case GLUT_KEY_LEFT:
		light_angle += 3;
		break;
	case GLUT_KEY_PAGE_UP:
		light_rad -= 0.5;
		break;
	case GLUT_KEY_PAGE_DOWN:
		light_rad += 0.5;
		break;
	default:
		break;
	}
	updateLight();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		rotateX -= 1;
		break;
	case 's':
		rotateX += 1;
		break;
	case 'q':
		rotateY -= 1;
		break;
	case 'e':
		rotateY += 1;
		break;
	case 'a':
		rotateZ -= 1;
		break;
	case 'd':
		rotateZ += 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(250, 5);
	glutCreateWindow("indiv3");

	glewInit();

	initBuffersTable();
	initBuffersApple();
	loadTexture();
	initShaders();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	updateLight();

	glutDisplayFunc(show);
	glutReshapeFunc(reshape);
	glutSpecialFunc(lightManager);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	glUseProgram(0);
	glDeleteProgram(shaderProgramTable);
	glDeleteProgram(shaderProgramApple);

	glDeleteBuffers(1, &VAOTable);
	glDeleteBuffers(1, &vertexbufferTable);
	glDeleteBuffers(1, &uvbufferTable);
	glDeleteBuffers(1, &normalbufferTable);
	glDeleteBuffers(1, &elementbufferTable);
	glDeleteBuffers(1, &VAOApple);
	glDeleteBuffers(1, &vertexbufferApple);
	glDeleteBuffers(1, &uvbufferApple);
	glDeleteBuffers(1, &normalbufferApple);
	glDeleteBuffers(1, &elementbufferApple);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}