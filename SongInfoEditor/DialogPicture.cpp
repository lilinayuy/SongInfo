// DialogPicture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SongInfoEditor.h"
#include "DialogPicture.h"
#include "SongInfoEditorDlg.h"



// CDialogPicture �Ի���

IMPLEMENT_DYNAMIC(CDialogPicture, CVarietyDialog)

CDialogPicture::CDialogPicture(CWnd* pParent /*=NULL*/)
	: CVarietyDialog(CDialogPicture::IDD, pParent)
{

}

CDialogPicture::~CDialogPicture()
{
}

void CDialogPicture::DoDataExchange(CDataExchange* pDX)
{
	CVarietyDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_PLAY, m_cBtnPlay);
	DDX_Control(pDX, IDC_BTN_STOP, m_cBtnStop);
	DDX_Control(pDX, IDC_BTN_PAUSE, m_cBtnPause);
	DDX_Control(pDX, IDC_BTN_PREV, m_cBtnPrev);
	DDX_Control(pDX, IDC_BTN_FF, m_cBtnFF);
}


BEGIN_MESSAGE_MAP(CDialogPicture, CVarietyDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_PLAY, &CDialogPicture::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDialogPicture::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CDialogPicture::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_PREV, &CDialogPicture::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_FF, &CDialogPicture::OnBnClickedBtnFf)
END_MESSAGE_MAP()


// CDialogPicture ��Ϣ�������

void CDialogPicture::SetButtonPosition(int nIndex, int nX, int nY, int nWidth, int nHeight, CString strPicturePath)
{
	switch(nIndex){
		case 0:
			m_cBtnPlay.MoveWindow(nX, nY, nWidth, nHeight);
			m_cBtnPlay.InitButton(this, nX, nY, nWidth, nHeight, RGB(0, 0, 0), strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0);
			break;
		case 1:
			m_cBtnStop.MoveWindow(nX, nY, nWidth, nHeight);
			m_cBtnStop.InitButton(this, nX, nY, nWidth, nHeight, RGB(0, 0, 0), strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0);
			break;
		case 2:
			m_cBtnPause.MoveWindow(nX, nY, nWidth, nHeight);
			m_cBtnPause.InitButton(this, nX, nY, nWidth, nHeight, RGB(0, 0, 0), strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0);
			break;
		case 3:
			m_cBtnPrev.MoveWindow(nX, nY, nWidth, nHeight);
			m_cBtnPrev.InitButton(this, nX, nY, nWidth, nHeight, RGB(0, 0, 0), strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0);
			break;
		case 4:
			m_cBtnFF.MoveWindow(nX, nY, nWidth, nHeight);
			m_cBtnFF.InitButton(this, nX, nY, nWidth, nHeight, RGB(0, 0, 0), strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0, strPicturePath, 0);
			break;
		default:
			break;
	}
}

void CDialogPicture::SaveButtonPosition()
{
	CRect rect;
	CSongInfoEditorDlg* pDlg = (CSongInfoEditorDlg*)m_pParentDlg;
	m_cBtnPlay.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDlg->SaveButtonPosition(0, rect);
	m_cBtnStop.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDlg->SaveButtonPosition(1, rect);
	m_cBtnPause.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDlg->SaveButtonPosition(2, rect);
	m_cBtnPrev.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDlg->SaveButtonPosition(3, rect);
	m_cBtnFF.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDlg->SaveButtonPosition(4, rect);

}

void CDialogPicture::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CVarietyDialog::OnPaint()
	CVarietyDialog::OnPaint();
	m_cBtnPlay.RedrawWindow();
	m_cBtnStop.RedrawWindow();
	m_cBtnPause.RedrawWindow();
	m_cBtnPrev.RedrawWindow();
	m_cBtnFF.RedrawWindow();
}


void CDialogPicture::OnBnClickedBtnPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSongInfoEditorDlg* pDlg = (CSongInfoEditorDlg*)m_pParentDlg;
	pDlg->OnBnClickedBtnPlay();
}


void CDialogPicture::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSongInfoEditorDlg* pDlg = (CSongInfoEditorDlg*)m_pParentDlg;
	pDlg->OnBnClickedBtnStop();
}


void CDialogPicture::OnBnClickedBtnPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSongInfoEditorDlg* pDlg = (CSongInfoEditorDlg*)m_pParentDlg;
	pDlg->OnBnClickedBtnPause();
}


void CDialogPicture::OnBnClickedBtnPrev()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSongInfoEditorDlg* pDlg = (CSongInfoEditorDlg*)m_pParentDlg;
	pDlg->OnBnClickedBtnPrev();
}


void CDialogPicture::OnBnClickedBtnFf()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSongInfoEditorDlg* pDlg = (CSongInfoEditorDlg*)m_pParentDlg;
	pDlg->OnBnClickedBtnFf();
}
