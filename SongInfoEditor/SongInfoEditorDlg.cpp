// SongInfoEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "SongInfoEditorDlg.h"
#include "DlgProxy.h"
#include "SetVolume.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSongInfoEditorDlg dialog




IMPLEMENT_DYNAMIC(CSongInfoEditorDlg, CDialog);

CSongInfoEditorDlg::CSongInfoEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSongInfoEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_bTabFinish = FALSE;
	m_nListNum = 0;
	m_strNowPlay = _T("");
	m_nNowPlay = -1;
	m_nSongLength = 0;
	m_nSongNowPos = 0;
	bNowPlaying = FALSE;
	m_nVersion = VERSION;
	TimerID = NULL;
	m_pDlgPicture = NULL;
	m_bPictureDialog = FALSE;
}

CSongInfoEditorDlg::~CSongInfoEditorDlg()
{
	
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CSongInfoEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_INFO, m_cTabInfo);
	DDX_Control(pDX, IDC_LIST_SONG, m_cListSong);
	DDX_Control(pDX, IDC_BTN_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BTN_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BTN_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_BTN_FF, m_btnFF);
	DDX_Control(pDX, IDC_SLIDER_PLAY, m_cSliderPlay);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_cSliderVolume);
	DDX_Control(pDX, IDC_STATIC_TIME, m_cStaTime);
	DDX_Control(pDX, IDC_EDIT_COPYALBUM, m_cEditCopyAlbum);
	DDX_Control(pDX, IDC_EDIT_COPYART, m_cEditCopyArtist);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_cEditFileName);
	DDX_Control(pDX, IDC_CMB_DEVICESELECT, m_cCmbDeviceSelect);
}

BEGIN_MESSAGE_MAP(CSongInfoEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_PLAY, &CSongInfoEditorDlg::OnBnClickedBtnPlay)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_INFO, &CSongInfoEditorDlg::OnTcnSelchangeTabInfo)
	ON_LBN_DBLCLK(IDC_LIST_SONG, &CSongInfoEditorDlg::OnLbnDblclkListSong)
	ON_LBN_KILLFOCUS(IDC_LIST_SONG, &CSongInfoEditorDlg::OnLbnKillfocusListSong)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_ADDSONG, &CSongInfoEditorDlg::OnBnClickedBtnAddsong)
	ON_BN_CLICKED(IDC_BTN_DELSONG, &CSongInfoEditorDlg::OnBnClickedBtnDelsong)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAY, &CSongInfoEditorDlg::OnNMReleasedcaptureSliderPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CSongInfoEditorDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CSongInfoEditorDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_FF, &CSongInfoEditorDlg::OnBnClickedBtnFf)
	ON_BN_CLICKED(IDC_BTN_PREV, &CSongInfoEditorDlg::OnBnClickedBtnPrev)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLUME, &CSongInfoEditorDlg::OnNMReleasedcaptureSliderVolume)
	ON_BN_CLICKED(IDC_BTN_COPYALBUMIN, &CSongInfoEditorDlg::OnBnClickedBtnCopyalbumin)
	ON_BN_CLICKED(IDC_BTN_COPYARTISTIN, &CSongInfoEditorDlg::OnBnClickedBtnCopyartistin)
	ON_BN_CLICKED(IDC_BTN_RENAME, &CSongInfoEditorDlg::OnBnClickedBtnRename)
	ON_CBN_SELCHANGE(IDC_CMB_DEVICESELECT, &CSongInfoEditorDlg::OnCbnSelchangeCmbDeviceselect)
	ON_MESSAGE(WM_HOTKEY,OnHotMsg)
END_MESSAGE_MAP()


// CSongInfoEditorDlg message handlers

BOOL CSongInfoEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitSL();
	if (m_bTabFinish == FALSE){
		//Tab用ダイアログを生成
		m_cTabInfo.DeleteAllItems();
		m_cTabInfo.InsertItem(0, _T("ID3v1"));
		m_cTabInfo.InsertItem(1, _T("ID3v2"));
		m_cID3v1.Create(IDD_DLG_ID3V1, &m_cTabInfo);
		m_cID3v2.Create(IDD_DLG_ID3V2, &m_cTabInfo);

		//ダイアログ位置調整
		CRect rc;
		m_cTabInfo.GetClientRect(rc);
		rc.top += 25;
		rc.bottom -= 8;
		rc.left += 1;
		rc.right -= 8;
		m_cID3v1.MoveWindow(&rc);
		m_cID3v2.MoveWindow(&rc);
		m_pTabAddr[0] = &m_cID3v1;
		m_pTabAddr[1] = &m_cID3v2;

		m_pTabAddr[0]->ShowWindow(SW_SHOW);
		m_pTabAddr[1]->ShowWindow(SW_HIDE);
		m_nTabSelNum = 0;
		m_bTabFinish = TRUE;
		m_cID3v1.m_pDlg = this;
		m_cID3v2.m_pDlg = this;


		m_btnPrev.SetTheBitmap(IDB_BMP_PREVUP, IDB_BMP_PREVUP, IDB_BMP_PREVDOWN, IDB_BMP_PREVNONE);
		m_btnStop.SetTheBitmap(IDB_BMP_STOPUP, IDB_BMP_STOPUP, IDB_BMP_STOPDOWN, IDB_BMP_STOPNONE);
		m_btnPlay.SetTheBitmap(IDB_BMP_PLAYUP, IDB_BMP_PLAYUP, IDB_BMP_PLAYDOWN, IDB_BMP_PLAYNONE);
		m_btnPause.SetTheBitmap(IDB_BMP_PAUSEUP, IDB_BMP_PAUSEUP, IDB_BMP_PAUSEDOWN, IDB_BMP_PAUSENONE);
		m_btnFF.SetTheBitmap(IDB_BMP_FFUP, IDB_BMP_FFUP, IDB_BMP_FFDOWN, IDB_BMP_FFNONE);
		m_cSliderVolume.SetRange(0, 100);
		CSetVolume cSetVolume;
		m_cSliderVolume.SetPos(cSetVolume.GetVolume(0));
	}
	Refresh();

//	m_pTabAddr[m_nTabSelNum]->OnInitDialog();



