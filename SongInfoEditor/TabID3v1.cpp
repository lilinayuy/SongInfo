// TabID3v1.cpp : implementation file
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "TabID3v1.h"
#include "SongInfoEditorDlg.h"

const CString wszGenre[] = {_T("Blues"), 
                            _T("Classic Rock"),
							_T("Country"),
							_T("Dance"),
							_T("Disco"),
							_T("Funk"),
							_T("Grunge"), 
                            _T("Hip-Hop"),
							_T("Jazz"),
							_T("Metal"),

							_T("New Age"), 
                            _T("Oldies"),
							_T("Other"),
							_T("Pop"),
							_T("R&B"),
							_T("Rap"),
							_T("Reggae"), 
                            _T("Rock"),
							_T("Techno"),
							_T("Industrial"),

							_T("Alternative"), 
                            _T("Ska"),
							_T("Death Metal"),
							_T("Pranks"),
							_T("Soundtrack"),
							_T("Euro-Techno"),
							_T("Ambient"), 
                            _T("Trip-Hop"),
							_T("Vocal"),
							_T("Jazz+Funk"),

							_T("Fusion"), 
                            _T("Trance"),
							_T("Classical"),
							_T("Instrumental"),
							_T("Acid"),
							_T("House"),
							_T("Game"), 
                            _T("Sound Clip"),
							_T("Gospel"),
							_T("Noise"),

							_T("AlternRock"), 
                            _T("Bass"),
							_T("Soul"),
							_T("Punk"),
							_T("Space"),
							_T("Meditative"),
							_T("Instrumental Pop"), 
                            _T("Instrumental Rock"),
							_T("Ethnic"),
							_T("Gothic"),

							_T("Darkwave"), 
                            _T("Techno-Industrial"),
							_T("Electronic"),
							_T("Pop-Folk"),
							_T("Eurodance"),
							_T("Dream"),
							_T("Southern Rock"), 
                            _T("Comedy"),
							_T("Cult"),
							_T("Gangsta"),

							_T("Top 40"), 
                            _T("Christian Rap"),
							_T("Pop/Funk"),
							_T("Jungle"),
							_T("Native American"),
							_T("Cabaret"),
							_T("New Wave"), 
                            _T("Psychadelic"),
							_T("Rave"),
							_T("Showtunes"),

							_T("Trailer"), 
                            _T("Lo-Fi"),
							_T("Tribal"),
							_T("Acid Punk"),
							_T("Acid Jazz"),
							_T("Polka"),
							_T("Retro"), 
                            _T("Musical"),
							_T("Rock & Roll"),
							_T("Hard Rock"),
       //Winamp
							_T("Folk"), 
                            _T("Folk-Rock"),
							_T("National Folk"),
							_T("Swing"),
							_T("Fast Fusion"),
							_T("Bebob"),
							_T("Latin"), 
                            _T("Revival"),
							_T("Celtic"),
							_T("Bluegrass"),

							_T("Avantgarde"), 
                            _T("Gothic Rock"),
							_T("Progressive Rock"),
							_T("Psychedelic Rock"),
							_T("Symphonic Rock"),
							_T("Slow Rock"),
							_T("Big Band"), 
                            _T("Chorus"),
							_T("Easy Listening"),
							_T("Acoustic"),

							_T("Humour"), 
                            _T("Speech"),
							_T("Chanson"),
							_T("Opera"),
							_T("Chamber Music"),
							_T("Sonata"),
							_T("Symphony"), 
                            _T("Booty Bass"),
							_T("Primus"),
							_T("Porn Groove"),

							_T("Satire"), 
                            _T("Slow Jam"),
							_T("Club"),
							_T("Tango"),
							_T("Samba"),
							_T("Folklore"),
							_T("Ballad"), 
                            _T("Power Ballad"),
							_T("Rhythmic Soul"),
							_T("Freestyle"),

							_T("Duet"), 
                            _T("Punk Rock"),
							_T("Drum Solo"),
							_T("Acapella"),
							_T("Euro-House"),
							_T("Dance Hall")};

