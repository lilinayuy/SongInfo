#include "StdAfx.h"
#include "TagID3v2.h"


const CStringA wszID2_2[] = {"TT2", 
                            "TP1",
							"TAL",
							"TYE",
							"TRK",
							"TCO",
							"TCM", 
                            "TXT",
							"TOA",
							"TCR",
							"TSS",
							"TEN",
							"TFT", 
                            "TMT",
							"COM",
							"ULT",
							"WXX"};

const CStringA wszID[] = {"TIT2", 
                            "TPE1",
							"TALB",
							"TYER",
							"TRCK",
							"TCON",
							"TCOM", 
                            "TEXT",
							"TOPE",
							"TCOP",
							"TSSE",
							"TENC",
							"TFLT", 
                            "TMED",
							"COMM",
							"USLT",
							"WXXX"};

CTagID3v2::CTagID3v2(void)
{
	m_pFile = NULL;
	m_dwHeadSize = 0;
	m_pID3v2Content = NULL;
	m_strErrMsg = _T("");
}

CTagID3v2::~CTagID3v2(void)
{
//	if (m_pID3v2Content) delete m_pID3v2Content;
	ReleaseOtherFrames();
	ReleaseMainFrames();
}

void CTagID3v2::ReleaseID3Strings()
{
	m_cID3Content.bUnicode = TRUE;
	m_cID3Content.nID3Version = 0;
	m_cID3Content.strTitle = _T("");
	m_cID3Content.strArtist = _T("");
	m_cID3Content.strAlbum = _T("");
	m_cID3Content.strYear = _T("");
	m_cID3Content.strGenre = _T("");
	m_cID3Content.strComment = _T("");
	m_cID3Content.strLyric = _T("");
	m_cID3Content.strComposer = _T("");
	m_cID3Content.strTrack = _T("");
	m_cID3Content.strCopyright = _T("");
	m_cID3Content.strOriArtist = _T("");
	m_cID3Content.strEncoder = _T("");
	m_cID3Content.strEncodedBy = _T("");
	m_cID3Content.strFileType = _T("");
	m_cID3Content.strMediaType = _T("");
	m_cID3Content.strURL = _T("");
	m_cID3Content.strLyricText = _T("");
}

BOOL CTagID3v2::LoadID3v2()
{
	ReleaseOtherFrames();
	ReleaseID3Strings();
	if (!m_pFile){
		m_strErrMsg = _T("ファイルを用意しませんでした。");
		return FALSE;
	}
	m_pFile->SeekToBegin();
	unsigned char m_szID3v2Flag[10];
	if (m_pFile->Read(m_szID3v2Flag, 10) < 10){
		m_strErrMsg = _T("ヘッダー情報のサイズが足りません。");
		return FALSE;
	}
	if (!((m_szID3v2Flag[0] == 0x49) &&
		(m_szID3v2Flag[1] == 0x44) &&
		(m_szID3v2Flag[2] == 0x33) &&
		(m_szID3v2Flag[3] < 0xFF) &&
		(m_szID3v2Flag[4] < 0xFF) &&
		((m_szID3v2Flag[5] == 0x00) ||
		(m_szID3v2Flag[5] == 0x80) ||
		(m_szID3v2Flag[5] == 0xC0)) &&
		(m_szID3v2Flag[6] < 0x80) &&
		(m_szID3v2Flag[7] < 0x80) &&
		(m_szID3v2Flag[8] < 0x80) &&
		(m_szID3v2Flag[9] < 0x80))){
		m_strErrMsg = _T("ID3v2タグではありません。");
		return FALSE;
	}
	m_dwHeadSize = ((DWORD)m_szID3v2Flag[6] << 21) | ((DWORD)m_szID3v2Flag[7] << 14) | ((DWORD)m_szID3v2Flag[8] << 7) | (DWORD)m_szID3v2Flag[9];
	m_pID3v2Content = new unsigned char[m_dwHeadSize];
	if (m_pFile->Read(m_pID3v2Content, m_dwHeadSize) < m_dwHeadSize){
		m_strErrMsg = _T("情報内容のサイズが足りません。");
		return FALSE;
	}
	//非同期化の解除
	if(m_szID3v2Flag[5] & 0x80){
		m_dwHeadSize = DecodeUnSynchronization(m_pID3v2Content, m_dwHeadSize);
	}
	//拡張ヘッダを読み飛ばす
	DWORD dwRemainSize = m_dwHeadSize;
	if(m_szID3v2Flag[5] & 0x40)
	{
		dwRemainSize -= (((DWORD)m_pID3v2Content[0] << 21) | ((DWORD)m_pID3v2Content[1] << 14) | ((DWORD)m_pID3v2Content[2] << 7) | (DWORD)m_pID3v2Content[3]) + 4;
	}
	m_szID3v2Flag[5] &= ~0x40;	//解除

	//フレームを読み込み
	while (dwRemainSize){
		DWORD dwReadSize;
		if (m_szID3v2Flag[3] < 0x03){
			dwReadSize = LoadFrame2_2(m_pID3v2Content + m_dwHeadSize - dwRemainSize, dwRemainSize);
		}else{
			dwReadSize = LoadFrame(m_pID3v2Content + m_dwHeadSize - dwRemainSize, dwRemainSize);
		}
		if (!dwReadSize) break;
		dwRemainSize -= dwReadSize;
	}
	if (m_szID3v2Flag[3] < 0x03){
		m_cID3Content.nID3Version = 0;
	}else if (m_szID3v2Flag[3] == 0x03){
		m_cID3Content.nID3Version = 1;
	}else if (m_szID3v2Flag[3] < 0x04){
		m_cID3Content.nID3Version = 2;
	}
	delete m_pID3v2Content;
	return TRUE;
}

DWORD CTagID3v2::DecodeUnSynchronization(unsigned char *pdata,DWORD dwSize)
{
	DWORD dwDecodeSize = 0;
	unsigned char *writePtr = pdata;
	BOOL bHitFF = FALSE;

	for (DWORD i = 0; i < dwSize; i ++)
	{
		if (pdata[i] == 0xff)
		{
			bHitFF = TRUE;
		}
		else
		{
			if (bHitFF && (pdata[i] == 0x00))
			{
				bHitFF = FALSE;
				continue;
			}
			bHitFF = FALSE;
		}
		writePtr[dwDecodeSize] = pdata[i];
		dwDecodeSize++;
	}
	return dwDecodeSize;
}

