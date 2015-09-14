#pragma once
#include "Application.h"

class MyApp : public Application
{
public:
	typedef	struct
	{
		float XYZW[4];
		float RGBA[4];
	}Vertex;
protected:
	virtual void init() override
	{
		info.windowHeight = 600;
		info.windowWidth = 800;
		static const char title[] = "MyApp";
		memcpy(info.title, title, sizeof(title));
		
	}
	virtual void Enter() override;


	virtual void Exit() override
	{
		DestroyVBO();
		DestroyShader();
	}
	void CreateShader();
	void CreateVBO();
	void DestroyVBO();
	void DestroyShader();

	virtual void Update(double dt) override;

private:

	GLuint iVbos;
	GLuint iVao;
	GLuint iFragID;
	GLuint iVertID;
	GLuint iProgramID;
};




