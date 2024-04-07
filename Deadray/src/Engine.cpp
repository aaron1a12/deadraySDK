#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include "Deadray/DebugText.h"
#include "Deadray/Scene.h"
#include "Deadray/MeshNode.h"
#include "Deadray/GameManager.h"
#include "Deadray/DebugDraw.h"

#include "Deadray/d3d9/D3d9Data.h"
#include <math.h> 
#include <limits>	
#include <cassert>
#include <string>	
//#include <boost/unordered_map.hpp>

using namespace Deadray;

struct EngineThreadParams {
	Engine** pEngine;
	HWND hWindow;
	RenderSettings settings;
	uint32 gameMgrClass;
	bool bReady;

	EngineThreadParams() {
		pEngine = nullptr;
		hWindow = nullptr;
		bReady = false;
	}
};

DWORD EngineThread(EngineThreadParams& params)
{
	// Create the thread singleton
	ThreadSingleton* ts = new ThreadSingleton(GetCurrentThreadId());

	// Create the engine
	Engine* engine = ts->CreateEngine(params.hWindow, params.settings, params.gameMgrClass);
	engine->EnableGrid(true);
	engine->EnableDebugText(true);

	// Area of research: A mutex for safe IO with engine thread
	engine->CreateMutex();
	ReleaseMutex(engine->GetMutex());

 	// Start the game manager
	engine->StartGameManager();

	// Update the engine pointer on the main thread
	*params.pEngine = engine;

	//
	// Start Game
	//

	engine->GetGameManager()->OnGameStart();

	if (engine->GetScene())
		engine->GetScene()->OnGameStart();

	//
	
	// Signal the main thread. This really should be an atom
	params.bReady = true;

	while( engine->bRequestShutdown == false )
	{
		// TODO: Is it sensible to do this every frame?
		// The reasoning is that the main thread can only signal (edit variables)
		// on the engine between the frames/ticks.
		WaitForSingleObject(engine->GetMutex(), INFINITE);
		
		engine->Render();
		engine->Tick();

		// This allows the main thread control over the mutex, which is
		// a signal that we are allowed to write to the engine object safely.
		ReleaseMutex(engine->GetMutex());
	}

	// Shutdown
	engine->Shutdown();

	delete ts;
	return 0;
}

Engine* Engine::CreateNew(const uint32 gameMgrClass, const HWND window, const RenderSettings& settings)
{
	Engine* newInstance = nullptr;
 
	EngineThreadParams params;
	params.hWindow = window;
	params.pEngine = &newInstance;
	params.gameMgrClass = gameMgrClass;
	params.settings = settings;
	params.bReady = false;

	DWORD threadId;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EngineThread, &params, 0, &threadId);

	while (!params.bReady) {
		Sleep(0); // Weirdly enough, this improves performance.
	}

	return newInstance;
}

Engine* Engine::Get()
{
	ThreadSingleton* ts = ThreadSingleton::Get();
	
	return ThreadSingleton::Get()->GetEngine();
}

Engine::~Engine()
{
	if(IsEngineThread())
	{
		this->Shutdown();
	}
	else
	{
		// Wait until the render/tick has finished
		WaitForSingleObject(mutex, INFINITE);

		bRequestShutdown = true;

		// Release the mutex back to the engine thread
		ReleaseMutex(mutex);

		while(!bCanSafelyExit) { 
			Sleep(0);
		}
	}
}

void Engine::Shutdown()
{
	OutputDebugStringA("\nDeadray shutting down...\n");

	if (g_pd3dDevice != NULL) g_pd3dDevice->Release();
	if (g_pD3D != NULL) g_pD3D->Release();
	//if (g_pFont!=NULL) g_pFont->Release();

	if (grid)
		grid->Destroy();

	if (debugText)
		debugText->Destroy();

	delete view;
	delete proj;
	delete camViewOld;
	delete camProjOld;
	delete camLookAt;


	// Get a handle to the mutex and close it.
	WaitForSingleObject(mutex, INFINITE);
	CloseHandle(mutex);

	bCanSafelyExit = true;
}

