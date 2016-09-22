///////////////////////////////////////////////////////////////////////
//  Filename	SkinDialog.h
//	Class:		CSkinDialog
//
//	Compiler:	Visual C++ 6.0
//
//	Version:	1.0
//
//	Created:	2005/02/18
//	Updated:	2003/02/18
//
//	Author:		WYL
///////////////////////////////////////////////////////////////////////

#ifndef SKINDIALOG_INCLUDED
#define SKINDIALOG_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinDialog dialog

class CSkinDialog
{
// Construction
public:
	CSkinDialog();   // 标准构造函数
	~CSkinDialog();

	static WNDPROC	m_cWndProc; //消息处理过程

	static void SetBorderBitmap(CBitmap *pActiveBmp, CBitmap *pInactiveBmp);
	static void SetCaptionBtnBitmap(CBitmap *pMaxBmp, CBitmap *pMinBmp, CBitmap *pRestoreBmp, CBitmap *pCloseBmp);
	static void SetMargins(UINT nTopLeft, UINT nTopRight, UINT nBottomLeft, UINT nBottomRight, UINT nToLeft, UINT nToRight, UINT nToTop, UINT nToBottom);
	static void SetMaxButton(UINT nAlign, POINT point, CBitmap *pMaxBitmap, CBitmap *pRestoreBitmap);
	static void SetMinButton(UINT nAlign, POINT point, CBitmap *pBitmap);
	static void SetCloseButton(UINT nAlign, POINT point, CBitmap *pBitmap);
	static LRESULT WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	static void UseSkin(CWnd* pWnd, BOOL bEnable);

// Attributes
protected:
	static CBitmap *m_pBmpActiveBorder;
	static CBitmap *m_pBmpInactiveBorder;
	static CBitmap *m_pBmpMaxBtn;
	static CBitmap *m_pBmpRestoreBtn;
	static CBitmap *m_pBmpMinBtn;
	static CBitmap *m_pBmpCloseBtn;
	static UINT m_nTopLeft;
	static UINT m_nTopRight;
	static UINT m_nBottomLeft;
	static UINT m_nBottomRight;
	static UINT m_nToLeft;
	static UINT m_nToRight;
	static UINT m_nToTop;
	static UINT m_nToBottom;

	static UINT m_nAlignMax;
	static UINT m_nAlignMin;
	static UINT m_nAlignClose;
	static POINT m_ptMax;
	static POINT m_ptMin;
	static POINT m_ptClose;

	static UINT m_nMoveHitTest;
	static UINT m_nDownHitTest;

protected:
	static void OnShowWindow( HWND hWnd, BOOL bShow, UINT nStatus );
	static void OnNcPaint(CWnd* pWnd);
	static BOOL OnNcActivate(CWnd* pWnd, BOOL bActive);
	static void OnActivate(CWnd* pWnd, UINT nState, CWnd *pWndOther, BOOL bMinimized);
	static void OnNcDestroy(HWND hWnd);
	static void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	static void OnSize( HWND hWnd, UINT nType, int cx, int cy );
	static void DrawBorder(CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bActive);
	static void DrawCaption(CWnd* pWnd, CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bActive);
	static void StretchDraw(CDC* pDCDest, CDC* pDCSrc, CRect rcDest, CRect rcSrc);

	static void GetCaptionButtonRect(CWnd* pWnd, CRect &rect, UINT nType);
	static BOOL PopupSysMenu(CWnd* pWnd, CPoint point);
	static BOOL HandleSysCommand(CWnd* pWnd, WPARAM wParam, LPARAM lParam);

	static UINT OnNcHitTest( CWnd* pWnd, CPoint point );
	static BOOL OnNcLButtonDown( CWnd* pWnd, UINT nHitTest, CPoint point );
	static BOOL OnNcLButtonUp( CWnd* pWnd, UINT nHitTest, CPoint point );
	static void OnNcLButtonDblClk( CWnd* pWnd, UINT nHitTest, CPoint point );
	static void OnNcRButtonDown( CWnd* pWnd, UINT nHitTest, CPoint point );
	static void OnNcRButtonUp( CWnd* pWnd, UINT nHitTest, CPoint point );
	static void OnNcMouseMove( CWnd* pWnd, UINT nHitTest, CPoint point );
	static void OnLButtonUp( CWnd* pWnd, UINT nFlag, CPoint point );
	static void OnMouseMove( CWnd* pWnd, UINT nFlag, CPoint point );
	static void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
};

#endif //!SKINDIALOG_INCLUDED