//	UpdateData(FALSE);
	m_nSongHour = m_nSongLength / 3600000;
	m_nSongMinute = (m_nSongLength - m_nSongHour * 3600000) / 60000;
	m_nSongSecond = (m_nSongLength - m_nSongHour * 3600000 - m_nSongMinute * 60000) / 1000;
	m_cDSPlayer.Create();
	m_cDSPlayer.GetAudioDeviceList(m_arrDeviceName);
	for (int ilop = 0; ilop < m_arrDeviceName.GetSize(); ilop ++){
		m_cCmbDeviceSelect.AddString(m_arrDeviceName[ilop]);
	}
	if (m_arrDeviceName.GetSize() > 0){
		m_cCmbDeviceSelect.SetCurSel(0);
		CString strDevice;
		m_cCmbDeviceSelect.GetLBText(0, strDevice);
		m_cDSPlayer.SetAudioDeviceName(strDevice);
	}
	TimerID = timeSetEvent(200, 0, (LPTIMECALLBACK)TimeProc, (DWORD_PTR)this, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); // 设定多媒体定时器,1000ms
	
	RegisterHotKey(GetSafeHwnd(), WM_HOTMSG, 0, VK_F6);
	RegisterHotKey(GetSafeHwnd(), WM_HOTMSG, 0, VK_F7);
	m_pDlgPicture = new CDialogPicture();
	m_pDlgPicture->Create(IDD_DIALOGPICTURE, this);
	m_pDlgPicture->m_pParentDlg = this;
	m_pDlgPicture->SetWindowPos(this, 0, 0, 480, 640, SWP_HIDEWINDOW);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSongInfoEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSongInfoEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSongInfoEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CSongInfoEditorDlg::OnClose()
{
	if (CanExit())
		CDialog::OnClose();
}

void CSongInfoEditorDlg::OnOK()
{
	if (CanExit())
		CDialog::OnOK();
}

void CSongInfoEditorDlg::OnCancel()
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CSongInfoEditorDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void CSongInfoEditorDlg::OnDropFiles(HDROP hDropInfo)
{
	int nDropFileNum = 0;
	TCHAR *lpszFileName = new TCHAR[512];
	nDropFileNum = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 512);
	if (nDropFileNum > 0){
		for(int i = 0; i < nDropFileNum; i ++){
			int nLength = DragQueryFile(hDropInfo, i, lpszFileName, 512);
			CString str(lpszFileName, nLength);
			int index = str.ReverseFind(_T('.'));
			CString ext = str.Right(str.GetLength() - index - 1);
			ext.MakeLower();
			if (ext != _T("mp3")) continue;
				
			SONGINFO cSongInfo;
			cSongInfo.strPath = str;
			CFile cfile;
			CFileException fe;
			if (!cfile.Open(lpszFileName, CFile::modeRead, &fe)){
				fe.ReportError();
				continue;
			}
			CTagID3v2 tag;
			tag.m_pFile = &cfile;
			if (tag.LoadID3v2()){
				cSongInfo.strShowName = tag.m_cID3Content.strTitle;
			}else{
				char szID3v1Flag[4];
				char szFlag[4] = "TAG";
				char szTitle[31];
				cfile.Seek(-128, CFile::end);
				cfile.Read(szID3v1Flag, 3);
				szID3v1Flag[3] = '\0';
				if (strcmp(szID3v1Flag, szFlag) == 0){
					cfile.Seek(-125, CFile::end); 
					cfile.Read(szTitle, 30);
					szTitle[30] = '\0';
					wchar_t wszTitle[31]; 
					MultiByteToWideChar(CP_ACP, 0, szTitle, 31, wszTitle, 31);
					cSongInfo.strShowName = wszTitle;
				}else{
					CString str = cSongInfo.strPath;
					int pos = str.ReverseFind(_T('\\'));
					CString str2 = str.Right(str.GetLength() - pos - 1);
					cSongInfo.strShowName = str2;
				}
			}
			cfile.Close();
			m_cSongInfo.Add(cSongInfo);
		}
	}
	delete lpszFileName;
	Refresh();

	CDialog::OnDropFiles(hDropInfo);
}

void CSongInfoEditorDlg::OnBnClickedBtnPlay()
{
	if (m_nVersion == 2){
		if (m_strNowPlay == _T("")) return;
		if (m_cDSPlayer.GetMediaFileName() == m_strNowPlay){
			if (m_cDSPlayer.IsPaused()){
				m_cDSPlayer.Run();
				return;
			}else if (m_cDSPlayer.IsStopped()){
		
			}else{
				return;
			}
		}
		//停止播放
		OnBnClickedBtnStop();
		m_cDSPlayer.Create();
		if (!m_cDSPlayer.RenderFile(m_strNowPlay)) return;
		m_cDSPlayer.Delete_DefaultDirectSoundDevice();
		double dLength;
		m_cDSPlayer.GetDuration(&dLength);
		m_nSongLength = dLength * 1000 ;
		m_nSongNowPos = 0;
		m_cSliderPlay.SetRange(0, m_nSongLength, TRUE);
		m_nSongHour = m_nSongLength / 3600000;
		m_nSongMinute = (m_nSongLength - m_nSongHour * 3600000) / 60000;
		m_nSongSecond = (m_nSongLength - m_nSongHour * 3600000 - m_nSongMinute * 60000) / 1000;
//		m_cDSPlayer.GetCurrentPosition(&duration);
		if (!m_cDSPlayer.Run()) return;
//		SetTimer(1, 1000, NULL);
		return;
	}

	if (m_nReadyPlay == -1) return;
	if (m_nReadyPlay != m_nNowPlay){
		KillTimer(1);
		CloseMci();
		mciOpen.lpstrDeviceType = _T("MCI_ALL_DEVICE_ID");
		mciOpen.lpstrElementName = m_strNowPlay;
		DWORD errMsg = 0;/* = mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen);*/

		if (!mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen)){
			// 時間単位の設定(ミリ秒に設定)
			mciMssp.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			//DWORD errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_SET, MCI_WAIT | MCI_SET_TIME_FORMAT, (DWORD)&mciMssp);
			if (errMsg) {
				ShowMciErrorMessage(errMsg);
				CloseMci();
				return;
			}
			// 長さの取得
			mciMsp.dwItem = MCI_STATUS_LENGTH; // 長さを指定
			errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)&mciMsp);
			if (errMsg) {
				ShowMciErrorMessage(errMsg);
				CloseMci();
				return;
			}
			m_nSongLength = mciMsp.dwReturn;
			m_nSongNowPos = 0;
			m_nSongHour = m_nSongLength / 3600000;
			m_nSongMinute = (m_nSongLength - m_nSongHour * 3600000) / 60000;
			m_nSongSecond = (m_nSongLength - m_nSongHour * 3600000 - m_nSongMinute * 60000) / 1000;
			ShowTime();

			MCI_PLAY_PARMS mciPlay;
			mciPlay.dwCallback = 0;
			mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, NULL, (DWORD)(LPVOID)&mciPlay);
			m_nNowPlay = m_nReadyPlay;
			bNowPlaying = TRUE;
			SetTimer(1, 1000, NULL);
		}else{
			bNowPlaying = FALSE;
			MessageBox(_T("まぁ…今このファイルを再生できません。"));
		}
	}else{
		MCI_PLAY_PARMS mciPlay;
		mciPlay.dwCallback = 0;
		mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, NULL, (DWORD)(LPVOID)&mciPlay);
		SetTimer(1, 1000, NULL);
	}
	

	
}