DWORD CTagID3v2::EncodeUnSynchronization(unsigned char *srcData,DWORD dwSize,unsigned char *dstData)
{
	DWORD dwDecodeSize = 0;
	unsigned char *writePtr = dstData;
	BOOL bHitFF = FALSE;

	for(DWORD i=0; i<dwSize; i++)
	{
		if(bHitFF && (((srcData[i]&0xe0) == 0xe0) || (srcData[i] == 0x00)) )
		{
			writePtr[dwDecodeSize] = 0x00;
			dwDecodeSize++;
		}
		if(srcData[i] == 0xff)
		{
			bHitFF = TRUE;
		}
		else
		{
			bHitFF = FALSE;
		}
		writePtr[dwDecodeSize] = srcData[i];
		dwDecodeSize++;
	}
	return dwDecodeSize;
}

DWORD CTagID3v2::LoadFrame2_2(unsigned char *pData, DWORD dwSize)
{
	if (dwSize < 6)
	{
		return 0;	//フレームヘッダがない場合は終了
	}
	DWORD size = (((DWORD)0 << 24) | ((DWORD)pData[3] << 16) | ((DWORD)pData[4] << 8) | (DWORD)pData[5]);
	if ((size + 6) > dwSize)
	{
		return 0;	//ヘッダサイズが入力データを超過している
	}
	if (size == 0){

		return 0;
	}
	char id[3+1];
	memcpy(&id, pData, sizeof(id));
	id[3] = '\0';
	WCHAR *pwszContent = NULL;
	//文字列の場合
	if (memcmp(id, "T", sizeof(char)) == 0){
		if (pData[6] == 0x00){
			m_cID3Content.bUnicode = FALSE;
			char *pszContent = new char[size];
			pwszContent = new WCHAR[size];
			memcpy(pszContent, &pData[7], size - 1);
			pszContent[size - 1] = 0x00;
			MultiByteToWideChar(CP_ACP, 0, pszContent, size, pwszContent, size);
			delete pszContent;
		}else if(pData[6] == 0x01){
			m_cID3Content.bUnicode = TRUE;
			if (size >= 3){
				pwszContent = new WCHAR[(size - 3) / 2 + 1];
				memcpy(pwszContent, &pData[9], (size - 3) / 2 * sizeof(WCHAR));
				pwszContent[(size - 3) / 2] = 0x0000;
			}else{
				pwszContent = new WCHAR[1];
				pwszContent[0] = 0x0000;
			}
		}else{
			return size + 6;
		}
		if (memcmp(id, "TT2", sizeof(id)) == 0){
			m_cID3Content.strTitle = pwszContent;
		}else if (memcmp(id, "TP1", sizeof(id)) == 0){
			m_cID3Content.strArtist = pwszContent;
		}else if (memcmp(id, "TAL", sizeof(id)) == 0){
			m_cID3Content.strAlbum = pwszContent;
		}else if (memcmp(id, "TRK", sizeof(id)) == 0){
			m_cID3Content.strTrack = pwszContent;
		}else if (memcmp(id, "TYE", sizeof(id)) == 0){
			m_cID3Content.strYear = pwszContent;
		}else if (memcmp(id, "TCO", sizeof(id)) == 0){
			m_cID3Content.strGenre = pwszContent;
		}else if (memcmp(id, "TCM", sizeof(id)) == 0){
			m_cID3Content.strComposer = pwszContent;
		}else if (memcmp(id, "TXT", sizeof(id)) == 0){
			m_cID3Content.strLyric = pwszContent;
		}else if (memcmp(id, "TOA", sizeof(id)) == 0){
			m_cID3Content.strOriArtist = pwszContent;
		}else if (memcmp(id, "TCR", sizeof(id)) == 0){
			m_cID3Content.strCopyright = pwszContent;
		}else if (memcmp(id, "TSS", sizeof(id)) == 0){
			m_cID3Content.strEncoder = pwszContent;
		}else if (memcmp(id, "TEN", sizeof(id)) == 0){
			m_cID3Content.strEncodedBy = pwszContent;
		}else if (memcmp(id, "TFT", sizeof(id)) == 0){
			m_cID3Content.strFileType = pwszContent;
		}else if (memcmp(id, "TMT", sizeof(id)) == 0){
			m_cID3Content.strMediaType = pwszContent;
		}
	//コメントと歌詞の場合COM,ULT
	}else if ((memcmp(id, "COM", sizeof(id)) == 0) || (memcmp(id, "ULT", sizeof(id)) == 0)){
		if (pData[6] == 0x00){
			m_cID3Content.bUnicode = FALSE;
			char *pszContent = new char[size - 4 + 1];
			char *pszCompare = new char[size - 4 + 1];
			memcpy(pszCompare, &pData[10], size - 4);
			pszCompare[size - 4] = 0x00;
			if (size > 5){
				DWORD i;
				for (i = 0; i < (size - 4 + 1); i ++){
					if (pszCompare[i] == 0x00){
						i ++;
						for (DWORD j = 0; j < (size - 4 - i + 1); j ++){
							pszContent[j] = pszCompare[i + j];
						}
						break;
					}
				}
				pwszContent = new WCHAR[size - 4 - i + 1];
				MultiByteToWideChar(CP_ACP, 0, pszContent, size - 4 - i + 1, pwszContent, size - 4 - i + 1);
			}else{
				pwszContent = new WCHAR[1];
				pwszContent[0] = 0x0000;
			}
			delete pszContent;
			delete pszCompare;
		}else if (pData[6] == 0x01){
			m_cID3Content.bUnicode = TRUE;
			pwszContent = new WCHAR[(size - 4) / 2 + 1];
			WCHAR* pwszCompare = new WCHAR[(size - 4) / 2 + 1];
			memcpy(pwszCompare, &pData[10], (size - 4) / 2 * sizeof(WCHAR));
			pwszCompare[(size - 4) / 2] = 0x0000;
			if (size > 12){
				DWORD i;
				for (i = 0; i < ((size - 4) / 2 + 1); i ++){
					if (pwszCompare[i] == 0x0000){
						i += 2;
						for (DWORD j = 0; j < (size - 4) / 2 - i + 1; j ++){
							pwszContent[j] = pwszCompare[i + j];
						}
						break;
					}
				}
			}else{
				pwszContent[0] = 0x0000;
			}
			delete pwszCompare;
			
		}else{
			return size + 6;
		}
		if (memcmp(id, "COM", sizeof(id)) == 0){
			m_cID3Content.strComment = pwszContent;
		}else{
			m_cID3Content.strLyricText = pwszContent;
		}

	//URLの場合
	}else if (memcmp(id, "WXX", sizeof(id)) == 0){
		if (pData[6] == 0x00){
			m_cID3Content.bUnicode = FALSE;
			char *pszContent = new char[size - 1 + 1];
			char *pszCompare = new char[size - 1 + 1];
			memcpy(pszCompare, &pData[7], size - 1);
			pszCompare[size - 1] = 0x00;
			DWORD i;
			if (size > 2){
				for (i = 0; i < (size - 1 + 1); i ++){
					if (pszCompare[i] == 0x00){
						i ++;
						for (DWORD j = 0; j < (size - 1 - i + 1); j ++){
							pszContent[j] = pszCompare[i + j];
						}
						break;
					}
				}
				pwszContent = new WCHAR[size - 1 - i];
				MultiByteToWideChar(CP_ACP, 0, pszContent, size - 1 - i + 1, pwszContent, size - 1 - i + 1);
			}else{
				pwszContent = new WCHAR[1];
				pwszContent[0] = 0x0000;
			}
			delete pszContent;
			delete pszCompare;
		}else if (pData[6] == 0x01){
			m_cID3Content.bUnicode = TRUE;
			pwszContent = new WCHAR[(size - 1) / 2 + 1];
			WCHAR* pwszCompare = new WCHAR[(size - 1) / 2 + 1];
			memcpy(pwszCompare, &pData[7], (size - 1) / 2 * sizeof(WCHAR));
			pwszCompare[(size - 1) / 2] = 0x0000;
			if (size > 9){
				DWORD i;
				for (i = 0; i < ((size - 1) / 2 + 1); i ++){
					if (pwszCompare[i] == 0x0000){
						i += 2;
						for (DWORD j = 0; j < (size - 1) / 2 - i + 1; j ++){
							pwszContent[j] = pwszCompare[i + j];
						}
						break;
					}
				}
			}else{
				pwszContent[0] = 0x0000; 
			}
			delete pwszCompare;
		}else{
			return size + 6;
		}
		m_cID3Content.strURL = pwszContent;
	}else{
		//その他フレーム保存
		ID3FRAME cFrame;
		cFrame.size = size + 6;
		cFrame.pszData = new unsigned char[size + 6];
		memcpy(cFrame.pszData, &pData[0], size + 6);
		m_cOtherFrames.Add(cFrame);
		return size + 6;
	}

	if (pwszContent) delete pwszContent;
	return size + 6;
/*
	TIT2（タイトル）TT2
	TPE1（アーティスト）TP1
	TALB（アルバム）TAL
	TYER（年）TYE
	TRCK（トラック）TRK
	TCON（タイプ）TCO
	TCOM（作曲）TCM
	TEXT（作詞）TXT
	TOPE（オリジナルアーティスト）TOA
	TCOP（Copyright Message）TCR
	TSSE（エンコーダ）TSS
	TENC（エンコードした人）TEN
	TFLT（ファイルタイプ）TFT
	TMED（メディアタイプ）TMT
	COMM（コメント）COM
	WXXX（URL）WXX
	USLT（歌詞）ULT
	*/
}

