// VarietyButton.cpp : implementation file
//

#include "stdafx.h"
#include "VarietyButton.h"
//#include "DCPlus.h"

// CVarietyButton

IMPLEMENT_DYNAMIC(CVarietyButton, CBitmapButton)

CVarietyButton::CVarietyButton()
{
	m_bTracking = FALSE;
	m_bSelected = FALSE;
	m_cColorTransparent = 0;
	m_bDoubleClick = FALSE;
	m_bMovable = FALSE;
	m_nButtonMode = 1;
	m_pBitmap = NULL;
	m_pBitmapSel = NULL;
	m_pBitmapHighLight = NULL;
	m_pBitmapSelHighLight = NULL;
	m_pBitmapDisabled = NULL;
	m_pBitmapRegion = NULL;
	m_cFontColor = 0;
	m_cFontColorSel = 0;
	m_cFontColorHighLight = 0;
	m_cFontColorSelHighLight = 0;
	m_cFontColorDisabled = 0;
	m_cColorTransparent = 0;
	m_pImage = NULL;
	m_pImageSel = NULL;
	m_pImageHighLight = NULL;
	m_pImageSelHighLight = NULL;
	m_pImageDisabled = NULL;
	m_pImageRegion = NULL;
}

CVarietyButton::~CVarietyButton()
{
	if (m_pBitmap){
		m_pBitmap->DeleteObject();
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	if (m_pBitmapSel){
		m_pBitmapSel->DeleteObject();
		delete m_pBitmapSel;
		m_pBitmapSel = NULL;
	}
	if (m_pBitmapHighLight){
		m_pBitmapHighLight->DeleteObject();
		delete m_pBitmapHighLight;
		m_pBitmapHighLight = NULL;
	}
	if (m_pBitmapSelHighLight){
		m_pBitmapSelHighLight->DeleteObject();
		delete m_pBitmapSelHighLight;
		m_pBitmapSelHighLight = NULL;
	}
	if (m_pBitmapDisabled){
		m_pBitmapDisabled->DeleteObject();
		delete m_pBitmapDisabled;
		m_pBitmapDisabled = NULL;
	}
	if (m_pBitmapRegion){
		m_pBitmapRegion->DeleteObject();
		delete m_pBitmapRegion;
		m_pBitmapRegion = NULL;
	}
	ClearImage();
}

BEGIN_MESSAGE_MAP(CVarietyButton, CBitmapButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CVarietyButton message handlers
void CVarietyButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CBitmapButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
	m_pDC = GetDC();
	m_pDC->SetBkMode(TRANSPARENT);
	m_hBtnDC = m_pDC->GetSafeHdc();
	//m_pBackground = &Graphics(m_pDC->GetSafeHdc());
	CRect rc;
	GetClientRect(&rc);
	m_nWidth = rc.Width();
	m_nHeight = rc.Height();
	ModifyStyleEx(0, WS_EX_TRANSPARENT);
}

void CVarietyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)//*lpDrawItemStruct*/)
{
	if (m_nButtonMode == 3) {
		return;	
	}
	UINT uStyle = DFCS_BUTTONPUSH;
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
	// If drawing selected, add the pushed style to DrawFrameControl.
//	m_pDC->Draw3dRect(0, 0, m_nWidth, m_nHeight, RGB(220, 220, 220), RGB(100, 100, 100));
//	ShowWindow(SW_HIDE);
	m_pDC->BitBlt(0, 0, m_nWidth, m_nHeight, m_pParentDC, rcWindow.left, rcWindow.top, SRCCOPY); 
//	ShowWindow(SW_SHOW);
	if (lpDrawItemStruct->itemState & ODS_SELECTED){
		//点中
	//	m_pDC->Draw3dRect(0, 0, m_nWidth, m_nHeight, RGB(100, 100, 100), RGB(100, 100, 100));
		uStyle |= DFCS_BUTTONPUSH;
		if (m_bSelected){
			if (m_pImageSelHighLight != 0){
				m_pImageSelHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
			}
		}else{
			if (m_pImageHighLight != 0){
				m_pImageHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
			}
		}
		CString strText;
		GetWindowText(strText);
		if (!m_bSelected){
			m_pDC->SetTextColor(m_cFontColorSelHighLight);
		}else{
			m_pDC->SetTextColor(m_cFontColorHighLight);
		}
		m_pDC->DrawText(strText, strText.GetLength(), CRect(0, 0, m_nWidth, m_nHeight), DT_SINGLELINE|DT_VCENTER|DT_CENTER);   //重绘文字
	//	m_bTracking = 1;
	}else if (lpDrawItemStruct->itemState & ODS_DISABLED){
		//无效化
		m_pImageDisabled->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
		CString strText;
		GetWindowText(strText);
		m_pDC->SetTextColor(m_cFontColorDisabled);
		m_pDC->DrawText(strText, strText.GetLength(), CRect(0, 0, m_nWidth, m_nHeight), DT_SINGLELINE|DT_VCENTER|DT_CENTER);   //重绘文字
	}else if (lpDrawItemStruct->itemState & ODS_FOCUS){
		//键盘焦点
		if (m_bSelected){
			if (m_pImageSelHighLight != 0){
				m_pImageSelHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
			}
		}else{
			if (m_pImageHighLight != 0){
				m_pImageHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
			}
		}
		CString strText;
		GetWindowText(strText);
		if (!m_bSelected){
			m_pDC->SetTextColor(m_cFontColorSelHighLight);
		}else{
			m_pDC->SetTextColor(m_cFontColorHighLight);
		}
		m_pDC->DrawText(strText, strText.GetLength(), CRect(0, 0, m_nWidth, m_nHeight), DT_SINGLELINE|DT_VCENTER|DT_CENTER);   //重绘文字
	}else if (lpDrawItemStruct->itemState & ODS_HOTLIGHT){
		//只在XP有用 高亮
		if (m_bSelected){
			if (m_pImageSelHighLight != 0){
				m_pImageSelHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
			}
		}else{
			if (m_pImageHighLight != 0){
				m_pImageHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
			}
		}
		CString strText;
		GetWindowText(strText);
		if (m_bSelected){
			m_pDC->SetTextColor(m_cFontColorSelHighLight);
		}else{
			m_pDC->SetTextColor(m_cFontColorHighLight);
		}
		m_pDC->DrawText(strText, strText.GetLength(), CRect(0, 0, m_nWidth, m_nHeight), DT_SINGLELINE|DT_VCENTER|DT_CENTER);   //重绘文字
	}else{
		//一般状态
		if (m_pImage == 0){
			return;
		}
		m_pImage->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
		CString strText;
		GetWindowText(strText);
		m_pDC->SetTextColor(m_cFontColor);
		m_pDC->DrawText(strText, strText.GetLength(), CRect(0, 0, m_nWidth, m_nHeight), DT_SINGLELINE|DT_VCENTER|DT_CENTER);   //重绘文字
	}
//	CButton::DrawItem(lpDrawItemStruct);
}


void CVarietyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMovable){
		CDialog* pDlg = (CDialog*)m_pParentWnd;
		if (HIBYTE(GetKeyState(VK_LCONTROL)) || HIBYTE(GetKeyState(VK_RCONTROL))){
			point.x += rcWindow.left;
			point.y += rcWindow.top;
			ButtonMoveWindow(this, m_pParentWnd, point);
		}else{
			m_bMovable = FALSE;
		}
	}else if (!m_bTracking){
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}else{
	}
	CBitmapButton::OnMouseMove(nFlags, point);
}

LRESULT CVarietyButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bMovable) return 0;
//	ShowWindow(SW_HIDE);
	m_pDC->BitBlt(0, 0, m_nWidth, m_nHeight, m_pParentDC, rcWindow.left, rcWindow.top, SRCCOPY); 
//	ShowWindow(SW_SHOW);
	if (m_bSelected){
	//	m_pDC->Draw3dRect(0, 0, m_nWidth, m_nHeight, RGB(100, 100, 100), RGB(100, 100, 100));
		if (m_pImageSel != 0){	
			m_pImageSel->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
		}
	}else{
		if (m_pImage != 0){
			m_pImage->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
		}
	}
	CString strText;
	GetWindowText(strText);
	if (m_bSelected){
		m_pDC->SetTextColor(m_cFontColorSel);
	}else{
		m_pDC->SetTextColor(m_cFontColor);
	}
	m_pDC->DrawText(strText, strText.GetLength(), CRect(0, 0, m_nWidth, m_nHeight), DT_SINGLELINE|DT_VCENTER|DT_CENTER);   //重绘文字
	m_bTracking = 0;
	return 0;
}
LRESULT CVarietyButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
//	ShowWindow(SW_HIDE);
	m_pDC->BitBlt(0, 0, m_nWidth, m_nHeight, m_pParentDC, rcWindow.left, rcWindow.top, SRCCOPY); 