void Engine::HandleError(HRESULT error)
{
	switch(error) {
		case DR_RENDER_ERROR: 
			MessageBoxA(NULL, "Rendering Error", "ERROR", MB_OK | MB_ICONERROR);
			TerminateProcess(GetCurrentProcess(), 0);
			break;
		case D3DERR_INVALIDCALL:
			MessageBoxA(NULL, "Invalid D3D call.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			break;
		default:
			MessageBoxA(hMainWindow, "Error", "Message", MB_ICONERROR);
	}

}

void debugPrintf(const char * _Format, ...)
{
	char buffer[256];
	va_list args;
	va_start (args, _Format);
	buffer[0] = '\n';
	vsnprintf_s(buffer+1, 254, 254, _Format, args);
	va_end (args);

	OutputDebugStringA(buffer);
}

Engine::Engine(HWND window, RenderSettings& settings, uint32 gameMgrClass)
{
	engineThreadId = GetCurrentThreadId();
	ThreadSingleton* ts = ThreadSingleton::Get();

	// Engine must instantiated on the engine thread
	assert(ts);
	assert(ts->GetThreadOwner() == engineThreadId);

	OutputDebugStringA("\nDeadray starting up...\n");

	
	DEBUG("sizeof vector: %d", sizeof(Vector3));

	bIsEditor = false;

	// Initialiazers
	bRequestShutdown = false;
	bCanSafelyExit = false;
	previousTime = (float)timeGetTime();
	lastDt = 0.0f;
	timeSinceFpsUpdate = 0.0f;
	activeScene = nullptr;
	currentRenderSettings = settings;
	bUpdateSettings = false;
	view = new D3DXMATRIX();
	proj = new D3DXMATRIX();
	camViewOld = new D3DXMATRIX();
	camProjOld = new D3DXMATRIX();
	camLookAt = new D3DXVECTOR3();

	hMainWindow = window;
	log = debugPrintf;	

	camPos = Vector3(8.f, 4.f, 0.f);
	camRot = Vector3(0.f, 0.f, 0.f);
	camPos.Rotate(camRot);
	camAim = Vector3(0.f, 0.f, 0.f);

	cameraPos = camPos;

	camX = 0.0f;
	camY = 1.f;
	camZ = 5.f;

	camUp = Vector3(0.f, 1.f, 0.f);
	camRight = Vector3(1.f, 0.f, 0.f);

	camYaw = -PI/4;
	camPitch = PI/8;
	
	camDistZ = 20.f;
	camDist = GetCamDistExp(camDistZ);

	this->UpdateCameraOrbit();


	grid = NULL;
	debugText = NULL;
	g_pD3D = NULL;
	g_pd3dDevice = NULL;
	

	bDeviceObjsStarted = false;
	
	// Initialize DirectX

	// Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		log("Failed to create DirectX9 object!");
		return;
	}

	log("Created DirectX9 object successfully.");

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// No vsync. Faster fps on vm XP
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hMainWindow,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED ,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
		log("Failed to create DirectX9 object!");
        return;
    }


	log("DirectX device set up.");

	SetDefaultRenderStates();

	RECT rcClient;
	GetClientRect(hMainWindow, &rcClient);

	D3DVIEWPORT9 viewport;
	ZeroMemory(&viewport, sizeof(D3DVIEWPORT9));

	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = rcClient.right - rcClient.left;
	viewport.Height = rcClient.bottom - rcClient.top;
	viewport.MinZ = 0;
	viewport.MaxZ = 1;

	g_pd3dDevice->SetViewport(&viewport);

	// Create/load all buffers to GPU
	//StartDeviceObjects();

	//
	// Game Manager
	// This is the class that manages the entire game life cycle
	// We can't create it yet since we're still instatiating the engine and
	// all calls to Engine::Get() are invalid at this point.
	//
	
	gameMgr = nullptr;
	gameMgrTypeId = gameMgrClass;
}

