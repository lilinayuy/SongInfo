// DlgUpload.cpp : implementation file
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "DlgUpload.h"
#include "afxdb.h"



// CDlgUpload dialog

IMPLEMENT_DYNAMIC(CDlgUpload, CDialog)

CDlgUpload::CDlgUpload(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpload::IDD, pParent)
	, m_strEditTitle(_T(""))
	, m_strEditArtist(_T(""))
	, m_strEditAlbum(_T(""))
	, m_strEditSource(_T(""))
	, m_strEditComposer(_T(""))
	, m_strEditLyricist(_T(""))
	, m_strEditArranger(_T(""))
	, m_strEditChorus(_T(""))
	, m_nChkKaraoke(0)
	, m_nChkLyric(0)
	, m_strEditLyric(_T(""))
	, m_strEditComment(_T(""))
{
	m_strErrMsg = _T("");
}

CDlgUpload::~CDlgUpload()
{
}

void CDlgUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strEditTitle);
	DDX_Text(pDX, IDC_EDIT_ARTIST, m_strEditArtist);
	DDX_Text(pDX, IDC_EDIT_ALBUM, m_strEditAlbum);
	DDX_Text(pDX, IDC_EDIT_SOURCE, m_strEditSource);
	DDX_Text(pDX, IDC_EDIT_COMPOSER, m_strEditComposer);
	DDX_Text(pDX, IDC_EDIT_LYRICIST, m_strEditLyricist);
	DDX_Text(pDX, IDC_EDIT_ARRANGER, m_strEditArranger);
	DDX_Text(pDX, IDC_EDIT_CHORUS, m_strEditChorus);
	DDX_Check(pDX, IDC_CHK_KARAOKE, m_nChkKaraoke);
	DDX_Check(pDX, IDC_CHK_LYRIC, m_nChkLyric);
	DDX_Text(pDX, IDC_EDIT_LYRIC, m_strEditLyric);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strEditComment);
}


BEGIN_MESSAGE_MAP(CDlgUpload, CDialog)
//	ON_BN_CLICKED(IDOK, &CDlgUpload::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgUpload::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// CDlgUpload message handlers
/*
void CDlgUpload::OnBnClickedOk()
{
	
}
*/
void CDlgUpload::OnBnClickedBtnOk()
{
	UpdateData(TRUE);
	if (m_strEditTitle == _T("")){
		MessageBox(_T("曲名を設定してください。"));
		return;
	}
	if (!mysql_init(&songSQL)){
		m_strErrMsg.Format(_T("Failed to initial mysql: Error: %s\n"),mysql_error(&songSQL));
		MessageBox(m_strErrMsg, _T("连接数据库错误"));
	}
	if (!mysql_real_connect(&songSQL, "192.168.1.102", "heero", "hiiro", "songbase",3306,NULL,0)){
		m_strErrMsg.Format(_T("Failed to connect to database: Error: %s\n"),mysql_error(&songSQL));
		MessageBox(m_strErrMsg, _T("连接数据库错误"));
	}
	CString strQuery;
	strQuery.Format(_T("INSERT INTO song (ID, Title, Artist, Album, Source, Lyricist, Composer, Arranger, Chorus, Karaoke, WithLyric, Lyric) VALUES (NULL, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%d', '%s')"), m_strEditTitle, m_strEditArtist, m_strEditAlbum, m_strEditSource, m_strEditLyricist, m_strEditComposer, m_strEditArranger, m_strEditChorus, m_nChkKaraoke, m_nChkLyric, m_strEditLyric);
	int dwUTF8Length;
	char* pszQuery;
	dwUTF8Length = WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strQuery, -1, NULL, 0, NULL, FALSE);
	pszQuery = new char[dwUTF8Length];
	if (!pszQuery){
		MessageBox(_T("Out of Memory!"));
		return;
	}
	WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)strQuery, -1, pszQuery, dwUTF8Length, NULL, FALSE);
	
	if (mysql_real_query(&songSQL, "set names utf8", 14)){
		m_strErrMsg.Format(_T("Failed to insert record: Error: %s\n"),mysql_error(&songSQL));
		MessageBox(m_strErrMsg, _T("连接数据库错误"));
	}
	if (mysql_real_query(&songSQL, pszQuery, dwUTF8Length)){
		m_strErrMsg.Format(_T("Failed to insert record: Error: %s\n"),mysql_error(&songSQL));
		MessageBox(m_strErrMsg, _T("连接数据库错误"));
	}
	delete []pszQuery;
	mysql_close(&songSQL);
	OnOK();
}