DWORD CTagID3v2::LoadFrame(unsigned char *pData, DWORD dwSize)
{
	if (dwSize < 10)
	{
		return 0;	//フレームヘッダがない場合は終了
	}
	DWORD size = (((DWORD)pData[4] << 24) | ((DWORD)pData[5] << 16) | ((DWORD)pData[6] << 8) | (DWORD)pData[7]);
	if ((size + 10) > dwSize)
	{
		return 0;	//ヘッダサイズが入力データを超過している
	}
	if (size == 0){
		return 0;
	}
	char id[4+1];
	memcpy(&id, pData, sizeof(id));
	id[4] = '\0';
	WCHAR *pwszContent = NULL;
	//文字列の場合
	if (memcmp(id, "T", sizeof(char)) == 0){
		if (pData[10] == 0x00){
			m_cID3Content.bUnicode = FALSE;
			char *pszContent = new char[size];
			pwszContent = new WCHAR[size];
			memcpy(pszContent, &pData[11], size - 1);
			pszContent[size - 1] = 0x00;
			MultiByteToWideChar(CP_ACP, 0, pszContent, size, pwszContent, size);
			delete pszContent;
		}else if(pData[10] == 0x01){
			m_cID3Content.bUnicode = TRUE;
			if (size >= 3){
				pwszContent = new WCHAR[(size - 3) / 2 + 1];
				memcpy(pwszContent, &pData[13], (size - 3) / 2 * sizeof(WCHAR));
				pwszContent[(size - 3) / 2] = 0x0000;
			}else{
				pwszContent = new WCHAR[1];
				pwszContent[0] = 0x0000;
			}
			
		}else{
			return size + 10;
		}
		if (memcmp(id, "TIT2", sizeof(id)) == 0){
			m_cID3Content.strTitle = pwszContent;
		}else if (memcmp(id, "TPE1", sizeof(id)) == 0){
			m_cID3Content.strArtist = pwszContent;
		}else if (memcmp(id, "TALB", sizeof(id)) == 0){
			m_cID3Content.strAlbum = pwszContent;
		}else if (memcmp(id, "TRCK", sizeof(id)) == 0){
			m_cID3Content.strTrack = pwszContent;
		}else if (memcmp(id, "TYER", sizeof(id)) == 0){
			m_cID3Content.strYear = pwszContent;
		}else if (memcmp(id, "TCON", sizeof(id)) == 0){
			m_cID3Content.strGenre = pwszContent;
		}else if (memcmp(id, "TCOM", sizeof(id)) == 0){
			m_cID3Content.strComposer = pwszContent;
		}else if (memcmp(id, "TEXT", sizeof(id)) == 0){
			m_cID3Content.strLyric = pwszContent;
		}else if (memcmp(id, "TOPE", sizeof(id)) == 0){
			m_cID3Content.strOriArtist = pwszContent;
		}else if (memcmp(id, "TCOP", sizeof(id)) == 0){
			m_cID3Content.strCopyright = pwszContent;
		}else if (memcmp(id, "TSSE", sizeof(id)) == 0){
			m_cID3Content.strEncoder = pwszContent;
		}else if (memcmp(id, "TENC", sizeof(id)) == 0){
			m_cID3Content.strEncodedBy = pwszContent;
		}else if (memcmp(id, "TFLT", sizeof(id)) == 0){
			m_cID3Content.strFileType = pwszContent;
		}else if (memcmp(id, "TMED", sizeof(id)) == 0){
			m_cID3Content.strMediaType = pwszContent;
		}
	//コメントと歌詞の場合COM,ULT
	}else if ((memcmp(id, "COMM", sizeof(id)) == 0) || (memcmp(id, "USLT", sizeof(id)) == 0)){
		if (pData[10] == 0x00){
			m_cID3Content.bUnicode = FALSE;
			char *pszContent = new char[size - 4 + 1];
			char *pszCompare = new char[size - 4 + 1];
			memcpy(pszCompare, &pData[14], size - 4);
			pszCompare[size - 4] = 0x00;
			if (size > 5){
				DWORD i;
				for (i = 0; i < (size - 4 + 1); i ++){
					if (pszCompare[i] == 0x00){
						i ++;
						for (DWORD j = 0; j < (size - 4 - i + 1); j ++){
							pszContent[j] = pszCompare[i + j];
						}
						break;
					}
				}
				pwszContent = new WCHAR[size - 4 - i + 1];
				MultiByteToWideChar(CP_ACP, 0, pszContent, size - 4 - i + 1, pwszContent, size - 4 - i + 1);
			}else{
				pwszContent = new WCHAR[1];
				pwszContent[0] = 0x0000;
			}
			delete pszContent;
			delete pszCompare;
		}else if (pData[10] == 0x01){
			m_cID3Content.bUnicode = TRUE;
			pwszContent = new WCHAR[(size - 4) / 2 + 1];
			WCHAR* pwszCompare = new WCHAR[(size - 4) / 2 + 1];
			memcpy(pwszCompare, &pData[14], (size - 4) / 2 * sizeof(WCHAR));
			pwszCompare[(size - 4) / 2] = 0x0000;
			if (size > 12){
				DWORD i;
				for (i = 0; i < ((size - 4) / 2 + 1); i ++){
					if (pwszCompare[i] == 0x0000){
						i += 2;
						for (DWORD j = 0; j < (size - 4) / 2 - i + 1; j ++){
							pwszContent[j] = pwszCompare[i + j];
						}
						break;
					}
				}
			}else{
				pwszContent[0] = 0x0000;
			}
			delete pwszCompare;
		}else{
			return size + 10;
		}
		if (memcmp(id, "COMM", sizeof(id)) == 0){
			m_cID3Content.strComment = pwszContent;
		}else{
			m_cID3Content.strLyricText = pwszContent;
		}

	//URLの場合
	}else if (memcmp(id, "WXXX", sizeof(id)) == 0){
		if (pData[10] == 0x00){
			m_cID3Content.bUnicode = FALSE;
			char *pszContent = new char[size - 1 + 1];
			char *pszCompare = new char[size - 1 + 1];
			memcpy(pszCompare, &pData[11], size - 1);
			pszCompare[size - 1] = 0x00;
			if (size > 2){
				DWORD i;
				for (i = 0; i < (size - 1 + 1); i ++){
					if (pszCompare[i] == 0x00){
						i ++;
						for (DWORD j = 0; j < (size - 1 - i + 1); j ++){
							pszContent[j] = pszCompare[i + j];
						}
						break;
					}
				}
				pwszContent = new WCHAR[size - 1 - i + 1];
				MultiByteToWideChar(CP_ACP, 0, pszContent, size - 1 - i + 1, pwszContent, size - 1 - i + 1);
			}else{
				pwszContent = new WCHAR[1];
				pwszContent[0] = 0x0000;
			}
			delete pszContent;
			delete pszCompare;
		}else if (pData[10] == 0x01){
			m_cID3Content.bUnicode = TRUE;
			pwszContent = new WCHAR[(size - 1) / 2 + 1];
			WCHAR* pwszCompare = new WCHAR[(size - 1) / 2 + 1];
			memcpy(pwszCompare, &pData[11], (size - 1) / 2 * sizeof(WCHAR));
			pwszCompare[(size - 1) / 2] = 0x0000;
			if (size > 9){
				DWORD i;
				for (i = 0; i < ((size - 1) / 2 + 1); i ++){
					if (pwszCompare[i] == 0x0000){
						i += 2;
						for (DWORD j = 0; j < (size - 1) / 2 - i + 1; j ++){
							pwszContent[j] = pwszCompare[i + j];
						}
						break;
					}
				}
			}else{
				pwszContent[0] = 0x0000;
			}
			delete pwszCompare;
		}else{
			return size + 10;
		}
		m_cID3Content.strURL = pwszContent;
	}else{
		//その他フレーム保存
		ID3FRAME cFrame;
		cFrame.size = size + 10;
		cFrame.pszData = new unsigned char[size + 10];
		memcpy(cFrame.pszData, &pData[0], size + 10);
		m_cOtherFrames.Add(cFrame);
		return size + 10;
	}
	if (pwszContent){
		delete pwszContent;
	}
	return size + 10;
/*
	TIT2（タイトル）TT2
	TPE1（アーティスト）TP1
	TALB（アルバム）TAL
	TYER（年）TYE
	TRCK（トラック）TRK
	TCON（タイプ）TCO
	TCOM（作曲）TCM
	TEXT（作詞）TXT
	TOPE（オリジナルアーティスト）TOA
	TCOP（Copyright Message）TCR
	TSSE（エンコーダ）TSS
	TENC（エンコードした人）TEN
	TFLT（ファイルタイプ）TFT
	TMED（メディアタイプ）TMT
	COMM（コメント）COM
	WXXX（URL）WXX
	*/
}