void Engine::StartGameManager() 
{
	gameMgr = (GameManager*)TypeManager.CreateNewObjectByType(gameMgrTypeId);
}

void Engine::SetDefaultRenderStates()
{
	// Turn off culling
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
}

void Engine::RegisterScene(Scene* scene)
{
	scenes.push_back(scene);
}

void Engine::DebugDrawLine(Vector3 start, Vector3 end, DWORD color, float duration)
{
	DebugLine* line = new DebugLine();

	line->points[0].x = start.x;
	line->points[0].y = start.y;
	line->points[0].z = start.z;
	line->points[0].color = color;

	line->points[1].x = end.x;
	line->points[1].y = end.y;
	line->points[1].z = end.z;
	line->points[1].color = color;

	line->life = duration;

	if( FAILED(g_pd3dDevice->CreateVertexBuffer( 2 * sizeof(DebugLineVertex), D3DUSAGE_WRITEONLY, D3DFVF_DEBUGLINEVERTEX, D3DPOOL_DEFAULT, &line->pVB, NULL )))
	{
		log("RIP");
		return;
	}

    VOID* pVertices;
    line->pVB->Lock(0, sizeof(line->points), (void**)&pVertices, 0 );
    memcpy(pVertices, line->points, sizeof(line->points));
    line->pVB->Unlock();

	debugLines.push_back(line);
}

void Engine::DebugDrawCube(Vector3 center, float size, DWORD color, float duration)
{
	float radius = size * 0.5;

	/* From the top view */

	//
	// First floor
	//

	Vector3 tl = Vector3(-1.0, 0.0, -1.0) * radius + center;
	Vector3 tr = Vector3(1.0, 0.0, -1.0) * radius + center;
	Vector3 bl = Vector3(-1.0, 0.0, 1.0) * radius + center;
	Vector3 br = Vector3(1.0, 0.0, 1.0) * radius + center;

	Vector3 top = Vector3(0.0, radius, 0.0);
	Vector3 bottom = Vector3(0.0, -radius, 0.0);

	// left line
	DebugDrawLine(top+tl, top+bl, color, duration);
	DebugDrawLine(top+tr, top+br, color, duration);
	DebugDrawLine(top+tl, top+tr, color, duration);
	DebugDrawLine(top+bl, top+br, color, duration);

	DebugDrawLine(bottom+tl, bottom+bl, color, duration);
	DebugDrawLine(bottom+tr, bottom+br, color, duration);
	DebugDrawLine(bottom+tl, bottom+tr, color, duration);
	DebugDrawLine(bottom+bl, bottom+br, color, duration);

	DebugDrawLine(top+tl, bottom+tl, color, duration);
	DebugDrawLine(top+tr, bottom+tr, color, duration);
	DebugDrawLine(top+br, bottom+br, color, duration);
	DebugDrawLine(top+bl, bottom+bl, color, duration);
}



