// SkinDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "SkinDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WNDPROC	CSkinDialog::m_cWndProc = NULL;

CBitmap* CSkinDialog::m_pBmpActiveBorder = NULL;
CBitmap* CSkinDialog::m_pBmpInactiveBorder = NULL;
CBitmap* CSkinDialog::m_pBmpMaxBtn = NULL;
CBitmap* CSkinDialog::m_pBmpRestoreBtn = NULL;
CBitmap* CSkinDialog::m_pBmpMinBtn = NULL;
CBitmap* CSkinDialog::m_pBmpCloseBtn = NULL;

UINT CSkinDialog::m_nTopLeft = 0;
UINT CSkinDialog::m_nTopRight = 0;
UINT CSkinDialog::m_nBottomLeft = 0;
UINT CSkinDialog::m_nBottomRight = 0;
UINT CSkinDialog::m_nToLeft = 0;
UINT CSkinDialog::m_nToRight = 0;
UINT CSkinDialog::m_nToTop = 0;
UINT CSkinDialog::m_nToBottom = 0;

UINT CSkinDialog::m_nAlignMax = 0;
UINT CSkinDialog::m_nAlignMin = 0;
UINT CSkinDialog::m_nAlignClose = 0;
POINT CSkinDialog::m_ptMax = {0,0};
POINT CSkinDialog::m_ptMin = {0,0};
POINT CSkinDialog::m_ptClose = {0,0};

UINT CSkinDialog::m_nMoveHitTest = 0;
UINT CSkinDialog::m_nDownHitTest = 0;

/////////////////////////////////////////////////////////////////////////////
// CSkinDialog dialog

CSkinDialog::CSkinDialog()
{}

CSkinDialog::~CSkinDialog()
{}

/////////////////////////////////////////////////////////////////////////////
// CSkinDialog message handlers

void CSkinDialog::UseSkin(CWnd* pWnd, BOOL bEnable)
{
	DWORD dwStyle = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
	if (bEnable)
	{
		dwStyle &= ~WS_CAPTION;		

		WNDPROC wndProc = (WNDPROC)SetWindowLong( pWnd->m_hWnd, GWL_WNDPROC, (LONG) CSkinDialog::WindowProc );
		if (CSkinDialog::m_cWndProc == NULL)
			CSkinDialog::m_cWndProc = wndProc;
	}
	// 取消皮肤，重新计算边框大小。SM_CYSMCAPTION的情况还没有考虑
	else
	{
		dwStyle |= WS_CAPTION;

		CSize szFrame = (dwStyle & WS_THICKFRAME) ?
			CSize(GetSystemMetrics(SM_CXSIZEFRAME),
			GetSystemMetrics(SM_CYSIZEFRAME)) :
		CSize(GetSystemMetrics(SM_CXFIXEDFRAME),
			GetSystemMetrics(SM_CYFIXEDFRAME));

		m_nToLeft = szFrame.cx;
		m_nToRight = szFrame.cx;
		m_nToTop = szFrame.cy + GetSystemMetrics(SM_CYCAPTION);
		m_nToBottom = szFrame.cy;

		if (CSkinDialog::m_cWndProc != NULL)
			SetWindowLong( pWnd->m_hWnd, GWL_WNDPROC, (LONG) CSkinDialog::m_cWndProc );
	}
	SetWindowLong( pWnd->m_hWnd, GWL_STYLE, dwStyle );

	CRect rcWin;
	pWnd->GetClientRect( &rcWin );
	rcWin.left -= m_nToLeft;
	rcWin.right += m_nToRight;
	rcWin.top -= m_nToTop;
	rcWin.bottom += m_nToBottom;

	//call SetWindowPos to force OnNcCalcSize when hWnd is a dialog
	pWnd->SetWindowPos( 0, 0, 0, rcWin.Width(), rcWin.Height(), 
		SWP_NOMOVE|SWP_FRAMECHANGED|SWP_NOACTIVATE);
}

void CSkinDialog::SetBorderBitmap(CBitmap *pActiveBmp, CBitmap *pInactiveBmp)
{
	m_pBmpActiveBorder = pActiveBmp;
	m_pBmpInactiveBorder = pInactiveBmp;
}

