#ifndef   DEADRAY_ENGINE_H
#define   DEADRAY_ENGINE_H

#include "Deadray/Core.h"
#include <vector>
#include <deque>
#include <string>

//#include <boost/unordered_map.hpp>

namespace Deadray {

	class Primitive;
	class Scene;
	struct DebugLine;

	class DEADRAY_API Engine {

	private:
		//static DMap<const char*, uint32> nodeTypes;

		//boost::unordered::unordered_map<std::string, uint32> foo;


		RenderSettings currentRenderSettings;
		RenderSettings pendingRenderSettings; // for thead safe updates
		bool bUpdateSettings;

		bool bMultithread;
		DWORD mainThreadId;
		int threadId;

		bool bIsEditor;

		// Time when the previous tick was made. (Used to calculate delta time)
		float previousTime;

		// Useful copy of latest delta time.
		float lastDt;

		float fps;
		float fpsMs;
		float timeSinceFpsUpdate;

		// Tick function for a high resolution timer. (calls Render())
		static void CALLBACK RenderTick(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

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


		//std::vector<Primitive*> primitives;

		std::deque<Scene*> scenes;
		Scene* activeScene;


		ID3DXFont* g_pFont;

	public:
		Engine(HWND window, RenderSettings& settings, void (*logFunc)(const char * format, ...), bool bEditor = false);
		~Engine();

		// Frees all related resources.
		void Shutdown();

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

		/// EVENTS

		/*std::deque<EventHandlerItem> evtHandlerItems;
		void AddEventHandler(EventHandler handler, void* owner);

		template <typename T>
		void BindEvent(void (__thiscall T::* )(LPCWSTR))
		{
		}

		void BindEventHandler(void (__thiscall CEvtHandler::* )(LPCWSTR))
		{
		}

		void BroadcastEvent(EvtMsg evtMsg);*/


		/*template <typename T>
		void BroadcastEvent(T)
		{
		}*/

		static bool RegisterNewType(const char* );



	};

}

#endif