// CTabID3v1 dialog

IMPLEMENT_DYNAMIC(CTabID3v1, CDialog)

CTabID3v1::CTabID3v1(CWnd* pParent /*=NULL*/)
	: CDialog(CTabID3v1::IDD, pParent)
	, m_strEditTitle(_T(""))
	, m_strEditArtist(_T(""))
	, m_strEditAlbum(_T(""))
	, m_strEditYear(_T(""))
	, m_strEditComment(_T(""))
	, m_nEditTrack(0)
{
	m_szTitle[0] = '\0';
	m_szArtist[0] = '\0';
	m_szAlbum[0] = '\0';
	m_szYear[0] = '\0';
	m_szComment[0] = '\0';
	m_szGenre = -1;
	m_szTrack[0] = 0x00;
	m_szTrack[1] = 0x00;
	m_bGenreAdded = FALSE;
	m_bEnable = FALSE;
}

CTabID3v1::~CTabID3v1()
{
}

void CTabID3v1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strEditTitle);
	DDV_MaxChars(pDX, m_strEditTitle, 30);
	DDX_Text(pDX, IDC_EDIT_ARTIST, m_strEditArtist);
	DDV_MaxChars(pDX, m_strEditArtist, 30);
	DDX_Text(pDX, IDC_EDIT_ALBUM, m_strEditAlbum);
	DDV_MaxChars(pDX, m_strEditAlbum, 30);
	DDX_Text(pDX, IDC_EDIT_YEAR, m_strEditYear);
	DDV_MaxChars(pDX, m_strEditYear, 4);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strEditComment);
	DDV_MaxChars(pDX, m_strEditComment, 28);
	DDX_Control(pDX, IDC_CMB_GENRE, m_cCmbGenre);
	DDX_Text(pDX, IDC_EDIT_TRACK, m_nEditTrack);
	DDV_MinMaxUInt(pDX, m_nEditTrack, 0, 255);
	DDX_Control(pDX, IDC_BTN_TOJAPANESE, m_cBtnToJapanese);
	DDX_Control(pDX, IDC_BTN_TOCHINESE, m_cBtnToChinese);
	DDX_Control(pDX, IDC_BTN_SAVEID3V1, m_cBtnSave);
}


BEGIN_MESSAGE_MAP(CTabID3v1, CDialog)
	ON_BN_CLICKED(IDC_BTN_TOJAPANESE, &CTabID3v1::OnBnClickedBtnTojapanese)
	ON_BN_CLICKED(IDC_BTN_TOCHINESE, &CTabID3v1::OnBnClickedBtnTochinese)
	ON_BN_CLICKED(IDC_BTN_SAVEID3V1, &CTabID3v1::OnBnClickedBtnSaveid3v1)
	ON_BN_CLICKED(IDC_BTN_COPYARTISTOUT, &CTabID3v1::OnBnClickedBtnCopyartistout)
	ON_BN_CLICKED(IDC_BTN_COPYALBUMOUT, &CTabID3v1::OnBnClickedBtnCopyalbumout)
	ON_BN_CLICKED(IDC_BTN_ADD, &CTabID3v1::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CTabID3v1::OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CTabID3v1 message handlers

void CTabID3v1::OnBnClickedBtnTojapanese()
{
	ChangeUnicode(932);//ANSI/OEM Japanese; Japanese (Shift-JIS)
}

void CTabID3v1::OnBnClickedBtnTochinese()
{
	ChangeUnicode(936);//ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
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
	TabRefresh();
	EnableDialog(m_bEnable);
//	m_cEditTitle.SetLimitText(30);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTabID3v1::TabRefresh()
{
	ChangeUnicode(CP_ACP);
}

void CTabID3v1::EnableDialog(BOOL bEnable)
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_EDIT_TITLE);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_ARTIST);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_ALBUM);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_YEAR);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_COMMENT);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_TRACK);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CMB_GENRE);
	pWnd->EnableWindow(bEnable);
}

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
	MultiByteToWideChar(nDes, 0, m_szComment, 29, wszTitle, 29);
	m_strEditComment.Format(_T("%s"), wszTitle);
	if (((int)(m_szGenre < 0)) || ((int)(m_szGenre) > 125)){
		m_cCmbGenre.SetCurSel(126);
	}else{
		m_cCmbGenre.SetCurSel((int)(m_szGenre));
	}
	m_nEditTrack = (UCHAR)m_szTrack[1];
	UpdateData(FALSE);
}
void CTabID3v1::OnBnClickedBtnSaveid3v1()
{
	if (ChangeMultiByte(CP_ACP)){
		if (((CSongInfoEditorDlg*)m_pDlg)->m_nReadyPlay != -1){
			((CSongInfoEditorDlg*)m_pDlg)->SaveInfoID3v1();
		}
	}
}

