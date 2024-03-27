#ifndef   DEADRAY_OBJREADER_H
#define   DEADRAY_OBJREADER_H

#include "Deadray/Core.h"
#include "Deadray/Types.h"
#include <vector>

namespace Deadray {

	struct PrimitiveMeshData;

	#define OBJ_VERTEX 0
	#define OBJ_NORMAL 1
	#define OBJ_UV 2
	#define OBJ_FACE 3

	typedef WORD OBJ_VTYPE;

	

	class DEADRAY_API OBJReader {

		static struct TinyValBuffer {
			uint16 size;
			uint8* buffer;
		};

		static struct FaceIndices {
			uint32 vertex;
			uint32 uv;
			uint32 normal;
		};

		static struct Triangle {
			uint32 vertexA;
			uint32 vertexB;
			uint32 vertexC;
		};

		static struct TriangleIndices {
			DWORD A;
			DWORD B;
			DWORD C;
		};

	public:
		static bool ReadFromFile(
			LPCWSTR objFile, PrimitiveMeshData* meshData
			);

	};

}

#endif