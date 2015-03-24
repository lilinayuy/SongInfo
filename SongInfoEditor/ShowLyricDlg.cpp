// ShowLyricDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "ShowLyricDlg.h"
#include "TabID3v2.h"


// CShowLyricDlg dialog

IMPLEMENT_DYNAMIC(CShowLyricDlg, CDialog)

CShowLyricDlg::CShowLyricDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowLyricDlg::IDD, pParent)
{

}

CShowLyricDlg::~CShowLyricDlg()
{
}

void CShowLyricDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LYRIC, m_cEditLyric);
}


BEGIN_MESSAGE_MAP(CShowLyricDlg, CDialog)
	ON_WM_SIZING()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CShowLyricDlg message handlers

void CShowLyricDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	RECT rect;
	GetClientRect(&rect);
	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;
	m_cEditLyric.MoveWindow(&rect);
	CDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
}

void CShowLyricDlg::OnClose()
{
	((CTabID3v2*)m_pDlg)->m_bShowLyric = FALSE;
	this->ShowWindow(SW_HIDE);
	return;
	CDialog::OnClose();
}

BOOL CShowLyricDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT rect;
	GetClientRect(&rect);
	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;
	m_cEditLyric.MoveWindow(&rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
