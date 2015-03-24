// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "DlgProxy.h"
#include "SongInfoEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSongInfoEditorDlgAutoProxy

IMPLEMENT_DYNCREATE(CSongInfoEditorDlgAutoProxy, CCmdTarget)

CSongInfoEditorDlgAutoProxy::CSongInfoEditorDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CSongInfoEditorDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CSongInfoEditorDlg)))
		{
			m_pDialog = reinterpret_cast<CSongInfoEditorDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CSongInfoEditorDlgAutoProxy::~CSongInfoEditorDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CSongInfoEditorDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSongInfoEditorDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSongInfoEditorDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_ISongInfoEditor to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {325B416B-17F1-4E89-9743-F73242C8A2AA}
static const IID IID_ISongInfoEditor =
{ 0x325B416B, 0x17F1, 0x4E89, { 0x97, 0x43, 0xF7, 0x32, 0x42, 0xC8, 0xA2, 0xAA } };

BEGIN_INTERFACE_MAP(CSongInfoEditorDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CSongInfoEditorDlgAutoProxy, IID_ISongInfoEditor, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {3B6D88D7-0D0E-4066-9579-DFF9972D3EDC}
IMPLEMENT_OLECREATE2(CSongInfoEditorDlgAutoProxy, "SongInfoEditor.Application", 0x3b6d88d7, 0xd0e, 0x4066, 0x95, 0x79, 0xdf, 0xf9, 0x97, 0x2d, 0x3e, 0xdc)


// CSongInfoEditorDlgAutoProxy message handlers
