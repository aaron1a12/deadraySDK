#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/TypeManager.h"

#include <d3dx9.h>
#include "Deadray/d3d9/D3d9Data.h"

#include "Deadray/OBJReader.h"
#include "Deadray/MeshNode.h"

using namespace Deadray;

REGISTER_NODE_TYPE(Types::MeshNode, MeshNode);

MeshNode::MeshNode(Node* parent) : SceneNode(parent)
{
	bIsTickable = true;

	diffusePath = L"";
	bHasDiffuse = false;
	bHasAlpha = false;

	meshData.vertexCount = 0;
	meshData.indexCount = 0;
	meshData.polyCount = 0;

	meshData.vertices = nullptr;
	meshData.indices = nullptr;

	meshData.vertexBuffSize = 0;
	meshData.indexBuffsize = 0;

	d3d9Data = new D3d9Data();
	d3d9Data->g_pVB = NULL;
	d3d9Data->g_pIB = NULL;
	d3d9Data->g_pTexture = NULL;

	GetScene()->RegisterMeshNode(this);
}

MeshNode::~MeshNode()
{
	delete d3d9Data;
}

void MeshNode::OnDeviceStart()
{
	LPDIRECT3DDEVICE9 d3d = GetEngine()->GetD3dDevice();
	
	//D3DPOOL_MANAGED?
	d3d->CreateVertexBuffer( GetVertexCount() * sizeof(PrimitiveVertex), 0, D3DFVF_PRIMVERTEX, D3DPOOL_DEFAULT, &d3d9Data->g_pVB, NULL);
	d3d->CreateIndexBuffer(GetIndexCount() * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &d3d9Data->g_pIB, NULL);

	VOID* pVertices;
    d3d9Data->g_pVB->Lock(0, meshData.vertexBuffSize, (void**)&pVertices, 0 );
    memcpy(pVertices, meshData.vertices, meshData.vertexBuffSize);
    d3d9Data->g_pVB->Unlock();

	VOID* pIndices;
    d3d9Data->g_pIB->Lock(0, meshData.indexBuffsize, (void**)&pIndices, 0 );
    memcpy(pIndices, meshData.indices, meshData.indexBuffsize);
    d3d9Data->g_pIB->Unlock();

	// Texture
	if (bHasDiffuse)
		ReloadDeviceTexture();

}

void MeshNode::OnDeviceShutdown()
{
	d3d9Data->g_pVB->Release();
	d3d9Data->g_pIB->Release();
	d3d9Data->g_pTexture->Release();
}

void MeshNode::SetMesh(LPCWSTR pSrcFile)
{
	bool bResult = OBJReader::ReadFromFile(pSrcFile, &meshData);

	if (!bResult)
		GetEngine()->log("Failed to load external mesh! (%ws)", pSrcFile);

	if (d3d9Data->g_pVB != NULL)
	{
		// Reset device resources
		OnDeviceShutdown();
		OnDeviceStart();
	}
	
}

uint32 MeshNode::GetVertexCount()
{
	return meshData.vertexCount;
}

uint32 MeshNode::GetIndexCount()
{
	return meshData.indexCount;
}

uint32 MeshNode::GetPolyCount()
{
	return meshData.polyCount;
}

bool MeshNode::HasDiffuse()
{
	return bHasDiffuse;
}

bool MeshNode::HasAlpha()
{
	return bHasAlpha;
}

void MeshNode::SetEnableAlpha(bool alpha)
{
	bHasAlpha = alpha;
}

void MeshNode::SetTextureFromFile(LPCWSTR pSrcFile)
{
	diffusePath = pSrcFile;
	bHasDiffuse = true;

	if (d3d9Data->g_pTexture)
	{
		d3d9Data->g_pTexture->Release();
		ReloadDeviceTexture();
	}	
}

void MeshNode::ReloadDeviceTexture()
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


        D3DXCreateTextureFromFileInMemory(GetEngine()->GetD3dDevice(), &errTex, 0x66, &d3d9Data->g_pTexture);
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
		&d3d9Data->g_pTexture);

	//HRESULT hr = D3DXCreateTextureFromFile( GetEngine()->GetD3dDevice(), diffusePath, &d3d9Data->g_pTexture );
}