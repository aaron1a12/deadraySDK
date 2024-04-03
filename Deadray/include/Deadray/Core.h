#ifndef   DEADRAY_CORE_H
#define   DEADRAY_CORE_H

#pragma warning( disable : 4251 )

#include "windows.h"
#include <mmsystem.h>

typedef uintptr_t          uintptr;
typedef uintptr_t          PTR;
typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned int       uint;
typedef unsigned long long uint64;
typedef unsigned long	   ulong;

#define PI	  (3.1415926535897932f)
#define PI_180 (0.0174532925199432f)
#define EPSILON (D3DX_16F_EPSILON)
#define DR_GENERIC_ERROR		MAKE_HRESULT(1, 0, 0)
#define DR_RENDER_ERROR		MAKE_HRESULT(1, 0, 1)

#ifdef DEADRAY_DLL
#define DEADRAY_API __declspec(dllexport)
#include <d3dx9.h>
#else
#define DEADRAY_API __declspec(dllimport)
// D3D9 Forward Declarations
struct D3DXMATRIX;
struct D3DXMATRIXA16;
struct D3DXVECTOR3;
struct IDirect3DDevice9;
struct IDirect3D9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DTexture9;
struct ID3DXFont;
typedef struct IDirect3DDevice9 *LPDIRECT3DDEVICE9, *PDIRECT3DDEVICE9;
typedef struct IDirect3D9 *LPDIRECT3D9, *PDIRECT3D9;
typedef struct IDirect3DVertexBuffer9 *LPDIRECT3DVERTEXBUFFER9, *PDIRECT3DVERTEXBUFFER9;
typedef struct IDirect3DIndexBuffer9 *LPDIRECT3DINDEXBUFFER9, *PDIRECT3DINDEXBUFFER9;
typedef struct IDirect3DTexture9 *LPDIRECT3DTEXTURE9, *PDIRECT3DTEXTURE9;
#endif

#if _MSC_VER < 1600 //MSVC version < 8
	const
	class nullptr_t 
	{
	public:
	template<class T>
	inline operator T*() const
	{ return 0; }

	template<class C, class T>
	inline operator T C::*() const
	{ return 0; }

	private:
	void operator&() const;

	} nullptr = {};
#endif

#define READ(_READ_BUFF, _READ_NEEDLE, _READ_TYPE)  *(_READ_TYPE*)((PTR)_READ_BUFF + _READ_NEEDLE); _READ_NEEDLE+= sizeof(_READ_TYPE);
#define READ_VOID(_READ_BUFF, _READ_NEEDLE)  (void*)((PTR)_READ_BUFF + _READ_NEEDLE); _READ_NEEDLE+= sizeof(PTR);
#define WRITE(_WRITE_BUFF, _WRITE_NEEDLE, _WRITE_TYPE)  memcpy_s((void*)((PTR)_WRITE_BUFF + _WRITE_NEEDLE), sizeof(_WRITE_TYPE), &_WRITE_TYPE, sizeof(_WRITE_TYPE)); _WRITE_NEEDLE+= sizeof(_WRITE_TYPE);
#define WRITE_FILL(_WRITE_BUFF, _WRITE_NEEDLE, _WRITE_LENGTH, _WRITE_VALUE)  memset((void*)((PTR)_WRITE_BUFF+_WRITE_NEEDLE), _WRITE_VALUE, _WRITE_LENGTH); _WRITE_NEEDLE+= _WRITE_LENGTH;

//#include "Deadray/DMap.hpp"
#include "Deadray/Test.h"
#include "Deadray/Types.h"
#include "Deadray/TypeManager.h"
#include "Deadray/Event.h"
#include "Deadray/Vector.h"
#include "Deadray/Node.h"
#include "Deadray/Grid.h"
#include "Deadray/Scene.h"


#define DEBUG(...) {char cad[512]; sprintf(cad, __VA_ARGS__);  OutputDebugStringA(cad);}

namespace Deadray {
	struct RenderSettings {
		RenderSettings() {
			FOV = PI / 4;
			NearClip = 0.1f;
			FarClip = 100.0f;
		}

		uint16 TargetFPS;
		uint32 Width;
		uint32 Height;
		uint32 Scale;
		uint32 BgColor;
		float FOV;
		float NearClip;
		float FarClip;
	};

	struct ScreenPtInfo {
		ScreenPtInfo() {}
		ScreenPtInfo(Vector3 inPos, Vector3 inDir) {Position = inPos; Direction = inDir;}

		Vector3 Position;
		Vector3 Direction;
	};

	struct Triangle {
		Vector3 A;
		Vector3 B;
		Vector3 C;
	};

	bool RayHitsTriangle(Vector3 rayStart, Vector3 rayDir, const Triangle& triangle, Vector3& hitPoint);
}

#endif