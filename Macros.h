#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gl/glew.h>
#include <gl/freeglut.h>
//#pragma comment(lib, "glew32.lib")
#define CHECK_ERROR_BEGIN GLenum ErrorCheckValue = glGetError();
#define  CHECK_ERROR_END   ErrorCheckValue = glGetError();\
if (ErrorCheckValue != GL_NO_ERROR)\
{\
	fprintf(\
		stderr,\
		"ERROR: Could not destroy the VBO: %s \n",\
		gluErrorString(ErrorCheckValue)\
		);\
	exit(-1);\
}


typedef struct
{
	float XYZW[4];
	float RGBA[4];
}Vertex;