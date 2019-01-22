#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "soil\SOIL.h"
#include "Object.h"

void display();
void init();
void resize(int w, int h);
void specialKey(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