void CSkinDialog::SetCaptionBtnBitmap(CBitmap *pMaxBmp, CBitmap *pMinBmp, CBitmap *pRestoreBmp, CBitmap *pCloseBmp)
{
	m_pBmpMaxBtn = pMaxBmp;
	m_pBmpMinBtn = pMinBmp;
	m_pBmpRestoreBtn = pRestoreBmp;
	m_pBmpCloseBtn = pCloseBmp;
}

void CSkinDialog::SetMargins(UINT nTopLeft, UINT nTopRight, UINT nBottomLeft, UINT nBottomRight, UINT nToLeft, UINT nToRight, UINT nToTop, UINT nToBottom)
{
	m_nTopLeft = nTopLeft;
	m_nTopRight = nTopRight;
	m_nBottomLeft = nBottomLeft;
	m_nBottomRight = nBottomRight;
	m_nToLeft = nToLeft;
	m_nToRight = nToRight;
	m_nToTop = nToTop;
	m_nToBottom = nToBottom;
}

void CSkinDialog::SetMaxButton(UINT nAlign, POINT point, CBitmap *pMaxBitmap, CBitmap *pRestoreBitmap)
{
	m_nAlignMax = nAlign;
	m_ptMax = point;
	m_pBmpMaxBtn = pMaxBitmap;
	m_pBmpRestoreBtn = pRestoreBitmap;
}

void CSkinDialog::SetMinButton(UINT nAlign, POINT point, CBitmap *pBitmap)
{
	m_nAlignMin = nAlign;
	m_ptMin = point;
	m_pBmpMinBtn = pBitmap;
}

void CSkinDialog::SetCloseButton(UINT nAlign, POINT point, CBitmap *pBitmap)
{
	m_nAlignClose = nAlign;
	m_ptClose = point;
	m_pBmpCloseBtn = pBitmap;
}

LRESULT CSkinDialog::WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
	CWnd* pWnd = CWnd::FromHandle( hWnd ); 
	if (pWnd == NULL)
		return 0;

	switch( message )
	{
	case WM_SHOWWINDOW:
		OnShowWindow( hWnd, BOOL(wParam), UINT(lParam));
		return 0;
	case WM_SETTEXT:
		break;
	case WM_SETICON:
		break;
	case WM_SETCURSOR:
		break;
	case WM_NCPAINT:
		OnNcPaint(pWnd);
		return 0;
	case WM_ACTIVATE:
		break;
//		OnActivate(pWnd, wParam, CWnd::FromHandle((HWND)lParam), 0);
//		return 0;
	case WM_NCACTIVATE:
		return OnNcActivate(pWnd, (BOOL)wParam);
	case WM_NCCALCSIZE:
		OnNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS *)lParam);
		return 0;
	case WM_SIZE:
		break;
	case WM_NCHITTEST:
		{
			UINT nHitTest = OnNcHitTest( pWnd, CPoint(lParam) );
			if( HTCLIENT != nHitTest )
				return nHitTest;
		}
		break;
	case WM_NCLBUTTONDOWN:
		if ( OnNcLButtonDown(pWnd, wParam, CPoint(lParam)) )
			return 0;
		break;
	case WM_NCLBUTTONUP:
		if ( OnNcLButtonUp(pWnd, wParam, CPoint(lParam)) )
			return 0;
		break;
	case WM_NCLBUTTONDBLCLK:
		OnNcLButtonDblClk( pWnd, wParam, CPoint(lParam) );
		break;
	case WM_NCRBUTTONDOWN:
		OnNcRButtonDown( pWnd, wParam, CPoint(lParam) );
		return 0;
	case WM_NCRBUTTONUP:
		OnNcRButtonUp( pWnd, wParam, CPoint(lParam) );
		return 0;
	case WM_NCMOUSEMOVE:
		OnNcMouseMove( pWnd, wParam,CPoint(lParam) );
		break;
	case WM_LBUTTONUP:
		OnLButtonUp( pWnd, UINT(wParam), CPoint(lParam) );
		break;
	case WM_MOUSEMOVE:
		OnMouseMove( pWnd, UINT(wParam), CPoint(lParam) );
		break;
	case WM_GETMINMAXINFO:
		OnGetMinMaxInfo( (MINMAXINFO *)lParam );
		return 0;
	case WM_COMMAND:
		if (HandleSysCommand(pWnd, wParam, lParam))
			return 0;
		break;
	case WM_NCDESTROY:
		OnNcDestroy(hWnd);
