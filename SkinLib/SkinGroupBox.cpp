// SkinGroupBox.cpp: implementation of the CSkinGroupBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkinGroupBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBitmap* CSkinGroupBox::m_pCaptionBitmap = NULL;
CBitmap* CSkinGroupBox::m_pFrameBitmap = NULL;
UINT CSkinGroupBox::m_nFrameSize = 0;
UINT CSkinGroupBox::m_nToLeft = 0;
UINT CSkinGroupBox::m_nToRight = 0;
UINT CSkinGroupBox::m_nToTop = 0;
UINT CSkinGroupBox::m_nToBottom = 0;

/////////////////////////////////////////////////////////////////////////////
// CSkinGroupBox
IMPLEMENT_DYNAMIC(CSkinGroupBox, CButton)

CSkinGroupBox::CSkinGroupBox()
{
}

CSkinGroupBox::~CSkinGroupBox()
{
}

BEGIN_MESSAGE_MAP(CSkinGroupBox, CButton)
	//{{AFX_MSG_MAP(CSkinGroupBox)
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinGroupBox message handlers

void CSkinGroupBox::PreSubclassWindow() 
{
//	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CSkinGroupBox::SetBitmap(CBitmap *pCaptionBmp, CBitmap *pFrameBmp)
{
	m_pCaptionBitmap = pCaptionBmp;
	m_pFrameBitmap = pFrameBmp;
}

void CSkinGroupBox::SetFrameSize(UINT nFrameSize)
{
	m_nFrameSize = nFrameSize;
}

void CSkinGroupBox::SetCaptionMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom)
{
	m_nToLeft = nLeft;
	m_nToRight = nRight;
	m_nToTop = nTop;
	m_nToBottom = nBottom;
}

void CSkinGroupBox::OnNcDestroy() 
{
	Default();
	UnsubclassWindow();
	delete this;
}

void CSkinGroupBox::StretchDraw(CDC* pDCDest, CDC* pDCSrc, CRect rcDest, CRect rcSrc)
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

void CSkinGroupBox::DrawFrame(CDC* pDC, CRect rect, int nState)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(m_pFrameBitmap);

	BITMAP bm;
	m_pFrameBitmap->GetBitmap(&bm);
	// left-top
	pDC->BitBlt(0,rect.top,m_nFrameSize,m_nFrameSize, 
				&memDC,0,0,SRCCOPY);
	// top
	pDC->StretchBlt(m_nFrameSize,rect.top,rect.Width()-2*m_nFrameSize,m_nFrameSize,
				&memDC,m_nFrameSize,0,bm.bmWidth-2*m_nFrameSize,m_nFrameSize,SRCCOPY);
	// right-top
	pDC->BitBlt(rect.Width()-m_nFrameSize,rect.top,m_nFrameSize,m_nFrameSize, 
				&memDC,bm.bmWidth-m_nFrameSize,0,SRCCOPY);
	// right
	pDC->StretchBlt(rect.Width()-m_nFrameSize,rect.top+m_nFrameSize,m_nFrameSize,rect.Height()-2*m_nFrameSize,
				&memDC,bm.bmWidth-m_nFrameSize,m_nFrameSize,m_nFrameSize,bm.bmWidth-2*m_nFrameSize,SRCCOPY);
	// right-bottom
	pDC->BitBlt(rect.Width()-m_nFrameSize,rect.bottom-m_nFrameSize,m_nFrameSize,m_nFrameSize, 
				&memDC,bm.bmWidth-m_nFrameSize,bm.bmHeight-m_nFrameSize,SRCCOPY);
	// bottom
	pDC->StretchBlt(m_nFrameSize,rect.bottom-m_nFrameSize,rect.Width()-2*m_nFrameSize,m_nFrameSize,
				&memDC,m_nFrameSize,bm.bmHeight-m_nFrameSize,bm.bmWidth-2*m_nFrameSize,m_nFrameSize,SRCCOPY);
	// left-bottom
	pDC->BitBlt(0,rect.bottom-m_nFrameSize,m_nFrameSize,m_nFrameSize, 
				&memDC,0,bm.bmHeight-m_nFrameSize,SRCCOPY);
	// left
	pDC->StretchBlt(0,rect.top+m_nFrameSize,m_nFrameSize,rect.Height()-2*m_nFrameSize,
				&memDC,0,m_nFrameSize,m_nFrameSize,bm.bmHeight-2*m_nFrameSize,SRCCOPY);

	memDC.SelectObject(&pOldBitmap);
	memDC.DeleteDC();
}

