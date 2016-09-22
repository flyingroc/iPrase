// SkinButton.cpp : implementation file
//

#include "StdAfx.h"
#include "SkinButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBitmap* CSkinButton::m_pBitmap = NULL;
UINT CSkinButton::m_nToLeft = 0;
UINT CSkinButton::m_nToRight = 0;
UINT CSkinButton::m_nToTop = 0;
UINT CSkinButton::m_nToBottom = 0;

/////////////////////////////////////////////////////////////////////////////
// CSkinButton
IMPLEMENT_DYNAMIC(CSkinButton, CButton)

CSkinButton::CSkinButton()
{
	m_bMouseOver	= FALSE;
	m_bIsPressed	= FALSE;
	m_bIsFocused	= FALSE;
	m_bIsDisabled	= FALSE;
}

CSkinButton::~CSkinButton()
{
}

BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	//{{AFX_MSG_MAP(CSkinButton)
	ON_WM_MOUSEMOVE()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinButton message handlers
LRESULT CSkinButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDBLCLK)
		message = WM_LBUTTONDOWN;

	return CButton::DefWindowProc(message, wParam, lParam);
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);
	
	CDC*    pDC      = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT    state    = lpDrawItemStruct->itemState;
	CRect   rcButton = lpDrawItemStruct->rcItem;

	m_bIsPressed = (state & ODS_SELECTED);
	m_bIsFocused  = (state & ODS_FOCUS);
	m_bIsDisabled = (state & ODS_DISABLED);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	BITMAP bm;
	m_pBitmap->GetBitmap(&bm);

	CRect rcBmp;
	if (m_bIsDisabled)
	{
		rcBmp = CRect(bm.bmWidth/5, 0, 2*bm.bmWidth/5, bm.bmHeight);
	}
	else if (m_bIsPressed)
	{
//		TRACE("1");
		rcBmp = CRect(4*bm.bmWidth/5, 0, bm.bmWidth, bm.bmHeight);
	}
	else if (m_bMouseOver)
	{
//		TRACE("2");
		rcBmp = CRect(3*bm.bmWidth/5, 0, 4*bm.bmWidth/5, bm.bmHeight);
	}
	else if ( m_bIsFocused || (state & ODS_DEFAULT) )
	{
//		TRACE("3");
		rcBmp = CRect(3*bm.bmWidth/5, 0, 4*bm.bmWidth/5, bm.bmHeight);
	}
	else
	{
//		TRACE("4");
		rcBmp = CRect(2*bm.bmWidth/5, 0, 3*bm.bmWidth/5, bm.bmHeight);
	}
	CBitmap bmp;
	bmp.CreateCompatibleBitmap( pDC, rcButton.Width(), rcButton.Height() );
	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);

	DrawButton(&memDC, rcButton, rcBmp);
	CRect rcText(&rcButton);
	rcText.DeflateRect(m_nToLeft/2,m_nToTop/2,m_nToRight/2,m_nToBottom/2);
	DrawText(&memDC, rcText);

	pDC->BitBlt(0,0,rcButton.Width(), rcButton.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(&pOldBitmap);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void CSkinButton::DrawButton(CDC* pDC, CRect rcDest, CRect rcSrc)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = memDC.SelectObject(m_pBitmap);
	if (pOldBitmap == NULL)
		return;

	// Corners
	pDC->BitBlt( 0, 0, m_nToLeft, m_nToTop,
		&memDC, rcSrc.left, rcSrc.top, SRCCOPY );
	pDC->BitBlt( 0, rcDest.bottom-m_nToBottom, m_nToLeft, m_nToBottom,
		&memDC, rcSrc.left, rcSrc.bottom-m_nToBottom, SRCCOPY );
	pDC->BitBlt( rcDest.right-m_nToRight, 0, m_nToRight, m_nToTop,
		&memDC, rcSrc.right-m_nToRight, rcSrc.top, SRCCOPY );
	pDC->BitBlt( rcDest.right-m_nToRight, rcDest.bottom-m_nToBottom, m_nToRight, m_nToBottom,
		&memDC, rcSrc.right-m_nToRight, rcSrc.bottom-m_nToBottom, SRCCOPY );

	// top
	pDC->StretchBlt( m_nToLeft,0,rcDest.Width()-m_nToLeft-m_nToRight, m_nToTop,
		&memDC, rcSrc.left+m_nToLeft,rcSrc.top,m_nToLeft, m_nToTop, SRCCOPY );
	// bottom
	pDC->StretchBlt( m_nToLeft, rcDest.Height()-m_nToBottom, rcDest.Width()-m_nToLeft-m_nToRight, m_nToBottom,
		&memDC, rcSrc.left+m_nToLeft, rcSrc.Height()-m_nToBottom, rcSrc.Width()-m_nToLeft-m_nToRight, m_nToBottom, SRCCOPY );

	if (rcSrc.Height() > rcDest.Height())
		rcSrc.bottom = rcSrc.top + rcDest.Height();

	// left
	pDC->StretchBlt( 0,m_nToTop,m_nToLeft, rcDest.Height()-m_nToTop-m_nToBottom,
		&memDC, rcSrc.left,rcSrc.top+m_nToTop,m_nToLeft, rcSrc.Height()-m_nToTop-m_nToBottom, SRCCOPY );
	// right
	pDC->StretchBlt( rcDest.right-m_nToRight,m_nToTop,m_nToRight, rcDest.Height()-m_nToTop-m_nToBottom,
		&memDC, rcSrc.right-m_nToRight,rcSrc.top+m_nToTop,m_nToRight, rcSrc.Height()-m_nToTop-m_nToBottom, SRCCOPY );
	
	if (rcSrc.Width() > rcDest.Width())
		rcSrc.right = rcSrc.left + rcDest.Width();

	// middle
	pDC->StretchBlt( m_nToLeft,m_nToTop,rcDest.Width()-m_nToLeft-m_nToRight, rcDest.Height()-m_nToTop-m_nToBottom,
		&memDC, rcSrc.left+m_nToLeft, rcSrc.top+m_nToTop, rcSrc.Width()-m_nToLeft-m_nToRight, rcSrc.Height()-m_nToTop-m_nToBottom, SRCCOPY );

	memDC.SelectObject(&pOldBitmap);
	memDC.DeleteDC();
}

