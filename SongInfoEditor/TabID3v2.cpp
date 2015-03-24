// TabID3v2.cpp : implementation file
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "TabID3v2.h"
#include "SongInfoEditorDlg.h"
#include "ShowLyricDlg.h"
#include "shellapi.h"

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
// CTabID3v2 dialog

IMPLEMENT_DYNAMIC(CTabID3v2, CDialog)

CTabID3v2::CTabID3v2(CWnd* pParent /*=NULL*/)
	: CDialog(CTabID3v2::IDD, pParent)
	, m_strTitle(_T(""))
	, m_strTrack(_T(""))
	, m_strArtist(_T(""))
	, m_strAlbum(_T(""))
	, m_strYear(_T(""))
	, m_strGenre(_T(""))
	, m_strComposer(_T(""))
	, m_strLyricist(_T(""))
	, m_strOriginalArtist(_T(""))
	, m_strCopyright(_T(""))
	, m_strEncoder(_T(""))
	, m_strEncodedBy(_T(""))
	, m_strComment(_T(""))
	, m_strURL(_T(""))
	, m_nID3Version(0)
{
	m_bGenreAdded = FALSE;
	m_bShowLyric = FALSE;
	m_bEnable = FALSE;
}

CTabID3v2::~CTabID3v2()
{
	delete m_pLyricDlg;
}

void CTabID3v2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_TRACK2, m_strTrack);
	DDX_Text(pDX, IDC_EDIT_ARTIST, m_strArtist);
	DDX_Text(pDX, IDC_EDIT_ALBUM, m_strAlbum);
	DDX_Text(pDX, IDC_EDIT_YEAR, m_strYear);
	DDV_MaxChars(pDX, m_strYear, 4);
	DDX_CBString(pDX, IDC_CMB_GENRE, m_strGenre);
	DDX_Text(pDX, IDC_EDIT_COMPOSER, m_strComposer);
	DDX_Text(pDX, IDC_EDIT_LYRIC, m_strLyricist);
	DDX_Text(pDX, IDC_EDIT_ORIARTIST, m_strOriginalArtist);
	DDX_Text(pDX, IDC_EDIT_COPYRIGHT, m_strCopyright);
	DDX_Text(pDX, IDC_EDIT_ENCODER, m_strEncoder);
	DDX_Text(pDX, IDC_EDIT_ENCODEBY, m_strEncodedBy);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDX_Text(pDX, IDC_EDIT_URL, m_strURL);
	DDX_CBIndex(pDX, IDC_CMB_ID3VER, m_nID3Version);
	DDX_Control(pDX, IDC_CMB_TEXTENCODE, m_cUnicode);
	DDX_Control(pDX, IDC_CMB_GENRE, m_cCmbGenre);
	DDX_Control(pDX, IDC_CMB_ID3VER, m_cCmbID3Version);
	DDX_Control(pDX, IDC_BTN_TOJAPANESE, m_cBtnToJapanese);
	DDX_Control(pDX, IDC_BTN_TOCHINESE, m_cBtnToChinese);
	DDX_Control(pDX, IDC_BTN_SAVE, m_cBtnSave);
}


BEGIN_MESSAGE_MAP(CTabID3v2, CDialog)
	ON_BN_CLICKED(IDC_BTN_COPYARTISTOUT, &CTabID3v2::OnBnClickedBtnCopyartistout)
	ON_BN_CLICKED(IDC_BTN_COPYALBUMOUT, &CTabID3v2::OnBnClickedBtnCopyalbumout)
	ON_BN_CLICKED(IDC_BTN_SHOWLYRIC, &CTabID3v2::OnBnClickedBtnShowlyric)
	ON_BN_CLICKED(IDC_BTN_GOTOURL, &CTabID3v2::OnBnClickedBtnGotourl)
	ON_BN_CLICKED(IDC_BTN_TOJAPANESE, &CTabID3v2::OnBnClickedBtnTojapanese)
	ON_BN_CLICKED(IDC_BTN_TOCHINESE, &CTabID3v2::OnBnClickedBtnTochinese)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CTabID3v2::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_ADD, &CTabID3v2::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CTabID3v2::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &CTabID3v2::OnBnClickedBtnUpload)