void CSongInfoEditorDlg::OnTcnSelchangeTabInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_pTabAddr[m_nTabSelNum]->ShowWindow(SW_HIDE);
	m_nTabSelNum = m_cTabInfo.GetCurSel();
	m_pTabAddr[m_nTabSelNum]->ShowWindow(SW_SHOW);
	*pResult = 0;
}

void CSongInfoEditorDlg::Refresh()
{ 
	m_btnPrev.EnableBmpButton(m_nReadyPlay >= 0);
	m_btnStop.EnableBmpButton(m_nReadyPlay >= 0);
	m_btnPlay.EnableBmpButton(m_nReadyPlay >= 0);
	m_btnPause.EnableBmpButton(m_nReadyPlay >= 0);
	m_btnFF.EnableBmpButton(m_nReadyPlay >= 0);
	if (m_cSongInfo.GetSize() > m_nListNum){
		for (int i = m_nListNum; i < m_cSongInfo.GetSize(); i ++){
			m_cListSong.AddString(m_cSongInfo[i].strShowName);
		}
	}
	m_nListNum = m_cSongInfo.GetSize();
	return;
}
void CSongInfoEditorDlg::OnLbnDblclkListSong()
{
	int index = m_cListSong.GetCurSel();
	OnBnClickedBtnStop();
	LoadSongInfo(index);
	Refresh();
	OnBnClickedBtnPlay();
}

void CSongInfoEditorDlg::LoadSongInfo(int nIndex)
{
	CFile file;
	CFileException fe;
	if (!file.Open(m_cSongInfo[nIndex].strPath, CFile::modeRead | CFile::shareDenyWrite, &fe)){
		fe.ReportError();
		return;
	}else{
		//ID3v1を読み込み
		char szID3v1Flag[3];
	//	char szFlag[4] = "TAG";
		file.Seek(-128, CFile::end);
		file.Read(szID3v1Flag, 3);
	//	szID3v1Flag[3] = '\0';
		if (memcmp(szID3v1Flag, "TAG", 3) == 0){
			m_cID3v1.m_bEnable = TRUE;
			file.Seek(-125, CFile::end); 
			file.Read(m_cID3v1.m_szTitle, 30);
			file.Seek(-95, CFile::end);
			file.Read(m_cID3v1.m_szArtist, 30);
			file.Seek(-65, CFile::end);
			file.Read(m_cID3v1.m_szAlbum, 30);
			file.Seek(-35, CFile::end);
			file.Read(m_cID3v1.m_szYear, 4);
			file.Seek(-31, CFile::end);
			file.Read(m_cID3v1.m_szComment, 28);
			file.Seek(-3, CFile::end);
			file.Read(m_cID3v1.m_szTrack, 2);
			file.Seek(-1, CFile::end);
			file.Read(&m_cID3v1.m_szGenre, 1);
			m_cID3v1.m_szTitle[30] = '\0';
			m_cID3v1.m_szArtist[30] = '\0';
			m_cID3v1.m_szAlbum[30] = '\0';
			m_cID3v1.m_szYear[4] = '\0';
			m_cID3v1.m_szComment[28] = '\0';
		}else{
			m_cID3v1.m_bEnable = FALSE;
			m_cID3v1.m_szTitle[0] = '\0';
			m_cID3v1.m_szArtist[0] = '\0';
			m_cID3v1.m_szAlbum[0] = '\0';
			m_cID3v1.m_szYear[0] = '\0';
			m_cID3v1.m_szComment[0] = '\0';
			m_cID3v1.m_szGenre = '\0';
		}
		//ID3v2を読み込み
		//ID3v2判定
		m_cTagID3v2.m_pFile = &file;
		m_cTagID3v2.ReleaseMainFrames();
		m_cTagID3v2.ReleaseOtherFrames();
		if (m_cTagID3v2.LoadID3v2()){
			m_cID3v2.m_bEnable = TRUE;
			m_cID3v2.m_strTitle = m_cTagID3v2.m_cID3Content.strTitle;
			m_cID3v2.m_strArtist = m_cTagID3v2.m_cID3Content.strArtist;
			m_cID3v2.m_strAlbum = m_cTagID3v2.m_cID3Content.strAlbum;
			m_cID3v2.m_strTrack = m_cTagID3v2.m_cID3Content.strTrack;
			m_cID3v2.m_strYear = m_cTagID3v2.m_cID3Content.strYear;
			m_cID3v2.m_strGenre = m_cTagID3v2.m_cID3Content.strGenre;
			m_cID3v2.m_strComposer = m_cTagID3v2.m_cID3Content.strComposer;
			m_cID3v2.m_strLyricist = m_cTagID3v2.m_cID3Content.strLyric;
			m_cID3v2.m_strOriginalArtist = m_cTagID3v2.m_cID3Content.strOriArtist;
			m_cID3v2.m_strCopyright = m_cTagID3v2.m_cID3Content.strCopyright;
			m_cID3v2.m_strEncoder = m_cTagID3v2.m_cID3Content.strEncoder;
			m_cID3v2.m_strEncodedBy = m_cTagID3v2.m_cID3Content.strEncodedBy;
			m_cID3v2.m_strComment = m_cTagID3v2.m_cID3Content.strComment;
			m_cID3v2.m_strURL = m_cTagID3v2.m_cID3Content.strURL;
			m_cID3v2.m_strLyricText = m_cTagID3v2.m_cID3Content.strLyricText;
			m_cID3v2.m_nID3Version = m_cTagID3v2.m_cID3Content.nID3Version;
		}else{
			m_cID3v2.m_bEnable = FALSE;
		}
		file.Close();
	}
	m_cID3v1.OnInitDialog();
	m_cID3v2.OnInitDialog();
	m_strNowPlay = m_cSongInfo[nIndex].strPath;
	m_nReadyPlay = nIndex;
	m_cEditFileName.SetWindowTextW(m_strNowPlay);
	if (m_cID3v2.m_bEnable){
		m_cSongInfo[nIndex].strShowName = m_cID3v2.m_strTitle;
	}else if (m_cID3v1.m_bEnable){
		m_cSongInfo[nIndex].strShowName = m_cID3v1.m_strEditTitle;
	}else{
		CString str = m_cSongInfo[nIndex].strPath;
		int pos = str.ReverseFind(_T('\\'));
		CString str2 = str.Right(str.GetLength() - pos - 1);
		m_cSongInfo[nIndex].strShowName = str2;
	}
	m_cListSong.DeleteString(nIndex);
	m_cListSong.InsertString(nIndex, m_cSongInfo[nIndex].strShowName);
	SelectPlaying(nIndex);
}

