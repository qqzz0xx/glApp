#include "Mat4.h"
#include "Vec3.h"

const Mat4 Mat4::ZERO= Mat4(0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0
	);
const Mat4 Mat4::IDENTITY = Mat4(1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
	);
void Mat4::set(float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	m[0] = m11;
	m[1] = m12;
	m[2] = m13;
	m[3] = m14;

	m[4] = m21;
	m[5] = m22;
	m[6] = m23;
	m[7] = m24;

	m[8] = m31;
	m[9] = m32;
	m[10] = m33;
	m[11] = m34;
	
	m[12] = m41;
	m[13] = m42;
	m[14] = m43;
	m[15] = m44;
}

void Mat4::CreatePerspectiveLH(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Mat4* dst)
{
	dst->SetZero();

	float ctan = 1.0f / tan(fieldOfView / 2.0f);

	dst->m[0] = ctan / aspectRatio;
	dst->m[5] = ctan;
	dst->m[10] = 1 / (zFarPlane - zNearPlane);
	dst->m[11] = 1;
	dst->m[14] = (zNearPlane * zFarPlane) / (zNearPlane - zFarPlane);
}

void Mat4::CreateLookAtLH(float eyePositionX, float eyePositionY, float eyePositionZ,
	float targetPositionX, float targetPositionY, float targetPositionZ,
	float upX, float upY, float upZ, Mat4* dst)
{
	Vec3 eye(eyePositionX, eyePositionY, eyePositionZ);
	Vec3 up(upX, upY, upZ);
	Vec3 zaxis(targetPositionX - eyePositionX,
		targetPositionY - eyePositionY, targetPositionZ - eyePositionZ);

	Vec3 yaxis;
	Vec3 xaxis;
	up.Normalize();

	Vec3::Cross(zaxis, up, &xaxis);
	Vec3::Cross(xaxis, zaxis, &yaxis);

	zaxis.Normalize();
	yaxis.Normalize();
	xaxis.Normalize();

	dst->m[0] = xaxis.x;
	dst->m[1] = yaxis.x;
	dst->m[2] = zaxis.x;
	dst->m[3] = 0;

	dst->m[4] = xaxis.y;
	dst->m[5] = yaxis.y;
	dst->m[6] = zaxis.y;
	dst->m[7] = 0;

	dst->m[8] = xaxis.z;
	dst->m[9] = yaxis.z;
	dst->m[10] = zaxis.z;
	dst->m[11] = 0;

	dst->m[12] = -Vec3::Dot(eye, xaxis);
	dst->m[13] = -Vec3::Dot(eye, yaxis);
	dst->m[14] = -Vec3::Dot(eye, zaxis);
	dst->m[15] = 0;
	
}
