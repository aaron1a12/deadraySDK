
// DeadEditor.h : main header file for the DeadEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

namespace Deadray {
	class Engine;
	class Scene;
	class EventSceneUpdate;
}

class CDeadEditorApp;

struct EngineThreadInitParams {
	CDeadEditorApp* pEditorApp;
	Deadray::Engine** pEngine;
	HWND hWindow;
};



class CMainFrame;

// CDeadEditorApp:
// See DeadEditor.cpp for the implementation of this class
//

class CDeadEditorApp : public CWinAppEx
{
private:

	CMainFrame* mainframe;

	//
	// DEADRAY
	//

	HWND hDeadrayWin;
	Deadray::Engine* engine;
	EngineThreadInitParams initParams;
	static DWORD EngineThread(EngineThreadInitParams& initParams);

public:
	CDeadEditorApp();

	void RegisterDeadrayWindow(HWND hWindow);

	void Log(LPCTSTR lpszItem);

	Deadray::Engine* GetEngine();

	void OnSceneUpdate(const Deadray::EventSceneUpdate& sceneUpdate);

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDeadEditorApp editorApp;