//	ShowWindow(SW_SHOW);
	if (m_bSelected){
	//	m_pDC->Draw3dRect(0, 0, m_nWidth, m_nHeight, RGB(100, 100, 100), RGB(100, 100, 100));
		if (m_pImageSelHighLight != 0){	
			m_pImageSelHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
		}
	}else{
	//	m_pDC->BitBlt(0, 0, m_nWidth, m_nHeight, m_pParentWnd->GetDC(), m_nLeft, m_nTop, SRCCOPY);
	//	m_pDC->Draw3dRect(0, 0, m_nWidth, m_nHeight, RGB(220, 220, 220), RGB(100, 100, 100));
		if (m_pImageHighLight != 0){
			m_pImageHighLight->Draw(m_hBtnDC, 0, 0, m_nWidth, m_nHeight);
		}
	//	dcTemp.SelectObject(m_pBitmapHighLight);
	}
	CString strText;
	GetWindowText(strText);
	if (m_bSelected){
		m_pDC->SetTextColor(m_cFontColorSelHighLight);
	}else{
		m_pDC->SetTextColor(m_cFontColorHighLight);
	}
	m_pDC->DrawText(strText, strText.GetLength(), CRect(0, 0, m_nWidth, m_nHeight), DT_SINGLELINE|DT_VCENTER|DT_CENTER);   //重绘文字
	return 0;
}


BOOL CVarietyButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
//	return CBitmapButton::OnEraseBkgnd(pDC);
}

BOOL CVarietyButton::LoadBitmaps(UINT nIDBitmapResource, 
					UINT nIDBitmapResourceSel,	
					UINT nIDBitmapResourceHighLight, 
					UINT nIDBitmapResourceDisabled, 
					UINT nIDBitmapResourceSelHighLight,
					UINT nIDBitmapResourceRegion)
{
/*	m_pBitmap = new CBitmap();
	m_pBitmap->LoadBitmap(nIDBitmapResource);
	m_pBitmapSel = new CBitmap();
	m_pBitmapSel->LoadBitmap(nIDBitmapResourceSel);
	m_pBitmapHighLight = new CBitmap();
	m_pBitmapHighLight->LoadBitmap(nIDBitmapResourceHighLight);
	m_pBitmapDisabled = new CBitmap();
	m_pBitmapDisabled->LoadBitmap(nIDBitmapResourceDisabled);
	m_pBitmapSelHighLight = new CBitmap();
	m_pBitmapSelHighLight->LoadBitmap(nIDBitmapResourceSelHighLight);
	m_pBitmapRegion = new CBitmap();
	m_pBitmapRegion->LoadBitmap(nIDBitmapResourceRegion);*/

	m_pImage = new CImage();
	ImageFromIDResource(nIDBitmapResource, _T("PNG"), m_pImage);
	m_pImageSel = new CImage();
	ImageFromIDResource(nIDBitmapResourceSel, _T("PNG"), m_pImageSel);
	m_pImageHighLight = new CImage();
	ImageFromIDResource(nIDBitmapResourceHighLight, _T("PNG"), m_pImageHighLight);
	m_pImageDisabled = new CImage();
	ImageFromIDResource(nIDBitmapResourceDisabled, _T("PNG"), m_pImageDisabled);
	m_pImageSelHighLight = new CImage();
	ImageFromIDResource(nIDBitmapResourceSelHighLight, _T("PNG"), m_pImageSelHighLight);
	m_pImageRegion = new CImage();
	ImageFromIDResource(nIDBitmapResourceRegion, _T("PNG"), m_pImageRegion);

	return TRUE;
}

