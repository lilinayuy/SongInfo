#pragma once
#include "afxwin.h"


// CShowLyricDlg dialog

class CShowLyricDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowLyricDlg)

public:
	CShowLyricDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowLyricDlg();

// Dialog Data
	enum { IDD = IDD_DLG_LYRIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_cEditLyric;
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CDialog* m_pDlg;
};