void CSongInfoEditorDlg::SelectPlaying(int nIndex)
{
	int nSel = m_cListSong.GetCurSel();
	m_cListSong.SetSel(nSel, FALSE);
//	for (int i = 0; i < m_cListSong.GetCount(); i ++){
//		m_cListSong.SetSel(i, FALSE);
//	}
	m_cListSong.SetSel(nIndex, TRUE);
}

void CSongInfoEditorDlg::OnLbnKillfocusListSong()
{
	int i = 0;
	i ++;
	// TODO: Add your control notification handler code here
}

void CSongInfoEditorDlg::OnBnClickedBtnAddsong()
{
	CFileDialog file(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, _T("(*.mp3ファイル)|*.mp3|"));
    if (file.DoModal()==IDOK)
    {
		POSITION pos = file.GetStartPosition();
		while(pos)   
		{   
			CString strFileName = file.GetNextPathName(pos);
			SONGINFO cSongInfo;
			cSongInfo.strPath = strFileName;
			CFile cfile;
			CFileException fe;
			if (!cfile.Open(strFileName, CFile::modeRead, &fe)){
				fe.ReportError();
				continue;
			}
			CTagID3v2 tag;
			tag.m_pFile = &cfile;
			if (tag.LoadID3v2()){
				cSongInfo.strShowName = tag.m_cID3Content.strTitle;
			}else{
				char szID3v1Flag[4];
				char szFlag[4] = "TAG";
				char szTitle[31];
				cfile.Seek(-128, CFile::end);
				cfile.Read(szID3v1Flag, 3);
				szID3v1Flag[3] = '\0';
				if (strcmp(szID3v1Flag, szFlag) == 0){
					cfile.Seek(-125, CFile::end); 
					cfile.Read(szTitle, 30);
					szTitle[30] = '\0';
					wchar_t wszTitle[31]; 
					MultiByteToWideChar(CP_ACP, 0, szTitle, 31, wszTitle, 31);
					cSongInfo.strShowName = wszTitle;
				}else{
					CString str = cSongInfo.strPath;
					int pos = str.ReverseFind(_T('\\'));
					CString str2 = str.Right(str.GetLength() - pos - 1);
					cSongInfo.strShowName = str2;
				}
			}
			cfile.Close();
			m_cSongInfo.Add(cSongInfo);
		}
		Refresh();
    }
}

void CSongInfoEditorDlg::OnBnClickedBtnDelsong()
{
	int count = m_cListSong.GetSelCount();
	CArray<int,int> arrListBoxSel;
	arrListBoxSel.SetSize(count);
	m_cListSong.GetSelItems(count, arrListBoxSel.GetData());
	for (int i = count - 1; i >= 0; i --){
		if (m_nNowPlay == arrListBoxSel[i]){
			OnBnClickedBtnStop();
			m_nNowPlay = -1;
			m_nReadyPlay = -1;
			m_strNowPlay = _T("");
			m_cEditFileName.SetWindowTextW(m_strNowPlay);
			m_cID3v1.EnableDialog(FALSE);
			m_cID3v2.EnableDialog(FALSE);
			CloseMci();
		}else if (m_nReadyPlay == arrListBoxSel[i]){
			m_cID3v1.EnableDialog(FALSE);
			m_cID3v2.EnableDialog(FALSE);
			m_nReadyPlay = -1;
		}else if (m_nNowPlay > arrListBoxSel[i]){
			m_nNowPlay --;
			m_nReadyPlay --;
		}
		m_cListSong.DeleteString(arrListBoxSel[i]);
		m_cSongInfo.RemoveAt(arrListBoxSel[i]);
	}
	Refresh();
	if (m_nReadyPlay != -1){
		SelectPlaying(m_nReadyPlay);
	}
}

void CSongInfoEditorDlg::ShowMciErrorMessage(DWORD fdwError) {
    TCHAR *buf = new TCHAR[255];
    if (mciGetErrorString(fdwError, buf, 255)) {
        MessageBox(buf, _T("エラー"), MB_OK);
    } else {
        MessageBox(_T("MCIに不明エラーが発生。"), _T("エラー"), MB_OK);
    }
    delete buf;
    return ;
}

void CSongInfoEditorDlg::CloseMci()
{
	MCI_GENERIC_PARMS mgp;
    mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD)&mgp);
    return;
}

void CSongInfoEditorDlg::SetPlaySlider()
{
	m_cSliderPlay.SetRange(0, m_nSongLength, TRUE);
	m_cSliderPlay.SetPos(m_nSongNowPos);
}

void CSongInfoEditorDlg::GetMciPlayPos()
{
	if (!mciOpen.wDeviceID) return;
    mciMsp.dwItem = MCI_STATUS_POSITION; // 長さを指定
	DWORD errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)&mciMsp);
	if (errMsg) {
		ShowMciErrorMessage(errMsg);
		CloseMci();
		return;
	}
	m_nSongNowPos = mciMsp.dwReturn;
}

void CSongInfoEditorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	GetMciPlayPos();
	SetPlaySlider();
	if (m_nSongNowPos == m_nSongLength){
		//データ変更を確認
	//	if (m_cID3v1.
		OnBnClickedBtnFf();
//		MCI_SEEK_PARMS mciSeek;
//		mciSeek.dwCallback = 0;
//		mciSendCommand(mciOpen.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)&mciSeek);
//		OnBnClickedBtnPlay();
	}
	ShowTime();
	CDialog::OnTimer(nIDEvent);
}

void CSongInfoEditorDlg::ShowTime()
{
//	m_cSliderPlay.SetRange(0, m_nSongLength, TRUE);
	m_cSliderPlay.SetPos(m_nSongNowPos);
	CString str;
	UINT nowHour = m_nSongNowPos / 3600000;
	UINT nowMinute = (m_nSongNowPos - nowHour * 3600000) / 60000;
	UINT nowSecond = (m_nSongNowPos - nowHour * 3600000 - nowMinute * 60000) / 1000;
	if (m_nSongHour != 0){	
		str.Format(_T("%d:%02d:%02d/%d:%02d:%02d"), nowHour, nowMinute, nowSecond, m_nSongHour, m_nSongMinute, m_nSongSecond);
	}else{
		str.Format(_T("%02d:%02d/%02d:%02d"), nowMinute, nowSecond, m_nSongMinute, m_nSongSecond);
	}
	m_cStaTime.SetWindowTextW(str);
}	

