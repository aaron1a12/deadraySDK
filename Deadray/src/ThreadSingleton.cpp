#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/Engine.h"
#include <cassert>

using namespace Deadray;

DWORD ThreadSingleton::tlsIndex = TlsAlloc();

ThreadSingleton::ThreadSingleton(DWORD currentThreadId)
{
	threadOwner = currentThreadId;
	TlsSetValue(tlsIndex, this);

	engine = nullptr;
}

ThreadSingleton* ThreadSingleton::Get()
{
    return static_cast<ThreadSingleton*>(TlsGetValue(tlsIndex));
}

Engine* ThreadSingleton::CreateEngine(HWND window, RenderSettings& settings, uint32 gameMgrClass)
{
	// Don't create the engine twice.
	assert(engine == nullptr);

	engine = new Engine(window, settings, gameMgrClass);
	return engine;
}

Engine* ThreadSingleton::GetEngine()
{
	// A nullptr engine might mean we're not being called from the engine thread
	assert(engine != nullptr);

	return engine;
}