
// DeadEditorDoc.h : interface of the CDeadEditorDoc class
//


#pragma once

namespace Deadray {
	class Engine;
}

class CDeadEditorDoc : public CDocument
{
protected: // create from serialization only
	CDeadEditorDoc();
	DECLARE_DYNCREATE(CDeadEditorDoc)

	// DEADRAY
	Deadray::Engine* deadInst;


// Attributes
public:

// Operations
public:
	void onUpdateViewSize(int cx, int cy);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDeadEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


