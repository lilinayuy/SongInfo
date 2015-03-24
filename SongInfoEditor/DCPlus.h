//CDCPlus v1.0
#pragma once
///////////////////////////////////////////////////////////////
// CDCPlus 
// DCͼƬ��ǿ��v1.00
// ���ߣ�����
// �÷�����ʼ����CreateDC��LoadPicture(�Զ��ж�BMP����JPG)��LoadResource(��ԴID)
//		 ��ȡͼƬ�ṹ��DCָ�롢���ݡ��ߴ磺GetPicSet��GetDC��GetBuffer��GetSize
//		 ��ȡ������Ϣ��GetErrMsg
//		 ɾ��ͼƬ��DeletePicture
//		 ����ͼƬ��SavePicture
///////////////////////////////////////////////////////////////

//�������ݴ�Žṹ
typedef struct tagCommonDCSet{
	CDC* pDC;							//DCָ��
	CBitmap* pBitmap;					//ͼƬָ��
	BYTE* pBuffer;						//����
	HBITMAP hBitmap;					//ͼƬ���
	int nWidth;							//���
	int nHeight;						//�߶�
	CString strName;					//����
	tagCommonDCSet &operator=(const tagCommonDCSet& cds){
		pDC = cds.pDC;
		pBitmap = cds.pBitmap;
		pBuffer = cds.pBuffer;
		nWidth = cds.nWidth;
		nHeight = cds.nHeight;
		strName = cds.strName;
		hBitmap = cds.hBitmap;
		return *this;
	};
	void Init(){						//��ʼ��
		pDC = NULL;
		pBitmap = NULL;
		pBuffer = NULL;
		nWidth = 0;
		nHeight = 0;
		strName = _T("");
		hBitmap = NULL;
	}
}COMMONDCSET;

//��ȡBMPͷ���ݴ��µ���
DWORD ReadBmpBuffer(BYTE* lpBuffer, BYTE* lpSrcBuffer, DWORD dOffset, DWORD dWidth, DWORD dHeight, UINT nBit = 32);
//��ȡBMPͷ���ݴ��ϵ���
DWORD ReadBmpBuffer2(BYTE* lpBuffer, BYTE* lpSrcBuffer, DWORD dOffset, DWORD dWidth, DWORD dHeight, UINT nBit = 32);
//��������
double MyRound(double fOrigin);
//�����ʺϴ�С
SIZE GetSuitableSize(SIZE cDestSize, SIZE cSrcSize);
//�����ʺϸ߶ȴ�С
SIZE GetSuitableSizeY(SIZE cDestSize, SIZE cSrcSize);
//����BMPͷ�ļ�
void SetBitmapHeader(LPBITMAPINFO lpBmpInfo, int dWidth, int dHeight, int nBit = 32);	

class CDCPlus
{
protected:
	COMMONDCSET m_sDCSet;											//ͼƬ���ݴ�Žṹ
	CString strErrMsg;												//������Ϣ
	BOOL LoadBmp(CString strFileName);								//��ȡBMP
	BOOL LoadJpg(CString strFileName);								//��ȡJPG
	CBitmap* m_pOldBitmap;											//��ͼƬָ����ʱ��
public:
	CDCPlus();														//��׼���캯��
	virtual ~CDCPlus();												//��׼���캯��	
	BOOL LoadPicture(CString strFileName);							//��ȡͼƬ����DC
	COMMONDCSET* GetPicSet(){return &m_sDCSet;};					//��ȡͼƬ��Žṹ
	void CreateDC(int nWidth, int nHeight, int nBit = 32);			//����С����DC
	CDC* GetDC(void);												//���DCָ��
	BYTE* GetBuffer(void);											//���ͼƬ����
	CSize GetSize(void);											//��óߴ�
	BOOL LoadResource(int nResourceID);								//��ȡ��ԴID����DC
	void DeletePicture(void);										//ɾ��ͼƬ
	HRESULT SavePicture(CString strFileName);						//����ͼƬ BMP JPG PNG GIF
	CString GetErrMsg(){return strErrMsg;};							//��ȡ������Ϣ
};




