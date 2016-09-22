// SkinRadio.cpp: implementation of the CSkinRadio class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkinRadio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "Msimg32.lib")

CBitmap* CSkinRadio::m_pBitmap = NULL;
CBitmap* CSkinRadio::m_pBkBitmap = NULL;
UINT CSkinRadio::m_nToLeft = 0;
UINT CSkinRadio::m_nToRight = 0;
UINT CSkinRadio::m_nToTop = 0;
UINT CSkinRadio::m_nToBottom = 0;

/////////////////////////////////////////////////////////////////////////////
// CSkinRadio
IMPLEMENT_DYNAMIC(CSkinRadio, CButton)

CSkinRadio::CSkinRadio()
{
	m_bMouseOver	= FALSE;
	m_bIsPressed	= FALSE;
	m_bIsFocused	= FALSE;
	m_bIsDisabled	= FALSE;
	m_bIsChecked	= FALSE;
}

CSkinRadio::~CSkinRadio()
{

}

BEGIN_MESSAGE_MAP(CSkinRadio, CButton)
	//{{AFX_MSG_MAP(CSkinRadio)
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETSTATE , OnBMSetState)
	ON_MESSAGE(BM_SETCHECK , OnBMSetCheck)
	ON_MESSAGE(BM_GETCHECK , OnBMGetCheck)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinRadio message handlers

void CSkinRadio::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CSkinRadio::SetBitmap(CBitmap *pBmp, CBitmap *pBkBmp)
{
	m_pBitmap = pBmp;
	m_pBkBitmap = pBkBmp;
}

void CSkinRadio::SetMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom)
{
	m_nToLeft = nLeft;
	m_nToRight = nRight;
	m_nToTop = nTop;
	m_nToBottom = nBottom;
}

LRESULT CSkinRadio::OnBMSetState(WPARAM wParam, LPARAM lParam)
{
	return Default();
}

LRESULT CSkinRadio::OnBMSetCheck(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case BST_CHECKED:
		case BST_INDETERMINATE:	// Indeterminate state is handled like checked state
			ModifyStyle( 0, WS_TABSTOP );
			break;
		default:
			ModifyStyle( WS_TABSTOP, 0 );
			break;
	}

	m_bIsChecked = wParam;
	Invalidate(FALSE);

	return 0;
}

LRESULT CSkinRadio::OnBMGetCheck(WPARAM wParam, LPARAM lParam)
{
	TRACE("\nCSkinRadio::OnBMGetCheck()");

	return m_bIsChecked;
}

void CSkinRadio::OnNcDestroy() 
{
	Default();
	UnsubclassWindow();
	delete this;
}

void CSkinRadio::DrawRadioButton(CDC* pDC, CRect rect, int nState)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(m_pBitmap);
	BITMAP bm;
	m_pBitmap->GetBitmap(&bm);

	CBrush br;
	br.CreatePatternBrush(m_pBkBitmap);
	pDC->FillRect(&rect, &br);
	br.DeleteObject();

	int nWidth = bm.bmWidth/4;
	int nHeight = bm.bmHeight;
	
	CRect rcRadio = rect;
	CRect rcText = rect;
	UINT nStyle = GetWindowLong( m_hWnd, GWL_STYLE);
	if (nStyle & BS_LEFTTEXT)
	{
		rcRadio.left = rcRadio.right - nWidth;
		rcText.right -= rcRadio.Width();
	}
	else
	{
		rcRadio.right = rcRadio.left + nWidth;
		rcText.left += rcRadio.Width();
		rcText.left += 3;
	}

	UINT nAlign = DT_LEFT;
	if ((nStyle & BS_CENTER) == BS_CENTER)
	{
		nAlign = DT_CENTER;
	}
	else if ((nStyle & BS_RIGHT) == BS_RIGHT)
	{
		nAlign = DT_RIGHT;
		rcText.right -= 3;
	}
	UINT nVAlign = DT_VCENTER;
	rcRadio.top = (rect.Height()-nHeight)/2;
	if ((nStyle & BS_TOP) == BS_TOP)
	{
		nVAlign = DT_TOP;
		rcRadio.top = 0;
	}
	else if ((nStyle & BS_BOTTOM) == BS_BOTTOM)
	{
		nVAlign = DT_BOTTOM;
		rcRadio.top = rect.Height()-nHeight;
	}

	rcText.DeflateRect(1,1);
	rcRadio.bottom = rcRadio.top + nHeight;

	// draw the radio
	pDC->BitBlt(rcRadio.left, rcRadio.top, nWidth, nHeight,
					&memDC, nState*nWidth, 0, SRCCOPY);

	// draw the text
	CString str;
	GetWindowText( str );
	CFont *pOldFont = pDC->SelectObject( GetParent()->GetFont() );

	if ( m_bIsDisabled )
		pDC->SetTextColor(RGB(100,100,100));
//	else
//		pDC->SetTextColor(m_crtext);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText( str, rcText, DT_SINGLELINE|nAlign|nVAlign );

	pDC->SelectObject(pOldFont);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

BOOL CSkinRadio::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CSkinRadio::OnSetFocus(CWnd* pOldWnd) 
{
//	CButton::OnSetFocus(pOldWnd);

	CWnd * pParent = GetParent();
	CWnd * pLast = this;
	BOOL bIsGroup = FALSE;

	while ( TRUE )
	{
		if ( pLast == pOldWnd )
		{
			bIsGroup = TRUE;
			break;
		}

		pLast = pParent->GetNextDlgGroupItem( pLast, TRUE );

		if ( pLast == this )
			break;
	}

	if ( bIsGroup )
		SendMessage( WM_COMMAND, BN_CLICKED, (LPARAM)( GetSafeHwnd() ) );
}

void CSkinRadio::OnKillFocus(CWnd* pNewWnd) 
{
//	CButton::OnKillFocus(pNewWnd);
}

void CSkinRadio::OnMouseMove(UINT nFlags, CPoint point) 
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

LRESULT CSkinRadio::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
//	TRACE("\CSkinRadio::OnMouseLeave()");
	if (m_bMouseOver)
	{
		m_bMouseOver = FALSE;
		Invalidate(FALSE);
	}
	return 0;
}

void CSkinRadio::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

	CBitmap bmp;
	bmp.CreateCompatibleBitmap( pDC, rcButton.Width(), rcButton.Height() );
	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);

	int nState = 0;
	if (m_bIsDisabled) nState += 2;
	if (GetCheck() == BST_CHECKED) nState += 1;

	DrawRadioButton( &memDC, rcButton, nState );
	pDC->BitBlt(0,0,rcButton.Width(), rcButton.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(&pOldBitmap);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void CSkinRadio::OnClicked() 
{
	CWnd * pParent = GetParent();
	CWnd * pLast;
		
	pLast = this;
	pLast->SendMessage( BM_SETCHECK, BST_CHECKED );

	while ( TRUE )
	{
		pLast = pParent->GetNextDlgGroupItem( pLast, TRUE );

		if ( pLast == this )
			break;

		if ( (pLast->SendMessage(WM_GETDLGCODE, 0, 0L)) & DLGC_RADIOBUTTON )
			pLast->SendMessage( BM_SETCHECK, BST_UNCHECKED );
	}
}

UINT CSkinRadio::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();

	// Tell the system if we want default state handling
	// (losing default state always allowed)
	nCode |= DLGC_RADIOBUTTON;

	return nCode;
}
