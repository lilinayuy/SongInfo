#pragma once
///////////////////////////////////////////////////////////////
// CVarietyButton
// 动态图片按钮v1.00
// 作者：徐磊
// 用法：和正常一样生成按钮。
//		 初始化：InitButton
//		 设置字体大小：SetButtonFont
//		 设置和获取选中状态：SetSelected、GetSelected 默认不选中
//		 是否支持左键双击：EnableDoubleClick 默认不支持
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
	BOOL m_bTracking;																				//鼠标悬浮用
	int m_nLeft;																					//左
	int m_nTop;																						//上
	int m_nWidth;																					//宽
	int m_nHeight;																					//高
	CRect rcWindow;																					//窗口位置
	BOOL m_bSelected;																				//是否选中
	BOOL m_bDoubleClick;																			//可否双击
	CDC* m_pDC;																						//本按钮DC
	HDC m_hBtnDC;
	CFont m_cFont;																					//字体
	COLORREF m_cFontColor;																			//普通状态文字颜色
	COLORREF m_cFontColorSel;																		//按下状态文字颜色
	COLORREF m_cFontColorHighLight;																	//平时状态鼠标悬浮文字颜色
	COLORREF m_cFontColorSelHighLight;																//按下状态鼠标悬浮文字颜色
	COLORREF m_cFontColorDisabled;																	//无效化时候文字颜色
	COLORREF m_cColorTransparent;																	//作为透明的颜色
	COLORREF m_cColorRegion;																		//作为外框去除的颜色
	CBitmap* m_pBitmap;			// normal image (REQUIRED)											//普通状态按钮图片
	CBitmap* m_pBitmapSel;        // selected image (OPTIONAL)										//按下状态按钮图片
	CBitmap* m_pBitmapHighLight;      // focused but not selected (OPTIONAL)							//平时状态鼠标悬浮图片
	CBitmap* m_pBitmapSelHighLight;   // focused and selected image (OPTIONAL)						//按下状态鼠标悬浮图片
	CBitmap* m_pBitmapDisabled;   // disabled bitmap (OPTIONAL)										//无效化时候图片
	CBitmap* m_pBitmapRegion;   // Region bitmap (OPTIONAL)											//外框图片
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
						CImage* pImage, //含有窗体形状的位图对象
						COLORREF TransColor   //透明色 
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
	//按钮移动
	BOOL m_bMovable;																	//可以动标志
	CPoint m_ptStart;																	//初始位置
	BYTE m_nButtonMode;																	//按钮模式 0 按钮 1 三态按钮 2 图片
	int m_nPictureNumber;																//图片数量
//	Graphics* m_pBackground;
	static BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, CImage* pImg);
public:
	//初始化按钮 宽 高 图片 文字颜色
	void InitButton(CWnd* pWnd,															//主窗口
					int nLeft, int nTop,												//左 上
					int nWidth, int nHeight,											//宽 高
					COLORREF nColorTransparent = -1,									//作为透明的颜色
					UINT nIDBitmapResource = 0,											//普通状态按钮图片ID
					COLORREF nColor = 0,												//普通状态文字颜色
					UINT nIDBitmapResourceSel = 0,										//按下状态按钮图片ID
					COLORREF nColorSel = 0,											//按下状态文字颜色
					UINT nIDBitmapResourceHighLight = 0,								//平时状态鼠标悬浮图片ID
					COLORREF nColorHighLight = 0,										//平时状态鼠标悬浮文字颜色
					UINT nIDBitmapResourceDisabled = 0,									//无效化时候图片ID
					COLORREF nColorDisabled = 0,										//无效化时候文字颜色
					UINT nIDBitmapResourceSelHighLight = 0,								//按下状态鼠标悬浮图片ID
					COLORREF nColorSelHighLight = 0,									//按下状态鼠标悬浮文字颜色
					UINT nIDBitmapResourceRegion = 0,									//外框图片
					COLORREF nColorRegion = 0);											//去除颜色
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
	//设为选定
	void SetSelected(BOOL bSelected);
	BOOL GetSelected();
	//设置字体和大小
	void SetButtonFont(CString strFont, int nFontSize);									//字体名称和尺寸
	void EnableDoubleClick(BOOL bEnable);												//设置支持鼠标左键双击
	void SetButtonMode(int nMode);														//设置按钮模式
	void ClearImage(void);
};