//		return 0; 
		break;
	default:
		break;
	}

	return CallWindowProc( m_cWndProc, hWnd, message, wParam, lParam );  
}

BOOL CSkinDialog::HandleSysCommand(CWnd* pWnd, WPARAM wParam, LPARAM lParam)
{
	TRACE("\nCSkinDialog::HandleSysCommand()");
	if (lParam == 0)
	{
		CMenu* pSysMenu = pWnd->GetSystemMenu(FALSE);
		int count = pSysMenu->GetMenuItemCount();
		for( int i = 0; i < count ; i++ )
			if ( pSysMenu->GetMenuItemID(i) == wParam )
				break;

		if ( i >= count )
			return FALSE;

		pWnd->SendMessage( WM_SYSCOMMAND, wParam, lParam );
		return TRUE;
	}

	return FALSE;
}

BOOL CSkinDialog::PopupSysMenu(CWnd* pWnd, CPoint point)
{
	CMenu* pSysMenu = pWnd->GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
//		DWORD dwState = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
//		dwState &= WS_MAXIMIZE;
		if (pWnd->IsZoomed()) //maximized
		{
			pSysMenu->EnableMenuItem( SC_MAXIMIZE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
			pSysMenu->EnableMenuItem( SC_RESTORE, MF_BYCOMMAND|MF_ENABLED );
		}
		else //restore
		{
			pSysMenu->EnableMenuItem( SC_MINIMIZE, MF_BYCOMMAND|MF_ENABLED );
			pSysMenu->EnableMenuItem( SC_MAXIMIZE, MF_BYCOMMAND|MF_ENABLED );
			pSysMenu->EnableMenuItem( SC_RESTORE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		}
		if (pWnd->IsIconic())
		{
			pSysMenu->EnableMenuItem( SC_MINIMIZE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
			pSysMenu->EnableMenuItem( SC_RESTORE, MF_BYCOMMAND|MF_ENABLED );
		}
		pSysMenu->TrackPopupMenu( 0, point.x, point.y, pWnd  );
	}

	return TRUE;
}

void CSkinDialog::StretchDraw(CDC* pDCDest, CDC* pDCSrc, CRect rcDest, CRect rcSrc)
{
	int nWidth = (rcDest.Width() > rcSrc.Width()) ? rcSrc.Width() : rcDest.Width();
	int nHeight = (rcDest.Height() > rcSrc.Height()) ? rcSrc.Height() : rcDest.Height();
	if (rcDest.Height() == rcSrc.Height())
	while (rcDest.Width() > 0)
	{
		pDCDest->BitBlt( rcDest.left, rcDest.top, nWidth, nHeight, 
			pDCSrc, rcSrc.left, rcSrc.top, SRCCOPY);
		rcDest.left += rcSrc.Width();
		nWidth = (rcDest.Width() > rcSrc.Width()) ? rcSrc.Width() : rcDest.Width();
	}
	else if (rcDest.Width() == rcSrc.Width())
	while (rcDest.Height() > 0)
	{
		pDCDest->BitBlt( rcDest.left, rcDest.top, nWidth, nHeight, 
			pDCSrc, rcSrc.left, rcSrc.top, SRCCOPY);
		rcDest.top += rcSrc.Height();
		nHeight = (rcDest.Height() > rcSrc.Height()) ? rcSrc.Height() : rcDest.Height();
	}
}

void CSkinDialog::DrawBorder(CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bActive)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(bActive ? m_pBmpActiveBorder : m_pBmpInactiveBorder);
	if (pOldBitmap == NULL)
		return;

/*	// left
	pDC->StretchBlt(0, m_nToTop, m_nToLeft, rcDest.Height()-m_nToTop-m_nToBottom, 
			&memDC, 0, m_nToTop, m_nToLeft, rcSrc.Height()-m_nToTop-m_nToBottom, SRCCOPY);
	//right
	pDC->StretchBlt(rcDest.right-m_nToRight, m_nToTop, m_nToRight, rcDest.Height()-m_nToTop-m_nToBottom, 
			&memDC, rcSrc.right-m_nToRight, m_nToTop, m_nToRight, rcSrc.Height()-m_nToTop-m_nToBottom, SRCCOPY);
	//bottom
	pDC->StretchBlt(m_nBottomLeft, rcDest.Height()-m_nToBottom, rcDest.Width()-m_nBottomLeft-m_nBottomRight, m_nToBottom,
			&memDC, m_nBottomLeft, rcSrc.Height()-m_nToBottom, rcSrc.Width()-m_nBottomLeft-m_nBottomRight, m_nToBottom, SRCCOPY);
*/
	// left
	CRect rc1(0, m_nToTop, m_nToLeft, rcDest.Height()-m_nToBottom);
	CRect rc2(0, m_nToTop, m_nToLeft, rcSrc.Height()-m_nToBottom);
	StretchDraw(pDC, &memDC, rc1, rc2); 
	//right
	rc1 = CRect(rcDest.right-m_nToRight, m_nToTop, rcDest.right, rcDest.bottom-m_nToBottom);
	rc2 = CRect(rcSrc.right-m_nToRight, m_nToTop, rcSrc.right, rcSrc.bottom-m_nToBottom);
	StretchDraw(pDC, &memDC, rc1, rc2); 
	//bottom
	rc1 = CRect(m_nBottomLeft, rcDest.Height()-m_nToBottom, rcDest.Width()-m_nBottomRight, rcDest.Height());
	rc2 = CRect(m_nBottomLeft, rcSrc.Height()-m_nToBottom, rcSrc.Width()-m_nBottomRight, rcSrc.Height());
	StretchDraw(pDC, &memDC, rc1, rc2); 
	//bottomLeft
	pDC->BitBlt(0, rcDest.Height()-m_nToBottom, m_nBottomLeft, m_nToBottom, 
		&memDC, 0, rcSrc.Height()-m_nToBottom, SRCCOPY);
	//bottomRight
	pDC->BitBlt(rcDest.right-m_nBottomRight, rcDest.Height()-m_nToBottom, m_nBottomRight, m_nToBottom, 
		&memDC, rcSrc.right-m_nBottomRight, rcSrc.Height()-m_nToBottom, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

void CSkinDialog::DrawCaption(CWnd* pWnd, CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bActive)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(bActive ? m_pBmpActiveBorder : m_pBmpInactiveBorder);
	if (pOldBitmap == NULL)
		return;

	//topLeft
	pDC->BitBlt(0, 0, m_nTopLeft, m_nToTop, 
		&memDC, 0, 0, SRCCOPY);
	//top
	CRect rc1(m_nTopLeft, 0, rcDest.Width()-m_nTopRight, m_nToTop);
	CRect rc2(m_nTopLeft, 0, rcSrc.Width()-m_nTopRight, m_nToTop);
	StretchDraw(pDC, &memDC, rc1, rc2);
//	pDC->StretchBlt(m_nTopLeft, 0, rcDest.Width()-m_nTopLeft-m_nTopRight, m_nToTop, 
//			&memDC, m_nTopLeft, 0, rcSrc.Width()-m_nTopLeft-m_nTopRight, m_nToTop, SRCCOPY);
	//topRight
	pDC->BitBlt(rcDest.right-m_nTopRight, 0, m_nTopRight, m_nToTop, 
		&memDC, rcSrc.right-m_nTopRight, 0, SRCCOPY);

	//text
	CString str;
	pWnd->GetWindowText( str );
	CFont *pOldFont = pDC->SelectObject( pWnd->GetFont() );

	if ( !bActive )
		pDC->SetTextColor(RGB(100,100,100));

	pDC->SetBkMode(TRANSPARENT);
	rcDest.left += m_nTopLeft;
	rcDest.right -= m_nTopRight;
	pDC->DrawText( str, rcDest, DT_SINGLELINE | DT_LEFT | DT_VCENTER );

	//CaptionButton
	DWORD dwStyle = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
	if (dwStyle & WS_SYSMENU)
	{
		UINT nHitTest = 0;
		UINT nButtonState = 0;
		if (m_nDownHitTest)
		{
			if (m_nDownHitTest == m_nMoveHitTest)
			{
				nHitTest = m_nDownHitTest;
				nButtonState = 1;
			}
		}
		else if (m_nMoveHitTest)
		{
			nHitTest = m_nMoveHitTest;
			nButtonState = 2;
		}
//		TRACE("\nnHitTest = %d\tnButtonState = %d", nHitTest, nButtonState);

		memDC.SelectObject(m_pBmpCloseBtn);
		CRect rc;
		GetCaptionButtonRect(pWnd, rc, SC_CLOSE);
		if (nHitTest == HTCLOSE)
			pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, rc.Width()*nButtonState, 0, SRCCOPY);
		else
			pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

		if ( dwStyle & WS_MAXIMIZEBOX )
		{
			if (pWnd->IsZoomed())
				memDC.SelectObject(m_pBmpRestoreBtn);
			else
				memDC.SelectObject(m_pBmpMaxBtn);

			GetCaptionButtonRect(pWnd, rc, SC_MAXIMIZE);
			if (nHitTest == HTMAXBUTTON)
				pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, rc.Width()*nButtonState, 0, SRCCOPY);
			else
				pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		}
		if ( dwStyle & WS_MINIMIZEBOX )
		{
			memDC.SelectObject(m_pBmpMinBtn);
			GetCaptionButtonRect(pWnd, rc, SC_MINIMIZE);
			if (nHitTest == HTMINBUTTON)
				pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, rc.Width()*nButtonState, 0, SRCCOPY);
			else
				pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		}
	}

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	pDC->SelectObject(pOldFont);
}

