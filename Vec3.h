#pragma once
#include <math.h>
class Vec3
{
public:
	float x, y, z;
public:
	Vec3();
	Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	~Vec3();

	void Normalize()
	{
		float num = x*x + y*y + z*z;
		if (num == 1.0f)
		{
			return;
		}
		num = sqrt(num);
		x = x / num;
		y = y / num;
		z = z / num;
	}

	static void Cross(const Vec3 &v1, const Vec3 &v2, Vec3 *dst)
	{
		dst->x = v1.y*v2.z - v2.x*v1.z;
		dst->y = v1.z*v2.x - v2.z*v1.x;
		dst->z = v1.x*v2.y - v2.x*v1.y;
	}

	static float Dot(const Vec3 &v1, const Vec3 &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	//inline Vec3 Lerp(const Vec3 &to, float alpha) const
	//{
	//	return *this*(1.f - alpha) + to * alpha;
	//}
	inline Vec3 Add(const Vec3 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

};