void CSongInfoEditorDlg::OnNMReleasedcaptureSliderPlay(NMHDR *pNMHDR, LRESULT *pResult)
{
	MCI_PLAY_PARMS mciPlay;
	mciPlay.dwCallback = 0;
	mciPlay.dwFrom = m_cSliderPlay.GetPos();
	mciSendCommand(mciOpen.wDeviceID, MCI_PLAY ,MCI_FROM, (DWORD)(LPVOID)&mciPlay);
	*pResult = 0;
}

void CSongInfoEditorDlg::OnBnClickedBtnStop()
{
	if (m_nVersion == 2){ 
		m_cDSPlayer.SetCurrentPosition(0);
		m_cDSPlayer.Stop();
		bNowPlaying = FALSE;
		m_nNowPlay = -1;
		m_nSongNowPos = 0;
		
		m_cSliderPlay.SetPos(m_nSongNowPos);
		m_cDSPlayer.Release();
		m_cDSPlayer.ClearMediaFileName();
		return;
	}
/*	MCI_GENERIC_PARMS mciMgp;
	mciMgp.dwCallback = 0;
	mciSendCommand(mciOpen.wDeviceID, MCI_PAUSE, NULL, (DWORD)(LPVOID)&mciMgp);
	MCI_SEEK_PARMS mciSeek;
	mciSeek.dwCallback = 0;
	mciSendCommand(mciOpen.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)&mciSeek);*/
	CloseMci();
	bNowPlaying = FALSE;
	m_nNowPlay = -1;
	m_nSongNowPos = 0;
	SetPlaySlider();
	KillTimer(1);
}

void CSongInfoEditorDlg::OnBnClickedBtnPause()
{
	if (m_nVersion == 2){
		if (m_cDSPlayer.IsInitedOK()){
			m_cDSPlayer.Pause();
		//	SetTimer ( SLIDER_TIMER, 100, NULL );
		}
	}
	MCI_GENERIC_PARMS mciMgp;
	mciMgp.dwCallback = 0;
	mciSendCommand(mciOpen.wDeviceID, MCI_PAUSE, NULL, (DWORD)(LPVOID)&mciMgp);
	KillTimer(1);
}

void CSongInfoEditorDlg::OnBnClickedBtnFf()
{
	int total = m_cSongInfo.GetSize();
	if (total == 0) return;
	if ((m_nReadyPlay + 1) >= total){
		m_nReadyPlay = 0;
	}else{
		m_nReadyPlay ++;
	}
	OnBnClickedBtnStop();
	LoadSongInfo(m_nReadyPlay);
	OnBnClickedBtnPlay();
}

void CSongInfoEditorDlg::OnBnClickedBtnPrev()
{
	int total = m_cSongInfo.GetSize();
	if (total == 0) return;
	if ((m_nReadyPlay - 1) < 0){
		m_nReadyPlay = total - 1;
	}else{
		m_nReadyPlay --;
	}
	OnBnClickedBtnStop();
	LoadSongInfo(m_nReadyPlay);
	OnBnClickedBtnPlay();
}

void CSongInfoEditorDlg::OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSetVolume cSetVolume;
	cSetVolume.SetVolume(0, m_cSliderVolume.GetPos());  
	*pResult = 0;
}

BOOL CSongInfoEditorDlg::SaveInfoID3v1(void)
{
	DWORD errMsg;
	int nFrom = 0;
	if (bNowPlaying){
		mciMsp.dwItem = MCI_STATUS_POSITION; // 長さを指定
		errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)&mciMsp);
		if (errMsg) {
			ShowMciErrorMessage(errMsg);
			CloseMci();
			return 0;
		}
		nFrom = mciMsp.dwReturn;
		KillTimer(1);
		CloseMci();
	}
	CFile file(m_cSongInfo[m_nReadyPlay].strPath, CFile::modeReadWrite);
	if (m_cID3v1.m_bEnable){
		char szID3v1Flag[4];
		char szFlag[4] = "TAG";
		file.Seek(-128, CFile::end);
		file.Read(szID3v1Flag, 3);
		szID3v1Flag[3] = '\0';
		if (strcmp(szID3v1Flag, szFlag) == 0){
			file.Seek(-125, CFile::end);
			file.Write(m_cID3v1.m_szTitle, 30);		
			file.Write(m_cID3v1.m_szArtist, 30);
			file.Write(m_cID3v1.m_szAlbum, 30);
			file.Write(m_cID3v1.m_szYear, 4);
			file.Write(m_cID3v1.m_szComment, 30);
			if (m_cID3v1.m_szGenre == 126){
				char sz = -1;
				file.Write(&sz, 1);
			}else{
				file.Write(&m_cID3v1.m_szGenre, 1);
			}
		}else{
			file.SeekToEnd();
			file.Write("TAG", 3);
			file.Write(m_cID3v1.m_szTitle, 30);		
			file.Write(m_cID3v1.m_szArtist, 30);
			file.Write(m_cID3v1.m_szAlbum, 30);
			file.Write(m_cID3v1.m_szYear, 4);
			file.Write(m_cID3v1.m_szComment, 30);
			if (m_cID3v1.m_szGenre == 126){
				char sz = -1;
				file.Write(&sz, 1);
			}else{
				file.Write(&m_cID3v1.m_szGenre, 1);
			}
		}
	}else{
		char szID3v1Flag[4];
		char szFlag[4] = "TAG";
		file.Seek(-128, CFile::end);
		file.Read(szID3v1Flag, 3);
		szID3v1Flag[3] = '\0';
		if (strcmp(szID3v1Flag, szFlag) == 0){
			file.SetLength(file.GetLength() - 128);
		}
	}
	file.Close();
	if (!m_cID3v2.m_bEnable){
		if (m_cID3v1.m_bEnable){
			m_cSongInfo[m_nReadyPlay].strShowName = m_cID3v1.m_strEditTitle;
		}else{
			CString str = m_cSongInfo[m_nReadyPlay].strPath;
			int pos = str.ReverseFind(_T('\\'));
			CString str2 = str.Right(str.GetLength() - pos - 1);
			m_cSongInfo[m_nReadyPlay].strShowName = str2;
		}
	}
	m_cListSong.DeleteString(m_nReadyPlay);
	m_cListSong.InsertString(m_nReadyPlay, m_cSongInfo[m_nReadyPlay].strShowName);
	SelectPlaying(m_nReadyPlay);

	if (bNowPlaying){
		mciOpen.lpstrDeviceType = _T("MCI_ALL_DEVICE_ID");
		mciOpen.lpstrElementName = m_strNowPlay;
		errMsg = 0;/* = mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen);*/
		if (!mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen)){
			// 時間単位の設定(ミリ秒に設定)
			mciMssp.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			//DWORD errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_SET, MCI_WAIT | MCI_SET_TIME_FORMAT, (DWORD)&mciMssp);
			if (errMsg) {
				ShowMciErrorMessage(errMsg);
				CloseMci();
				return 0;
			}
			MCI_PLAY_PARMS mciPlay;
			mciPlay.dwCallback = 0;
			mciPlay.dwFrom = nFrom;
			mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_FROM, (DWORD)(LPVOID)&mciPlay);
			m_nNowPlay = m_nReadyPlay;
			bNowPlaying = TRUE;
			SetTimer(1, 1000, NULL);
		}else{
			bNowPlaying = FALSE;
			MessageBox(_T("まぁ…今このファイルを再生できません。"));
		}
	}

	
	return 1;
}