BOOL CTabID3v1::ChangeMultiByte(int nDes)
{
	UpdateData(TRUE);
	wchar_t wszTitle[30];
	wchar_t wszYear[4];
	BOOL bOK, bOKAll;
	bOKAll = FALSE;
	ZeroMemory(wszTitle, 30 * sizeof(TCHAR));
	CopyMemory(wszTitle, m_strEditTitle.GetString(), m_strEditTitle.GetLength() * sizeof(TCHAR));
	WideCharToMultiByte(nDes, 0, wszTitle, 30, m_szTitle, 30, "?", &bOK);
	bOKAll |= bOK;
	ZeroMemory(wszTitle, 30 * sizeof(TCHAR));
	CopyMemory(wszTitle, m_strEditArtist.GetString(), m_strEditArtist.GetLength() * sizeof(TCHAR));
	WideCharToMultiByte(nDes, 0, wszTitle, 30, m_szArtist, 30, "?", &bOK);
	bOKAll |= bOK;
	ZeroMemory(wszTitle, 30 * sizeof(TCHAR));
	CopyMemory(wszTitle, m_strEditAlbum.GetString(), m_strEditAlbum.GetLength() * sizeof(TCHAR));
	WideCharToMultiByte(nDes, 0, wszTitle, 30, m_szAlbum, 30, "?", &bOK);
	bOKAll |= bOK;
	ZeroMemory(wszYear, 4 * sizeof(TCHAR));
	CopyMemory(wszYear, m_strEditYear.GetString(), m_strEditYear.GetLength() * sizeof(TCHAR));
	WideCharToMultiByte(nDes, 0, wszYear, 4, m_szYear, 4, "?", &bOK);
	bOKAll |= bOK;
	ZeroMemory(wszTitle, 30 * sizeof(TCHAR));
	CopyMemory(wszTitle, m_strEditComment.GetString(), m_strEditComment.GetLength() * sizeof(TCHAR));
	WideCharToMultiByte(nDes, 0, wszTitle, 28, m_szComment, 28, "?", &bOK);
	bOKAll |= bOK;
	m_szGenre = (char)m_cCmbGenre.GetCurSel();
	m_szTrack[0] = 0x00;
	m_szTrack[1] = m_nEditTrack;
	if (bOKAll){
		if (MessageBox(_T("Unicode変換中表示できない文字があります、確認してください。"), _T("変更しますか？"), MB_YESNO) == IDNO) return FALSE;
	}
	return TRUE;
}

void CTabID3v1::OnBnClickedBtnCopyartistout()
{
	UpdateData(TRUE);
	((CSongInfoEditorDlg*)m_pDlg)->m_cEditCopyArtist.SetWindowTextW(m_strEditArtist);
}

void CTabID3v1::OnBnClickedBtnCopyalbumout()
{
	UpdateData(TRUE);
	((CSongInfoEditorDlg*)m_pDlg)->m_cEditCopyAlbum.SetWindowTextW(m_strEditAlbum);
}

void CTabID3v1::OnBnClickedBtnAdd()
{
	if (m_bEnable) return;
	m_bEnable = TRUE;
	EnableDialog(m_bEnable);
}

void CTabID3v1::OnBnClickedBtnDelete()
{
	if (!m_bEnable) return;
	m_bEnable = FALSE;
	EnableDialog(m_bEnable);
}
