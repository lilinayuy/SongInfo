// dllDSPlayer.h : main header file for the DLLDSPLAYER DLL
//

#if !defined(AFX_DLLDSPLAYER_H__1C61008B_E4E5_4904_B949_BCE3C52CE2D0__INCLUDED_)
#define AFX_DLLDSPLAYER_H__1C61008B_E4E5_4904_B949_BCE3C52CE2D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <streams.h>
#include "DSPlayer.h"

/////////////////////////////////////////////////////////////////////////////
// CDllDSPlayerApp
// See dllDSPlayer.cpp for the implementation of this class
//

class CDllDSPlayerApp : public CWinApp
{
public:
	CDllDSPlayerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllDSPlayerApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDllDSPlayerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

int hwprGetCDromDriver ( OUT CString &csCDROMDriver );
int GetCDTrackNum ( LPCTSTR lpszCDROM );

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDSPLAYER_H__1C61008B_E4E5_4904_B949_BCE3C52CE2D0__INCLUDED_)
