// SkinEdit.cpp : implementation file
//

#include "StdAfx.h"
//#include "	\ add additional includes here"
#include "SkinEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF CSkinEdit::m_crBack = 0;
CBitmap* CSkinEdit::m_pFrameBitmap = NULL;
UINT CSkinEdit::m_nFrameSize = 0;

/////////////////////////////////////////////////////////////////////////////
// CSkinEdit
IMPLEMENT_DYNAMIC(CSkinEdit, CEdit)

CSkinEdit::CSkinEdit()
{
}

CSkinEdit::~CSkinEdit()
{
}

BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
	//{{AFX_MSG_MAP(CSkinEdit)
	ON_CONTROL_REFLECT(EN_HSCROLL, OnHscroll)
	ON_CONTROL_REFLECT(EN_VSCROLL, OnVscroll)
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinEdit message handlers

void CSkinEdit::SetBkColor(COLORREF crBack)
{
	m_crBack = crBack;
}

void CSkinEdit::SetBitmap(CBitmap* pBmp)
{
	m_pFrameBitmap = pBmp;
}

void CSkinEdit::SetFrameSize(UINT nFrameSize)
{
	m_nFrameSize = nFrameSize;
}

void CSkinEdit::OnHscroll() 
{	
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_SCROLL flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void CSkinEdit::OnVscroll() 
{
	// TODO: Add your control notification handler code here
	
}

void CSkinEdit::OnNcDestroy() 
{
	UnsubclassWindow();
//	CEdit::OnNcDestroy();
	delete this;
}


void CSkinEdit::OnPaint() 
{
	Default();

	CDC* pDC = GetDC();
	int nSavedDC = pDC->SaveDC();
	CRect rcFrame;
	GetWindowRect(rcFrame);
	ScreenToClient(rcFrame);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(m_pFrameBitmap);

	BITMAP bm;
	m_pFrameBitmap->GetBitmap(&bm);

	// left-top
	pDC->BitBlt(rcFrame.left,rcFrame.top,m_nFrameSize,m_nFrameSize, 
				&memDC,0,0,SRCCOPY);
	// top
	pDC->StretchBlt(rcFrame.left+m_nFrameSize,rcFrame.top,rcFrame.Width()-2*m_nFrameSize,m_nFrameSize,
				&memDC,m_nFrameSize,0,bm.bmWidth-2*m_nFrameSize,m_nFrameSize,SRCCOPY);
	// right-top
	pDC->BitBlt(rcFrame.right-m_nFrameSize,rcFrame.top,m_nFrameSize,m_nFrameSize, 
				&memDC,bm.bmWidth-m_nFrameSize,0,SRCCOPY);
	// right
	pDC->StretchBlt(rcFrame.right-m_nFrameSize,rcFrame.top+m_nFrameSize,m_nFrameSize,rcFrame.Height()-2*m_nFrameSize,
				&memDC,bm.bmWidth-m_nFrameSize,m_nFrameSize,m_nFrameSize,bm.bmWidth-2*m_nFrameSize,SRCCOPY);
	// right-bottom
	pDC->BitBlt(rcFrame.right-m_nFrameSize,rcFrame.bottom-m_nFrameSize,m_nFrameSize,m_nFrameSize, 
				&memDC,bm.bmWidth-m_nFrameSize,bm.bmHeight-m_nFrameSize,SRCCOPY);
	// bottom
	pDC->StretchBlt(rcFrame.left+m_nFrameSize,rcFrame.bottom-m_nFrameSize,rcFrame.Width()-2*m_nFrameSize,m_nFrameSize,
				&memDC,m_nFrameSize,bm.bmHeight-m_nFrameSize,bm.bmWidth-2*m_nFrameSize,m_nFrameSize,SRCCOPY);
	// left-bottom
	pDC->BitBlt(rcFrame.left,rcFrame.bottom-m_nFrameSize,m_nFrameSize,m_nFrameSize, 
				&memDC,0,bm.bmHeight-m_nFrameSize,SRCCOPY);
	// left
	pDC->StretchBlt(rcFrame.left,rcFrame.top+m_nFrameSize,m_nFrameSize,rcFrame.Height()-2*m_nFrameSize,
				&memDC,0,m_nFrameSize,m_nFrameSize,bm.bmHeight-2*m_nFrameSize,SRCCOPY);

	memDC.SelectObject(&pOldBitmap);
	memDC.DeleteDC();

	pDC->RestoreDC( nSavedDC );
	ReleaseDC(pDC);
}