void Engine::Render()
{
	/*threadId = GetCurrentThreadId();

	if (threadId == mainThreadId)
	{
			log("FATAL MISTAKE");
	}*/

	// Maybe create a special function that performs all main thread updates?
	if (bUpdateSettings)
	{
		//log("GetCurrentThreadId: %i", GetCurrentThreadId());
		//MessageBoxA(hMainWindow, "bUpdateSettings triggered", "",MB_OK);
		SetRenderSettings(pendingRenderSettings);
		bUpdateSettings = false;
	}


	D3DXMATRIXA16 matWorldIdentity;
	D3DXMatrixIdentity(&matWorldIdentity);

	D3DXVECTOR3 dxCamPos = D3DXVECTOR3(camPos.x, camPos.y, camPos.z);
	D3DXVECTOR3 dxCamLookAt = D3DXVECTOR3(camAim.x, camAim.y, camAim.z);
	D3DXVECTOR3 dxCamUp = D3DXVECTOR3(camUp.x, camUp.y, camUp.z);

	D3DXMatrixLookAtRH( view, &dxCamPos, &dxCamLookAt, &dxCamUp );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, view );


	// 2D screen projection

	float aspectRatio = currentRenderSettings.Width / (float)currentRenderSettings.Height;
	D3DXMatrixPerspectiveFovRH( proj, currentRenderSettings.FOV, aspectRatio, currentRenderSettings.NearClip, currentRenderSettings.FarClip );

	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, proj );

	if( NULL == g_pd3dDevice )
	{
		MessageBoxA(hMainWindow, "NO DEVICE FOUND", "",MB_OK | MB_ICONERROR);
        return;
	}

    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, currentRenderSettings.BgColor, 1.0f, 0 );
	//D3DCOLOR_XRGB( 160, 160, 160 )
	//g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 128, 128 ), 1.0f, 0 );
	//g_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0x3F, 0xAF, 0xFF ), 1.0f, 0L );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Rendering of scene objects can happen here

		if (grid) grid->Render();
		if (debugText) debugText->Render();

		// Render the primitives in the current scene

		if (activeScene)
		{
			///////////////
			// Debug lines 
			// TODO: precompiler statement here would be nice

			if (debugLines.size() != 0)
			{
				g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorldIdentity );

				std::deque<DebugLine*>::iterator it = debugLines.begin();

				while(it != debugLines.end()) {

					DebugLine* line = *it;

					if (line->pVB)
					{
						g_pd3dDevice->SetStreamSource( 0, line->pVB, 0, sizeof( DebugLineVertex ) );
						g_pd3dDevice->SetFVF(D3DFVF_DEBUGLINEVERTEX);
						g_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
					}


  					line->life -= lastDt;

					if (line->life <= 0.0)											
					{
						if (line->pVB)
							line->pVB->Release();
						delete line;

						it = debugLines.erase(it);
					}
					else ++it;
				}
			}

			////////////////////
			// Scene mesh nodes
			
			std::vector<MeshNode*>& meshNodes = activeScene->GetMeshNodeList();

			for (int i = 0; i < meshNodes.size(); i++)
			{
				MeshNode* msh = meshNodes[i];


				// TODO: Overlays need their own list since they need to be rendered last.
				//if (prim->bOverlay)
				//{
				//	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
				//}

				if(msh->HasDiffuse())
				{
					g_pd3dDevice->SetTexture( 0, msh->GetD3D9Data()->g_pTexture );

					if (msh->HasAlpha())
					{
						g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
						g_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
						g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true); 
					}
					else
					{
						g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
					}
					
					//g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					//g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);  
					//g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					//g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					//g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
					//g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				}
				else
				{
					g_pd3dDevice->SetTexture(0, NULL);
				}

				g_pd3dDevice->SetTransform( D3DTS_WORLD, &msh->GetWorldMatrix() );

				g_pd3dDevice->SetStreamSource( 0, msh->GetD3D9Data()->g_pVB, 0, sizeof( PrimitiveVertex ) );
				g_pd3dDevice->SetIndices(msh->GetD3D9Data()->g_pIB);
				g_pd3dDevice->SetFVF( D3DFVF_PRIMVERTEX );
				g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, msh->GetVertexCount(), 0, msh->GetPolyCount());
				
				//g_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, 0, prim->GetVertexCount());
				
			}

		}

		g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false); 

		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorldIdentity );
		g_pd3dDevice->SetTexture(0, NULL);	

        // End the scene
        g_pd3dDevice->EndScene();
    }
	else
	{
		HandleError(DR_RENDER_ERROR);
	}

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


RenderSettings Engine::GetRenderSettings()
{
	return currentRenderSettings;
}

void Engine::StartDeviceObjects()
{
	log("StartDeviceObjects()");

	if (bDeviceObjsStarted)
	{
		log("StartDeviceObjects() | early exit");
		return;
	}

	if (grid)
	{
		grid->OnDeviceStart();
	}

	if (debugText)
	{
		debugText->OnDeviceStart();
	}

	for (uint32 i = 0; i < scenes.size(); i++)
	{
		scenes[i]->OnDeviceStart();
    }

	bDeviceObjsStarted = true;
}

