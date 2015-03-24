// SongInfoEditorDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "bmpbutton.h"
#include "TabID3v1.h"
#include "TabID3v2.h"
#include "afxwin.h"
#include "Windows.h"
#include "TagID3v2.h"
#include "DlgUpload.h"
#include "DSPlayer.h"
#include "ini.hpp"
#include "DialogPicture.h"

#include   "mmsystem.h"
#pragma   comment(lib,   "winmm.lib")

#define VERSION 2
#define WM_HOTMSG  WM_USER+107
typedef struct SongInfo{
	CString strPath;
	CString strShowName;
} SONGINFO;

typedef struct PictureBGInfo{
	CString strPicturePath;
	CString strListPicturePath;
	CString strPrevPicturePath;
	CRect rectPrev;
	CString strFFPicturePath;
	CRect rectFF;
	CString strStopPicturePath;
	CRect rectStop;
	CString strPlayPicturePath;
	CRect rectPlay;
	CString strPausePicturePath;
	CRect rectPause;
	DWORD dwTransparentColor;
} PICBGINFO;

class CSongInfoEditorDlgAutoProxy;


// CSongInfoEditorDlg dialog
class CSongInfoEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(CSongInfoEditorDlg);
	friend class CSongInfoEditorDlgAutoProxy;

// Construction
public:
	CSongInfoEditorDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CSongInfoEditorDlg();

// Dialog Data
	enum { IDD = IDD_SONGINFOEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CSongInfoEditorDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	int m_nVersion;
	int TimerID;
	static void CALLBACK TimeProc(UINT uID,UINT uMsg, DWORD dwUser,DWORD dw1,DWORD dw2); // 定时器回调函数
	//DirectShow
	CDSPlayer m_cDSPlayer;
	CStringArray m_arrDeviceName;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CTabCtrl m_cTabInfo;
	CTabID3v1 m_cID3v1;
	CTabID3v2 m_cID3v2;
	int m_nTabSelNum;
	CDialog* m_pTabAddr[2];
	BOOL m_bTabFinish;
	int m_nListNum;
	CListBox m_cListSong;
	CTagID3v2 m_cTagID3v2;
	CDlgUpload m_cDlgUpload;
	CString m_strPath;
	CIni m_cIniReader;

	CArray<SongInfo, SongInfo&> m_cSongInfo;

	void Refresh();
	void ShowMciErrorMessage(DWORD fdwError);
	void CloseMci();
	void SetPlaySlider();
	void GetMciPlayPos();
	void LoadSongInfo(int nIndex);
	void ShowTime();
	void SelectPlaying(int nIndex);
	afx_msg void OnBnClickedBtnPlay();

	afx_msg void OnTcnSelchangeTabInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnDblclkListSong();
	afx_msg void OnLbnKillfocusListSong();
	afx_msg void OnBnClickedBtnAddsong();
	afx_msg void OnBnClickedBtnDelsong();


	CString m_strNowPlay;
	int m_nNowPlay;
	int m_nReadyPlay;
	CBmpButton m_btnPlay;
	CBmpButton m_btnPrev;
	CBmpButton m_btnStop;
	CBmpButton m_btnPause;
	CBmpButton m_btnFF;
	CSliderCtrl m_cSliderPlay;
	CSliderCtrl m_cSliderVolume;
	UINT m_nSongLength;//ms
	UINT m_nSongNowPos;//ms
	UINT m_nSongHour;
	UINT m_nSongMinute;
	UINT m_nSongSecond;
	MCI_OPEN_PARMS mciOpen;
	// 帪娫扨埵偺愝掕(儈儕昩偵愝掕)
    MCI_SEQ_SET_PARMS mciMssp;
	MCI_STATUS_PARMS mciMsp;
	BOOL bNowPlaying;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMReleasedcaptureSliderPlay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnFf();
	afx_msg void OnBnClickedBtnPrev();
	CStatic m_cStaTime;
	afx_msg void OnNMReleasedcaptureSliderVolume(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnHotMsg(WPARAM wParam, LPARAM lParam);


	BOOL SaveInfoID3v1(void);
	BOOL SaveInfoID3v2(void);
	afx_msg void OnBnClickedBtnCopyalbumin();
	CEdit m_cEditCopyAlbum;
	CEdit m_cEditCopyArtist;
	afx_msg void OnBnClickedBtnCopyartistin();
	CEdit m_cEditFileName;
	afx_msg void OnBnClickedBtnRename();
	virtual BOOL DestroyWindow();
	CComboBox m_cCmbDeviceSelect;
	afx_msg void OnCbnSelchangeCmbDeviceselect();
	void InitSL(BOOL bLoad = TRUE);
	CArray<PictureBGInfo, PictureBGInfo&> m_cPicureBGInfo;
	CString m_strAppPath;
	int m_nPictureBGIndex;
	CDialogPicture* m_pDlgPicture;
	BOOL m_bPictureDialog;
	void SaveButtonPosition(int nButton, CRect& rcPosition);
};
