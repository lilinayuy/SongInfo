//CDCPlus v1.0
#pragma once
///////////////////////////////////////////////////////////////
// CDCPlus 
// DC图片增强类v1.00
// 作者：徐磊
// 用法：初始化：CreateDC、LoadPicture(自动判定BMP或者JPG)、LoadResource(资源ID)
//		 获取图片结构、DC指针、数据、尺寸：GetPicSet、GetDC、GetBuffer、GetSize
//		 获取错误信息：GetErrMsg
//		 删除图片：DeletePicture
//		 保存图片：SavePicture
///////////////////////////////////////////////////////////////

//必须内容存放结构
typedef struct tagCommonDCSet{
	CDC* pDC;							//DC指针
	CBitmap* pBitmap;					//图片指针
	BYTE* pBuffer;						//数据
	HBITMAP hBitmap;					//图片句柄
	int nWidth;							//宽度
	int nHeight;						//高度
	CString strName;					//名称
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
	void Init(){						//初始化
		pDC = NULL;
		pBitmap = NULL;
		pBuffer = NULL;
		nWidth = 0;
		nHeight = 0;
		strName = _T("");
		hBitmap = NULL;
	}
}COMMONDCSET;

//读取BMP头数据从下到上
DWORD ReadBmpBuffer(BYTE* lpBuffer, BYTE* lpSrcBuffer, DWORD dOffset, DWORD dWidth, DWORD dHeight, UINT nBit = 32);
//读取BMP头数据从上到下
DWORD ReadBmpBuffer2(BYTE* lpBuffer, BYTE* lpSrcBuffer, DWORD dOffset, DWORD dWidth, DWORD dHeight, UINT nBit = 32);
//四舍五入
double MyRound(double fOrigin);
//计算适合大小
SIZE GetSuitableSize(SIZE cDestSize, SIZE cSrcSize);
//计算适合高度大小
SIZE GetSuitableSizeY(SIZE cDestSize, SIZE cSrcSize);
//生成BMP头文件
void SetBitmapHeader(LPBITMAPINFO lpBmpInfo, int dWidth, int dHeight, int nBit = 32);	

class CDCPlus
{
protected:
	COMMONDCSET m_sDCSet;											//图片数据存放结构
	CString strErrMsg;												//错误信息
	BOOL LoadBmp(CString strFileName);								//读取BMP
	BOOL LoadJpg(CString strFileName);								//读取JPG
	CBitmap* m_pOldBitmap;											//老图片指针临时用
public:
	CDCPlus();														//标准构造函数
	virtual ~CDCPlus();												//标准构造函数	
	BOOL LoadPicture(CString strFileName);							//读取图片生成DC
	COMMONDCSET* GetPicSet(){return &m_sDCSet;};					//获取图片存放结构
	void CreateDC(int nWidth, int nHeight, int nBit = 32);			//按大小生成DC
	CDC* GetDC(void);												//获得DC指针
	BYTE* GetBuffer(void);											//获得图片数据
	CSize GetSize(void);											//获得尺寸
	BOOL LoadResource(int nResourceID);								//读取资源ID生成DC
	void DeletePicture(void);										//删除图片
	HRESULT SavePicture(CString strFileName);						//保存图片 BMP JPG PNG GIF
	CString GetErrMsg(){return strErrMsg;};							//获取错误信息
};




