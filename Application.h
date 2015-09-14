#pragma once
#include "Macros.h"
#include "Log.h"
#include "AppUtils.h"
#include <gl/glfw3.h>
#pragma comment(lib,"glfw3dll.lib")

class Application
{
public:
	Application()  { mApp = this; }
	~Application() {}
	virtual void run()
	{
		bool running = true;
		init();
		if (!glfwInit())
		{
			Log::error("glfwInit Failed");
			return;
		}
		GLFWwindow* wndptr = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, nullptr, nullptr);
		if (!wndptr)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(wndptr);
		glfwSwapInterval(1);
		glfwSetKeyCallback(wndptr, KeyCallback);
		glfwSetMouseButtonCallback(wndptr, MouseCallback);

		//glew Init
		glewExperimental = GL_TRUE;
		auto gr = glewInit();
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
		Enter();
		do 
		{
			Update(glfwGetTime());
			glfwSwapBuffers(wndptr);
			glfwPollEvents();

			running &= (glfwGetKey(wndptr, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
			running &= (!glfwWindowShouldClose(wndptr));

		} while (running);
		Exit();
		glfwDestroyWindow(wndptr);
		glfwTerminate();
	}

protected:
	virtual void init() {}
	virtual void Enter() {}
	virtual void Update(double dt) {}
	virtual void Exit() {}
	virtual void OnKey(int key, int action)				 {}
	virtual void OnMouse(int button, int action)	     {}
	virtual void LoadFragmentShader(const char *name, GLuint *shaderID)  {
		char extName[128];
		strcpy(extName, name);
		strcat(extName, ".frag.glsl");
		*shaderID = AppUtils::LoadShader(extName, GL_FRAGMENT_SHADER);
	}
	virtual void LoadVertexShader(const char* name, GLuint *shaderID)
	{
		char extName[128];
		strcpy(extName, name);
		strcat(extName, ".vert.glsl");
		*shaderID = AppUtils::LoadShader(extName, GL_VERTEX_SHADER);
	}
	virtual void CreateShaderProgram(const char *name, GLuint *programID)
	{
		GLuint fragID, vertID, pID;
		LoadVertexShader(name, &vertID);
		LoadFragmentShader(name, &fragID);

		*programID = glCreateProgram();
		pID = *programID;
		glAttachShader(pID, vertID);
		glAttachShader(pID, fragID);

		glLinkProgram(pID);
		glUseProgram(pID);
		printf("ProgramID = %d", programID);
	}
public:
	struct AppInfo
	{
		char title[128] = {};
		int windowWidth = 800;
		int windowHeight = 600;
	};
	static Application *mApp;
protected:
	AppInfo info;
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		mApp->OnKey(key, action);
	}
	static void MouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mApp->OnMouse(button, action);
	}
};


#define DECLARE_MAIN(a)                             \
int CALLBACK WinMain(HINSTANCE hInstance,           \
                     HINSTANCE hPrevInstance,       \
                     LPSTR lpCmdLine,               \
                     int nCmdShow)                  \
{                                                   \
    a *app = new a;                                 \
    app->run();                                  \
    delete app;                                     \
    return 0;                                       \
}

#define  App Application::mApp;