void CTagID3v2::ReleaseOtherFrames()
{
	for (int i = 0; i < m_cOtherFrames.GetSize(); i ++){
		delete m_cOtherFrames[i].pszData;
	}
	m_cOtherFrames.RemoveAll();
}

void CTagID3v2::ReleaseMainFrames()
{
	for (int i = 0; i < m_cMainFrames.GetSize(); i ++){
		delete m_cMainFrames[i].pszData;
	}
	m_cMainFrames.RemoveAll();
}

BOOL CTagID3v2::SaveID3Tag(CString strFileName)
{
	CFile file;
	CFileException fe;
	if (!file.Open(strFileName, CFile::modeRead, &fe)){
		fe.ReportError();
		return FALSE;
	}else{
		file.SeekToBegin();
		unsigned char m_szID3v2Flag[10];
		if (file.Read(m_szID3v2Flag, 10) < 10){
			m_strErrMsg = _T("ヘッダー情報のサイズが足りません。");
			return FALSE;
		}
		DWORD dwContentLength;
		unsigned char* pszSavedContent;
		unsigned char* pszFullContent;
		if (!((m_szID3v2Flag[0] == 0x49) &&
			(m_szID3v2Flag[1] == 0x44) &&
			(m_szID3v2Flag[2] == 0x33) &&
			(m_szID3v2Flag[3] < 0xFF) &&
			(m_szID3v2Flag[4] < 0xFF) &&
			((m_szID3v2Flag[5] == 0x00) ||
			(m_szID3v2Flag[5] == 0x80) ||
			(m_szID3v2Flag[5] == 0xC0)) &&
			(m_szID3v2Flag[6] < 0x80) &&
			(m_szID3v2Flag[7] < 0x80) &&
			(m_szID3v2Flag[8] < 0x80) &&
			(m_szID3v2Flag[9] < 0x80))){
			dwContentLength = file.GetLength();
			pszSavedContent = new unsigned char[dwContentLength];
			file.SeekToBegin();
			file.Read(pszSavedContent, dwContentLength);
			ZeroMemory(m_szID3v2Flag, 10);
		}else{
			m_dwHeadSize = ((DWORD)m_szID3v2Flag[6] << 21) | ((DWORD)m_szID3v2Flag[7] << 14) | ((DWORD)m_szID3v2Flag[8] << 7) | (DWORD)m_szID3v2Flag[9];
			dwContentLength = file.GetLength() - m_dwHeadSize - 10;
			pszSavedContent = new unsigned char[dwContentLength];
			file.Seek(m_dwHeadSize, CFile::current);
			file.Read(pszSavedContent, dwContentLength);
		}
		DWORD dwFullSize;
		if (m_bEnable){
			//ID3v2Tag再構成
			if (m_cID3Content.nID3Version == 0){
				StrToFrame2_2();
			}else{
				StrToFrame();
			}
			DWORD dwTotalFrameSize = GetTotalFrameSize();
			DWORD dwResultFrameSize = 0;
			//フレームバッファ作成
			unsigned char* pszFrameContent;
			unsigned char* pszFrameContentOri;
			DWORD dwPointer = 0;
			pszFrameContentOri = new unsigned char[dwTotalFrameSize];
			for (int i = 0; i < m_cMainFrames.GetSize(); i ++){
				memcpy(pszFrameContentOri + dwPointer, m_cMainFrames[i].pszData, m_cMainFrames[i].size);
				dwPointer += m_cMainFrames[i].size;
			}
			for (int i = 0; i < m_cOtherFrames.GetSize(); i ++){
				memcpy(pszFrameContentOri + dwPointer, m_cOtherFrames[i].pszData, m_cOtherFrames[i].size);
				dwPointer += m_cOtherFrames[i].size;
			}
			if ((m_szID3v2Flag[5] & 0x80) == 0x80){
				pszFrameContent = new unsigned char[dwTotalFrameSize * 2];
				dwResultFrameSize = EncodeUnSynchronization(pszFrameContentOri, dwTotalFrameSize, pszFrameContent);
				delete pszFrameContentOri;
			}else{
				pszFrameContent = new unsigned char[dwTotalFrameSize];
				memcpy(pszFrameContent, pszFrameContentOri, dwTotalFrameSize);
				delete pszFrameContentOri;
				dwResultFrameSize = dwTotalFrameSize;
			}

			//ヘッダー作成
			pszFullContent = new unsigned char[dwResultFrameSize + dwContentLength + 10];
			memcpy(pszFullContent, "ID3", 3);
			if (m_cID3Content.nID3Version == 0){
				pszFullContent[3] = 0x02;
			}else if (m_cID3Content.nID3Version == 1){
				pszFullContent[3] = 0x03;
			}else if (m_cID3Content.nID3Version == 2){
				pszFullContent[3] = 0x04;
			}
			pszFullContent[4] = 0x00;
			pszFullContent[5] = m_szID3v2Flag[5] & 0x80;
			pszFullContent[6] = (dwResultFrameSize >> 21) & 0x7F;
			pszFullContent[7] = (dwResultFrameSize >> 14) & 0x7F;
			pszFullContent[8] = (dwResultFrameSize >> 7) & 0x7F;
			pszFullContent[9] = dwResultFrameSize & 0x7F;
			//フレーム書き込み
			memcpy(&pszFullContent[10], pszFrameContent, dwResultFrameSize);
			//raw書き込み
			memcpy(&pszFullContent[dwResultFrameSize + 10], pszSavedContent, dwContentLength);
			//要らないバッファ削除
			delete pszFrameContent;
			delete pszSavedContent;
			dwFullSize = dwResultFrameSize + dwContentLength + 10;
		}else{
			pszFullContent = new unsigned char[dwContentLength];
			memcpy(pszFullContent, pszSavedContent, dwContentLength);
			delete pszSavedContent;
			dwFullSize = dwContentLength;
		}
		file.Close();
		//テンポラリ保存
		int index;
		index = strFileName.ReverseFind(_T('.'));
		CString strTempName;
		strTempName.Format(_T("%s.tmp"), strFileName.Left(index));
		CFile tempFile;
		if (!tempFile.Open(strTempName, CFile::modeCreate | CFile::modeWrite, &fe)){ 
			fe.ReportError();
			delete pszFullContent;
			return FALSE;
		}
		tempFile.SeekToBegin();
		tempFile.Write(pszFullContent, dwFullSize);
		delete pszFullContent;
		tempFile.Close();
		//オリジナルファイルをリネーム
		DeleteFile(strFileName);
		MoveFile(strTempName, strFileName);
	}
	return TRUE;
	
}

