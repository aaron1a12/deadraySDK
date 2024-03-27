
// DeadEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "DeadEditor.h"
#include "MainFrm.h"

#include "DeadEditorDoc.h"
#include "DeadEditorView.h"

// Deadray
#include "Deadray.h"
#include "EditorHelperNode.h"
#include "Deadray/StaticMesh.h"


// bind2nd example
#include <iostream>
#include <functional>
#include <algorithm>
using namespace std;


using namespace Deadray;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




void debugPrintf(const char * _Format, ...)
{
	char buffer[256];
	va_list args;
	va_start (args, _Format);
	//vsnprintf (buffer, 255, _Format, args);
	vsnprintf_s(buffer, 255, 255, _Format, args);
	va_end (args);


	size_t newsize = strlen(_Format) + 1;
	wchar_t * wcstring = new wchar_t[newsize];

	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, buffer, _TRUNCATE);

	editorApp.Log(wcstring);

	delete wcstring;
	return;
}

DWORD CDeadEditorApp::EngineThread(EngineThreadInitParams& initParams)
{
	RECT winRect;
	GetWindowRect(initParams.hWindow, &winRect);

	Deadray::RenderSettings settings;
	settings.Width = winRect.right-winRect.left;
	settings.Height = winRect.bottom-winRect.top;
	settings.TargetFPS = 60;
	settings.BgColor = 0x808080;

	// Instatiate Deadray

	Engine* engine = new Deadray::Engine(initParams.hWindow, settings, &debugPrintf);

	//DMap<char*, int> ages;
	//ages.Insert("Aaron", 31);
	
	//engine->AddEventHandler(&initParams.pEditorApp->DispatchEvent, &initParams.pEditorApp);
	//engine->BindEvent<A>(&A::func);
	
	//engine->BindEventHandler(&reinterpret_cast<Deadray::CEvtHandler::func>(Deadray::FooEvent::func));

	engine->EnableGrid(true);

	// Update the engine pointer on the main thread.
	*initParams.pEngine = engine;

	// This node will listen for events in the Deadray scene
	EditorHelperNode* helper = new EditorHelperNode(engine);

	// Create the main scene and add a listener
	Deadray::Scene* myScene = new Deadray::Scene(engine);
	myScene->EvtSceneUpdate.AddListener(helper);
	
	//
	//
	//

	StaticMesh* warlock = myScene->CreateChildNode<StaticMesh>();

	//warlock->SetMesh("warlock.obj");
	//warlock->SetTextureFromFile(L"data\\warlock.jpg");
	//warlock->SetEnableAlpha(true);
	//warlock->SetPosition(Vector3(0.0, 0.0, 0.0));

	engine->SwitchScene( myScene );


	//float rot = 0;

	
	//TODO: The engine should do this on startup (StartDeviceObjects())
	engine->SetRenderSettings(settings);

	// engine loop

	engine->log("Starting render loop...");

	while (true)
	{	
		//rot += 90 * engine->GetDeltaTime();
		//warlock->SetRotation(Vector3(0.0, rot, 0.0));

		engine->Render();
		engine->Tick();
	}

	return 0;

}

void CDeadEditorApp::OnSceneUpdate(const Deadray::EventSceneUpdate& sceneUpdate)
{
	mainframe->GetScenePane().OnSceneUpdate(sceneUpdate);
}


// CDeadEditorApp

BEGIN_MESSAGE_MAP(CDeadEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDeadEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDeadEditorApp construction

CDeadEditorApp::CDeadEditorApp()
{

	m_bHiColorIcons = false;
	hDeadrayWin = NULL;
	engine = NULL;
	mainframe = NULL;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CDeadEditorApp object

CDeadEditorApp editorApp;


// CDeadEditorApp initialization

BOOL CDeadEditorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Wild Montage"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	editorApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDeadEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDeadEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//////////////////////////////////////////

	mainframe = (CMainFrame*)AfxGetMainWnd();

	//
	// Deadray Initialization
	//

	if (hDeadrayWin)
	{
		initParams.pEditorApp = this;
		initParams.hWindow = hDeadrayWin;
		initParams.pEngine = &engine;

		DWORD ThreadID;
		::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EngineThread, &initParams, 0, &ThreadID);
		mainframe->GetOutputPane().Output(L"Deadray thread started.");
	}

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CDeadEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDeadEditorApp customization load/save methods

void CDeadEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CDeadEditorApp::LoadCustomState()
{
}

void CDeadEditorApp::SaveCustomState()
{
}

// CDeadEditorApp message handlers

void CDeadEditorApp::RegisterDeadrayWindow(HWND hWindow)
{
	if (!hWindow)
	{
		MessageBoxA(m_pMainWnd->GetSafeHwnd(), "Bad window handle!", "",MB_OK | MB_ICONERROR);
	}

	hDeadrayWin = hWindow;
}

Deadray::Engine* CDeadEditorApp::GetEngine()
{
	return engine;
}

void CDeadEditorApp::Log(LPCTSTR lpszItem)
{
	if (mainframe)
	{
		COutputWnd& out = mainframe->GetOutputPane();
		out.Output(lpszItem);
	}
}

