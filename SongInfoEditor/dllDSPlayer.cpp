// dllDSPlayer.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "dllDSPlayer.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HW_CD_MEDIA_ALIAS _T("hwCDMediaAlias")
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CDllDSPlayerApp

BEGIN_MESSAGE_MAP(CDllDSPlayerApp, CWinApp)
	//{{AFX_MSG_MAP(CDllDSPlayerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDllDSPlayerApp construction

CDllDSPlayerApp::CDllDSPlayerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDllDSPlayerApp object

CDllDSPlayerApp theApp;

int CDllDSPlayerApp::ExitInstance() 
{
	CoUninitialize();
	return CWinApp::ExitInstance();
}

BOOL CDllDSPlayerApp::InitInstance() 
{
	CoInitialize(NULL);
	return CWinApp::InitInstance();
}

void DispatchMciError ( MCIERROR mciError, LPCTSTR lpszCmd )
{
	if ( mciError )		//get error
	{
		TCHAR chError[100];
		mciGetErrorString ( mciError,chError,lstrlen(chError) );
		#ifdef _DEBUG
		CString csLastError;
		csLastError.Format (_T("%s : %s"), lpszCmd, chError );
		AfxMessageBox ( csLastError );
		#endif
	}
}

BOOL hwMciSendString (
		LPCTSTR lpszCommand,  
		LPTSTR lpszReturnString,  
		UINT cchReturn,       
		HWND hwndCallback
	)
{
	if ( !lpszCommand ) return FALSE;
	MCIERROR mciError = ::mciSendString ( lpszCommand, lpszReturnString, cchReturn, hwndCallback );
//	DbgLog ( L_DEBUG, "mciSendString : %s", lpszCommand );
	if ( mciError )
	{
		DispatchMciError ( mciError, lpszCommand );
		return FALSE;
	}

	return TRUE;
}

BOOL hwMciSendCommand (
		MCIDEVICEID IDDevice,  
		UINT uMsg,             
		DWORD fdwCommand,      
		DWORD dwParam          
	)
{
	MCIERROR mciError = ::mciSendCommand ( IDDevice, uMsg, fdwCommand, dwParam );
	if ( mciError )
	{
		CString csCmd;
		csCmd.Format (_T("Command <%d>"), fdwCommand );
		DispatchMciError ( mciError, csCmd );
		return FALSE;
	}

	return TRUE;
}

//
// 获取 CD-ROM 驱动器符号，以'\t'分隔。如：E:\tF:\tG:\t
// return : -----------------------------------------------------------------------------
//		>0	-	找到多少个CD-ROM驱动器
//		-1	-	失败
//
int hwprGetCDromDriver ( OUT CString &csCDROMDriver )
{
	int nCount = 0;

	for ( int nLoop = _T('A') ; nLoop <= _T('Z'); nLoop ++ )
	{
		CString csRoot;
		csRoot.Format (_T("%c:"), nLoop );

		if ( GetDriveType(csRoot) == DRIVE_CDROM )
		{
			csCDROMDriver += csRoot;
			csCDROMDriver += _T("\t");
			nCount ++;
		}
	}

	return nCount;
}

BOOL hwMciClose ( MCIDEVICEID &wDeviceID )
{
	if ( wDeviceID == (MCIDEVICEID)0 )
		return TRUE;
	CString csMciCommand;
	csMciCommand.Format (_T("close %s"), HW_CD_MEDIA_ALIAS );
	if ( !hwMciSendString ( csMciCommand, NULL, 0, NULL ) )
		return FALSE;
	wDeviceID = 0;

	return TRUE;
}

BOOL CheckCDValidity ( MCIDEVICEID &wDeviceID )
{
	if ( wDeviceID == 0 )
		return -1;
	MCI_STATUS_PARMS	mciStatusParms;
	mciStatusParms.dwItem = MCI_CDA_STATUS_TYPE_TRACK;
	mciStatusParms.dwTrack = 1;
    if ( !hwMciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms) )
		return FALSE;
	if ( mciStatusParms.dwReturn != MCI_CDA_TRACK_AUDIO )
		return FALSE;

	return TRUE;
}

int GetAudioDiskInfo ( MCIDEVICEID &wDeviceID )
{
	if ( wDeviceID == 0 )
		return -1;
	MCI_STATUS_PARMS mciStatusParms;
	mciStatusParms.dwItem = MCI_STATUS_READY;
    if ( !hwMciSendCommand(wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms) )
	{
		goto failed;
	}
	if ( !mciStatusParms.dwReturn )
	{
		goto failed;
	}

	mciStatusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
    if ( !hwMciSendCommand ( wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD) (LPVOID) &mciStatusParms ) )
	{
		goto failed;
	}

	if ( mciStatusParms.dwReturn < 1 )
	{
		goto failed;
	}
	if ( !CheckCDValidity ( wDeviceID ) ) goto failed;
	return (int)mciStatusParms.dwReturn;

	return TRUE;

failed:
	hwMciClose ( wDeviceID );
	return -1;

}
//
// 获取CD的轨道数，失败时返回-1
//
int GetCDTrackNum ( LPCTSTR lpszCDROM )
{
	if ( !(lpszCDROM && _tcslen(lpszCDROM) == DRIVER_ROOT_LENGTH && lpszCDROM[1] == ':') )
		return -1;
	MCIDEVICEID wDeviceID = -1;
	MCI_OPEN_PARMS	mciOpenParms;
    MCI_SET_PARMS	mciSetParms;
	mciOpenParms.lpstrDeviceType = _T("cdaudio");
	mciOpenParms.lpstrElementName = lpszCDROM;
	mciOpenParms.lpstrAlias = HW_CD_MEDIA_ALIAS;
	if ( !hwMciSendCommand ( NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_ALIAS | MCI_OPEN_TYPE, (DWORD) (LPVOID) &mciOpenParms ) )
	{
		return -1;
	}
	wDeviceID = mciOpenParms.wDeviceID;

	// Set the time format to track/minute/second/frame (TMSF).
    mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
    if ( !hwMciSendCommand ( wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &mciSetParms ) )
    {
		hwMciClose ( wDeviceID );
		return -1;
	}

	int nTrackNum = GetAudioDiskInfo ( wDeviceID );
	hwMciClose ( wDeviceID );
	return nTrackNum;
}