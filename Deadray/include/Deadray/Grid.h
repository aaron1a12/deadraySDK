#ifndef   DEADRAY_GRID_H
#define   DEADRAY_GRID_H

#include "Deadray/Core.h"

namespace Deadray {

	class Engine;

	struct DEADRAY_API GridVertex
	{
		GridVertex() {}
		GridVertex(float _x, float _y, float _z, DWORD _color) {
			x = _x; y = _y; z = _z; color = _color;
		}

		float x,y,z;
		DWORD color;
	};

	typedef struct LineSegment { GridVertex points[2]; } LineSegment;

	class DEADRAY_API Grid {

	private:

		Engine* inst;

		LPDIRECT3DVERTEXBUFFER9 gridVertexBuffer;
		LPDIRECT3DVERTEXBUFFER9 axisVertexBuffer;

		GridVertex GridVertices[2];

		#define GRIDVERTEX_FVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)

		uint32 numLines;



	public:
		Grid(Engine* engine);
		~Grid();

		void OnDeviceStart();
		void OnDeviceShutdown();
		void Destroy();

		void Render();
	};

}

#endif