END_MESSAGE_MAP()


// CTabID3v2 message handlers

void CTabID3v2::OnBnClickedBtnCopyartistout()
{
	UpdateData(TRUE);
	((CSongInfoEditorDlg*)m_pDlg)->m_cEditCopyArtist.SetWindowTextW(m_strArtist);
}

void CTabID3v2::OnBnClickedBtnCopyalbumout()
{
	UpdateData(TRUE);
	((CSongInfoEditorDlg*)m_pDlg)->m_cEditCopyAlbum.SetWindowTextW(m_strAlbum);
}

void CTabID3v2::OnBnClickedBtnShowlyric()
{
	if (m_bShowLyric){
		m_pLyricDlg->ShowWindow(SW_HIDE);
		m_bShowLyric = FALSE;
	}else{
		m_pLyricDlg->ShowWindow(SW_SHOW);
		m_bShowLyric = TRUE;
	}
}

void CTabID3v2::OnBnClickedBtnGotourl()
{
	UpdateData(TRUE);
	ShellExecute(NULL, _T("open"), m_strURL, NULL, NULL, SW_SHOWNORMAL); 
}

void CTabID3v2::OnBnClickedBtnTojapanese()
{
	ChangeUnicode(932);
}

void CTabID3v2::OnBnClickedBtnTochinese()
{
	ChangeUnicode(936);
}

void CTabID3v2::OnBnClickedBtnSave()
{
	UpdateData(TRUE);
	((CShowLyricDlg*)m_pLyricDlg)->m_cEditLyric.GetWindowTextW(m_strLyricText);
	if (((CSongInfoEditorDlg*)m_pDlg)->m_nReadyPlay != -1){
		((CSongInfoEditorDlg*)m_pDlg)->SaveInfoID3v2();
	}
}

BOOL CTabID3v2::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!m_bGenreAdded){
		m_cCmbGenre.ResetContent();
		for (int i = 0; i < 126; i ++){
			m_cCmbGenre.AddString(wszGenre[i]);
		}
		m_cCmbGenre.AddString(_T(""));
		m_bGenreAdded = TRUE;
		m_cCmbID3Version.AddString(_T("v2.2"));
		m_cCmbID3Version.AddString(_T("v2.3"));
		m_cCmbID3Version.AddString(_T("v2.4"));
		m_cUnicode.AddString(_T("ISO-8859-1"));
		m_cUnicode.AddString(_T("UTF-16(Unicode)"));
		m_pLyricDlg = new CShowLyricDlg;
		((CShowLyricDlg*)m_pLyricDlg)->Create(IDD_DLG_LYRIC, this);
		if (m_bShowLyric){
			m_pLyricDlg->ShowWindow(SW_SHOWNORMAL);
		}else{
			m_pLyricDlg->ShowWindow(SW_HIDE);
		}
		((CShowLyricDlg*)m_pLyricDlg)->m_pDlg = this;
	}
	UpdateData(FALSE);
	if (m_bUnicode){
		m_cUnicode.SetCurSel(1);
	}else{
		m_cUnicode.SetCurSel(0);
	}
	EnableDialog(m_bEnable);
	((CShowLyricDlg*)m_pLyricDlg)->m_cEditLyric.SetWindowTextW(m_strLyricText);
	((CShowLyricDlg*)m_pLyricDlg)->SetWindowTextW(m_strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTabID3v2::EnableDialog(BOOL bEnable)
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
	pWnd = GetDlgItem(IDC_EDIT_TRACK2);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CMB_GENRE);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_COMPOSER);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_LYRIC);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_ORIARTIST);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_COPYRIGHT);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_ENCODER);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_ENCODEBY);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_EDIT_URL);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CMB_ID3VER);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CMB_TEXTENCODE);
	pWnd->EnableWindow(bEnable);
	pWnd = ((CShowLyricDlg*)m_pLyricDlg)->GetDlgItem(IDC_EDIT_LYRIC);
	pWnd->EnableWindow(bEnable);
}

