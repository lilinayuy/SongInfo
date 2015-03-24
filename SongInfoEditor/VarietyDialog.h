#pragma once


// CVarietyDialog dialog

class CVarietyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVarietyDialog)

public:
	CVarietyDialog(CWnd* pParent = NULL);   // standard constructor
	CVarietyDialog(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~CVarietyDialog();

// Dialog Data
	enum { IDD = IDD_VARIETYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CImage* m_pImage;
	CImage* m_pImageRegion;
	COLORREF m_cColorTransparent;
	CDC* m_pDC;
	HDC m_hDialogDC;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetupRegion(CImage* pImage, COLORREF TransparentColor);
	void PreSubclassWindow();
	virtual void OnOK();
	virtual void OnCancel();
	CSize sizePicture;
	CPoint m_ptLeftTop;
	CPoint m_ptStart;
	BOOL m_bMovable;

	DECLARE_MESSAGE_MAP()
public:
	BOOL InitDialog(int nX, int nY, CString strPicName, DWORD dwTransParentColor);
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void GetDialogPosition(CPoint* point);
};
