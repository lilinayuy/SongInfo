// TabID3v1.cpp : implementation file
//

#include "stdafx.h"
#include "TabID3v1.h"
#include "SongInfoEditor.h"

// CTabID3v1 dialog

IMPLEMENT_DYNAMIC(CTabID3v1, CDialog)

CTabID3v1::CTabID3v1(CWnd* pParent /*=NULL*/)
	: CDialog(CTabID3v1::IDD, pParent)
{
	m_strEditTitle = _T("");
	m_strEditArtist = _T("");
	m_strEditAlbum = _T("");
	m_strEditYear = _T("");
	m_strEditComment = _T("");
	m_bGenreAdded = FALSE;
}

CTabID3v1::~CTabID3v1()
{
}

void CTabID3v1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strEditTitle);
	DDX_Text(pDX, IDC_EDIT_ARTIST, m_strEditArtist);
	DDX_Text(pDX, IDC_EDIT_ALBUM, m_strEditAlbum);
	DDX_Text(pDX, IDC_EDIT_YEAR, m_strEditYear);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strEditComment);
	DDX_Control(pDX, IDC_CMB_GENRE, m_cCmbGenre);
}


BEGIN_MESSAGE_MAP(CTabID3v1, CDialog)
	ON_BN_CLICKED(IDC_BTN_TOJAPANESE, &CTabID3v1::OnBnClickedBtnTojapanese)
	ON_BN_CLICKED(IDC_BTN_TOCHINESE, &CTabID3v1::OnBnClickedBtnTochinese)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CTabID3v1::ChangeUnicode(int nDes)
{
	wchar_t wszTitle[31];
	wchar_t wszYear[5];
	
	MultiByteToWideChar(nDes, 0, m_szTitle, 31, wszTitle, 31);
	m_strEditTitle.Format(_T("%s"), wszTitle);
	MultiByteToWideChar(nDes, 0, m_szArtist, 31, wszTitle, 31);
	m_strEditArtist.Format(_T("%s"), wszTitle);
	MultiByteToWideChar(nDes, 0, m_szAlbum, 31, wszTitle, 31);
	m_strEditAlbum.Format(_T("%s"), wszTitle);
	MultiByteToWideChar(nDes, 0, m_szYear, 5, wszYear, 5);
	m_strEditYear.Format(_T("%s"), wszYear);
	MultiByteToWideChar(nDes, 0, m_szComment, 31, wszTitle, 31);
	m_strEditComment.Format(_T("%s"), wszTitle);
	if (((int)(m_szGenre < 0)) || ((int)(m_szGenre) > 125)){
		m_cCmbGenre.SetCurSel(126);
	}else{
		m_cCmbGenre.SetCurSel((int)(m_szGenre));
	}
	UpdateData(FALSE);
}

// CTabID3v1 message handlers

void CTabID3v1::OnBnClickedBtnTojapanese()
{
	ChangeUnicode(932);//ANSI/OEM Japanese; Japanese (Shift-JIS)
}

void CTabID3v1::OnBnClickedBtnTochinese()
{
	ChangeUnicode(936);//ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
}

void CTabID3v1::TabRefresh()
{
	ChangeUnicode(CP_ACP);
}

BOOL CTabID3v1::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (!m_bGenreAdded){
		m_cCmbGenre.ResetContent();
		for (int i = 0; i < 126; i ++){
			m_cCmbGenre.AddString(wszGenre[i]);
		}
		m_cCmbGenre.AddString(_T(""));
		m_bGenreAdded = TRUE;
	}
	ChangeUnicode(CP_ACP);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CTabID3v1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