BOOL CTabID3v2::ChangeUnicode(int Des)
{
	UpdateData(TRUE);
	char* pszString;
	WCHAR* pwszString;
	DWORD dwLength;
	DWORD dwSize;
	BOOL bOK, bOkAll;
	bOkAll = FALSE;
	CStringArray cArrString;
	cArrString.Add(m_strTitle);
	cArrString.Add(m_strArtist);
	cArrString.Add(m_strAlbum);
	cArrString.Add(m_strTrack);
	cArrString.Add(m_strYear);
	cArrString.Add(m_strGenre);
	cArrString.Add(m_strComposer);
	cArrString.Add(m_strLyricist);
	cArrString.Add(m_strOriginalArtist);
	cArrString.Add(m_strCopyright);
	cArrString.Add(m_strEncoder);
	cArrString.Add(m_strEncodedBy);
	cArrString.Add(m_strComment);
	cArrString.Add(m_strURL);
	cArrString.Add(m_strLyricText);
	cArrString.Add(m_strFileType);
	cArrString.Add(m_strMediaType);
	for (int i = 0; i < cArrString.GetSize(); i ++){
		dwLength = cArrString[i].GetLength() + 1;
		pwszString = new WCHAR[dwLength];
		pszString = new char[dwLength * sizeof(WCHAR)];
		memcpy(pwszString, cArrString[i].GetString(), dwLength * sizeof(WCHAR));
		dwSize = WideCharToMultiByte(CP_ACP, 0, pwszString, dwLength, pszString, dwLength * sizeof(WCHAR), "?", &bOK) + 7;
		delete pwszString;
		pwszString = new WCHAR[dwSize];
		MultiByteToWideChar(Des, 0, pszString, dwSize, pwszString, dwSize);
		cArrString.RemoveAt(i);
		cArrString.InsertAt(i, pwszString);
		bOkAll |= bOK;
		delete pwszString;
		delete pszString;
	}
	m_strTitle = cArrString[0];
	m_strArtist = cArrString[1];
	m_strAlbum = cArrString[2];
	m_strTrack = cArrString[3];
	m_strYear = cArrString[4];
	m_strGenre = cArrString[5];
	m_strComposer = cArrString[6];
	m_strLyricist = cArrString[7];
	m_strOriginalArtist = cArrString[8];
	m_strCopyright = cArrString[9];
	m_strEncoder = cArrString[10];
	m_strEncodedBy = cArrString[11];
	m_strComment = cArrString[12];
	m_strURL = cArrString[13];
	m_strLyricText = cArrString[14];
	m_strFileType = cArrString[15];
	m_strMediaType = cArrString[16];
	UpdateData(FALSE);
	return TRUE;
}
void CTabID3v2::OnBnClickedBtnAdd()
{
	if (m_bEnable) return;
	m_bEnable = TRUE;
	EnableDialog(m_bEnable);
}

void CTabID3v2::OnBnClickedBtnDelete()
{
	if (!m_bEnable) return;
	m_bEnable = FALSE;
	EnableDialog(m_bEnable);
}
void CTabID3v2::OnBnClickedBtnUpload()
{
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditTitle = m_strTitle;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditArtist = m_strArtist;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditAlbum = m_strAlbum;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditComposer = m_strComposer;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditLyricist = m_strLyricist;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditComposer = m_strComposer;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditLyric = m_strLyricText;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.m_strEditComment = m_strComment;
	((CSongInfoEditorDlg*)m_pDlg)->m_cDlgUpload.DoModal();
}
