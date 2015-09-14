#include "MyApp.h"

//int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
//{
//	Application *app = new MyApp;
//	app->run();
//	delete app;
//	return 0;
//}

//DECLARE_MAIN(MyApp)

void MyApp::Enter()
{
	CreateShader();
	CreateVBO();
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MyApp::CreateVBO()
{

	Vertex Vertices[] = {
			{ { -0.8f, -0.8f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { 0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};
	CHECK_ERROR_BEGIN;
	const size_t bufferSize = sizeof(Vertices);
	const size_t vertSize = sizeof(Vertices[0]);
	const size_t RGBOffset = sizeof(Vertices[0].XYZW);
	glGenVertexArrays(1, &iVao);

	glBindVertexArray(iVao);

	glGenBuffers(1, &iVbos);
	glBindBuffer(GL_ARRAY_BUFFER, iVbos);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, false, vertSize, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, false, vertSize, (void*)RGBOffset);
	glEnableVertexAttribArray(1);

	CHECK_ERROR_END
}

void MyApp::CreateShader()
{
	CreateShaderProgram("Simple", &iProgramID);
}


void MyApp::Update(double dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyApp::DestroyVBO()
{
	
}

void MyApp::DestroyShader()
{

}

