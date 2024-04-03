#ifndef   DEADRAY_MESHNODE_H
#define   DEADRAY_MESHNODE_H

#include "Deadray/Core.h"
#include "Deadray/SceneNode.h"

namespace Deadray {

	struct D3d9Data;

	struct DEADRAY_API PrimitiveVertex
	{
		float x, y, z; // The transformed position for the vertex.
		DWORD color;   // The vertex color.
		float u, v;
	};

	#define D3DFVF_PRIMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

	struct PrimitiveMeshData {
		uint32 vertexCount;
		uint32 indexCount;
		uint32 polyCount;
		
		PrimitiveVertex* vertices;
		WORD* indices;
		
		uint32 vertexBuffSize;
		uint32 indexBuffsize;
	};


	class DEADRAY_API MeshNode : public SceneNode {
	public:
		void SetMesh(LPCWSTR pSrcFile);

		uint32 GetVertexCount();
		uint32 GetIndexCount();
		uint32 GetPolyCount();

		bool HasDiffuse();
		bool HasAlpha();

		void SetEnableAlpha(bool alpha);

		void SetTextureFromFile(LPCWSTR pSrcFile);

		inline D3d9Data* GetD3D9Data() { return d3d9Data; }

	private:

		LPCWSTR diffusePath;
		bool bHasDiffuse;
		bool bHasAlpha;

		void ReloadDeviceTexture();

		PrimitiveMeshData meshData;

		D3d9Data* d3d9Data;

	public:
		MeshNode(Node* parent);
		~MeshNode();

		virtual void OnDeviceStart() override;
		virtual void OnDeviceShutdown() override;
		
		virtual uint32 GetNodeType() override {return Types::MeshNode;}
	};
}

#endif