void CSkinDialog::OnShowWindow( HWND hWnd, BOOL bShow, UINT nStatus )
{
	TRACE("\nCSkinDialog::OnShowWindow()");
	//call SetWindowPos to force OnNcCalcSize when hWnd is a dialog
	if ( bShow )
	{
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		dwStyle &= ~WS_CAPTION;
		SetWindowLong( hWnd, GWL_STYLE, dwStyle );
		
		CRect rcWin;
		GetClientRect( hWnd, &rcWin );
		CWnd* pWnd = CWnd::FromHandle( hWnd ); 
		pWnd->ClientToScreen(&rcWin);
		rcWin.left -= m_nToLeft;
		rcWin.right += m_nToRight;
		rcWin.top -= m_nToTop;
		rcWin.bottom += m_nToBottom;
//		SetWindowPos( hWnd, 0, 0, 0, rcWin.Width(), rcWin.Height(), 
//			SWP_NOMOVE|SWP_FRAMECHANGED );
		SetWindowPos( hWnd, 0, rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(), 
			SWP_FRAMECHANGED );

		//使Dialog在第一次OnShow()的时候，任务栏上的按钮处于选中状态
		CallWindowProc( m_cWndProc, hWnd, WM_NCACTIVATE, WPARAM(bShow), LPARAM(nStatus) );  	
	}
}