void CVarietyButton::InitButton(CWnd* pWnd, int nLeft, int nTop, int nWidth, int nHeight, COLORREF nColorTransparent, UINT nIDBitmapResource, COLORREF nColor, UINT nIDBitmapResourceSel, COLORREF nColorSel,
								UINT nIDBitmapResourceHighLight, COLORREF nColorHighLight, UINT nIDBitmapResourceDisabled, COLORREF nColorDisabled,
								UINT nIDBitmapResourceSelHighLight, COLORREF nColorSelHighLight, UINT nIDBitmapResourceRegion, COLORREF nColorRegion)
{
	m_pParentWnd = pWnd;
	m_pParentDC = m_pParentWnd->GetDC();
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	rcWindow = CRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
	MoveWindow(&rcWindow);
	ClearImage();
	LoadBitmaps(nIDBitmapResource, nIDBitmapResourceSel, nIDBitmapResourceHighLight, nIDBitmapResourceDisabled, nIDBitmapResourceSelHighLight, nIDBitmapResourceRegion);
	m_cFontColor = nColor;
	if (nColorSel <= 16777215){
		m_cFontColorSel = nColorSel;
	}else{
		m_cFontColorSel = m_cFontColor;
	}
	if (nColorHighLight <= 16777215){
		m_cFontColorHighLight = nColorHighLight;
	}else{
		m_cFontColorHighLight = m_cFontColorSel;
	}
	if (nColorSelHighLight <= 16777215){
		m_cFontColorSelHighLight = nColorSelHighLight;
	}else{
		m_cFontColorSelHighLight = m_cFontColorSel;
	}
	if (nColorDisabled <= 16777215){
		m_cFontColorDisabled = nColorDisabled;
	}else{
		m_cFontColorDisabled = RGB(200, 200, 200);
	}
	m_cColorTransparent = nColorTransparent;
	if (m_pImageRegion){
		m_pImageRegion->SetTransparentColor(nColorRegion);
		SetupRegion(m_pImageRegion, nColorRegion);
	}
}

BOOL CVarietyButton::LoadBitmaps(LPCTSTR lpszBitmapResource,
			LPCTSTR lpszBitmapResourceSel,
			LPCTSTR lpszBitmapResourceHighLight,
			LPCTSTR lpszBitmapResourceDisabled,
			LPCTSTR lpszBitmapResourceSelHighLight, 
			LPCTSTR lpszBitmapResourceRegion)
{
	m_pImage = new CImage();
	m_pImage->Load(lpszBitmapResource);
	m_pImageSel = new CImage();
	m_pImageSel->Load(lpszBitmapResourceSel);
	m_pImageHighLight = new CImage();
	m_pImageHighLight->Load(lpszBitmapResourceHighLight);
	m_pImageDisabled = new CImage();
	m_pImageDisabled->Load(lpszBitmapResourceDisabled);
	m_pImageSelHighLight = new CImage();
	m_pImageSelHighLight->Load(lpszBitmapResourceSelHighLight);
	m_pImageRegion = new CImage();
	m_pImageRegion->Load(lpszBitmapResourceRegion);

	return TRUE;
}

