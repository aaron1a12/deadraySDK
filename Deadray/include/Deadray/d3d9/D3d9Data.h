#ifndef   DEADRAY_D3D9DATA_H
#define   DEADRAY_D3D9DATA_H

/*
	The purpose of this struct is to avoid forward declarations of D3D types
	for client users of the Deadray library.

	This file is reserved for Deadray source files ONLY. No header should ever include this file.
*/

namespace Deadray {
	struct D3d9Data {
		LPDIRECT3DVERTEXBUFFER9 g_pVB; // Buffer to hold Vertices
		LPDIRECT3DINDEXBUFFER9 g_pIB; // Stores VB Indices
		LPDIRECT3DTEXTURE9 g_pTexture;
	};
}

#endif