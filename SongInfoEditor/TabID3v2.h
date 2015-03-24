#pragma once
#include "afxwin.h"



// CTabID3v2 dialog

class CTabID3v2 : public CDialog
{
	DECLARE_DYNAMIC(CTabID3v2)

public:
	CTabID3v2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabID3v2();

// Dialog Data
	enum { IDD = IDD_DLG_ID3V2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString m_strTitle;
	CString m_strTrack;
	CString m_strArtist;
	CString m_strAlbum;
	CString m_strYear;
	CString m_strGenre;
	CString m_strComposer;
	CString m_strLyricist;
	CString m_strOriginalArtist;
	CString m_strCopyright;
	CString m_strEncoder;
	CString m_strEncodedBy;
	CString m_strComment;
	CString m_strURL;
	CString m_strLyricText;
	CString m_strFileType;
	CString m_strMediaType;
	afx_msg void OnBnClickedBtnCopyartistout();
	afx_msg void OnBnClickedBtnCopyalbumout();
	int m_nID3Version;
	CComboBox m_cUnicode;
	BOOL m_bUnicode;
	afx_msg void OnBnClickedBtnShowlyric();
	afx_msg void OnBnClickedBtnGotourl();
	afx_msg void OnBnClickedBtnTojapanese();
	afx_msg void OnBnClickedBtnTochinese();
	afx_msg void OnBnClickedBtnSave();
	virtual BOOL OnInitDialog();
	BOOL ChangeUnicode(int Des);
	BOOL m_bEnable;
	void EnableDialog(BOOL bEnable);

	BOOL m_bGenreAdded;
	CComboBox m_cCmbGenre;
	CComboBox m_cCmbID3Version;
	CDialog* m_pDlg;
	CDialog* m_pLyricDlg;
	BOOL m_bShowLyric;
	CButton m_cBtnToJapanese;
	CButton m_cBtnToChinese;
	CButton m_cBtnSave;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnUpload();
};
