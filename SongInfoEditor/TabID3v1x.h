#pragma once

#include "afxwin.h"
// CTabID3v1 dialog

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
	void ChangeUnicode(int nDes);
	void TabRefresh(void);

	char m_szTitle[31];
	char m_szArtist[31];
	char m_szAlbum[31];
	char m_szYear[5];
	char m_szComment[31];
	char m_szGenre;
	CString m_strEditTitle;
	CString m_strEditArtist;
	CString m_strEditAlbum;
	CString m_strEditYear;
	CString m_strEditComment;
	
	virtual BOOL OnInitDialog();
	CComboBox m_cCmbGenre;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	BOOL m_bGenreAdded;
};
