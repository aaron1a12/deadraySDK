
#include "stdafx.h"
#include "MainFrm.h"
#include "SceneView.h"
#include "Resource.h"
#include "DeadEditor.h"

// Deadray
#include "Deadray.h"

class CSceneViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CSceneView;

	DECLARE_SERIAL(CSceneViewMenuButton)

public:
	CSceneViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CSceneViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneView::CSceneView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CSceneView::~CSceneView()
{
}

BEGIN_MESSAGE_MAP(CSceneView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SCENE_ADD_MEMBER_FUNCTION, OnSceneAddMemberFunction)
	ON_COMMAND(ID_SCENE_ADD_MEMBER_VARIABLE, OnSceneAddMemberVariable)
	ON_COMMAND(ID_SCENE_DEFINITION, OnSceneDefinition)
	ON_COMMAND(ID_SCENE_PROPERTIES, OnSceneProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneView message handlers

int CSceneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndSceneView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CSceneViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CSceneViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CSceneViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Fill in some static tree view data (dummy code, nothing magic here)
	//FillSceneView();

	return 0;
}

void CSceneView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CSceneView::OnSceneUpdate(const Deadray::EventSceneUpdate& sceneUpdate)
{
	//LPCSTR nodeType = (LPCSTR)Deadray::Engine::GetNodeTypeName(sceneUpdate.newNode->GetNodeType());


	char* str = new char[100];
	sprintf(str, "%s (%u)", Deadray::Engine::GetNodeTypeName(sceneUpdate.newNode->GetNodeType()), sceneUpdate.newNode->GetNodeType());



	// convert to wide string
	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);


	
	HTREEITEM hRoot = m_wndSceneView.InsertItem(wcstring, 0, 0);
	m_wndSceneView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	delete wcstring;
	delete str;
}

void CSceneView::FillSceneView()
{
	HTREEITEM hRoot = m_wndSceneView.InsertItem(_T("Untitled Scene"), 0, 0);
	m_wndSceneView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndSceneView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	m_wndSceneView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_wndSceneView.Expand(hRoot, TVE_EXPAND);

	hClass = m_wndSceneView.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	m_wndSceneView.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_wndSceneView.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_wndSceneView.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_wndSceneView.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	m_wndSceneView.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_wndSceneView.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_wndSceneView.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_wndSceneView.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	m_wndSceneView.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_wndSceneView.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_wndSceneView.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_wndSceneView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndSceneView.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	m_wndSceneView.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	m_wndSceneView.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	m_wndSceneView.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_wndSceneView.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_wndSceneView.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = m_wndSceneView.InsertItem(_T("Globals"), 2, 2, hRoot);
	m_wndSceneView.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	m_wndSceneView.Expand(hClass, TVE_EXPAND);
}

void CSceneView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndSceneView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CSceneView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndSceneView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CSceneView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CSceneView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CSceneViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CSceneViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CSceneView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CSceneView::OnSceneAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CSceneView::OnSceneAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnSceneDefinition()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnSceneProperties()
{
	// TODO: Add your command handler code here
}

void CSceneView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CSceneView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndSceneView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CSceneView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndSceneView.SetFocus();
}

void CSceneView::OnChangeVisualStyle()
{
	m_SceneViewImages.DeleteImageList();

	UINT uiBmpId = editorApp.m_bHiColorIcons ? IDB_SCENE_VIEW_24 : IDB_SCENE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (editorApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_SceneViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_SceneViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndSceneView.SetImageList(&m_SceneViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(editorApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}