void Engine::FreeDeviceObjects()
{
	//if (bDeviceObjsStarted==false)
	//	return;

	if (grid)
	{
		grid->OnDeviceShutdown();
	}

	if (debugText)
	{
		debugText->OnDeviceShutdown();
	}

	for (int i = 0; i < debugLines.size(); i++)
	{
		if (debugLines[i]->pVB)
		{
			debugLines[i]->pVB->Release();
			debugLines[i]->pVB = NULL;
		}
    }

	for (int i = 0; i < scenes.size(); i++)
	{
		scenes[i]->OnDeviceShutdown();
    }

	bDeviceObjsStarted = false;
}


void Engine::SetRenderSettings(RenderSettings& newRenderSettings)
{
	currentRenderSettings = newRenderSettings;
	
	FreeDeviceObjects(); // You must release all memory before the reset.

	LPDIRECT3DSWAPCHAIN9 sc;
	g_pd3dDevice->GetSwapChain(0, &sc);
	//sc->Release(); ????

	D3DPRESENT_PARAMETERS pp;
	sc->GetPresentParameters(&pp);

	pp.BackBufferWidth = currentRenderSettings.Width;
	pp.BackBufferHeight = currentRenderSettings.Height;

	HRESULT hr = g_pd3dDevice->Reset(&pp); 
	if(hr != D3D_OK)
	{
		log("Failed to reset");
		///HandleError(hr);
	}

	SetDefaultRenderStates(); 
	StartDeviceObjects();
}

void Engine::SetRenderSettingsThreadSafe(RenderSettings& newRenderSettings)
{
	// Wait until the render/tick has finished
	WaitForSingleObject(mutex, INFINITE);

	pendingRenderSettings = newRenderSettings;
	bUpdateSettings = true;
	
	// Release the mutex back to the engine thread
	ReleaseMutex(mutex);

	// Synchronous wait

	do
	{
		WaitForSingleObject(mutex, INFINITE);

		if (bUpdateSettings==false)
			break;

		ReleaseMutex(mutex);
		Sleep(0);
	}
	while(true);
}


void Engine::EnableGrid(bool bEnable)
{
	if (bEnable && grid == NULL)
	{
		grid = new Grid(this);
		grid->OnDeviceStart();
	}
	else if (!bEnable && grid != NULL)
	{
		grid->Destroy();
		free(grid);

		grid = NULL;
	}
}

void Engine::EnableDebugText(bool bEnable)
{
	if (bEnable && debugText == NULL)
	{
		debugText = new DebugText();
		debugText->OnDeviceStart();
	}
	else if (!bEnable && debugText != NULL)
	{
		debugText->Destroy();
		free(debugText);

		debugText = NULL;
	}
}

LPDIRECT3DDEVICE9 Engine::GetD3dDevice()
{
	return g_pd3dDevice;
}



void Engine::SetCameraPos(float x, float y, float z)
{
	//camX = x;
	//camY = y;
	//camZ = z;
	camPos = Vector3(x, y, z);
}

ScreenPtInfo Engine::ScreenToScene(float x, float y, bool bCustomMatrices, D3DXMATRIX* customView, D3DXMATRIX* customProj)
{
	D3DXMATRIX matInverse;

	// Convert coordinates into the -1 to +1 range.
	float mX = ((2.0f * (float)x) / (float) currentRenderSettings.Width) - 1.0f;
	float mY = (((2.0f * (float)y) / (float) currentRenderSettings.Height) - 1.0f) * -1.0f;

	if (bCustomMatrices)
		D3DXMatrixInverse(&matInverse, NULL, &(*customView * *customProj));
	else
		D3DXMatrixInverse(&matInverse, NULL, &(*view * *proj));

	D3DXVECTOR3 v = D3DXVECTOR3(mX, mY, 0.f);
	D3DXVECTOR3 vEnd = D3DXVECTOR3(mX, mY, 0.999f);

	D3DXVec3TransformCoord(&v, &v, &matInverse);
	D3DXVec3TransformCoord(&vEnd, &vEnd, &matInverse);

	D3DXVECTOR3 vDir = vEnd-v;
	D3DXVec3Normalize(&vDir, &vDir);

	Vector3 origin = Vector3(v);
	Vector3 dir = Vector3(vDir);

	return ScreenPtInfo(Vector3(v), Vector3(vDir));
}

