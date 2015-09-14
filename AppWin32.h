#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#pragma comment(lib,"glew32.lib")


class AppWin32
{
public:
	int mWndHandle;

	int mWidth;
	int mHeight;

	unsigned mFrameCount;

	GLuint
		VertexShaderId,
		FragmentShaderId,
		ProgramId,
		VaoId,
		VboId,
		ColorBufferId;
	AppWin32() : mWidth(800), mHeight(600), mWndHandle(0), mFrameCount(0) {}
	~AppWin32() {
		Cleanup();
	}
	void Initialize(int, char*[]);
	void MainLoop() {
		glutMainLoop();
	}
	void Exit() {
		exit(EXIT_SUCCESS);
	}
	static AppWin32& GetInstance()
	{

		static AppWin32 Inst;
		return Inst;
	}
	inline void ExitOnGLError(const char* error_message)
	{
		const GLenum ErrorValue = glGetError();

		if (ErrorValue != GL_NO_ERROR)
		{
			fprintf(stderr, "%s: %s\n", error_message, gluErrorString(ErrorValue));
			exit(EXIT_FAILURE);
		}
	}
public:
	void InitWindow(int, char*[]);
	void ResizeFunction(int, int);
	void RenderFunction();
	void TimerFunction(int);
	void IdleFunction();
	void Cleanup();
	void CreateVBO();
	void DestroyVBO();
	void CreateShaders();
	void DestroyShaders();
	GLuint LoadShader(const char*, GLenum);

};
#define WINDOW_TITLE_PREFIX "gl_Samples"
#define CHECK_ERROR_BEGIN GLenum ErrorCheckValue = glGetError();
#define  CHECK_ERROR_END   ErrorCheckValue = glGetError();\
if (ErrorCheckValue != GL_NO_ERROR)\
{\
	fprintf(\
		stderr,\
		"ERROR: %S: %s ",\
		__FUNCTION__ , \
		gluErrorString(ErrorCheckValue)\
		);\
	exit(-1);\
}
#define App (&AppWin32::GetInstance())