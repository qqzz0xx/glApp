#pragma once
#include "Macros.h"
#include "string.h"
class Mat4
{
public:
	float m[16];
public:
	Mat4() {
		memset(m, 0, 16);
	}
	~Mat4() {}
	Mat4(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44)
	{
		set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
	}
	void set(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);
	void SetZero()
	{
		memset(m, 0, 16);
	}
	static void CreatePerspectiveLH(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Mat4* dst);
	static void CreateLookAtLH(float eyePositionX, float eyePositionY, float eyePositionZ,
		float targetPositionX, float targetPositionY, float targetPositionZ,
		float upX, float upY, float upZ, Mat4* dst);
	static const Mat4 ZERO;
	static const Mat4 IDENTITY;
};