void CSkinDialog::OnNcPaint(CWnd* pWnd)
{
	CDC* pDC = pWnd->GetWindowDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

//	BOOL bActive = (BOOL)GetWindowLong(pWnd->m_hWnd, GWL_USERDATA);
	BOOL bActive = TRUE;
	BITMAP bm;
	CBitmap* pBmpBorder = bActive ? m_pBmpActiveBorder : m_pBmpInactiveBorder;
	if (pBmpBorder == NULL)
		return;
	pBmpBorder->GetBitmap(&bm);

	CRect rcBmp;
	rcBmp.left = 0;
	rcBmp.right = bm.bmWidth;
	rcBmp.top = 0;
	rcBmp.bottom = bm.bmHeight;

	CRect rcWin;
	pWnd->GetWindowRect(&rcWin);
	rcWin -= CPoint(rcWin.left, rcWin.top);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap( pDC, rcWin.Width(), m_nToTop );
	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);

//	pDC->ExcludeClipRect(0, 0, rcWin.Width(), m_nToTop );
	DrawBorder(pDC, rcWin, rcBmp, bActive);
//	pDC->SelectClipRgn( NULL ); 

	rcWin.bottom = rcWin.top + m_nToTop;
	DrawCaption(pWnd, &memDC, rcWin, rcBmp, bActive);

	pDC->BitBlt(0,0,rcWin.Width(), rcWin.Height(), &memDC, 0,0,SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	pWnd->ReleaseDC(pDC);
}

