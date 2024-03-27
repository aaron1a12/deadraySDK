#ifndef	DEADRAY_DEBUGDRAW_H
#define	DEADRAY_DEBUGDRAW_H

#include "Deadray/Core.h"

namespace Deadray {
	struct DebugLineVertex
	{
		float x, y, z; // The transformed position for the vertex.
		DWORD color;        // The vertex color.
	};

	#define D3DFVF_DEBUGLINEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

	struct DEADRAY_API DebugLine {
		DebugLineVertex points[2];
		LPDIRECT3DVERTEXBUFFER9 pVB;
		float life;
	};


	void DebugDrawLine(Engine* engine, Vector3 start, Vector3 end, DWORD color, float duration);
}

#endif