// VarietyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "VarietyDialog.h"
#include "afxdialogex.h"


// CVarietyDialog dialog

IMPLEMENT_DYNAMIC(CVarietyDialog, CDialogEx)

CVarietyDialog::CVarietyDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVarietyDialog::IDD, pParent)
{
	m_pImage = NULL;
	m_pImageRegion = NULL;
	m_bMovable = FALSE;
}

CVarietyDialog::CVarietyDialog(UINT nIDTemplate, CWnd *pParent)
{
	m_pImage = NULL;
	m_pImageRegion = NULL;
	m_bMovable = FALSE;
	m_ptLeftTop.x = 0;
	m_ptLeftTop.y = 0;
	CDialogEx(nIDTemplate, pParent);
}

CVarietyDialog::~CVarietyDialog()
{
	if (m_pImage){
		delete m_pImage;
		m_pImage = NULL;
	}
	if (m_pImageRegion){
		delete m_pImageRegion;
		m_pImageRegion = NULL;
	}
}

void CVarietyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVarietyDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CVarietyDialog message handlers


BOOL CVarietyDialog::InitDialog(int nX, int nY, CString strPicName, DWORD dwTransParentColor)
{
	if (m_pImage){
		delete m_pImage;
		m_pImage = NULL;
	}
	if (m_pImageRegion){
		delete m_pImageRegion;
		m_pImageRegion = NULL;
	}
	m_pImage = new CImage();
	m_pImageRegion = new CImage();
	if (m_pImage->Load(strPicName) == S_OK){
		m_pImageRegion->Load(strPicName);
		sizePicture.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		MoveWindow(nX, nY, sizePicture.cx, sizePicture.cy);
		m_cColorTransparent = dwTransParentColor;
		if (m_pImageRegion){
			m_pImageRegion->SetTransparentColor(m_cColorTransparent);
			SetupRegion(m_pImageRegion, m_cColorTransparent);
		}
		DWORD aaa = RGB(200,255,200);
		m_ptLeftTop.x = nX;
		m_ptLeftTop.y = nY;
		return TRUE;
	}else{
		delete m_pImage;
		m_pImage = NULL;
		delete m_pImageRegion;
		m_pImageRegion = NULL;
	}
	return FALSE;
}

void CVarietyDialog::SetupRegion(CImage* pImage, COLORREF TransparentColor)
{
	CRgn   wndRgn; 
	//�����ܵĴ������򣬳�ʼregionΪ0 
	wndRgn.CreateRectRgn(0,0,0,0); 
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	int nX, nY;
	for(nY = 0; nY < nHeight; nY++){
		CRgn rgnTemp;//������ʱregion 
		nX = 0; 
		do{ 
			//����͸��ɫ�ҵ���һ����͸��ɫ�ĵ�. 
			while (nX < nWidth && pImage->GetPixel(nX, nY) == TransparentColor){
				nX ++;
			}
			int nLeft = nX;
			//Ѱ���¸�͸��ɫ�ĵ�
			while (nX < nWidth && pImage->GetPixel(nX, nY) != TransparentColor){
				nX ++;
			}
			//����һ������������ص���Ϊ1���ص���ʱ��region�� 
			rgnTemp.CreateRectRgn(nLeft, nY, nX, nY+1);
			//�ϲ����� "region ". 
			wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_OR); 
            //ɾ����ʱ "region ",�����´δ���ʱ�ͳ��� 
			rgnTemp.DeleteObject(); 
		}while(nX < nWidth);
	}
	SetWindowRgn(wndRgn,TRUE);
	wndRgn.DeleteObject();
}

void CVarietyDialog::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW | WS_POPUP);
	m_pDC = GetDC();
	m_hDialogDC = m_pDC->GetSafeHdc();
	m_pDC->SetBkMode(TRANSPARENT);
	ModifyStyleEx(0, WS_EX_TRANSPARENT | WS_EX_APPWINDOW);
}

void CVarietyDialog::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)//*lpDrawItemStruct*/)
{
	if (m_pImage){
		m_pImage->Draw(m_hDialogDC, 0, 0, sizePicture.cx, sizePicture.cy);
	}
//	CButton::DrawItem(lpDrawItemStruct);
}

void CVarietyDialog::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	return;
	CDialogEx::OnOK();
}


void CVarietyDialog::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	return;
	CDialogEx::OnCancel();
}


void CVarietyDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_pImage){
		m_pImage->Draw(m_hDialogDC, 0, 0, sizePicture.cx, sizePicture.cy);
	}
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CVarietyDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (HIBYTE(GetKeyState(VK_LCONTROL)) || HIBYTE(GetKeyState(VK_RCONTROL))){
		//���¿�ʼ�ƶ�
		m_bMovable = TRUE;
		ClientToScreen(&point);
		m_ptStart = point;
	//	TRACE2(_T("��ť����%d,%d\r\n"), point.x, point.y);
		return;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CVarietyDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMovable = FALSE;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CVarietyDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ClientToScreen(&point);
	if (m_bMovable){
		CDialog* pDlg = (CDialog*)m_pParentWnd;
		if (HIBYTE(GetKeyState(VK_LCONTROL)) || HIBYTE(GetKeyState(VK_RCONTROL))){
			m_ptLeftTop.x += point.x - m_ptStart.x;
			m_ptLeftTop.y += point.y - m_ptStart.y;
			MoveWindow(m_ptLeftTop.x, m_ptLeftTop.y, sizePicture.cx, sizePicture.cy, 0);
			m_ptStart = point;
		}else{
			m_bMovable = FALSE;
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CVarietyDialog::GetDialogPosition(CPoint* point)
{
	point->x = m_ptLeftTop.x;
	point->y = m_ptLeftTop.y;
}