
#include "stdafx.h"
//#include "useful.h"
#include "DCPlus.h"
#include <math.h>
#include ".\dcplus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDCPlus::CDCPlus()
{	
	m_sDCSet.Init();
}

CDCPlus::~CDCPlus()
{
	if (m_sDCSet.pDC){
		m_sDCSet.pDC->SelectObject(m_pOldBitmap);
		m_sDCSet.pDC->DeleteDC();
		DeleteObject(m_sDCSet.hBitmap);
		delete m_sDCSet.pDC;
		m_sDCSet.pDC = NULL;
		delete m_sDCSet.pBitmap;
	}
}

BOOL CDCPlus::LoadPicture(CString strFileName)
{
	CString strExt = (strFileName.Right(3)).MakeLower();
	if (strExt == _T("bmp")){
		return LoadBmp(strFileName);
	}else if ((strExt == _T("jpg")) || (strExt == _T("jpeg"))){
		return LoadJpg(strFileName);
	}else{
		return FALSE;
	}
}

BOOL CDCPlus::LoadBmp(CString strFileName)
{
	LPBITMAPINFO m_lpBmpInfo;
	IPicture *pPic;
	IStream *pStm;
	CFileStatus fStatus;
	CFile cFile;
	CDC cDisplayDC;
	LONG cb;
	if (cFile.Open(strFileName.GetBuffer(), CFile::modeRead) && cFile.GetStatus(strFileName.GetBuffer(), fStatus) && ((cb = fStatus.m_size) != -1)){     
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
		if (hGlobal != NULL){
			LPVOID pData;// = new BYTE[cb];
			if ((pData = GlobalLock(hGlobal)) != NULL){
				cFile.Read(pData, cb);
				GlobalUnlock(hGlobal);
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
				if (SUCCEEDED(OleLoadPicture(pStm, fStatus.m_size, TRUE, IID_IPicture, (LPVOID*)&pPic))){ 
					m_lpBmpInfo =(LPBITMAPINFO) new char[sizeof(BITMAPINFOHEADER)];
					OLE_XSIZE_HIMETRIC hmWidth;     
					OLE_YSIZE_HIMETRIC hmHeight;
					pPic->get_Width(&hmWidth);     
					pPic->get_Height(&hmHeight);
					//查找分辨率
					WORD dWidth;
					WORD dHeight;
					BYTE* pBuffer = (BYTE*)pData;
					dWidth = ((DWORD)pBuffer[0x0012]) + ((DWORD)pBuffer[0x0013] << 8) + ((DWORD)pBuffer[0x0014] << 16) + ((DWORD)pBuffer[0x0015] << 24);
					dHeight = ((DWORD)pBuffer[0x0016]) + ((DWORD)pBuffer[0x0017] << 8) + ((DWORD)pBuffer[0x0018] << 16) + ((DWORD)pBuffer[0x0019] << 24);
					cDisplayDC.CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);
					m_sDCSet.pDC = new CDC();
					m_sDCSet.pDC->CreateCompatibleDC(&cDisplayDC);
					cDisplayDC.DeleteDC();
					SetBitmapHeader(m_lpBmpInfo, dWidth, dHeight);
					m_sDCSet.hBitmap = CreateDIBSection(m_sDCSet.pDC->GetSafeHdc(),m_lpBmpInfo,DIB_RGB_COLORS,(void **)&m_sDCSet.pBuffer,NULL,0);
					if(m_lpBmpInfo){
						delete ((char*)m_lpBmpInfo);
						m_lpBmpInfo  = NULL;
					}
					m_sDCSet.pBitmap = new CBitmap();
					m_sDCSet.pBitmap->Attach(m_sDCSet.hBitmap);
					m_pOldBitmap = m_sDCSet.pDC->SelectObject(m_sDCSet.pBitmap);
					m_sDCSet.nWidth = dWidth;
					m_sDCSet.nHeight = dHeight;
					m_sDCSet.strName = strFileName;

					if (FAILED(pPic->Render(m_sDCSet.pDC->GetSafeHdc(), 0, 0, (DWORD)dWidth, (DWORD)dHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
						strErrMsg = _T("Failed To Render The picture!");
					return FALSE;
					pPic->Release();
					//DC操作

					//清除


				}else{
					strErrMsg = _T("Error Loading Picture From Stream!");
					return FALSE;
				}
			}
		}
		GlobalFree(hGlobal);
		cFile.Close();
		return TRUE;
	}else{
		strErrMsg = _T("Can't Open Image File!");
		return FALSE;
	}
}

BOOL CDCPlus::LoadJpg(CString strFileName)
{
	LPBITMAPINFO m_lpBmpInfo;
	IPicture *pPic;
	CDC cDisplayDC;
	IStream *pStm;
	CFileStatus fStatus;
	CFile cFile;
	LONG cb;
	if (cFile.Open(strFileName.GetBuffer(), CFile::modeRead) && cFile.GetStatus(strFileName.GetBuffer(), fStatus) && ((cb = fStatus.m_size) != -1)){     
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
		if (hGlobal != NULL){
			LPVOID pData;// = new BYTE[cb];
			if ((pData = GlobalLock(hGlobal)) != NULL){
				cFile.Read(pData, cb);
				GlobalUnlock(hGlobal);
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
				if (SUCCEEDED(OleLoadPicture(pStm, fStatus.m_size, TRUE, IID_IPicture, (LPVOID*)&pPic))){ 
					m_lpBmpInfo =(LPBITMAPINFO) new char[sizeof(BITMAPINFOHEADER)];
					OLE_XSIZE_HIMETRIC hmWidth;     
					OLE_YSIZE_HIMETRIC hmHeight;
					pPic->get_Width(&hmWidth);     
					pPic->get_Height(&hmHeight);
					//查找分辨率
					WORD dWidth;
					WORD dHeight;
					BYTE* pBuffer = (BYTE*)pData;
					for (int ilop = 0; ilop < cb - 1; ilop ++){
						if ((pBuffer[ilop] == 0xff) && (pBuffer[ilop + 1] == 0xc0)){
							dWidth = pBuffer[ilop + 7] * 256 + pBuffer[ilop + 8];
							dHeight = pBuffer[ilop + 5] * 256 + pBuffer[ilop + 6];
						}
					}
					cDisplayDC.CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL);
					m_sDCSet.pDC = new CDC();
					m_sDCSet.pDC->CreateCompatibleDC(&cDisplayDC);
					cDisplayDC.DeleteDC();
					SetBitmapHeader(m_lpBmpInfo, dWidth, dHeight);
					m_sDCSet.hBitmap = CreateDIBSection(m_sDCSet.pDC->GetSafeHdc(),m_lpBmpInfo,DIB_RGB_COLORS,(void **)&m_sDCSet.pBuffer,NULL,0);
					m_sDCSet.pBitmap = new CBitmap();
					m_sDCSet.pBitmap->Attach(m_sDCSet.hBitmap);	
					m_pOldBitmap = m_sDCSet.pDC->SelectObject(m_sDCSet.pBitmap);
					m_sDCSet.nWidth = dWidth;
					m_sDCSet.nHeight = dHeight;
					m_sDCSet.strName = strFileName;
					if(m_lpBmpInfo){
						delete ((char*)m_lpBmpInfo);
						m_lpBmpInfo  = NULL;
					}
					if (FAILED(pPic->Render(m_sDCSet.pDC->GetSafeHdc(), 0, 0, (DWORD)dWidth, (DWORD)dHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
						strErrMsg = _T("Failed To Render The picture!");
					return FALSE;
					pPic->Release();
					//DC操作
					//清除
				}else{
					strErrMsg = _T("Error Loading Picture From Stream!");
					return FALSE;
				}
			}
		}
		GlobalFree(hGlobal);
		cFile.Close();
		return TRUE;
	}else{
		strErrMsg = _T("Can't Open Image File!");
		return FALSE;
	}
}

void CDCPlus::CreateDC(int nWidth, int nHeight, int nBit)
{
	LPBITMAPINFO m_lpBmpInfo;
	CDC cDisplayDC;
	m_lpBmpInfo =(LPBITMAPINFO) new char[sizeof(BITMAPINFOHEADER)];
	cDisplayDC.CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);
	m_sDCSet.pDC = new CDC();
	m_sDCSet.pDC->CreateCompatibleDC(&cDisplayDC);
	cDisplayDC.DeleteDC();
	SetBitmapHeader(m_lpBmpInfo, nWidth, nHeight, nBit);
	m_sDCSet.hBitmap = CreateDIBSection(m_sDCSet.pDC->GetSafeHdc(),m_lpBmpInfo,DIB_RGB_COLORS,(void **)&m_sDCSet.pBuffer,NULL,0);
	m_sDCSet.pBitmap = new CBitmap();
	m_sDCSet.pBitmap->Attach(m_sDCSet.hBitmap);
	m_pOldBitmap = m_sDCSet.pDC->SelectObject(m_sDCSet.pBitmap);
	m_sDCSet.nWidth = nWidth;
	m_sDCSet.nHeight = nHeight;
	m_sDCSet.strName = _T("");
	if(m_lpBmpInfo){
		delete ((char*)m_lpBmpInfo);
		m_lpBmpInfo  = NULL;
	}

}

DWORD ReadBmpBuffer(BYTE* lpBuffer, BYTE* lpSrcBuffer, DWORD dOffset, DWORD dWidth, DWORD dHeight, UINT nBit)
{
	UINT nBitNum = nBit / 8;
	DWORD dResult = 0;
	for(DWORD ilop = 1; ilop <= dHeight; ilop ++){
		memcpy(lpBuffer + (ilop - 1) * dWidth * nBitNum, lpSrcBuffer + dOffset + (dHeight - ilop) * dWidth * nBitNum, dWidth * nBitNum);
		dResult += (dWidth * nBitNum);
	}
	return dResult;
};

DWORD ReadBmpBuffer2(BYTE* lpBuffer, BYTE* lpSrcBuffer, DWORD dOffset, DWORD dWidth, DWORD dHeight, UINT nBit)
{
	UINT nBitNum = nBit / 8;
	DWORD dResult = 0;
	memcpy(lpBuffer, lpSrcBuffer + dOffset, dWidth * dHeight * nBitNum);
	dResult += dWidth * dHeight * nBitNum;
	return dResult;
};

double MyRound(double fOrigin)
{
	if ((fOrigin - floor(fOrigin)) > 0.5){
		return ceil(fOrigin);
	}else{
		return floor(fOrigin);
	}
}

SIZE GetSuitableSize(SIZE cDestSize, SIZE cSrcSize)
{
	SIZE size;
	if (((double)cDestSize.cx / (double)cDestSize.cy) < ((double)cSrcSize.cx / (double)cSrcSize.cy)){
		size.cx = cDestSize.cx;
		size.cy = MyRound((double)cSrcSize.cy / cSrcSize.cx * cDestSize.cx);
	}else{
		size.cy = cDestSize.cy;
		size.cx = MyRound((double)cSrcSize.cx / cSrcSize.cy * cDestSize.cy);
	}
	return size;
}

SIZE GetSuitableSizeY(SIZE cDestSize, SIZE cSrcSize)
{
	SIZE size;
	size.cy = cDestSize.cy;
	size.cx = MyRound((double)cSrcSize.cx / cSrcSize.cy * cDestSize.cy);
	return size;
}

void SetBitmapHeader(LPBITMAPINFO lpBmpInfo, int dWidth, int dHeight, int nBit)	//生成BMP头文件
{
	lpBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBmpInfo->bmiHeader.biWidth = dWidth;
	lpBmpInfo->bmiHeader.biHeight = dHeight;
	lpBmpInfo->bmiHeader.biPlanes = 1;
	lpBmpInfo->bmiHeader.biBitCount = nBit;
	lpBmpInfo->bmiHeader.biCompression = BI_RGB;
	lpBmpInfo->bmiHeader.biSizeImage = 0;
	lpBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	lpBmpInfo->bmiHeader.biClrUsed = 0;
	lpBmpInfo->bmiHeader.biClrImportant = 0;
	return;
}


CDC* CDCPlus::GetDC(void)
{
	return m_sDCSet.pDC;
}

BYTE* CDCPlus::GetBuffer(void)
{
	return m_sDCSet.pBuffer;
}

CSize CDCPlus::GetSize(void)
{
	return CSize(m_sDCSet.nWidth, m_sDCSet.nHeight);
}

BOOL CDCPlus::LoadResource(int nResourceID)
{
	CDC cDisplayDC;
	cDisplayDC.CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);
	CDC dcResource;
	CBitmap bmpResource;
	BITMAP bmpInfo;
	dcResource.CreateCompatibleDC(&cDisplayDC);
	bmpResource.LoadBitmap(nResourceID);
	bmpResource.GetBitmap(&bmpInfo);
	dcResource.SelectObject(&bmpResource);
	LPBITMAPINFO m_lpBmpInfo;
	m_lpBmpInfo =(LPBITMAPINFO) new char[sizeof(BITMAPINFOHEADER)];
	m_sDCSet.pDC = new CDC();
	m_sDCSet.pDC->CreateCompatibleDC(&cDisplayDC);
	cDisplayDC.DeleteDC();
	SetBitmapHeader(m_lpBmpInfo, bmpInfo.bmWidth, bmpInfo.bmHeight);
	m_sDCSet.hBitmap = CreateDIBSection(m_sDCSet.pDC->GetSafeHdc(),m_lpBmpInfo,DIB_RGB_COLORS,(void **)&m_sDCSet.pBuffer,NULL,0);
	m_sDCSet.pBitmap = new CBitmap();
	m_sDCSet.pBitmap->Attach(m_sDCSet.hBitmap);
	m_pOldBitmap = m_sDCSet.pDC->SelectObject(m_sDCSet.pBitmap);
	m_sDCSet.pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcResource, 0, 0, SRCCOPY);
	m_sDCSet.nWidth = bmpInfo.bmWidth;
	m_sDCSet.nHeight = bmpInfo.bmHeight;
	m_sDCSet.strName = _T("");
	if(m_lpBmpInfo){
		delete ((char*)m_lpBmpInfo);
		m_lpBmpInfo  = NULL;
	}
	return TRUE;
}

void CDCPlus::DeletePicture(void)
{
	if (m_sDCSet.pDC){
		m_sDCSet.pDC->SelectObject(m_pOldBitmap);
		m_sDCSet.pDC->DeleteDC();
		DeleteObject(m_sDCSet.hBitmap);
		delete m_sDCSet.pDC;
		m_sDCSet.pDC = NULL;
		delete m_sDCSet.pBitmap;
	}
}

HRESULT CDCPlus::SavePicture(CString strFileName)
{
	CString strExt = (strFileName.Right(3)).MakeLower();
	CImage img;
		img.Attach(m_sDCSet.hBitmap);
	if (strExt == _T("bmp")){
		return img.Save(strFileName, Gdiplus::ImageFormatBMP);
	}else if ((strExt == _T("jpg")) || (strExt == _T("jpeg"))){
		return img.Save(strFileName, Gdiplus::ImageFormatJPEG);
	}else if (strExt == _T("png")){
		return img.Save(strFileName, Gdiplus::ImageFormatPNG);
	}else if (strExt == _T("gif")){
		return img.Save(strFileName, Gdiplus::ImageFormatGIF);
	}else{
		return -1;
	}
}