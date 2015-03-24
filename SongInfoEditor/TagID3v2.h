#pragma once

#include "afxwin.h"

typedef struct ID3TagContent
{
	CString strTitle;
	CString strArtist;
	CString strAlbum;
	CString strYear;
	CString strGenre;
	CString strComment;
	CString strLyric;
	CString strComposer;
	CString strTrack;
	CString strCopyright;
	CString strOriArtist;
	CString strEncoder;
	CString strEncodedBy;
	CString strFileType;
	CString strMediaType;
	CString strURL;
	CString strLyricText;
	UINT nID3Version; //0 2.2,1 2.3, 2 2.4
	BOOL bUnicode;
}ID3TAGCONTENT;

typedef struct ID3Frame
{
	DWORD size;
	unsigned char* pszData;
}ID3FRAME;

class CTagID3v2
{
public:
	CTagID3v2(void);
	~CTagID3v2(void);


	CFile* m_pFile;
	BOOL LoadID3v2(void);
	DWORD m_dwHeadSize;
	unsigned char* m_pID3v2Content;
	CString m_strErrMsg;
	ID3TAGCONTENT m_cID3Content;
	CArray<ID3FRAME, ID3FRAME&> m_cOtherFrames;
	CArray<ID3FRAME, ID3FRAME&> m_cMainFrames;
	BOOL m_bEnable;//ID3v2‚ð•Û‘¶‚·‚é‚©

	DWORD DecodeUnSynchronization(unsigned char* pdata, DWORD dwSize);
	DWORD EncodeUnSynchronization(unsigned char *srcData, DWORD dwSize, unsigned char *dstData);
	DWORD LoadFrame2_2(unsigned char* pData,DWORD dwSize);
	DWORD LoadFrame(unsigned char* pData,DWORD dwSize);
	void ReleaseOtherFrames(void);
	void ReleaseMainFrames(void);
	void ReleaseID3Strings(void);
	BOOL SaveID3Tag(CString strFileName);
	DWORD StrToFrame(void);
	DWORD StrToFrame2_2(void);
	DWORD GetTotalFrameSize();
};