void Engine::BeginCameraOrbit()
{
	preOrbitYaw = camYaw;
	preOrbitPitch = camPitch;
	preOrbitPos = camPos;
	preOrbitUp = camUp;

	preOrbitAngle = atan2 (camPos.x, camPos.z) * 180.0 / PI;


	GetCursorPos(&preOrbitMouse);

	camDistZOld = camDistZ;
	camDistOld = camDist;

	camAimOld = camAim;
	//camViewOld = view;
	//camProjOld = proj;
	memcpy(camViewOld, view, sizeof(D3DMATRIX));
	memcpy(camProjOld, proj, sizeof(D3DMATRIX));

	camAimMouseOffsetOld = GetMouseOrbitPlanePos(false);
}

void Engine::OrbitCamera(float pitch, float yaw)
{
	//log("camPitch: %f", camPitch);
	camYaw = preOrbitYaw + yaw * 0.04;
	camPitch = preOrbitPitch - pitch * 0.04;

	UpdateCameraOrbit();
}

float Engine::GetCamDistExp(float z)
{
	// exp func

	float a = 1.f;
	float r = 0.1f;

	float y = pow(a*(1.f+r), z);
	
	if (y < currentRenderSettings.NearClip + EPSILON)
		return currentRenderSettings.NearClip + EPSILON;
	else
		return y;
}

float Engine::GetCamOrbitDist()
{
	return camDist;
}

void Engine::OrbitCameraDist(float distance)
{
	if (distance == 0.0)
	{
		POINT mouse;
		GetCursorPos(&mouse);

		float x = (mouse.x - preOrbitMouse.x);
		float y = (mouse.y - preOrbitMouse.y);
		distance = (x+y)*-0.5*0.05;
	}

	//log();
	
	
	camDistZ = camDistZOld + distance;
	camDist = GetCamDistExp(camDistZ);

	if (camDist == currentRenderSettings.NearClip + EPSILON)
	{
		// Reset mouse
		GetCursorPos(&preOrbitMouse);

		camAim += camForward * 4.f;
		camDistZOld = 14.8f; // hand-calibrated. terrible

		camDistZ = camDistZOld;
		camDist = GetCamDistExp(camDistZ);
	}

	UpdateCameraOrbit();
}

void Engine::OrbitCameraPan()
{
	Vector3 hitPoint = GetMouseOrbitPlanePos(true);


	Vector3 newOffset = camAimMouseOffsetOld - hitPoint;
	Vector3 newOffsetOff = newOffset;

	//DebugDrawCube(hitPoint, 1.0f, 0xffbbaa, 4.f);
	//DebugDrawLine(camAimMouseOffsetOld, hitPoint, 0xffff00ff, 2.0);

	camAim = camAimOld + newOffset; // working

	UpdateCameraOrbit();
}