BOOL CSongInfoEditorDlg::SaveInfoID3v2(void)
{
	DWORD errMsg;
	int nFrom = 0;
	if (bNowPlaying){
		mciMsp.dwItem = MCI_STATUS_POSITION; // 長さを指定
		errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)&mciMsp);
		if (errMsg) {
			ShowMciErrorMessage(errMsg);
			CloseMci();
			return 0;
		}
		nFrom = mciMsp.dwReturn;
		KillTimer(1);
		CloseMci();
	}

	m_cTagID3v2.m_cID3Content.strTitle = m_cID3v2.m_strTitle;
	m_cTagID3v2.m_cID3Content.strArtist = m_cID3v2.m_strArtist;
	m_cTagID3v2.m_cID3Content.strAlbum = m_cID3v2.m_strAlbum;
	m_cTagID3v2.m_cID3Content.strTrack = m_cID3v2.m_strTrack;
	m_cTagID3v2.m_cID3Content.strYear = m_cID3v2.m_strYear;
	m_cTagID3v2.m_cID3Content.strGenre = m_cID3v2.m_strGenre;
	m_cTagID3v2.m_cID3Content.strComposer = m_cID3v2.m_strComposer;
	m_cTagID3v2.m_cID3Content.strLyric = m_cID3v2.m_strLyricist;
	m_cTagID3v2.m_cID3Content.strOriArtist = m_cID3v2.m_strOriginalArtist;
	m_cTagID3v2.m_cID3Content.strCopyright = m_cID3v2.m_strCopyright;
	m_cTagID3v2.m_cID3Content.strEncoder = m_cID3v2.m_strEncoder;
	m_cTagID3v2.m_cID3Content.strEncodedBy = m_cID3v2.m_strEncodedBy;
	m_cTagID3v2.m_cID3Content.strComment = m_cID3v2.m_strComment;
	m_cTagID3v2.m_cID3Content.strURL = m_cID3v2.m_strURL;
	m_cTagID3v2.m_cID3Content.strLyricText = m_cID3v2.m_strLyricText;
	m_cTagID3v2.m_cID3Content.nID3Version = m_cID3v2.m_nID3Version;
	m_cTagID3v2.m_cID3Content.bUnicode = m_cID3v2.m_bUnicode;
	m_cTagID3v2.m_bEnable = m_cID3v2.m_bEnable;
	m_cTagID3v2.SaveID3Tag(m_cSongInfo[m_nReadyPlay].strPath);
	//リスト名変更
	if (m_cID3v2.m_bEnable){
		m_cSongInfo[m_nReadyPlay].strShowName = m_cID3v2.m_strTitle;
	}else if (m_cID3v1.m_bEnable){
		m_cSongInfo[m_nReadyPlay].strShowName = m_cID3v1.m_strEditTitle;
	}else{
		CString str = m_cSongInfo[m_nReadyPlay].strPath;
		int pos = str.ReverseFind(_T('\\'));
		CString str2 = str.Right(str.GetLength() - pos - 1);
		m_cSongInfo[m_nReadyPlay].strShowName = str2;
	}
	m_cListSong.DeleteString(m_nReadyPlay);
	m_cListSong.InsertString(m_nReadyPlay, m_cSongInfo[m_nReadyPlay].strShowName);
	SelectPlaying(m_nReadyPlay);
	//Lyricタイトル変更
	m_cID3v2.m_pLyricDlg->SetWindowTextW(m_cID3v2.m_strTitle);

	if (bNowPlaying){
		mciOpen.lpstrDeviceType = _T("MCI_ALL_DEVICE_ID");
		mciOpen.lpstrElementName = m_strNowPlay;
		errMsg = 0;/* = mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen);*/
		if (!mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen)){
			// 時間単位の設定(ミリ秒に設定)
			mciMssp.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			//DWORD errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_SET, MCI_WAIT | MCI_SET_TIME_FORMAT, (DWORD)&mciMssp);
			if (errMsg) {
				ShowMciErrorMessage(errMsg);
				CloseMci();
				return 0;
			}
			MCI_PLAY_PARMS mciPlay;
			mciPlay.dwCallback = 0;
			mciPlay.dwFrom = nFrom;
			mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_FROM, (DWORD)(LPVOID)&mciPlay);
			m_nNowPlay = m_nReadyPlay;
			bNowPlaying = TRUE;
			SetTimer(1, 1000, NULL);
		}else{
			bNowPlaying = FALSE;
			MessageBox(_T("まぁ…今このファイルを再生できません。"));
		}
	}

	
	return 1;
}

void CSongInfoEditorDlg::OnBnClickedBtnCopyalbumin()
{
	CString strCopy;
	m_cEditCopyAlbum.GetWindowTextW(strCopy);
	if (m_nTabSelNum == 0){
		m_cID3v1.m_strEditAlbum = strCopy;
		m_cID3v1.UpdateData(FALSE);
	}else if (m_nTabSelNum == 1){
		m_cID3v2.m_strAlbum = strCopy;
		m_cID3v2.UpdateData(FALSE);
	}
}

void CSongInfoEditorDlg::OnBnClickedBtnCopyartistin()
{
	CString strCopy;
	m_cEditCopyArtist.GetWindowTextW(strCopy);
	if (m_nTabSelNum == 0){
		m_cID3v1.m_strEditArtist = strCopy;
		m_cID3v1.UpdateData(FALSE);
	}else if (m_nTabSelNum == 1){
		m_cID3v2.m_strArtist = strCopy;
		m_cID3v2.UpdateData(FALSE);
	}
}

