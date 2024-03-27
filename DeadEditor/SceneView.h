
#pragma once

#include "ViewTree.h"

namespace Deadray {
	class EventSceneUpdate;
}

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CSceneView : public CDockablePane
{
public:
	CSceneView();
	virtual ~CSceneView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	void OnSceneUpdate(const Deadray::EventSceneUpdate& sceneUpdate);

protected:
	CClassToolBar m_wndToolBar;
	CViewTree m_wndSceneView;
	CImageList m_SceneViewImages;
	UINT m_nCurrSort;

	void FillSceneView();

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSceneAddMemberFunction();
	afx_msg void OnSceneAddMemberVariable();
	afx_msg void OnSceneDefinition();
	afx_msg void OnSceneProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

