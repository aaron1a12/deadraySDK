
// DeadEditorDoc.cpp : implementation of the CDeadEditorDoc class
//
#define D3D_DEBUG_INFO

#include "stdafx.h"
#include "DeadEditor.h"

#include "DeadEditorDoc.h"

// aaron added
#include "MainFrm.h"


using namespace Deadray;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// CDeadEditorDoc

IMPLEMENT_DYNCREATE(CDeadEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CDeadEditorDoc, CDocument)
END_MESSAGE_MAP()


// CDeadEditorDoc construction/destruction

CDeadEditorDoc::CDeadEditorDoc()
{
	// TODO: add one-time construction code here
	//MessageBoxA(NULL, "Doc created", "",MB_OK);

	//deadInst = NULL;
}

CDeadEditorDoc::~CDeadEditorDoc()
{
	//deadInst->Shutdown();
	//delete deadInst;
}

BOOL CDeadEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//MessageBoxA(AfxGetMainWnd()->GetSafeHwnd(), "CDeadEditorDoc::OnNewDocument()", "",MB_OK);

	//CMainFrame* mainFrame = (CMainFrame*)AfxGetMainWnd();
	//mainFrame->GetOutputPane().Output(L"HELLO WORLD FROM DOCUMENT!");
	//editorApp.Foobar();

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	//POSITION pos = GetFirstViewPosition();
	//CView* mainView = GetNextView(pos);

	return TRUE;
}




// CDeadEditorDoc serialization

void CDeadEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CDeadEditorDoc diagnostics

#ifdef _DEBUG
void CDeadEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDeadEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDeadEditorDoc commands

void CDeadEditorDoc::onUpdateViewSize(int cx, int cy)
{
	//MessageBoxA(NULL, "UPDATE SIZE", "",MB_OK);

	/*if (deadInst)
	{
		Deadray::RenderSettings settings = deadInst->GetRenderSettings();
		settings.Width = cx;
		settings.Height = cy;

		deadInst->SetRenderSettings(settings);
	}*/
}