DWORD CTagID3v2::StrToFrame2_2()
{
	ReleaseMainFrames();
	int i;
	ID3FRAME cFrame;
	if (m_cID3Content.bUnicode){
		//Unicode
		UINT nLength;
		//文字列の場合
		for (i = 0; i < 14; i ++){
			switch(i){
				case 0:
					nLength = m_cID3Content.strTitle.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strTitle.GetString(), cFrame.size - 9);
					break;
				case 1:
					nLength = m_cID3Content.strArtist.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strArtist.GetString(), cFrame.size - 9);
					break;
				case 2:
					nLength = m_cID3Content.strAlbum.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strAlbum.GetString(), cFrame.size - 9);
					break;
				case 3:
					nLength = m_cID3Content.strYear.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strYear.GetString(), cFrame.size - 9);
					break;
				case 4:
					nLength = m_cID3Content.strTrack.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strTrack.GetString(), cFrame.size - 9);
					break;
				case 5:
					nLength = m_cID3Content.strGenre.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strGenre.GetString(), cFrame.size - 9);
					break;
				case 6:
					nLength = m_cID3Content.strComposer.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strComposer.GetString(), cFrame.size - 9);
					break;
				case 7:
					nLength = m_cID3Content.strLyric.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strLyric.GetString(), cFrame.size - 9);
					break;
				case 8:
					nLength = m_cID3Content.strOriArtist.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strOriArtist.GetString(), cFrame.size - 9);
					break;
				case 9:
					nLength = m_cID3Content.strCopyright.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strCopyright.GetString(), cFrame.size - 9);
					break;
				case 10:
					nLength = m_cID3Content.strEncoder.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strEncoder.GetString(), cFrame.size - 9);
					break;
				case 11:
					nLength = m_cID3Content.strEncodedBy.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strEncodedBy.GetString(), cFrame.size - 9);
					break;
				case 12:
					nLength = m_cID3Content.strFileType.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strFileType.GetString(), cFrame.size - 9);
					break;
				case 13:
					nLength = m_cID3Content.strMediaType.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 9;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[9], m_cID3Content.strMediaType.GetString(), cFrame.size - 9);
					break;
			}	
			memcpy(cFrame.pszData, wszID2_2[i].GetString(), 3);
			cFrame.pszData[3] = ((cFrame.size - 6) >> 16) & 0xFF;
			cFrame.pszData[4] = ((cFrame.size - 6) >> 8) & 0xFF;
			cFrame.pszData[5] = (cFrame.size - 6) & 0xFF;
			cFrame.pszData[6] = 0x01;
			cFrame.pszData[7] = 0xFF;
			cFrame.pszData[8] = 0xFE;
			m_cMainFrames.Add(cFrame);
		}
		//コメント&歌詞
		for (i = 14; i < 16; i ++){
			switch(i){
				case 14:
					nLength = m_cID3Content.strComment.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 16;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[16], m_cID3Content.strComment.GetString(), cFrame.size - 16);
					break;
				case 15:
					nLength = m_cID3Content.strLyricText.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 16;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[16], m_cID3Content.strLyricText.GetString(), cFrame.size - 16);
					break;
			}
			memcpy(cFrame.pszData, wszID2_2[i].GetString(), 3);
			cFrame.pszData[3] = ((cFrame.size - 6) >> 16) & 0xFF;
			cFrame.pszData[4] = ((cFrame.size - 6) >> 8) & 0xFF;
			cFrame.pszData[5] = (cFrame.size - 6) & 0xFF;
			cFrame.pszData[6] = 0x01;
			memcpy(&cFrame.pszData[7], "eng", 3);
			cFrame.pszData[10] = 0xFF;
			cFrame.pszData[11] = 0xFE;
			cFrame.pszData[12] = 0x00;
			cFrame.pszData[13] = 0x00;
			cFrame.pszData[14] = 0xFF;
			cFrame.pszData[15] = 0xFE;
			m_cMainFrames.Add(cFrame);
		}
		//URL
		nLength = m_cID3Content.strURL.GetLength() + 1;
		cFrame.size = nLength * sizeof(WCHAR) + 13;
		cFrame.pszData = new unsigned char[cFrame.size];
		memcpy(&cFrame.pszData[13], m_cID3Content.strURL.GetString(), cFrame.size - 13);
		memcpy(cFrame.pszData, wszID2_2[16].GetString(), 3);
		cFrame.pszData[3] = ((cFrame.size - 6) >> 16) & 0xFF;
		cFrame.pszData[4] = ((cFrame.size - 6) >> 8) & 0xFF;
		cFrame.pszData[5] = (cFrame.size - 6) & 0xFF;
		cFrame.pszData[6] = 0x01;
		cFrame.pszData[7] = 0xFF;
		cFrame.pszData[8] = 0xFE;
		cFrame.pszData[9] = 0x00;
		cFrame.pszData[10] = 0x00;
		cFrame.pszData[11] = 0xFF;
		cFrame.pszData[12] = 0xFE;
		m_cMainFrames.Add(cFrame);
	}else{
		//ANSI
		//文字列の場合
		BOOL bOK, bOKAll;
		bOKAll = FALSE;
		WCHAR* pwszString;
		char* pszString;
		UINT nLength;
		for (i = 0; i < 14; i ++){
			switch(i){
				case 0:
					nLength = m_cID3Content.strTitle.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strTitle.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 1:
					nLength = m_cID3Content.strArtist.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strArtist.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 2:
					nLength = m_cID3Content.strAlbum.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strAlbum.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 3:
					nLength = m_cID3Content.strYear.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strYear.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 4:
					nLength = m_cID3Content.strTrack.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strTrack.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 5:
					nLength = m_cID3Content.strGenre.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strGenre.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 6:
					nLength = m_cID3Content.strComposer.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strComposer.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 7:
					nLength = m_cID3Content.strLyric.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strLyric.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 8:
					nLength = m_cID3Content.strOriArtist.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strOriArtist.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 9:
					nLength = m_cID3Content.strCopyright.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strCopyright.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 10:
					nLength = m_cID3Content.strEncoder.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strEncoder.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 11:
					nLength = m_cID3Content.strEncodedBy.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strEncodedBy.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 12:
					nLength = m_cID3Content.strFileType.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strFileType.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
				case 13:
					nLength = m_cID3Content.strMediaType.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strMediaType.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 7;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[7], pszString, cFrame.size - 7);
					break;
			}
			memcpy(cFrame.pszData, wszID2_2[i].GetString(), 3);
			cFrame.pszData[3] = ((cFrame.size - 6) >> 16) & 0xFF;
			cFrame.pszData[4] = ((cFrame.size - 6) >> 8) & 0xFF;
			cFrame.pszData[5] = (cFrame.size - 6) & 0xFF;
			cFrame.pszData[6] = 0x00;
			m_cMainFrames.Add(cFrame);
		}
		//コメント&歌詞
		for (i = 14; i < 16; i ++){
			switch(i){
				case 14:
					nLength = m_cID3Content.strComment.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strComment.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 15:
					nLength = m_cID3Content.strLyricText.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strLyricText.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
			}
			memcpy(cFrame.pszData, wszID2_2[i].GetString(), 3);
			cFrame.pszData[3] = ((cFrame.size - 6) >> 16) & 0xFF;
			cFrame.pszData[4] = ((cFrame.size - 6) >> 8) & 0xFF;
			cFrame.pszData[5] = (cFrame.size - 6) & 0xFF;
			cFrame.pszData[6] = 0x00;
			memcpy(&cFrame.pszData[7], "eng", 3);
			cFrame.pszData[10] = 0x00;
			m_cMainFrames.Add(cFrame);
		}
		//URL
		nLength = m_cID3Content.strComment.GetLength() + 1;
		pwszString = new WCHAR[nLength];
		memcpy(pwszString, m_cID3Content.strComment.GetString(), nLength * sizeof(WCHAR));
		pszString = new char[nLength * sizeof(WCHAR)];
		cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 8;
		cFrame.pszData = new unsigned char[cFrame.size];
		memcpy(&cFrame.pszData[8], pszString, cFrame.size - 8);
		memcpy(cFrame.pszData, wszID2_2[16].GetString(), 3);
		cFrame.pszData[3] = ((cFrame.size - 6) >> 16) & 0xFF;
		cFrame.pszData[4] = ((cFrame.size - 6) >> 8) & 0xFF;
		cFrame.pszData[5] = (cFrame.size - 6) & 0xFF;
		cFrame.pszData[6] = 0x00;
		cFrame.pszData[7] = 0x00;
		m_cMainFrames.Add(cFrame);

		delete pwszString, pszString;
	}
	return 1;
}