BOOL CSkinDialog::OnNcActivate(CWnd* pWnd, BOOL bActive)
{
	TRACE("\nCSkinDialog::OnNcActivate()");
	if (m_nDownHitTest || m_nMoveHitTest)
	{
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
	}
	SetWindowLong(pWnd->m_hWnd, GWL_USERDATA, bActive);
//	OnNcPaint(pWnd);

	return !bActive;
}

void CSkinDialog::OnActivate(CWnd* pWnd, UINT nState, CWnd *pWndOther, BOOL bMinimized)
{
//	TRACE("\nCSkinDialog::OnActivate()");
	if (m_nDownHitTest || m_nMoveHitTest)
	{
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
	}
	BOOL bActive = ( nState == WA_ACTIVE || nState == WA_CLICKACTIVE );
//	OnNcPaint(pWnd);
	OnNcActivate(pWnd, bActive);
}

void CSkinDialog::OnNcDestroy(HWND hWnd)
{
	SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_cWndProc);
}

void CSkinDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	TRACE("\nCSkinDialog::OnNcCalcSize()");

	if ( bCalcValidRects )
	{
		lpncsp->rgrc[0].left += m_nToLeft;
		lpncsp->rgrc[0].right -= m_nToRight;
		lpncsp->rgrc[0].top += m_nToTop;
		lpncsp->rgrc[0].bottom -= m_nToBottom;
//		lpncsp->rgrc[1] = lpncsp->rgrc[0];
	}
//	else
//		CallWindowProc( m_cWndProc, hWnd, WM_NCCALCSIZE, WPARAM(bCalcValidRects), LPARAM(lpncsp) );  	
}

void CSkinDialog::OnSize( HWND hWnd, UINT nType, int cx, int cy )
{}

void CSkinDialog::GetCaptionButtonRect(CWnd* pWnd, CRect &rect, UINT nType)
{
	UINT nAlign;
	POINT ptOffset;
	CBitmap* pBitmap = NULL;
	BITMAP bm;
	switch (nType)
	{
	case SC_CLOSE:
		nAlign = m_nAlignClose;
		pBitmap = m_pBmpCloseBtn;
		ptOffset = m_ptClose;
		break;
	case SC_MAXIMIZE:
		nAlign = m_nAlignMax;
		pBitmap = m_pBmpMaxBtn;
		ptOffset = m_ptMax;
		break;
	case SC_MINIMIZE:
		nAlign = m_nAlignMin;
		pBitmap = m_pBmpMinBtn;
		ptOffset = m_ptMin;
		break;
	default:
		break;
	}
	if (NULL != pBitmap)
		pBitmap->GetBitmap(&bm);
	
	CRect rcWin;
	pWnd->GetWindowRect(&rcWin);

	// 右对齐
	if (nAlign)
	{
		rect.right = rcWin.Width() - ptOffset.x;
		rect.left = rect.right - bm.bmWidth/4;
	}
	// 左对齐
	else
	{
		rect.left = ptOffset.x;
		rect.right = rect.left + bm.bmWidth/4;
	}
	rect.top = ptOffset.y;
	rect.bottom = rect.top + bm.bmHeight;
}

UINT CSkinDialog::OnNcHitTest( CWnd* pWnd, CPoint point )
{
	CRect rcWin;
	pWnd->GetWindowRect(&rcWin);
	point -= CPoint(rcWin.left, rcWin.top);

	CRect rect;
	DWORD dwStyle = GetWindowLong(pWnd->m_hWnd, GWL_STYLE);
	if (dwStyle & WS_SYSMENU)
	{
		GetCaptionButtonRect( pWnd, rect, SC_CLOSE );
		if (PtInRect(rect, point))
			return HTCLOSE;
		
		if ( dwStyle & WS_MAXIMIZEBOX )
		{
			GetCaptionButtonRect( pWnd, rect, SC_MAXIMIZE );
			if (PtInRect(rect, point))
				return HTMAXBUTTON;
		}
		
		if ( dwStyle & WS_MINIMIZEBOX )
		{
			GetCaptionButtonRect( pWnd, rect, SC_MINIMIZE );
			if (PtInRect(rect, point))
				return HTMINBUTTON;
		}
	}

	CRect rcCaption(&rcWin);
	rcCaption -= CPoint(rcWin.left, rcWin.top);
	rcCaption.bottom = rcCaption.top + m_nToTop;
	// 需要去掉边框，暂时定义为4，实际上应由边框的类型来取得
	rcCaption.DeflateRect(4,4,4,4);
	if ( PtInRect( rcCaption, point ) )
		return HTCAPTION;

	return HTCLIENT;
}

