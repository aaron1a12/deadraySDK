/*////////////////////////////////////////////////////////////////

ThreadSingleton.h

The ThreadSingleton is a thread-specific object that locally
stores the engine pointer.

Meant for nodes only, this object allows Engine::Get() to work.


////////////////////////////////////////////////////////////////*/


#ifndef   DEADRAY_THREADSINGLETON_H
#define   DEADRAY_THREADSINGLETON_H

#include "Deadray/Core.h"

namespace Deadray {

	class Engine;
	struct RenderSettings;

	class DEADRAY_API ThreadSingleton 
	{
		static DWORD tlsIndex;
		Engine* engine;
		DWORD threadOwner;

	public:

		ThreadSingleton(DWORD currentThreadId);

		// Gets the thread singleton. Call only from the engine thread!
		static ThreadSingleton* Get();

		Engine* CreateEngine(HWND window, RenderSettings& settings, uint32 gameMgrClass);

		inline Engine* GetEngine();
		inline DWORD GetThreadOwner() { return threadOwner; }
	};
}

#endif