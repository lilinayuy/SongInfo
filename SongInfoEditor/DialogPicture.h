#pragma once

#include "VarietyDialog.h"
#include "VarietyButton.h"
#include "afxwin.h"
// CDialogPicture �Ի���

class CDialogPicture : public CVarietyDialog
{
	DECLARE_DYNAMIC(CDialogPicture)

public:
	CDialogPicture(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPicture();

// �Ի�������
	enum { IDD = IDD_DIALOGPICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CVarietyButton m_cBtnPlay;
	void SetButtonPosition(int nIndex, int nX, int nY, int nWidth, int nHeight, CString strPicturePath);
	CDialog* m_pParentDlg;
	CVarietyButton m_cBtnStop;
	CVarietyButton m_cBtnPause;
	CVarietyButton m_cBtnPrev;
	CVarietyButton m_cBtnFF;
	void SaveButtonPosition();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnFf();
};