void CVarietyButton::InitButton(CWnd* pWnd, int nLeft, int nTop, int nWidth, int nHeight, COLORREF nColorTransparent, CString strImage, COLORREF nColor, CString strImageSel, COLORREF nColorSel,
								CString strImageHighLight, COLORREF nColorHighLight, CString strImageDisabled, COLORREF nColorDisabled,
								CString strImageSelHighLight, COLORREF nColorSelHighLight, CString strImageRegion, COLORREF nColorRegion)
{
	m_pParentWnd = pWnd;
	m_pParentDC = m_pParentWnd->GetDC();
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	rcWindow = CRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
	MoveWindow(&rcWindow);
	ClearImage();
	LoadBitmaps(strImage.GetBuffer(), strImageSel.GetBuffer(), strImageHighLight.GetBuffer(), strImageDisabled.GetBuffer(), strImageSelHighLight.GetBuffer(), strImageRegion.GetBuffer());
	m_cFontColor = nColor;
	if (nColorSel <= 16777215){
		m_cFontColorSel = nColorSel;
	}else{
		m_cFontColorSel = m_cFontColor;
	}
	if (nColorHighLight <= 16777215){
		m_cFontColorHighLight = nColorHighLight;
	}else{
		m_cFontColorHighLight = m_cFontColorSel;
	}
	if (nColorSelHighLight <= 16777215){
		m_cFontColorSelHighLight = nColorSelHighLight;
	}else{
		m_cFontColorSelHighLight = m_cFontColorSel;
	}
	if (nColorDisabled <= 16777215){
		m_cFontColorDisabled = nColorDisabled;
	}else{
		m_cFontColorDisabled = RGB(200, 200, 200);
	}
	m_cColorTransparent = nColorTransparent;
	if (m_pImageRegion){
		m_pImageRegion->SetTransparentColor(nColorRegion);
		SetupRegion(m_pImageRegion, nColorRegion);
	}
}

void CVarietyButton::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
	m_bTracking = 0;
}

void CVarietyButton::SetupRegion(CImage* pImage, COLORREF TransparentColor)
{
	CRgn   wndRgn; 
	//创建总的窗体区域，初始region为0 
	wndRgn.CreateRectRgn(0,0,0,0); 
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	int nX, nY;
	for(nY = 0; nY < nHeight; nY++){
		CRgn rgnTemp;//保存临时region 
		nX = 0; 
		do{ 
			//跳过透明色找到下一个非透明色的点. 
			while (nX < nWidth && pImage->GetPixel(nX, nY) == TransparentColor){
				nX ++;
			}
			int nLeft = nX;
			//寻找下个透明色的点
			while (nX < nWidth && pImage->GetPixel(nX, nY) != TransparentColor){
				nX ++;
			}
			//创建一个包含起点与重点间高为1像素的临时“region” 
			rgnTemp.CreateRectRgn(nLeft, nY, nX, nY+1);
			//合并到主 "region ". 
			wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_OR); 
            //删除临时 "region ",否则下次创建时和出错 
			rgnTemp.DeleteObject(); 
		}while(nX < nWidth);
	}
	SetWindowRgn(wndRgn,TRUE);
	wndRgn.DeleteObject();
}

void CVarietyButton::SetButtonFont(CString strFont, int nFontSize)
{
	m_cFont.DeleteObject();
	m_cFont.CreateFont(nFontSize, nFontSize / 2, 0, 0, FW_DONTCARE, FALSE, FALSE, 0, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, strFont);
	m_pDC->SelectObject(&m_cFont);
}

LRESULT CVarietyButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_bDoubleClick){
		if (message == WM_LBUTTONDBLCLK){
			message = WM_LBUTTONDOWN;
		}
	}
	return CBitmapButton::DefWindowProc(message, wParam, lParam);
}

BOOL CVarietyButton::GetSelected()
{
	return m_bSelected;
}