//Version 2.3,2.4
DWORD CTagID3v2::StrToFrame()
{
	ReleaseMainFrames();
	int i;
	ID3FRAME cFrame;
	if (m_cID3Content.bUnicode){
		//Unicode
		UINT nLength;
		//文字列の場合
		for (i = 0; i < 14; i ++){
			switch(i){
				case 0:
					nLength = m_cID3Content.strTitle.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strTitle.GetString(), cFrame.size - 13);
					break;
				case 1:
					nLength = m_cID3Content.strArtist.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strArtist.GetString(), cFrame.size - 13);
					break;
				case 2:
					nLength = m_cID3Content.strAlbum.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strAlbum.GetString(), cFrame.size - 13);
					break;
				case 3:
					nLength = m_cID3Content.strYear.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strYear.GetString(), cFrame.size - 13);
					break;
				case 4:
					nLength = m_cID3Content.strTrack.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strTrack.GetString(), cFrame.size - 13);
					break;
				case 5:
					nLength = m_cID3Content.strGenre.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strGenre.GetString(), cFrame.size - 13);
					break;
				case 6:
					nLength = m_cID3Content.strComposer.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strComposer.GetString(), cFrame.size - 13);
					break;
				case 7:
					nLength = m_cID3Content.strLyric.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strLyric.GetString(), cFrame.size - 13);
					break;
				case 8:
					nLength = m_cID3Content.strOriArtist.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strOriArtist.GetString(), cFrame.size - 13);
					break;
				case 9:
					nLength = m_cID3Content.strCopyright.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strCopyright.GetString(), cFrame.size - 13);
					break;
				case 10:
					nLength = m_cID3Content.strEncoder.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strEncoder.GetString(), cFrame.size - 13);
					break;
				case 11:
					nLength = m_cID3Content.strEncodedBy.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strEncodedBy.GetString(), cFrame.size - 13);
					break;
				case 12:
					nLength = m_cID3Content.strFileType.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strFileType.GetString(), cFrame.size - 13);
					break;
				case 13:
					nLength = m_cID3Content.strMediaType.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 13;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[13], m_cID3Content.strMediaType.GetString(), cFrame.size - 13);
					break;
			}	
			memcpy(cFrame.pszData, wszID[i].GetString(), 4);
			cFrame.pszData[4] = ((cFrame.size - 10) >> 24) & 0xFF;
			cFrame.pszData[5] = ((cFrame.size - 10) >> 16) & 0xFF;
			cFrame.pszData[6] = ((cFrame.size - 10) >> 8) & 0xFF;
			cFrame.pszData[7] = (cFrame.size - 10) & 0xFF;
			cFrame.pszData[8] = 0x00;
			cFrame.pszData[9] = 0x00;
			cFrame.pszData[10] = 0x01;
			cFrame.pszData[11] = 0xFF;
			cFrame.pszData[12] = 0xFE;
			m_cMainFrames.Add(cFrame);
		}
		//コメント&歌詞
		for (i = 14; i < 16; i ++){
			switch(i){
				case 14:
					nLength = m_cID3Content.strComment.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 20;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[20], m_cID3Content.strComment.GetString(), cFrame.size - 20);
					break;
				case 15:
					nLength = m_cID3Content.strLyricText.GetLength() + 1;
					cFrame.size = nLength * sizeof(WCHAR) + 20;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[20], m_cID3Content.strLyricText.GetString(), cFrame.size - 20);
					break;
			}
			memcpy(cFrame.pszData, wszID[i].GetString(), 4);
			cFrame.pszData[4] = ((cFrame.size - 10) >> 24) & 0xFF;
			cFrame.pszData[5] = ((cFrame.size - 10) >> 16) & 0xFF;
			cFrame.pszData[6] = ((cFrame.size - 10) >> 8) & 0xFF;
			cFrame.pszData[7] = (cFrame.size - 10) & 0xFF;
			cFrame.pszData[8] = 0x00;
			cFrame.pszData[9] = 0x00;
			cFrame.pszData[10] = 0x01;
			memcpy(&cFrame.pszData[11], "eng", 3);
			cFrame.pszData[14] = 0xFF;
			cFrame.pszData[15] = 0xFE;
			cFrame.pszData[16] = 0x00;
			cFrame.pszData[17] = 0x00;
			cFrame.pszData[18] = 0xFF;
			cFrame.pszData[19] = 0xFE;
			m_cMainFrames.Add(cFrame);
		}
		//URL
		nLength = m_cID3Content.strURL.GetLength() + 1;
		cFrame.size = nLength * sizeof(WCHAR) + 17;
		cFrame.pszData = new unsigned char[cFrame.size];
		memcpy(&cFrame.pszData[17], m_cID3Content.strURL.GetString(), cFrame.size - 17);
		memcpy(cFrame.pszData, wszID[16].GetString(), 4);
		cFrame.pszData[4] = ((cFrame.size - 10) >> 24) & 0xFF;
		cFrame.pszData[5] = ((cFrame.size - 10) >> 16) & 0xFF;
		cFrame.pszData[6] = ((cFrame.size - 10) >> 8) & 0xFF;
		cFrame.pszData[7] = (cFrame.size - 10) & 0xFF;
		cFrame.pszData[8] = 0x00;
		cFrame.pszData[9] = 0x00;
		cFrame.pszData[10] = 0x01;
		cFrame.pszData[11] = 0xFF;
		cFrame.pszData[12] = 0xFE;
		cFrame.pszData[13] = 0x00;
		cFrame.pszData[14] = 0x00;
		cFrame.pszData[15] = 0xFF;
		cFrame.pszData[16] = 0xFE;
		m_cMainFrames.Add(cFrame);
	}else{
		//ANSI
		//文字列の場合
		BOOL bOK, bOKAll;
		bOKAll = FALSE;
		WCHAR* pwszString;
		char* pszString;
		UINT nLength;
		for (i = 0; i < 14; i ++){
			switch(i){
				case 0:
					nLength = m_cID3Content.strTitle.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strTitle.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 1:
					nLength = m_cID3Content.strArtist.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strArtist.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 2:
					nLength = m_cID3Content.strAlbum.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strAlbum.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 3:
					nLength = m_cID3Content.strYear.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strYear.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 4:
					nLength = m_cID3Content.strTrack.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strTrack.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 5:
					nLength = m_cID3Content.strGenre.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strGenre.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 6:
					nLength = m_cID3Content.strComposer.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strComposer.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 7:
					nLength = m_cID3Content.strLyric.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strLyric.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 8:
					nLength = m_cID3Content.strOriArtist.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strOriArtist.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 9:
					nLength = m_cID3Content.strCopyright.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strCopyright.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 10:
					nLength = m_cID3Content.strEncoder.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strEncoder.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 11:
					nLength = m_cID3Content.strEncodedBy.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strEncodedBy.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 12:
					nLength = m_cID3Content.strFileType.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strFileType.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
				case 13:
					nLength = m_cID3Content.strMediaType.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strMediaType.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 11;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[11], pszString, cFrame.size - 11);
					break;
			}
			memcpy(cFrame.pszData, wszID[i].GetString(), 4);
			cFrame.pszData[4] = ((cFrame.size - 10) >> 24) & 0xFF;
			cFrame.pszData[5] = ((cFrame.size - 10) >> 16) & 0xFF;
			cFrame.pszData[6] = ((cFrame.size - 10) >> 8) & 0xFF;
			cFrame.pszData[7] = (cFrame.size - 10) & 0xFF;
			cFrame.pszData[8] = 0x00;
			cFrame.pszData[9] = 0x00;
			cFrame.pszData[10] = 0x00;
			m_cMainFrames.Add(cFrame);
		}
		//コメント&歌詞
		for (i = 14; i < 16; i ++){
			switch(i){
				case 14:
					nLength = m_cID3Content.strComment.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strComment.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 15;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[15], pszString, cFrame.size - 15);
					break;
				case 15:
					nLength = m_cID3Content.strLyricText.GetLength() + 1;
					pwszString = new WCHAR[nLength];
					memcpy(pwszString, m_cID3Content.strLyricText.GetString(), nLength * sizeof(WCHAR));
					pszString = new char[nLength * sizeof(WCHAR)];
					cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 15;
					cFrame.pszData = new unsigned char[cFrame.size];
					memcpy(&cFrame.pszData[15], pszString, cFrame.size - 15);
					break;
			}
			memcpy(cFrame.pszData, wszID[i].GetString(), 4);
			cFrame.pszData[4] = ((cFrame.size - 10) >> 24) & 0xFF;
			cFrame.pszData[5] = ((cFrame.size - 10) >> 16) & 0xFF;
			cFrame.pszData[6] = ((cFrame.size - 10) >> 8) & 0xFF;
			cFrame.pszData[7] = (cFrame.size - 10) & 0xFF;
			cFrame.pszData[8] = 0x00;
			cFrame.pszData[9] = 0x00;
			cFrame.pszData[10] = 0x00;
			memcpy(&cFrame.pszData[11], "eng", 3);
			cFrame.pszData[14] = 0x00;
			m_cMainFrames.Add(cFrame);
		}
		//URL
		nLength = m_cID3Content.strComment.GetLength() + 1;
		pwszString = new WCHAR[nLength];
		memcpy(pwszString, m_cID3Content.strComment.GetString(), nLength * sizeof(WCHAR));
		pszString = new char[nLength * sizeof(WCHAR)];
		cFrame.size = WideCharToMultiByte(CP_ACP, 0, pwszString, nLength, pszString, nLength * sizeof(WCHAR), "?", &bOK) + 12;
		cFrame.pszData = new unsigned char[cFrame.size];
		memcpy(&cFrame.pszData[12], pszString, cFrame.size - 12);
		memcpy(cFrame.pszData, wszID[i].GetString(), 4);
		cFrame.pszData[4] = ((cFrame.size - 10) >> 24) & 0xFF;
		cFrame.pszData[5] = ((cFrame.size - 10) >> 16) & 0xFF;
		cFrame.pszData[6] = ((cFrame.size - 10) >> 8) & 0xFF;
		cFrame.pszData[7] = (cFrame.size - 10) & 0xFF;
		cFrame.pszData[8] = 0x00;
		cFrame.pszData[9] = 0x00;
		cFrame.pszData[10] = 0x00;
		cFrame.pszData[11] = 0x00;
		m_cMainFrames.Add(cFrame);

		delete pwszString, pszString;
	}
	return 1;
}


DWORD CTagID3v2::GetTotalFrameSize()
{
	DWORD dwSize = 0;
	int i;
	for (i = 0; i < m_cMainFrames.GetSize(); i ++){
		dwSize += m_cMainFrames[i].size;
	}
	for (i = 0; i < m_cOtherFrames.GetSize(); i ++){
		dwSize += m_cOtherFrames[i].size;
	}
	return dwSize;
}