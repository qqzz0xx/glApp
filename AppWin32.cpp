#include "AppWin32.h"

void Cleanup()
{
	App->DestroyShaders();
	App->DestroyVBO();
}
void AppWin32::Initialize(int argc, char* argv[])
{
	CHECK_ERROR_BEGIN

	InitWindow(argc, argv);
	glewExperimental = GL_TRUE;
	glewInit();

	CHECK_ERROR_END

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

	CreateShaders();
	CreateVBO();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void AppWin32::InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);
	glutInitWindowSize(mWidth, mHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	mWndHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if (mWndHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	/*glutCloseFunc(::Cleanup);*/
	//glutReshapeFunc([](int x, int y){App->ResizeFunction(x, y); });
	//glutDisplayFunc([](){App->RenderFunction(); });
	//glutIdleFunc([](){App->IdleFunction(); });
	//glutTimerFunc(0, [](int x){App->TimerFunction(x); }, 0);

}
void AppWin32::ResizeFunction(int width, int height)
{
	mWidth = width;
	mHeight = height;

	glViewport(0, 0, width, height);
}
void AppWin32::RenderFunction(void)
{
	++mFrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}
void  AppWin32::IdleFunction(void)
{
	glutPostRedisplay();
}
void AppWin32::TimerFunction(int Value)
{
	if (0 != Value) {
		char* TempString = (char*)
			malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			mFrameCount * 4,
			mWidth,
			mHeight
			);

		glutSetWindowTitle(TempString);
		free(TempString);
	}

	mFrameCount = 0;
	glutTimerFunc(250, [](int x){App->TimerFunction(x); }, 1);
}

void AppWin32::DestroyVBO(void)
{
	CHECK_ERROR_BEGIN;
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(NULL);
	glDeleteVertexArrays(1, &VaoId);

	CHECK_ERROR_END
}

void AppWin32::CreateVBO(void)
{
	GLfloat Vertices[] = {
		-0.8f, -0.8f, 0.0f, 1.0f,
		0.0f, 0.8f, 0.0f, 1.0f,
		0.8f, -0.8f, 0.0f, 1.0f
	};

	GLfloat Colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	CHECK_ERROR_BEGIN;

	glGenVertexArrays(1, &VaoId);

	glBindVertexArray(VaoId);

	glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);

	CHECK_ERROR_END
}
void AppWin32::CreateShaders(void)
{
	CHECK_ERROR_BEGIN

	VertexShaderId = LoadShader("Simple.vert.glsl", GL_VERTEX_SHADER);
	FragmentShaderId = LoadShader("Simple.frag.glsl", GL_FRAGMENT_SHADER);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);
	glUseProgram(ProgramId);

	CHECK_ERROR_END

}

GLuint AppWin32::LoadShader(const char* filename, GLenum shader_type)
{
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	char* glsl_source;

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);

		if (NULL != (glsl_source = (char*)malloc(file_size + 1)))
		{
			if (file_size == (long)fread(glsl_source, sizeof(char), file_size, file))
			{
				glsl_source[file_size] = '\0';

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1, &glsl_source, NULL);
					glCompileShader(shader_id);
					ExitOnGLError("Could not compile a shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create a shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);

			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", file_size);

		fclose(file);
	}
	else
	{
		if (NULL != file)
			fclose(file);
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);
	}

	return shader_id;
}
void AppWin32::Cleanup()
{
	DestroyShaders();
	DestroyVBO();
}


void AppWin32::DestroyShaders()
{
	CHECK_ERROR_BEGIN;
	glUseProgram(NULL);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	glDeleteProgram(ProgramId);

	CHECK_ERROR_END
}

int main(int argc, char* argv[])
{
	App->Initialize(argc, argv);
	App->MainLoop();
	App->Exit();
}