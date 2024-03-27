#ifndef   DEADRAY_PRIMITIVE_H
#define   DEADRAY_PRIMITIVE_H

#include "Deadray/Core.h"
#include "Deadray/SceneNode.h"

namespace Deadray {

	struct DEADRAY_API PrimitiveVertex
	{
		/*PrimitiveVertex() {}
		PrimitiveVertex(float inX, float inY, float inZ, DWORD inColor, float inU, float inV) {
			x = inX;
			y = inY;
			z = inZ;

			color = inColor;
			u = inU;
			v = inV;

		}*/

		float x, y, z; // The transformed position for the vertex.
		DWORD color;        // The vertex color.
		float u, v;
	};

	#define D3DFVF_PRIMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)



	struct DEADRAY_API PrimitiveParams
	{
		PrimitiveParams() {}
		PrimitiveParams(LPCWSTR inFile) {pSrcFile = inFile;}

		LPCWSTR pSrcFile;
	};

	struct PrimitiveMeshData {
		uint32 vertexCount;
		uint32 indexCount;
		uint32 polyCount;
		
		PrimitiveVertex* vertices;
		WORD* indices;
		
		uint32 vertexBuffSize;
		uint32 indexBuffsize;		
	};


	class DEADRAY_API Primitive : public SceneNode {

	private:
		uint32 primitiveId;

		LPCWSTR diffusePath;
		bool bHasDiffuse;
		bool bHasAlpha;
		void ReloadDeviceTexture();

		PrimitiveMeshData meshData;

		void CreateQuad();


	public:
		Primitive(Node* parent, void* extra = nullptr);
		~Primitive();

		bool bOverlay;

		virtual void OnDeviceStart() override;
		virtual void OnDeviceShutdown() override;

		virtual void Destroy() override;

		virtual NodeType GetNodeType() override {
			return Types::Primitive;
		}

		LPDIRECT3DVERTEXBUFFER9 g_pVB; // Buffer to hold Vertices
		LPDIRECT3DINDEXBUFFER9 g_pIB; // Stores VB Indices
		LPDIRECT3DTEXTURE9 g_pTexture;


		//void 
		uint32 GetVertexCount();
		uint32 GetIndexCount();
		uint32 GetPolyCount();

		bool HasDiffuse();
		bool HasAlpha();

		void SetEnableAlpha(bool alpha);

		void SetTextureFromFile(LPCWSTR pSrcFile);


		//static Primitive* CreateFromOBJ(Node* parentLPCWSTR pSrcFile);


		//virtual bool ShouldRegisterAsTickable() override;	

		virtual void OnTick(float dt) override;		
	};

}

#endif