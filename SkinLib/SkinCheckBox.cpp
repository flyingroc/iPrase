// SkinCheckBox.cpp: implementation of the CSkinCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkinCheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBitmap* CSkinCheckBox::m_pBitmap = NULL;
CBitmap* CSkinCheckBox::m_pBkBitmap = NULL;
UINT CSkinCheckBox::m_nToLeft = 0;
UINT CSkinCheckBox::m_nToRight = 0;
UINT CSkinCheckBox::m_nToTop = 0;
UINT CSkinCheckBox::m_nToBottom = 0;

/////////////////////////////////////////////////////////////////////////////
// CSkinCheckBox
IMPLEMENT_DYNAMIC(CSkinCheckBox, CButton)

CSkinCheckBox::CSkinCheckBox()
{
	m_bMouseOver	= FALSE;
	m_bIsPressed	= FALSE;
	m_bIsFocused	= FALSE;
	m_bIsDisabled	= FALSE;
	m_bIsChecked	= FALSE;
}

CSkinCheckBox::~CSkinCheckBox()
{

}

BEGIN_MESSAGE_MAP(CSkinCheckBox, CButton)
	//{{AFX_MSG_MAP(CSkinCheckBox)
	ON_WM_NCDESTROY()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETSTATE , OnBMSetState)
	ON_MESSAGE(BM_SETCHECK , OnBMSetCheck)
	ON_MESSAGE(BM_GETCHECK , OnBMGetCheck)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinCheckBox message handlers

void CSkinCheckBox::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

LRESULT CSkinCheckBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDBLCLK)
		message = WM_LBUTTONDOWN;

	return CButton::DefWindowProc(message, wParam, lParam);
}

void CSkinCheckBox::SetBitmap(CBitmap *pBmp, CBitmap *pBkBmp)
{
	m_pBitmap = pBmp;
	m_pBkBitmap = pBkBmp;
}

void CSkinCheckBox::SetMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom)
{
	m_nToLeft = nLeft;
	m_nToRight = nRight;
	m_nToTop = nTop;
	m_nToBottom = nBottom;
}

LRESULT CSkinCheckBox::OnBMSetState(WPARAM wParam, LPARAM lParam)
{
	return Default();
}

LRESULT CSkinCheckBox::OnBMSetCheck(WPARAM wParam, LPARAM lParam)
{
/*	CString s;
	GetWindowText(s);
	TRACE("\n%s", s);
	TRACE("CSkinCheckBox::OnBMSetCheck()");
*/
	switch (wParam)
	{
		case BST_CHECKED:
		case BST_INDETERMINATE:	// Indeterminate state is handled like checked state
			m_bIsChecked = BST_CHECKED;
			break;
		default:
			m_bIsChecked = BST_UNCHECKED;
			break;
	}
	Invalidate(FALSE);
	return 0;
}

LRESULT CSkinCheckBox::OnBMGetCheck(WPARAM wParam, LPARAM lParam)
{
	return m_bIsChecked;
}

void CSkinCheckBox::OnNcDestroy() 
{
	Default();
	UnsubclassWindow();
	delete this;
}

void CSkinCheckBox::DrawCheckBox(CDC* pDC, CRect rect, int nState)
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
	pDC->BitBlt(0, 0, nWidth, nHeight,
					&memDC, nState*nWidth, 0, SRCCOPY);

	// draw the text
	rect.left += nWidth+4;
	rect.top += 1;
	CString str;
	GetWindowText( str );
	CFont *pOldFont = pDC->SelectObject( GetParent()->GetFont() );

	if ( m_bIsDisabled )
		pDC->SetTextColor(RGB(100,100,100));
//	else
//		pDC->SetTextColor(m_crtext);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText( str, rect, DT_LEFT | DT_SINGLELINE | DT_TOP );

	pDC->SelectObject(pOldFont);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

void CSkinCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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

	DrawCheckBox( &memDC, rcButton, nState );
	pDC->BitBlt(0,0,rcButton.Width(), rcButton.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(&pOldBitmap);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void CSkinCheckBox::OnClicked() 
{
	// TODO: Add your control notification handler code here
	TRACE("\nCSkinCheckBox::OnClicked()");

	SetCheck( (GetCheck()==BST_CHECKED)? BST_UNCHECKED : BST_CHECKED);
}

void CSkinCheckBox::OnMouseMove(UINT nFlags, CPoint point) 
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

LRESULT CSkinCheckBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
//	TRACE("\CSkinCheckBox::OnMouseLeave()");
	if (m_bMouseOver)
	{
		m_bMouseOver = FALSE;
		Invalidate(FALSE);
	}
	return 0;
}