Vector3 Engine::GetMouseOrbitPlanePos(bool useOldMatrix)
{
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient( hMainWindow, &pos );

	ScreenPtInfo ptInfo = ScreenToScene(
		(float)pos.x,
		(float)pos.y,
		true,
		(useOldMatrix) ? camViewOld : view, 
		(useOldMatrix) ? camProjOld : proj);

	// Figure out how big the orbit plane triangle should be
	// (not the best method since we're using camDist, we're getting a slightly curved dist)
	ScreenPtInfo topLeft = ScreenToScene(0.f, 0.f);
	ScreenPtInfo bottomRight = ScreenToScene((float)currentRenderSettings.Width, (float)currentRenderSettings.Height);
	float size = Vector3::Dist(topLeft.Position + topLeft.Direction*camDist, bottomRight.Position + bottomRight.Direction*camDist);

	// just make it huge
	size *= 100.f;

	Triangle tri;
	tri.A = camAimOld + camUp*size;
	tri.B = camAimOld - camUp*size + camRight*size;
	tri.C = camAimOld - camUp*size - camRight*size;

	Vector3 hitPoint;
	RayHitsTriangle(ptInfo.Position, ptInfo.Direction, tri, hitPoint);
	
	//DebugDrawCube(hitPoint, 0.5f, 0xffbbaa, 4.f);
	return hitPoint;
}

void Engine::UpdateCameraOrbit()
{
	D3DXQUATERNION camQuat;

	//
	// Yaw
	//

	D3DXVECTOR3 yawAxis;
	yawAxis.x = 0.0f;
	yawAxis.y = 1.0f;
	yawAxis.z = 0.0f;

	// Rotate the quat around the yaw axis
	D3DXQuaternionRotationAxis(&camQuat, &yawAxis, camYaw);

	//
	// Pitch
	//

	D3DXVECTOR3 pitchAxis;
	pitchAxis.x = 0.0f;
	pitchAxis.y = 0.0f;
	pitchAxis.z = 1.0f;

	// The pitch axis should now be rotated by the above yaw rotation

	D3DXMATRIXA16 pitchRotation;
	D3DXMatrixRotationYawPitchRoll(&pitchRotation, camYaw, 0, 0);
	D3DXVec3TransformCoord(&pitchAxis, &pitchAxis, &pitchRotation);

	// Create the pitch quat and rotate it by the axis made above

	D3DXQUATERNION pitchQuat;
	D3DXQuaternionRotationAxis(&pitchQuat, &pitchAxis, camPitch);

	// Multiply it into the camQuat (already has yaw)

	D3DXQuaternionMultiply(&camQuat, &camQuat, &pitchQuat);


	// Build final rotation matrix from quat

	D3DXMATRIXA16 rotationMatrix;
	D3DXMatrixRotationQuaternion(&rotationMatrix, &camQuat);

	// Create the initial camera position, and apply the total rotation

	Vector3 initPos = Vector3(camDist, 0.0, 0.0);
	Vector3 newPos = initPos;

	initPos.Rotate(rotationMatrix);

	initPos += camAim;

	// Up vector (Controls cam orientation/roll). Keep relatively up.

	Vector3 up = Vector3(0.0, 1.0, 0.0);
	up.Rotate(rotationMatrix);

	// Right vector (used for panning)
	Vector3 right = Vector3(0.0, 0.0, 1.0);
	right.Rotate(rotationMatrix);

	// Forward vector (for ray casts)
	Vector3 forward = Vector3(-1.0, 0.0, 0.0);
	forward.Rotate(rotationMatrix);

	// Update cam

	camPos = initPos;
	camUp = up;
	camRight = right;
	camForward = forward;

	return;	
}

void Engine::EndCameraOrbit()
{
	return;
	camYaw = tmpOrbitYaw;
	camPitch = tmpOrbitPitch;
	camRot = camTmpRot;
}

Scene* Engine::GetScene()
{
	return activeScene;
}

void Engine::SwitchScene(Scene* newScene)
{
	activeScene = newScene;
}

void Engine::Tick()
{
	// Process events



	//

	float currentTime = (float)timeGetTime();
	float deltaTime = (currentTime - previousTime) * 0.001f;

	previousTime = currentTime;
	lastDt = deltaTime;

	// Tick the active scene and it'll automatically tick all child nodes

	if (activeScene != nullptr)
	{
		activeScene->Tick(deltaTime);
	}
}


float Engine::GetDeltaTime()
{
	return lastDt;
}

void Engine::SetAsMultithreaded(int deadrayThreadId)
{
	bMultithread = true;
	//engineThreadId = deadrayThreadId;
}