
// DeadEditorView.cpp : implementation of the CDeadEditorView class
//

#include "stdafx.h"

#include "Deadray.h"
using namespace Deadray;

#include "DeadEditor.h"

#include "DeadEditorDoc.h"
#include "DeadEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CDeadEditorView

IMPLEMENT_DYNCREATE(CDeadEditorView, CView)

BEGIN_MESSAGE_MAP(CDeadEditorView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()

	// Enable cam controls
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()

	// General orbit updates
	ON_WM_MOUSEMOVE()

	// Tumble
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()

	// Pan/Dolly
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()

	// Zoom
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDeadEditorView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDeadEditorView construction/destruction

CDeadEditorView::CDeadEditorView()
{
	// TODO: add construction code here
	
	/*CDeadEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;*/

	bOrbiting = false;
	bPanning = false;
	bZooming = false;
	bAltIsDown = false;
}

CDeadEditorView::~CDeadEditorView()
{
	//MessageBoxA(this->GetSafeHwnd(), "View destroyed!", "",MB_OK);
}

BOOL CDeadEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CDeadEditorView::OnSize(UINT nType, int cx, int cy)
{
	Engine* engine = editorApp.GetEngine();

	if (engine)
	{
		RenderSettings settings = engine->GetRenderSettings();
		settings.Width = cx;
		settings.Height = cy;

		engine->SetRenderSettingsThreadSafe(settings);

		//engine->log("OnSize");
	}
}

int CDeadEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   // Call the base class to create the window
   if (CView::OnCreate(lpCreateStruct) == 0) {

      // If the window was successfully created
      editorApp.RegisterDeadrayWindow(this->GetSafeHwnd());
      return 0;
   }
   // Otherwise, return -1
   return -1;
}

// CDeadEditorView drawing

void CDeadEditorView::OnDraw(CDC* pDC)
{
	return;

	CDeadEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//MessageBoxA(this->GetSafeHwnd(), "Draw", "",MB_OK);

	RECT clRect;
	GetClientRect(&clRect);

	CRect rect(clRect);

	pDC->Rectangle(rect);

	CBrush brush;
	brush.CreateSolidBrush(RGB(128,128,128));
	pDC->FillRect(&rect,&brush);
}


BOOL CDeadEditorView::OnEraseBkgnd(CDC* pDC)
{
	return false;
}
// CDeadEditorView printing


void CDeadEditorView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CDeadEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDeadEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDeadEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


void CDeadEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	editorApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CDeadEditorView diagnostics

#ifdef _DEBUG
void CDeadEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CDeadEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDeadEditorDoc* CDeadEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDeadEditorDoc)));
	return (CDeadEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CDeadEditorView message handlers


void CDeadEditorView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 18)
		bAltIsDown = true;
}

void CDeadEditorView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	MessageBox(L"", L"", MB_OK);
	editorApp.GetEngine()->log("Char: %u", nChar);
	if (nChar == 18)
		bAltIsDown = false;
}

void CDeadEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetAsyncKeyState(VK_MENU) & 0x8000) // alt key down
	{
		POINT pos;
		GetCursorPos(&pos);

		int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
		int desktopHeight = GetSystemMetrics(SM_CYSCREEN);

		// How many degrees should we rotate per a full mouse sweep across the desktop
		const float sensitivity = 360;
	
		float x = (pos.x - firstMousePos.x) / (float)desktopWidth * sensitivity;
		float y = (pos.y - firstMousePos.y) / (float)desktopHeight * sensitivity;


		if (bOrbiting)
		{
			x *= -1.0;
			y *= -1.0;

			editorApp.GetEngine()->OrbitCamera(y, x);
		}

		if (bZooming)
		{
			editorApp.GetEngine()->OrbitCameraDist();
		}

		if (bPanning)
		{
			editorApp.GetEngine()->OrbitCameraPan();
		}
	}
}

void CDeadEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetCursorPos(&firstMousePos);

	if (bOrbiting == false)
	{
		editorApp.GetEngine()->BeginCameraOrbit();
		bOrbiting = true;
	}

}

void CDeadEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (bOrbiting)
		bOrbiting = false;
}

void CDeadEditorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	GetCursorPos(&firstMousePos);
	
	if (bPanning == false)
	{
		editorApp.GetEngine()->BeginCameraOrbit();
		bPanning = true;
	}
}

void CDeadEditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	bPanning = false;
}

void CDeadEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetCursorPos(&firstMousePos);

	if (bZooming == false)
	{
		editorApp.GetEngine()->BeginCameraOrbit();
		bZooming = true;
	}

}

void CDeadEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	bZooming = false;

	if (!bAltIsDown)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}
}

BOOL CDeadEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	editorApp.GetEngine()->BeginCameraOrbit();
	editorApp.GetEngine()->OrbitCameraDist(-(float)zDelta/120);

	return false;
}