void CSkinButton::DrawText(CDC* pDC, CRect rc)
{
	CString str;
	GetWindowText( str );
	CFont *pOldFont = pDC->SelectObject( GetParent()->GetFont() );

	if ( m_bIsDisabled )
		pDC->SetTextColor(RGB(100,100,100));
//	else
//		pDC->SetTextColor(m_crtext);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText( str, rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER );

	pDC->SelectObject(pOldFont);
}

void CSkinButton::PreSubclassWindow() 
{
	ModifyStyle(0, BS_OWNERDRAW);
	
	CButton::PreSubclassWindow();
}

void CSkinButton::SetBitmap(CBitmap* pBmp)
{
	m_pBitmap = pBmp;
}

void CSkinButton::SetMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom)
{
	m_nToLeft = nLeft;
	m_nToRight = nRight;
	m_nToTop = nTop;
	m_nToBottom = nBottom;
}

void CSkinButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;

	CButton::OnMouseMove(nFlags, point);

	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOver == FALSE) return;

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_bMouseOver)
		{
			m_bMouseOver = TRUE;
			Invalidate(FALSE);

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		} // if
	} 
	else if (m_bMouseOver)
	{
		m_bMouseOver = FALSE;
		Invalidate(FALSE);
	}
}

LRESULT CSkinButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
//	TRACE("\nCSkinButton::OnMouseLeave()");
	if (m_bMouseOver)
	{
		m_bMouseOver = FALSE;
		Invalidate(FALSE);
	}
	return 0;
}

void CSkinButton::OnNcDestroy() 
{
	Default();
	UnsubclassWindow();
	delete this;
}