void CSongInfoEditorDlg::OnBnClickedBtnRename()
{
	DWORD errMsg;
	int nFrom = 0;
	if (bNowPlaying){
		mciMsp.dwItem = MCI_STATUS_POSITION; // 長さを指定
		errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)&mciMsp);
		if (errMsg) {
			ShowMciErrorMessage(errMsg);
			CloseMci();
			return;
		}
		nFrom = mciMsp.dwReturn;
		KillTimer(1);
		CloseMci();
	}
	CString strNewFileName;
	if (m_cID3v2.m_bEnable){
		strNewFileName.Format(_T("%s.%s"), m_cID3v2.m_strTrack, m_cID3v2.m_strTitle);
	}else if (m_cID3v1.m_bEnable){
		strNewFileName.Format(_T("%d.%s"), m_cID3v1.m_nEditTrack, m_cID3v1.m_strEditTitle);
	}else{
		strNewFileName = m_cSongInfo[m_nReadyPlay].strPath;
	}
	if (strNewFileName != m_cSongInfo[m_nReadyPlay].strPath){
		CString strPath = m_cSongInfo[m_nReadyPlay].strPath;
		int pos = strPath.ReverseFind(_T('\\'));
		CString str2 = strPath.Left(pos);
		strPath.Format(_T("%s\\%s.mp3"), str2, strNewFileName);
		MoveFile(m_cSongInfo[m_nReadyPlay].strPath, strPath);
		m_cSongInfo[m_nReadyPlay].strPath= strPath;
	}
	m_strNowPlay = m_cSongInfo[m_nReadyPlay].strPath;
	m_cEditFileName.SetWindowTextW(m_strNowPlay);
	if (bNowPlaying){
		mciOpen.lpstrDeviceType = _T("MCI_ALL_DEVICE_ID");
		mciOpen.lpstrElementName = m_strNowPlay;
		errMsg = 0;/* = mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen);*/
		if (!mciSendCommand( 
				NULL, //指定设备标识,MCI_OPEN此处为NULL,
				//成功OPEN后，标识放入MCI_OPEN_PARMS结构的wDeviceID变量中
				MCI_OPEN, 
				MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
				(DWORD)(LPVOID)&mciOpen)){
			// 時間単位の設定(ミリ秒に設定)
			mciMssp.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			//DWORD errMsg = mciSendCommand(mciOpen.wDeviceID, MCI_SET, MCI_WAIT | MCI_SET_TIME_FORMAT, (DWORD)&mciMssp);
			if (errMsg) {
				ShowMciErrorMessage(errMsg);
				CloseMci();
				return;
			}
			MCI_PLAY_PARMS mciPlay;
			mciPlay.dwCallback = 0;
			mciPlay.dwFrom = nFrom;
			mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_FROM, (DWORD)(LPVOID)&mciPlay);
			m_nNowPlay = m_nReadyPlay;
			bNowPlaying = TRUE;
			SetTimer(1, 1000, NULL);
		}else{
			bNowPlaying = FALSE;
			MessageBox(_T("まぁ…今このファイルを再生できません。"));
		}
	}
}


BOOL CSongInfoEditorDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	KillTimer(1);
	timeKillEvent(TimerID);
	CloseMci();
	if (m_pDlgPicture){
		delete m_pDlgPicture;
		m_pDlgPicture = NULL;
	}
	return CDialog::DestroyWindow();
}

void CALLBACK CSongInfoEditorDlg::TimeProc(UINT uID,UINT uMsg, DWORD dwUser,DWORD dw1,DWORD dw2)
{
	CSongInfoEditorDlg* dlg = (CSongInfoEditorDlg*)dwUser;
	double dPos;
	dlg->m_cDSPlayer.GetCurrentPosition(&dPos);
	dlg->m_nSongNowPos = dPos * 1000;
	dlg->ShowTime();
}

void CSongInfoEditorDlg::OnCbnSelchangeCmbDeviceselect()
{
	int nCurSel = m_cCmbDeviceSelect.GetCurSel();
	if (nCurSel >= 0)
	{
		CString strDevice;
		m_cCmbDeviceSelect.GetLBText(nCurSel, strDevice);
		m_cDSPlayer.SetAudioDevice(strDevice);
	}
}

LRESULT CSongInfoEditorDlg::OnHotMsg(WPARAM wParam, LPARAM lParam)
{
	if(wParam == WM_HOTMSG)
	{
		int nBtn = (BYTE)(lParam >> 16);
		if (nBtn == 0x75){ 
			//F6
			if (m_cPicureBGInfo.GetSize() == 0) return 0;
			//初始化图片窗口
			if (m_bPictureDialog){
				m_bPictureDialog = FALSE;
				m_pDlgPicture->SaveButtonPosition();
				ShowWindow(SW_SHOW);
				m_pDlgPicture->ShowWindow(SW_HIDE);
			}else{
				CRect rect;
				GetWindowRect(&rect);
				if (!m_pDlgPicture->InitDialog(rect.left, rect.top, m_cPicureBGInfo[m_nPictureBGIndex].strPicturePath, m_cPicureBGInfo[m_nPictureBGIndex].dwTransparentColor)) return 0;
				m_pDlgPicture->SetButtonPosition(0, m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.left, m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.top, m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strPlayPicturePath);
				m_pDlgPicture->SetButtonPosition(1, m_cPicureBGInfo[m_nPictureBGIndex].rectStop.left, m_cPicureBGInfo[m_nPictureBGIndex].rectStop.top, m_cPicureBGInfo[m_nPictureBGIndex].rectStop.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectStop.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strStopPicturePath);
				m_pDlgPicture->SetButtonPosition(2, m_cPicureBGInfo[m_nPictureBGIndex].rectPause.left, m_cPicureBGInfo[m_nPictureBGIndex].rectPause.top, m_cPicureBGInfo[m_nPictureBGIndex].rectPause.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectPause.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strPausePicturePath);
				m_pDlgPicture->SetButtonPosition(3, m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.left, m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.top, m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strPrevPicturePath);
				m_pDlgPicture->SetButtonPosition(4, m_cPicureBGInfo[m_nPictureBGIndex].rectFF.left, m_cPicureBGInfo[m_nPictureBGIndex].rectFF.top, m_cPicureBGInfo[m_nPictureBGIndex].rectFF.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectFF.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strFFPicturePath);
				ShowWindow(SW_HIDE);
				m_pDlgPicture->ShowWindow(SW_SHOW);
				m_bPictureDialog = TRUE;
			}
		}else if (nBtn == 0x76){ 
			//F7
			if (m_cPicureBGInfo.GetSize() == 0) return 0;
			//初始化图片窗口
			m_pDlgPicture->SaveButtonPosition();
			m_nPictureBGIndex ++;
			if (m_nPictureBGIndex >= m_cPicureBGInfo.GetSize()) m_nPictureBGIndex = 0;
			CPoint ptPos;
			m_pDlgPicture->GetDialogPosition(&ptPos);
			if (!m_pDlgPicture->InitDialog(ptPos.x, ptPos.y, m_cPicureBGInfo[m_nPictureBGIndex].strPicturePath, m_cPicureBGInfo[m_nPictureBGIndex].dwTransparentColor)) return 0;
			m_pDlgPicture->SetButtonPosition(0, m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.left, m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.top, m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectPlay.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strPlayPicturePath);
			m_pDlgPicture->SetButtonPosition(1, m_cPicureBGInfo[m_nPictureBGIndex].rectStop.left, m_cPicureBGInfo[m_nPictureBGIndex].rectStop.top, m_cPicureBGInfo[m_nPictureBGIndex].rectStop.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectStop.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strStopPicturePath);
			m_pDlgPicture->SetButtonPosition(2, m_cPicureBGInfo[m_nPictureBGIndex].rectPause.left, m_cPicureBGInfo[m_nPictureBGIndex].rectPause.top, m_cPicureBGInfo[m_nPictureBGIndex].rectPause.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectPause.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strPausePicturePath);
			m_pDlgPicture->SetButtonPosition(3, m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.left, m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.top, m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectPrev.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strPrevPicturePath);
			m_pDlgPicture->SetButtonPosition(4, m_cPicureBGInfo[m_nPictureBGIndex].rectFF.left, m_cPicureBGInfo[m_nPictureBGIndex].rectFF.top, m_cPicureBGInfo[m_nPictureBGIndex].rectFF.Width(), m_cPicureBGInfo[m_nPictureBGIndex].rectFF.Height(), m_cPicureBGInfo[m_nPictureBGIndex].strFFPicturePath);
		}
	}
	return 0;
}

