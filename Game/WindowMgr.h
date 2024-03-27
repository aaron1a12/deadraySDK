#pragma once
#define MAX_LOADSTRING 100

class WindowMgr {
private:
	HWND hWnd;

	// Global Variables:
	HINSTANCE hInst;								// current instance
	TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

	// Forward declarations of functions included in this code module:
	static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	WindowMgr();

	inline HWND GetWindowHandle() {
		return hWnd;
	};
};