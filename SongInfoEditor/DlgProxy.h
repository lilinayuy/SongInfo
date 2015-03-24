// DlgProxy.h: header file
//

#pragma once

class CSongInfoEditorDlg;


// CSongInfoEditorDlgAutoProxy command target

class CSongInfoEditorDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CSongInfoEditorDlgAutoProxy)

	CSongInfoEditorDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CSongInfoEditorDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CSongInfoEditorDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSongInfoEditorDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