void CSongInfoEditorDlg::InitSL(BOOL bLoad)
{
	CString strEvent;
	m_cIniReader.AutoIniFileName();
	m_strAppPath = m_cIniReader.GetAppPath();
	int nNum = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), _T("背景数量"), _T(""), _T("0")));
	m_nPictureBGIndex = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), _T("当前背景"), _T(""), _T("0")));
	for (int ilop = 0; ilop < nNum; ilop ++){
		PICBGINFO pbi;
		strEvent.Format(_T("背景%d图片名称"), ilop + 1);
		pbi.strPicturePath = m_strAppPath + m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T(""));
		strEvent.Format(_T("背景%d列表图片名称"), ilop + 1);
		pbi.strListPicturePath = m_strAppPath + m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T(""));
		strEvent.Format(_T("背景%d播放图片名称"), ilop + 1);
		pbi.strPlayPicturePath = m_strAppPath + m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T(""));
		strEvent.Format(_T("背景%d播放左边界"), ilop + 1);
		pbi.rectPlay.left = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d播放右边界"), ilop + 1);
		pbi.rectPlay.right = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d播放上边界"), ilop + 1);
		pbi.rectPlay.top = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d播放下边界"), ilop + 1);
		pbi.rectPlay.bottom = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d停止图片名称"), ilop + 1);
		pbi.strStopPicturePath = m_strAppPath + m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T(""));
		strEvent.Format(_T("背景%d停止左边界"), ilop + 1);
		pbi.rectStop.left = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d停止右边界"), ilop + 1);
		pbi.rectStop.right = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d停止上边界"), ilop + 1);
		pbi.rectStop.top = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d停止下边界"), ilop + 1);
		pbi.rectStop.bottom = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d暂停图片名称"), ilop + 1);
		pbi.strPausePicturePath = m_strAppPath + m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T(""));
		strEvent.Format(_T("背景%d暂停左边界"), ilop + 1);
		pbi.rectPause.left = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d暂停右边界"), ilop + 1);
		pbi.rectPause.right = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d暂停上边界"), ilop + 1);
		pbi.rectPause.top = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d暂停下边界"), ilop + 1);
		pbi.rectPause.bottom = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d上一首图片名称"), ilop + 1);
		pbi.strPrevPicturePath = m_strAppPath + m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T(""));
		strEvent.Format(_T("背景%d上一首左边界"), ilop + 1);
		pbi.rectPrev.left = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d上一首右边界"), ilop + 1);
		pbi.rectPrev.right = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d上一首上边界"), ilop + 1);
		pbi.rectPrev.top = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d上一首下边界"), ilop + 1);
		pbi.rectPrev.bottom = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d下一首图片名称"), ilop + 1);
		pbi.strFFPicturePath = m_strAppPath + m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T(""));
		strEvent.Format(_T("背景%d下一首左边界"), ilop + 1);
		pbi.rectFF.left = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d下一首右边界"), ilop + 1);
		pbi.rectFF.right = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d下一首上边界"), ilop + 1);
		pbi.rectFF.top = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d下一首下边界"), ilop + 1);
		pbi.rectFF.bottom = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		strEvent.Format(_T("背景%d透明色"), ilop + 1);
		pbi.dwTransparentColor = _ttoi(m_cIniReader.UpdateKey(_T("系统配置"), strEvent, _T(""), _T("0")));
		m_cPicureBGInfo.Add(pbi);
	}
}

void CSongInfoEditorDlg::SaveButtonPosition(int nButton, CRect& rcPosition)
{
	CString strEvent, strButton;
	switch(nButton){
		case 0:
			strButton = _T("播放");
			m_cPicureBGInfo[m_nPictureBGIndex].rectPlay = rcPosition;
			break;
		case 1:
			strButton = _T("停止");
			m_cPicureBGInfo[m_nPictureBGIndex].rectStop = rcPosition;
			break;
		case 2:
			strButton = _T("暂停");
			m_cPicureBGInfo[m_nPictureBGIndex].rectPause = rcPosition;
			break;
		case 3:
			strButton = _T("上一首");
			m_cPicureBGInfo[m_nPictureBGIndex].rectPrev = rcPosition;
			break;
		case 4:
			strButton = _T("下一首");
			m_cPicureBGInfo[m_nPictureBGIndex].rectFF = rcPosition;
			break;
		default:
			break;
	}
	m_cIniReader.AutoIniFileName();
	strEvent.Format(_T("背景%d%s左边界"), m_nPictureBGIndex + 1, strButton);
	m_cIniReader.UpdateKey(_T("系统配置"), strEvent, rcPosition.left);
	strEvent.Format(_T("背景%d%s右边界"), m_nPictureBGIndex + 1, strButton);
	m_cIniReader.UpdateKey(_T("系统配置"), strEvent, rcPosition.right);
	strEvent.Format(_T("背景%d%s上边界"), m_nPictureBGIndex + 1, strButton);
	m_cIniReader.UpdateKey(_T("系统配置"), strEvent, rcPosition.top);
	strEvent.Format(_T("背景%d%s下边界"), m_nPictureBGIndex + 1, strButton);
	m_cIniReader.UpdateKey(_T("系统配置"), strEvent, rcPosition.bottom);
}