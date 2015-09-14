#include <gl/glew.h>
#include <gl/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#pragma comment(lib,"glew32.lib")

#define WINDOW_TITLE_PREFIX "gl_Samples"
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

int g_Width = 800;
int g_Height = 600;
int g_WndHandle = 0;

typedef struct
{	
	float XYZW[4];
	float RGBA[4];
}Vertex;

unsigned int g_FrameCount = 0;

GLuint
VertexShaderId,
FragmentShaderId,
ProgramId,
VaoId,
VboId,
ColorBufferId;

const GLchar* VertexShader =
{
	"#version 400\n"\

	"layout(location=0) in vec4 in_Position;\n"\
	"layout(location=1) in vec4 in_Color;\n"\
	"out vec4 ex_Color;\n"\

	"void main(void)\n"\
	"{\n"\
	"  gl_Position = in_Position;\n"\
	"  ex_Color = in_Color;\n"\
	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 400\n"\

	"in vec4 ex_Color;\n"\
	"out vec4 out_Color;\n"\

	"void main(void)\n"\
	"{\n"\
	"  out_Color = ex_Color;\n"\
	"}\n"
};


void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
void Cleanup(void);
void CreateVBO(void);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);

int main(int argc, char* argv[])
{
	Initialize(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

void checkError(const char *functionName)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "GL error 0x%X detected in %s\n", error, functionName);
	}
}

void dumpInfo(void)
{
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void Initialize(int argc, char* argv[])
{
	GLenum gr;
	InitWindow(argc, argv);
	glewExperimental = GL_TRUE;
	gr = glewInit();
	if (GLEW_OK != gr) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(gr)
			);
		exit(EXIT_FAILURE);
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

	CreateShaders();
	CreateVBO();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);
	glutInitWindowSize(g_Width, g_Height);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	g_WndHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if (g_WndHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);
	glutCloseFunc(Cleanup);

	//dumpInfo();
}

void ResizeFunction(int width, int height)
{
	g_Width = width;
	g_Height = height;

	glViewport(0, 0, width, height);
}

void RenderFunction(void)
{
	++g_FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

void IdleFunction(void)
{
	glutPostRedisplay();
}


void TimerFunction(int Value)
{
	if (0 != Value) {
		char* TempString = (char*)
			malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			g_FrameCount * 4,
			g_Width,
			g_Height
			);

		glutSetWindowTitle(TempString);
		free(TempString);
	}

	g_FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}

void Cleanup(void)
{
	DestroyVBO();
	DestroyShaders();
}

void DestroyVBO(void)
{
	CHECK_ERROR_BEGIN
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(NULL);
	glDeleteVertexArrays(1, &VaoId);
	
	CHECK_ERROR_END
}


void CreateVBO(void)
{
	//GLfloat Vertices[] = {
	//	-0.8f, -0.8f, 0.0f, 1.0f,
	//	0.0f, 0.8f, 0.0f, 1.0f,
	//	0.8f, -0.8f, 0.0f, 1.0f
	//};

	//GLfloat Colors[] = {
	//	1.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f, 1.0f,
	//	0.0f, 0.0f, 1.0f, 1.0f
	//};

	Vertex Vertices[] = {
			{ { -0.8f, -0.8f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
			{ { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { 0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};
	CHECK_ERROR_BEGIN;
	const size_t bufferSize = sizeof(Vertices);
	const size_t vertSize = sizeof(Vertices[0]);
	const size_t RGBOffset = sizeof(Vertices[0].XYZW);
	glGenVertexArrays(1, &VaoId);

	glBindVertexArray(VaoId);

	glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, false, vertSize, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, false, vertSize, (void*)RGBOffset);
	glEnableVertexAttribArray(1);

	CHECK_ERROR_END
}

void CreateShaders(void)
{
	CHECK_ERROR_BEGIN

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);
	glUseProgram(ProgramId);

	CHECK_ERROR_END

}
void DestroyShaders(void)
{
	CHECK_ERROR_BEGIN
	glUseProgram(NULL);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	glDeleteProgram(ProgramId);

	CHECK_ERROR_END

}

