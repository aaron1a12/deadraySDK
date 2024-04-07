// Main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"
#include "WindowMgr.h"

#include "Game.h"

using namespace Deadray;

Deadray::Engine* engine;

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

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  LPTSTR lpCmdLine, int nCmdShow)
{
	/*char* str = new char[16];
	sprintf(str, "Main Thread ID: %u", GetCurrentThreadId());
	MessageBoxA(NULL, str, "", MB_OK);*/

	WindowMgr wndMgr;

	RECT winRect;
	GetWindowRect(wndMgr.GetWindowHandle(), &winRect);

	Deadray::RenderSettings settings;
	settings.Width = winRect.right-winRect.left;
	settings.Height = winRect.bottom-winRect.top;
	settings.TargetFPS = 60;
	settings.BgColor = 0;

	Engine* engine = Deadray::Engine::CreateNew(GAME_CLASS, wndMgr.GetWindowHandle(), settings);

	engine->log("Resolution: %i, %i", settings.Width, settings.Height);

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete engine;
	return (int) msg.wParam;
}