BOOL CSkinDialog::OnNcLButtonDown( CWnd* pWnd, UINT nHitTest, CPoint point )
{
	if ( (HTMAXBUTTON == nHitTest) ||
		 (HTMINBUTTON == nHitTest) ||
		 (HTCLOSE == nHitTest) )
	{
		m_nDownHitTest = nHitTest;
		m_nMoveHitTest = nHitTest;

		pWnd->SetCapture();
		OnNcPaint(pWnd);
		return TRUE;
	}
	else if (m_nDownHitTest != 0)
	{
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
		OnNcPaint(pWnd);
	}

	return FALSE;
}

BOOL CSkinDialog::OnNcLButtonUp( CWnd* pWnd, UINT nHitTest, CPoint point )
{
	if (m_nDownHitTest != 0)
	{
		ReleaseCapture();

		if (m_nDownHitTest == nHitTest)
		switch (m_nDownHitTest)
		{
		case HTMAXBUTTON:
			if ( pWnd->IsZoomed() )
				pWnd->SendMessage( WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
			else
				pWnd->SendMessage( WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));
			return TRUE;

		case HTMINBUTTON:
			pWnd->SendMessage( WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(point.x, point.y));
			return TRUE;

		case HTCLOSE:
			pWnd->SendMessage( WM_SYSCOMMAND, SC_CLOSE, MAKELPARAM(point.x, point.y));
			return TRUE;

		default:
			return FALSE;
		}
	}

	return FALSE;
}

void CSkinDialog::OnNcLButtonDblClk( CWnd* pWnd, UINT nHitTest, CPoint point )
{}

void CSkinDialog::OnNcRButtonDown( CWnd* pWnd, UINT nHitTest, CPoint point )
{}

void CSkinDialog::OnNcRButtonUp( CWnd* pWnd, UINT nHitTest, CPoint point )
{
	TRACE("\nCSkinDialog::OnNcRButtonUp()");
	if ( HTCAPTION == nHitTest )
		PopupSysMenu(pWnd, point);
}

void CSkinDialog::OnNcMouseMove( CWnd* pWnd, UINT nHitTest, CPoint point )
{
	if ( (HTMAXBUTTON == nHitTest) ||
		 (HTMINBUTTON == nHitTest) ||
		 (HTCLOSE == nHitTest) )
	{
		m_nMoveHitTest = nHitTest;
		OnNcPaint(pWnd);
	}
	else if (m_nMoveHitTest != 0)
	{
		m_nMoveHitTest = 0;
		OnNcPaint(pWnd);
	}
}

void CSkinDialog::OnLButtonUp( CWnd* pWnd, UINT nFlag, CPoint point )
{
	CPoint pt = point;
	pWnd->ClientToScreen(&pt);
	OnNcLButtonUp(pWnd, OnNcHitTest(pWnd, pt), point);

	if (m_nDownHitTest || m_nMoveHitTest)
	{
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
		OnNcPaint(pWnd);
	}
}

void CSkinDialog::OnMouseMove( CWnd* pWnd, UINT nFlag, CPoint point )
{
	if (m_nDownHitTest || m_nMoveHitTest)
	{
		CPoint pt = point;
		pWnd->ClientToScreen(&pt);
		OnNcMouseMove(pWnd, OnNcHitTest(pWnd, pt), point);
	}
}

void CSkinDialog::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI )
{
	TRACE("\nCSkinDialog::OnGetMinMaxInfo()");

	int nWidth = m_nTopLeft+m_nTopRight;
	int nHeight = m_nToTop+m_nToBottom;
	if ( nWidth < lpMMI->ptMinTrackSize.x )
		nWidth = lpMMI->ptMinTrackSize.x;
	if ( nHeight < lpMMI->ptMinTrackSize.y )
		nHeight = lpMMI->ptMinTrackSize.y;

	lpMMI->ptMinTrackSize = CPoint( nWidth, nHeight );
}
