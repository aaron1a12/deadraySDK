#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Primitive.h"
#include "Deadray/Scene.h"
#include "Deadray/Engine.h"
#include "Deadray/MeshFile.h"
#include "Deadray/OBJReader.h"
#include "Deadray/DebugDraw.h"

using namespace Deadray;

REGISTER_NODE_TYPE(Types::Primitive, Primitive);

Primitive::Primitive(Node* parent, void* extra) : SceneNode(parent)
{
	bIsTickable = true;

	g_pVB = NULL;
	g_pIB = NULL;
	g_pTexture = NULL;

	bOverlay = false;
	bHasDiffuse = false;
	bHasAlpha = false;

	meshData.vertexCount = 0;
	meshData.indexCount = 0;
	meshData.polyCount = 0;

	
	meshData.vertices = nullptr;
	meshData.indices = nullptr;

	meshData.vertexBuffSize = 0;
	meshData.indexBuffsize = 0;

	primitiveId = GetScene()->RegisterPrimitive(this);

	//GetEngine()->log("Primitive #%d registered.", primitiveId);

	bool bCreateDefaultMesh = true;

	if(extra!=nullptr)
	{
		PrimitiveParams* params = (PrimitiveParams*)extra;

		if (wcslen(params->pSrcFile) != 0)
		{
			// Read from file

			if (!OBJReader::ReadFromFile(params->pSrcFile, &meshData))
			{
				GetEngine()->log("Failed to load external mesh! (%ws)", params->pSrcFile);
				bCreateDefaultMesh = true;
			}
			else
			{
				bCreateDefaultMesh = false;
			}
		}
	}


	if (bCreateDefaultMesh)
	{
		CreateQuad();
	}
}

Primitive::~Primitive(void)
{
}


void Primitive::CreateQuad()
{
	meshData.vertexCount = 4;
	meshData.indexCount = 6;
	meshData.polyCount = 2;

	// Initialize four Vertices for rendering a Quad
	
    PrimitiveVertex quadVerts[] =
    {
        { -1.0f, 1.0f, 0.0f, 0xffffffff, 0.0, 0.0 },
        {  1.0f, 1.0f, 0.0f, 0xffffffff, 1.0, 0.0},
        {  -1.0f, -1.0f, 0.0f, 0xffffffff, 0.0, 1.0},
		{  1.0f, -1.0f, 0.0f, 0xffffffff, 1.0, 1.0}
    };
	
	meshData.vertexBuffSize = sizeof(quadVerts);

	meshData.vertices = (PrimitiveVertex*)malloc(meshData.vertexBuffSize);
	memcpy((void*)meshData.vertices, &quadVerts, meshData.vertexBuffSize);

	// Make two joined triangles by using the previous verts

	WORD quadIndices[6] = {
		0, 1, 2, // first triangle
		2, 1, 3  // second
	};

	meshData.indexBuffsize = sizeof(quadIndices);

	meshData.indices = (WORD*)malloc(meshData.indexBuffsize);
	memcpy((void*)meshData.indices, &quadIndices, meshData.indexBuffsize);
}

uint32 Primitive::GetVertexCount()
{
	return meshData.vertexCount;
}

uint32 Primitive::GetIndexCount()
{
	return meshData.indexCount;
}

uint32 Primitive::GetPolyCount()
{
	return meshData.polyCount;
}

bool Primitive::HasDiffuse()
{
	return bHasDiffuse;
}

bool Primitive::HasAlpha()
{
	return bHasAlpha;
}

void Primitive::SetEnableAlpha(bool alpha)
{
	bHasAlpha = alpha;
}

void Primitive::SetTextureFromFile(LPCWSTR pSrcFile)
{
	diffusePath = pSrcFile;
	bHasDiffuse = true;

	if (g_pTexture)
	{
		g_pTexture->Release();
		ReloadDeviceTexture();
	}	
}

void Primitive::ReloadDeviceTexture()
{
	D3DXIMAGE_INFO info;
		
	if( FAILED(D3DXGetImageInfoFromFile(diffusePath, &info)) )
	{
		const unsigned char errTex[0x66] = {
			0x42, 0x4D, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
			0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
			0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
			0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00
		};


        D3DXCreateTextureFromFileInMemory(GetEngine()->GetD3dDevice(), &errTex, 0x66, &g_pTexture);
		bHasDiffuse = true;

		return;
	}
	
	HRESULT hr = D3DXCreateTextureFromFileExW(
		GetEngine()->GetD3dDevice(),
		diffusePath,
		info.Width,
		info.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255,0,255),
		&info,
		NULL,
		&g_pTexture);

	//HRESULT hr = D3DXCreateTextureFromFile( GetEngine()->GetD3dDevice(), diffusePath, &g_pTexture );
}


void Primitive::OnDeviceStart()
{
	LPDIRECT3DDEVICE9 d3d = GetEngine()->GetD3dDevice();
	
	//D3DPOOL_MANAGED?
	d3d->CreateVertexBuffer( GetVertexCount() * sizeof(PrimitiveVertex), 0, D3DFVF_PRIMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL);
	d3d->CreateIndexBuffer(GetIndexCount() * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

	VOID* pVertices;
    g_pVB->Lock(0, meshData.vertexBuffSize, (void**)&pVertices, 0 );
    memcpy(pVertices, meshData.vertices, meshData.vertexBuffSize);
    g_pVB->Unlock();

	VOID* pIndices;
    g_pIB->Lock(0, meshData.indexBuffsize, (void**)&pIndices, 0 );
    memcpy(pIndices, meshData.indices, meshData.indexBuffsize);
    g_pIB->Unlock();

	// Texture
	if (bHasDiffuse)
		ReloadDeviceTexture();

}

void Primitive::OnDeviceShutdown()
{
	g_pVB->Release();
	g_pIB->Release();
	g_pTexture->Release();
}

void Primitive::Destroy() 
{
	// Call parent class method
	SceneNode::Destroy();

	// TODO: release d3d resources here (vertex buffers, etc.)
	g_pVB->Release();
	g_pIB->Release();
	g_pTexture->Release();

	delete meshData.vertices;
	delete meshData.indices;

	// TODO: Unregister tick!
}


void Primitive::OnTick(float dt)
{
	Vector3 rot = GetRotation();
	//rot.x += 22.5f * dt;
	//rot.y += 45.f * dt;
	//rot.z += 90.f * dt;

	//this->SetRotation(Vector3(0.0, 45.0, 0.0));
	this->SetRotation(rot);



	// todo: MOVE UVS IN REAL TIMe!??

	VOID* pVertices;
    g_pVB->Lock(0, meshData.vertexBuffSize, (void**)&pVertices, 0 );
    //memcpy(pVertices, meshData.vertices, meshData.vertexBuffSize);
	g_pVB->Unlock();

	//GetEngine()->DebugDrawLine(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 1.0), 0xffffffff, 0);
}