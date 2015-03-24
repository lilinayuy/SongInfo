#pragma once
///////////////////////////////////////////////////////////////
// CVarietyButton
// ��̬ͼƬ��ťv1.00
// ���ߣ�����
// �÷���������һ�����ɰ�ť��
//		 ��ʼ����InitButton
//		 ���������С��SetButtonFont
//		 ���úͻ�ȡѡ��״̬��SetSelected��GetSelected Ĭ�ϲ�ѡ��
//		 �Ƿ�֧�����˫����EnableDoubleClick Ĭ�ϲ�֧��
///////////////////////////////////////////////////////////////

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class CVarietyButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CVarietyButton)

public:
	CVarietyButton();
	virtual ~CVarietyButton();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CWnd* m_pParentWnd;
	CDC* m_pParentDC;
	BOOL m_bTracking;																				//���������
	int m_nLeft;																					//��
	int m_nTop;																						//��
	int m_nWidth;																					//��
	int m_nHeight;																					//��
	CRect rcWindow;																					//����λ��
	BOOL m_bSelected;																				//�Ƿ�ѡ��
	BOOL m_bDoubleClick;																			//�ɷ�˫��
	CDC* m_pDC;																						//����ťDC
	HDC m_hBtnDC;
	CFont m_cFont;																					//����
	COLORREF m_cFontColor;																			//��ͨ״̬������ɫ
	COLORREF m_cFontColorSel;																		//����״̬������ɫ
	COLORREF m_cFontColorHighLight;																	//ƽʱ״̬�������������ɫ
	COLORREF m_cFontColorSelHighLight;																//����״̬�������������ɫ
	COLORREF m_cFontColorDisabled;																	//��Ч��ʱ��������ɫ
	COLORREF m_cColorTransparent;																	//��Ϊ͸������ɫ
	COLORREF m_cColorRegion;																		//��Ϊ���ȥ������ɫ
	CBitmap* m_pBitmap;			// normal image (REQUIRED)											//��ͨ״̬��ťͼƬ
	CBitmap* m_pBitmapSel;        // selected image (OPTIONAL)										//����״̬��ťͼƬ
	CBitmap* m_pBitmapHighLight;      // focused but not selected (OPTIONAL)							//ƽʱ״̬�������ͼƬ
	CBitmap* m_pBitmapSelHighLight;   // focused and selected image (OPTIONAL)						//����״̬�������ͼƬ
	CBitmap* m_pBitmapDisabled;   // disabled bitmap (OPTIONAL)										//��Ч��ʱ��ͼƬ
	CBitmap* m_pBitmapRegion;   // Region bitmap (OPTIONAL)											//���ͼƬ
	CImage* m_pImage;
	CImage* m_pImageSel;
	CImage* m_pImageHighLight;
	CImage* m_pImageSelHighLight;
	CImage* m_pImageDisabled;
	CImage* m_pImageRegion;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetupRegion(
						CImage* pImage, //���д�����״��λͼ����
						COLORREF TransColor   //͸��ɫ 
					);
	BOOL LoadBitmaps(LPCTSTR lpszBitmapResource,
			LPCTSTR lpszBitmapResourceSel = NULL,
			LPCTSTR lpszBitmapResourceHighLight = NULL,
			LPCTSTR lpszBitmapResourceDisabled = NULL,
			LPCTSTR lpszBitmapResourceSelHighLight = NULL, 
			LPCTSTR lpszBitmapResourceRegion = NULL);
	BOOL LoadBitmaps(UINT nIDBitmapResource,
			UINT nIDBitmapResourceSel = 0,
			UINT nIDBitmapResourceHighLight = 0,
			UINT nIDBitmapResourceDisabled = 0,
			UINT nIDBitmapResourceSelHighLight = 0,
			UINT nIDBitmapResourceRegion = 0);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void ButtonMoveWindow(CWnd* pWnd, CWnd* pParentWnd, CPoint point);
	//��ť�ƶ�
	BOOL m_bMovable;																	//���Զ���־
	CPoint m_ptStart;																	//��ʼλ��
	BYTE m_nButtonMode;																	//��ťģʽ 0 ��ť 1 ��̬��ť 2 ͼƬ
	int m_nPictureNumber;																//ͼƬ����
//	Graphics* m_pBackground;
	static BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, CImage* pImg);
public:
	//��ʼ����ť �� �� ͼƬ ������ɫ
	void InitButton(CWnd* pWnd,															//������
					int nLeft, int nTop,												//�� ��
					int nWidth, int nHeight,											//�� ��
					COLORREF nColorTransparent = -1,									//��Ϊ͸������ɫ
					UINT nIDBitmapResource = 0,											//��ͨ״̬��ťͼƬID
					COLORREF nColor = 0,												//��ͨ״̬������ɫ
					UINT nIDBitmapResourceSel = 0,										//����״̬��ťͼƬID
					COLORREF nColorSel = 0,											//����״̬������ɫ
					UINT nIDBitmapResourceHighLight = 0,								//ƽʱ״̬�������ͼƬID
					COLORREF nColorHighLight = 0,										//ƽʱ״̬�������������ɫ
					UINT nIDBitmapResourceDisabled = 0,									//��Ч��ʱ��ͼƬID
					COLORREF nColorDisabled = 0,										//��Ч��ʱ��������ɫ
					UINT nIDBitmapResourceSelHighLight = 0,								//����״̬�������ͼƬID
					COLORREF nColorSelHighLight = 0,									//����״̬�������������ɫ
					UINT nIDBitmapResourceRegion = 0,									//���ͼƬ
					COLORREF nColorRegion = 0);											//ȥ����ɫ
	void CVarietyButton::InitButton(CWnd* pWnd, 
					int nLeft, int nTop, 
					int nWidth, int nHeight, 
					COLORREF nColorTransparent, 
					CString strImage, COLORREF nColor, 
					CString strImageSel, COLORREF nColorSel,	
					CString strImageHighLight, COLORREF nColorHighLight, 
					CString strImageDisabled, COLORREF nColorDisabled, 
					CString strImageSelHighLight, COLORREF nColorSelHighLight, 
					CString strImageRegion, COLORREF nColorRegion);
	//��Ϊѡ��
	void SetSelected(BOOL bSelected);
	BOOL GetSelected();
	//��������ʹ�С
	void SetButtonFont(CString strFont, int nFontSize);									//�������ƺͳߴ�
	void EnableDoubleClick(BOOL bEnable);												//����֧��������˫��
	void SetButtonMode(int nMode);														//���ð�ťģʽ
	void ClearImage(void);
};


