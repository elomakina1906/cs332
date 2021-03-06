#include "Control.h"

glShaderManager SM;
glShader * shader;
vector<Object*> objects;
Vector3D camera_position;
Vector3D camera_rotation;
PointLight point_light;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(camera_rotation.x, 1, 0, 0);
	glRotatef(camera_rotation.y, 0, 1, 0);
	glRotatef(camera_rotation.z, 0, 0, 1);
	glTranslatef(camera_position.x, camera_position.y, camera_position.z);
	if (shader) {
		shader->begin();
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		shader->setUniformMatrix4fv("light_model_matrix", 1, GL_FALSE, &matrix[0]);
		shader->setUniform1i("point_active", point_light.active);
		if (point_light.active) {
			shader->setUniform3f("point_position", point_light.position.x, point_light.position.y, point_light.position.z);
			shader->setUniform3f("point_ambient", point_light.ambient.x, point_light.ambient.y, point_light.ambient.z);
			shader->setUniform3f("point_diffuse", point_light.diffuse.x, point_light.diffuse.y, point_light.diffuse.z);
			shader->setUniform3f("point_specular", point_light.specular.x, point_light.specular.y, point_light.specular.z);
		}
		/*Object *teapot = objects.at(1);
		if (teapot) {
			Vector3D rotation = teapot->getRotation();
			rotation.y += 0.1;
			teapot->setRotation(rotation);
			Vector3D model_rotation = teapot->getModelRotation();
			model_rotation.y -= 0.3;
			teapot->setModelRotation(model_rotation);
		}
		Object *cube = objects.at(0);
		if (cube) {
			Vector3D model_rotation = cube->getModelRotation();
			model_rotation.y -= 0.01;
			cube->setModelRotation(model_rotation);
		}
		*/
		for (auto i : objects) {
			i->draw(shader);
		}
		shader->end();
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	if (point_light.active) {
		glPushMatrix();
		glTranslatef(point_light.position.x, point_light.position.y, point_light.position.z);
		glutSolidSphere(0.1, 10, 10);
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}

void init()
{
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	Material material;
	material.ambient = Vector3D(0.1, 0.1, 0.1);
	material.diffuse = Vector3D(1, 0, 1);
	material.specular = Vector3D(1, 1, 1);
	Object* object = new Object();
	object->loadModel("f_1.obj");
	object->loadTexture("box.png");
	object->setMaterial(material);
	objects.push_back(object);
	Object* object2 = new Object();;
	object2->loadModel("f_2.obj");
	object2->loadTexture("marble.png");
	object2->setMaterial(material);
	object2->setPosition(Vector3D(0.0f, 0.5f, 0));
	object2->setSize(0.3f);
	objects.push_back(object2);
	point_light.active = false;
	point_light.position = Vector3D(1, 1, 1);
	point_light.ambient = Vector3D(1.0, 1.0, 1.0);
	point_light.diffuse = Vector3D(0.0, 1.0, 1.0);
	point_light.specular = Vector3D(1.0, 1.0, 1.0);
	shader = SM.loadfromFile("vertex.shader", "frag.shader");
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, (float)w / (float)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 5.0f, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);
}

void specialKey(int key, int x, int y)
{
	int mod = glutGetModifiers();
	switch (key) {
	case GLUT_KEY_UP:
		if (mod == GLUT_ACTIVE_SHIFT)
			camera_position.y += 0.05f;
		else if (mod == GLUT_ACTIVE_CTRL)
			camera_position.z -= 0.05f;
		else
			camera_rotation.x += 5.0f;
		break;
	case GLUT_KEY_DOWN:
		if (mod == GLUT_ACTIVE_SHIFT)
			camera_position.y -= 0.05f;
		else if (mod == GLUT_ACTIVE_CTRL)
			camera_position.z += 0.05f;
		else
			camera_rotation.x -= 5.0f;
		break;
	case GLUT_KEY_RIGHT:
		if (mod == GLUT_ACTIVE_SHIFT)
			camera_position.x += 0.05f;
		else
			camera_rotation.y += 5.0f;
		break;
	case GLUT_KEY_LEFT:
		if (mod == GLUT_ACTIVE_SHIFT)
			camera_position.x -= 0.05f;
		else
			camera_rotation.y -= 5.0f;
		break;
	case GLUT_KEY_F1:
		point_light.position.x += mod == GLUT_ACTIVE_SHIFT ? 0.05f : -0.05f;
		break;
	case GLUT_KEY_F2:
		point_light.position.y += mod == GLUT_ACTIVE_SHIFT ? 0.05f : -0.05f;
		break;
	case GLUT_KEY_F3:
		point_light.position.z += mod == GLUT_ACTIVE_SHIFT ? 0.05f : -0.05f;
		break;
	}
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'z':
		point_light.active = !point_light.active;
		break;
	}
}