void CSkinGroupBox::DrawCaption(CDC* pDC, CRect rect, CString sTitle, int nState)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(m_pCaptionBitmap);

	memDC.SelectObject(m_pCaptionBitmap);

	BITMAP bm;
	m_pCaptionBitmap->GetBitmap(&bm);

	/*******************BackGround*******************/

	// Corners
	pDC->BitBlt( rect.left, rect.top, m_nToLeft, m_nToTop,
		&memDC, 0, 0, SRCCOPY );
	pDC->BitBlt( rect.left, rect.bottom-m_nToBottom, m_nToLeft, m_nToBottom,
		&memDC, 0, bm.bmHeight-m_nToBottom, SRCCOPY );
	pDC->BitBlt( rect.right-m_nToRight, rect.top, m_nToRight, m_nToTop,
		&memDC, bm.bmWidth-m_nToRight, 0, SRCCOPY );
	pDC->BitBlt( rect.right-m_nToRight, rect.bottom-m_nToBottom, m_nToRight, m_nToBottom,
		&memDC, bm.bmWidth-m_nToRight, bm.bmHeight-m_nToBottom, SRCCOPY );

	// top
	pDC->StretchBlt( rect.left+m_nToLeft,rect.top,rect.Width()-m_nToLeft-m_nToRight, m_nToTop,
		&memDC, m_nToLeft,0,m_nToLeft, m_nToTop, SRCCOPY );
	// bottom
	pDC->StretchBlt( rect.left+m_nToLeft, rect.bottom-m_nToBottom, rect.Width()-m_nToLeft-m_nToRight, m_nToBottom,
		&memDC, m_nToLeft, bm.bmHeight-m_nToBottom, bm.bmWidth-m_nToLeft-m_nToRight, m_nToBottom, SRCCOPY );

	if (bm.bmHeight > rect.Height())
		bm.bmHeight = rect.Height();

	// left
	pDC->StretchBlt( rect.left,rect.top+m_nToTop,m_nToLeft, rect.Height()-m_nToTop-m_nToBottom,
		&memDC, 0,m_nToTop,m_nToLeft, bm.bmHeight-m_nToTop-m_nToBottom, SRCCOPY );
	// right
	pDC->StretchBlt( rect.right-m_nToRight,rect.top+m_nToTop,m_nToRight, rect.Height()-m_nToTop-m_nToBottom,
		&memDC, bm.bmWidth-m_nToRight,0+m_nToTop,m_nToRight, bm.bmHeight-m_nToTop-m_nToBottom, SRCCOPY );
	
	if (bm.bmWidth > rect.Width())
		bm.bmWidth = rect.Width();

	// middle
	pDC->StretchBlt( rect.left+m_nToLeft,rect.top+m_nToTop,rect.Width()-m_nToLeft-m_nToRight, rect.Height()-m_nToTop-m_nToBottom,
		&memDC, m_nToLeft, m_nToTop, bm.bmWidth-m_nToLeft-m_nToRight, bm.bmHeight-m_nToTop-m_nToBottom, SRCCOPY );
	/*******************BackGround*******************/

	// Text
//	if ( m_bIsDisabled )
//		pDC->SetTextColor(RGB(100,100,100));
//	else
//		pDC->SetTextColor(m_crtext);

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(sTitle, rect, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

	memDC.SelectObject(&pOldBitmap);
	memDC.DeleteDC();
}

void CSkinGroupBox::OnPaint() 
{
	TRACE("\nCSkinGroupBox::OnPaint()");
	Default();
	CDC* pDC = GetDC();
//	CPaintDC dc(this); // device context for painting
	
	CFont *pOldFont = pDC->SelectObject(GetFont());
	CString sTitle;
	GetWindowText(sTitle);
	CSize szText = pDC->GetTextExtent(sTitle);
	if (strstr(sTitle, "&")!=0)
		szText.cx -= pDC->GetTextExtent("&").cx;

	CRect rcFrame;
	GetClientRect(&rcFrame);

	DWORD dwStyle = GetStyle();
	int nTextAlign = dwStyle & 0x00f00;
	nTextAlign >>= 8;

	CRect rcText;
	// align left
	if (nTextAlign == 0 || nTextAlign == 1)
		rcText.left = 5;
	// center
	else if (nTextAlign == 3)
		rcText.left = (rcFrame.Width()-szText.cx)/2-4;
	// right
	else
		rcText.left = rcFrame.right-szText.cx-13;

	rcText.right = rcText.left+8+szText.cx;
	rcText.top = 0;
	rcText.bottom = rcText.top+6+szText.cy;

	rcFrame.top += rcText.Height()/2;

	DrawFrame(pDC, rcFrame, 0);

	DrawCaption(pDC, rcText, sTitle, 0);

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

BOOL CSkinGroupBox::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
