#pragma once
#include "afxwin.h"


// CTabID3v1 dialog

class CTabID3v1 : public CDialog
{
	DECLARE_DYNAMIC(CTabID3v1)

public:
	CTabID3v1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabID3v1();

// Dialog Data
	enum { IDD = IDD_DLG_ID3V1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTojapanese();
	afx_msg void OnBnClickedBtnTochinese();
	CString m_strEditTitle;
	CString m_strEditArtist;
	CString m_strEditAlbum;
	CString m_strEditYear;
	CString m_strEditComment;
	UINT m_nEditTrack;
	CComboBox m_cCmbGenre;
	BOOL m_bGenreAdded;
	CDialog* m_pDlg;

	char m_szTitle[31];
	char m_szArtist[31];
	char m_szAlbum[31];
	char m_szYear[5];
	char m_szComment[29];
	char m_szGenre;
	char m_szTrack[2];
	virtual BOOL OnInitDialog();
	void TabRefresh(void);
	void ChangeUnicode(int nDes);
	void EnableDialog(BOOL bEnable);
	BOOL ChangeMultiByte(int nDes);
	afx_msg void OnBnClickedBtnSaveid3v1();
	afx_msg void OnBnClickedBtnCopyartistout();
	afx_msg void OnBnClickedBtnCopyalbumout();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CButton m_cBtnToJapanese;
	CButton m_cBtnToChinese;
	CButton m_cBtnSave;
	BOOL m_bEnable;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
};