BOOL CVarietyButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONUP){
		m_bTracking = 0;
	}else if(pMsg->message == WM_LBUTTONDOWN){
		
	}
	return CBitmapButton::PreTranslateMessage(pMsg);
}

void CVarietyButton::EnableDoubleClick(BOOL bEnable)
{
	m_bDoubleClick = bEnable;
}

void CVarietyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (HIBYTE(GetKeyState(VK_LCONTROL)) || HIBYTE(GetKeyState(VK_RCONTROL))){
		//按下开始移动
		m_bMovable = TRUE;
		point.x += rcWindow.left;
		point.y += rcWindow.top;
		m_ptStart = point;
	//	TRACE2(_T("按钮坐标%d,%d\r\n"), point.x, point.y);
		return;
	}
	if (m_nButtonMode == 1){
		m_bSelected = !m_bSelected;
	}
	CBitmapButton::OnLButtonDown(nFlags, point);
}


void CVarietyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMovable = FALSE;
	CBitmapButton::OnLButtonUp(nFlags, point);
}

void CVarietyButton::ButtonMoveWindow(CWnd* pWnd, CWnd* pParentWnd, CPoint point)
{
	if (m_bMovable && (HIBYTE(GetKeyState(VK_LCONTROL))) || (HIBYTE(GetKeyState(VK_RCONTROL)))){
		rcWindow.left += (point.x - m_ptStart.x);
		rcWindow.right += (point.x - m_ptStart.x);
		rcWindow.top += (point.y - m_ptStart.y);
		rcWindow.bottom += (point.y - m_ptStart.y);
		if (rcWindow.left < 0) rcWindow.left = 0;
		rcWindow.right = rcWindow.left + m_nWidth;
		if (rcWindow.top < 0) rcWindow.top = 0;
		rcWindow.bottom = rcWindow.top + m_nHeight;
//		TRACE2(_T("按钮位置坐标%d,%d**\r\n"), rcWindow.left, rcWindow.top);
		MoveWindow(&rcWindow, 1);
		m_ptStart = point;
//		TRACE2(_T("按钮位置坐标%d,%d//\r\n"), point.x, point.y);
//	}else{
//		m_bMovable = FALSE;
//	}
	}
}

BOOL CVarietyButton::ImageFromIDResource(UINT nID, LPCTSTR szType, CImage* pImg)  
{
	HINSTANCE hInst = AfxGetResourceHandle();  
	HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(nID), szType); // type  
	if (!hRsrc) return FALSE;
	// load resource into memory  
	DWORD len = SizeofResource(hInst, hRsrc);  
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
	if (!lpRsrc)  
	return FALSE;  
	// Allocate global memory on which to create stream  
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);  
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
	memcpy(pmem,lpRsrc,len);  
	IStream* pstm;  
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
	// load from stream  
	pImg->Load(pstm);
	// free/release stuff  
	GlobalUnlock(m_hMem);  
	pstm->Release();  
	FreeResource(lpRsrc);  
	return TRUE;  
}

void CVarietyButton::ClearImage()
{
	if (m_pImage){
		m_pImage->Destroy();
		delete m_pImage;
		m_pImage = NULL;
	}
	if (m_pImageSel){
		m_pImageSel->Destroy();
		delete m_pImageSel;
		m_pImageSel = NULL;
	}
	if (m_pImageHighLight){
		m_pImageHighLight->Destroy();
		delete m_pImageHighLight;
		m_pImageHighLight = NULL;
	}
	if (m_pImageSelHighLight){
		m_pImageSelHighLight->Destroy();
		delete m_pImageSelHighLight;
		m_pImageSelHighLight = NULL;
	}
	if (m_pImageDisabled){
		m_pImageDisabled->Destroy();
		delete m_pImageDisabled;
		m_pImageDisabled = NULL;
	}
	if (m_pImageRegion){
		m_pImageRegion->Destroy();
		delete m_pImageRegion;
		m_pImageRegion = NULL;
	}
}