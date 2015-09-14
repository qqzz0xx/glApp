#include "Cube.h"

DECLARE_MAIN(Cube)

void Cube::init()
{
	__super::init();
	
	info.windowWidth = 800;
	info.windowHeight = 600;
	static const char title[] = "Cube";
	strcpy(info.title, title);

	ModelMatrix = Mat4::IDENTITY;
	ProjectionMatrix = Mat4::IDENTITY;
	ViewMatrix = Mat4::IDENTITY;
}

void Cube::Enter()
{
	LoadVertexShader("Cube", &mShaderIds[1]);
	LoadFragmentShader("Cube", &mShaderIds[2]);

	mShaderIds[0] = glCreateProgram();
	glAttachShader(mShaderIds[0], mShaderIds[1]);
	glAttachShader(mShaderIds[0], mShaderIds[2]);

	glLinkProgram(mShaderIds[0]);
	glUseProgram(mShaderIds[0]);

	mModelMatrixUniformLocation = glGetUniformLocation(mShaderIds[0], "ModelMatrix");
	mViewMatrixUniformLocation = glGetUniformLocation(mShaderIds[0], "ViewMatrix");
	mProjectionMatrixUniformLocation = glGetUniformLocation(mShaderIds[0], "ProjectionMatrix");
	CreateCubeVBO();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void Cube::Update(double dt)
{
	mFrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform4fv(mModelMatrixUniformLocation, 1, ModelMatrix.m);
	glUniform4fv(mViewMatrixUniformLocation, 1, ViewMatrix.m);
	glUniform4fv(mProjectionMatrixUniformLocation, 1, ProjectionMatrix.m);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, mIndex);
}

void Cube::Exit()
{
	
}

void Cube::CreateCubeVBO()
{
	static const Vertex VERTICES[8] =
	{
		{ { -.5f, -.5f, .5f, 1 }, { 0, 0, 1, 1 } },
		{ { -.5f, .5f, .5f, 1 }, { 1, 0, 0, 1 } },
		{ { .5f, .5f, .5f, 1 }, { 0, 1, 0, 1 } },
		{ { .5f, -.5f, .5f, 1 }, { 1, 1, 0, 1 } },
		{ { -.5f, -.5f, -.5f, 1 }, { 1, 1, 1, 1 } },
		{ { -.5f, .5f, -.5f, 1 }, { 1, 0, 0, 1 } },
		{ { .5f, .5f, -.5f, 1 }, { 1, 0, 1, 1 } },
		{ { .5f, -.5f, -.5f, 1 }, { 0, 0, 1, 1 } }
	};

	static const GLuint INDICES[36] =
	{
		0, 2, 1, 0, 3, 2,
		4, 3, 0, 4, 7, 3,
		4, 1, 5, 4, 0, 1,
		3, 6, 2, 3, 7, 6,
		1, 6, 5, 1, 2, 6,
		7, 5, 6, 7, 4, 5
	};

	mVertexs = VERTICES;
	mIndex = INDICES;

	glGenVertexArrays(1, &mBufferIds[0]);
	glBindVertexArray(mBufferIds[0]);

	glGenBuffers(2, &mBufferIds[1]);

	glBindBuffer(GL_ARRAY_BUFFER, mBufferIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(VERTICES[0]), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mBufferIds[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(VERTICES[0]), (void*) sizeof(VERTICES[0].XYZW));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
void Cube::CreateCubeShader()
{
	CreateShaderProgram("Cube", &mShaderIds[0]);

	mModelMatrixUniformLocation = glGetUniformLocation(mShaderIds[0], "ModelMatrix");
	mViewMatrixUniformLocation = glGetUniformLocation(mShaderIds[0], "ViewMatrix");
	mProjectionMatrixUniformLocation = glGetUniformLocation(mShaderIds[0], "ProjectionMatrix");

}
