#pragma once
#include "Application.h"
#include "Mat4.h"
class Cube : public Application
{
public:
	Cube() {
		mFrameCount = 0; 
	}

	virtual void init() override;

	virtual void Enter() override;

	virtual void Update(double dt) override;

	virtual void Exit() override;
	void CreateCubeVBO();
	void CreateCubeShader();
private:
	INT64 mFrameCount;

	GLuint
		mProjectionMatrixUniformLocation,
		mViewMatrixUniformLocation,
		mModelMatrixUniformLocation;
	GLuint mBufferIds[3];
	GLuint mShaderIds[3];
	Mat4 
		ProjectionMatrix,
		ViewMatrix,
		ModelMatrix;
	const Vertex *mVertexs;
	const GLuint *mIndex;
};

