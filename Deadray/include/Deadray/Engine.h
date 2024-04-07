/*////////////////////////////////////////////////////////////////

Engine.h

The one and only: Engine. The lifecycle timeline is like so:

CreateNew() --> Starts Engine Thread
				--> Creates Thread Singleton
				--> Instatiates `Engine`
				--> Starts Game manager
			--> Engine pointer is returned.
				--> Starts render/tick loop




////////////////////////////////////////////////////////////////*/

#ifndef   DEADRAY_ENGINE_H
#define   DEADRAY_ENGINE_H

#include "Deadray/Core.h"
#include "Deadray/DMap.h"
#include <vector>
#include <deque>
#include <string>

namespace Deadray {

	class DebugText;
	class Scene;
	class GameManager;
	struct DebugLine;

	class DEADRAY_API Engine {

	private:

		RenderSettings currentRenderSettings;
		RenderSettings pendingRenderSettings; // for thead safe updates
		bool bUpdateSettings;

		bool bMultithread;
		DWORD engineThreadId;

		bool bIsEditor;

		// Time when the previous tick was made. (Used to calculate delta time)
		float previousTime;

		// Useful copy of latest delta time.
		float lastDt;

		float fps;
		float fpsMs;
		float timeSinceFpsUpdate;

		// The main rendering view
		HWND hMainWindow;

		D3DXMATRIX* view;
		D3DXMATRIX* proj;

		LPDIRECT3D9			g_pD3D; // Used to create the D3DDevice
		LPDIRECT3DDEVICE9	g_pd3dDevice ; // Our rendering device
		LPDIRECT3DVERTEXBUFFER9 g_pVB; // Buffer to hold Vertices
		LPDIRECT3DINDEXBUFFER9 g_pIB; // Stores VB Indices
		LPDIRECT3DTEXTURE9 g_pTexture;

		// These states don't survive device Reset() so we set them during startup and here
		void SetDefaultRenderStates();

		// Trigger all GPU buffer creation and loads stuff into GPU memory.
		void StartDeviceObjects();

		// Free all GPU buffers and unloads from memory. Must run before device Reset().
		void FreeDeviceObjects();

		// Boolean to check if we have already loaded resources into GPU devices.
		bool bDeviceObjsStarted;

		struct CUSTOMVERTEX
		{
			FLOAT x, y, z; // The transformed position for the vertex.
			DWORD color;        // The vertex color.
			FLOAT u, v;
		};

		#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)


		// Lines pending draw or release
		std::deque<DebugLine*> debugLines;

		uint32 width;
		uint32 height;

		Grid* grid;
		DebugText* debugText;

		std::deque<Scene*> scenes;
		Scene* activeScene;

		uint32 gameMgrTypeId;
		GameManager* gameMgr;

		HANDLE mutex;

	public:
		Engine(HWND window, RenderSettings& settings, uint32 gameMgrClass);
		~Engine();

		static Engine* CreateNew(const uint32 gameMgrClass, const HWND window, const RenderSettings& settings);
		static Engine* Get();

		inline bool IsEngineThread() { return (GetCurrentThreadId()==engineThreadId);}

		inline HANDLE CreateMutex() {mutex = ::CreateMutex(NULL, false, NULL); return mutex;}
		inline HANDLE GetMutex() {return mutex;}


		void StartGameManager();

		GameManager* GetGameManager() { return gameMgr; }


		template <class T>
		T* CreateGame ()
		{
			T* ptr = new T( this );
			return ptr;
		}

		// Frees all related resources.
		void Shutdown();
		bool bRequestShutdown;
		bool bCanSafelyExit;

		void SetAsMultithreaded(int deadrayThreadId);

		// Logging function. Maybe could be a fancy class someday?
		void (*log)(const char * format, ...);

		void HandleError(HRESULT error);

		void RegisterScene(Scene* scene);

		void SwitchScene(Scene* newScene);
		Scene* GetScene();

		void Render();
		void Tick();
		float GetDeltaTime();

		void RenderLoop();
		void TickLoop();

		void EnableGrid(bool bEnable = true);
		void EnableDebugText(bool bEnable = true);

		void DebugDrawLine(Vector3 start, Vector3 end, DWORD color, float duration);
		void DebugDrawCube(Vector3 center, float size, DWORD color, float duration);

		float camX;
		float camY;
		float camZ;

		//
		Vector3 camPos;
		Vector3 camRot;
		Vector3 camAim;
		Vector3 camUp;
		Vector3 camRight;
		Vector3 camForward;

		Vector3 camTmpRot;



		Vector3 cameraPos;
		Vector3 standIn;
		float preOrbitAngle;
		//

		POINT preOrbitMouse;

		Vector3 preOrbitPos;
		Vector3 preOrbitUp;

		Vector3 preOrbitUpRot;
		Vector3 tmpOrbitUpRot;


		float camYaw;
		float camPitch;

		float preOrbitYaw;
		float preOrbitPitch;

		float tmpOrbitYaw;
		float tmpOrbitPitch;

		float camDistZOld;
		float camDistZ;

		float camDistOld;
		float camDist;

		Vector3 camAimOld;
		Vector3 camAimMouseOffsetOld;
		D3DXMATRIX* camViewOld;
		D3DXMATRIX* camProjOld;

		D3DXVECTOR3* camLookAt;

		void SetCameraPos(float x, float y, float z);

		ScreenPtInfo ScreenToScene(float x, float y, bool bCustomMatrices = false, D3DXMATRIX* customView = nullptr, D3DXMATRIX* customProj = nullptr);

		void BeginCameraOrbit();
		void OrbitCamera(float pitch, float yaw);
		void OrbitCameraDist(float distance = 0);
		float GetCamDistExp(float z);
		float GetCamOrbitDist();
		void OrbitCameraPan();
		
		// Helper function for OrbitCameraPan(), returns mouse cursor position on a
		// 2D plane sitting at the orbit aim, orientated toward the view.
		Vector3 GetMouseOrbitPlanePos(bool useOldMatrix);

		void UpdateCameraOrbit();
		void EndCameraOrbit();
		

		RenderSettings GetRenderSettings();
		void SetRenderSettings(RenderSettings& newRenderSettings);
		void SetRenderSettingsThreadSafe(RenderSettings& newRenderSettings);
	
		LPDIRECT3DDEVICE9 GetD3dDevice();

		void DrawTextString(int x, int y, DWORD color, const char * str);
	};
}

#endif