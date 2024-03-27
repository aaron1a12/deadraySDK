#ifndef   DEADRAY_VECTOR_H
#define   DEADRAY_VECTOR_H

#include <math.h>
#include <stdio.h>

namespace Deadray {

	struct DEADRAY_API Vector3
	{
		Vector3() {}
		Vector3(float inX, float inY, float inZ) {x = inX; y = inY; z = inZ;}
		Vector3(D3DXVECTOR3 inVector3);

		float x,y,z;

		inline Vector3 operator+(const Vector3& b) const
		{
			return Vector3(
				x + b.x,
				y + b.y,
				z + b.z
			);
		}

		inline Vector3& operator+=(const Vector3& b)
		{
			x += b.x;
			y += b.y;
			z += b.z;

			return *this;
		}

		inline Vector3 operator-(const Vector3& b) const
		{
			return Vector3(
				x - b.x,
				y - b.y,
				z - b.z
			);
		}

		inline Vector3 operator*(const Vector3& b) const
		{
			return Vector3(
				x * b.x,
				y * b.y,
				z * b.z
			);
		}

		inline Vector3 operator*(const float& scale) const
		{
			return Vector3(
				x * scale,
				y * scale,
				z * scale
			);
		}

		inline static float Dot(Vector3 a, Vector3 b)
		{
			return a.x*b.x + a.y*b.y + a.z*b.z;
		}

		inline static Vector3 Cross(Vector3 a, Vector3 b)
		{
			return Vector3(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
				);
		}

		inline static float Dist(Vector3 a, Vector3 b)
		{
			return sqrt(DistSqr(a, b));
		}

		inline static float DistSqr(Vector3 a, Vector3 b)
		{
			float x = b.x-a.x;
			float y = b.y-a.y;
			float z = b.z-a.z;

			return (x*x) + (y*y) + (z*z);
		}

		void Rotate(float pitch, float yaw, float roll);

		inline void Rotate(Vector3 rotation)
		{
			 Rotate(rotation.x, rotation.y, rotation.z);
		}

		void Rotate(D3DXMATRIXA16& rotationMatrix);
	};


	struct DEADRAY_API Vector2
	{
		Vector2() {}
		Vector2(float inX, float inY) {x = inX; y = inY;}

		float x,y;
	};

}

#endif