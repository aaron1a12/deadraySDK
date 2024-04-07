#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/DebugText.h"

using namespace Deadray;

float roundf(float x)
{
   return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}

DebugText::DebugText()
{
	fps = 0.0f;
	fpsMs = 0.0f;
	timeSinceFpsUpdate = 0.0f;
}

DebugText::~DebugText()
{
}

void DebugText::OnDeviceStart()
{
	int g_nFontSize = 12;

	HDC hDC = GetDC( NULL );
    int nLogPixelsY = GetDeviceCaps( hDC, LOGPIXELSY );
    ReleaseDC( NULL, hDC );

    int nHeight = -g_nFontSize * nLogPixelsY / 72;

	HRESULT hr = D3DXCreateFont( Engine::Get()->GetD3dDevice(),            // D3D device
								 nHeight,               // Height
								 0,                     // Width
								 FW_BOLD,               // Weight
								 1,                     // MipLevels, 0 = autogen mipmaps
								 FALSE,                 // Italic
								 DEFAULT_CHARSET,       // CharSet
								 OUT_DEFAULT_PRECIS,    // OutputPrecision
								 NONANTIALIASED_QUALITY,       // Quality
								 DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
								 L"Lucida Console",              // pFaceName
								 &g_pFont );              // ppFont

	if (!FAILED(hr))
	{
		Engine::Get()->log("\nDebug text font created.\n");
	}
}

void DebugText::OnDeviceShutdown()
{
	if (g_pFont)
	{
		
		g_pFont->OnResetDevice();
		g_pFont->Release();
		g_pFont = NULL;
	}
}

void DebugText::Destroy()
{
	OnDeviceShutdown();
}

void DebugText::Render()
{
	float dt = Engine::Get()->GetDeltaTime();

	// Font

    UINT nHeight = Engine::Get()->GetRenderSettings().Height;

	RECT rc;

    // Pass in DT_NOCLIP so we don't have to calc the bottom/right of the rect
    SetRect( &rc, 11, 11, 0, 0 );

	// TODO: implement average FPS.
	timeSinceFpsUpdate += dt;
	if (timeSinceFpsUpdate > 0.05f)
	{
		timeSinceFpsUpdate = 0.0f;
		fps = 1/dt;
		fps *= 100.0f;
		fps = roundf(fps);
		fps /= 100.0f;
		fpsMs = dt * 1000;
	}


	const char fmt[] = "Deadray Engine\nfps: %.2f\nms:  %.2f";
	char buf[255];
	_snprintf(buf, 255, fmt, fps, fpsMs);

    g_pFont->DrawTextA( NULL, buf, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );

	SetRect( &rc, 10, 10, 0, 0 );

	g_pFont->DrawTextA( NULL, buf, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
}