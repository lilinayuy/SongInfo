#pragma once
#include   <winsock.h>   
#include   <mysql.h>

// CDlgUpload dialog

class CDlgUpload : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpload)

public:
	CDlgUpload(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUpload();

// Dialog Data
	enum { IDD = IDD_DLG_UPLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strEditTitle;
	CString m_strEditArtist;
	CString m_strEditAlbum;
	CString m_strEditSource;
	CString m_strEditComposer;
	CString m_strEditLyricist;
	CString m_strEditArranger;
	CString m_strEditChorus;
	int m_nChkKaraoke;
	int m_nChkLyric;
	CString m_strEditLyric;
	CString m_strEditComment;
//	afx_msg void OnBnClickedOk();
	MYSQL songSQL;
	CString m_strErrMsg;
	afx_msg void OnBnClickedBtnOk();
};
