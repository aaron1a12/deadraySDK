
// DeadEditorView.h : interface of the CDeadEditorView class
//


#pragma once


class CDeadEditorView : public CView
{
protected: // create from serialization only
	CDeadEditorView();
	DECLARE_DYNCREATE(CDeadEditorView)

// Attributes
public:
	CDeadEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDeadEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool bOrbiting;
	bool bPanning;
	bool bZooming;
	bool bAltIsDown;
	POINT firstMousePos;

	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT, CPoint);
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg void OnMButtonDown(UINT, CPoint);
	afx_msg void OnMButtonUp(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
	afx_msg void OnRButtonUp(UINT, CPoint);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DeadEditorView.cpp
inline CDeadEditorDoc* CDeadEditorView::GetDocument() const
   { return reinterpret_cast<CDeadEditorDoc*>(m_pDocument); }
#endif

