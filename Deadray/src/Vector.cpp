#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Vector.h"

namespace Deadray {
	Vector3::Vector3(D3DXVECTOR3 inVector3) {
		x = inVector3.x; y = inVector3.y; z = inVector3.z;
	}

	void Vector3::Rotate(float pitch, float yaw, float roll)
	{
		D3DXVECTOR3 me = D3DXVECTOR3(x, y, z);

		D3DXMATRIXA16 rotation;
		D3DXMatrixRotationYawPitchRoll(&rotation, yaw * PI_180, pitch * PI_180, roll * PI_180);


		D3DXVec3TransformCoord(&me, &me, &rotation);

		x = me.x;
		y = me.y;
		z = me.z;

		return;

		/*
		char cad[512];
		sprintf(cad, "float: %f", x);
		OutputDebugStringA(cad);
		// x -> pitch
		// y -> yaw
		// z -> roll

		
		//xAxis /= 180.0 / PI;
		//yAxis /= 180.0 / PI;
		//zAxis = 0; // ???
		

		float vecX = x * cos(yAxis) * sin(x);
		float vecY = y * cos(xAxis);
		float vecZ = z * sin(yAxis) * sin(x);

		x = vecX;
		y = vecY;
		z = vecZ;*/
	}

	void Vector3::Rotate(D3DXMATRIXA16& rotationMatrix)
	{
		D3DXVECTOR3 me = D3DXVECTOR3(x, y, z);

		D3DXVec3TransformCoord(&me, &me, &rotationMatrix);

		x = me.x;
		y = me.y;
		z = me.z;

		return;
	}
}