///////////////////////////////////////////////////////////////////////
//  Filename	SkinGroupBox.h
//	Class:		CSkinGroupBox
//
//	Compiler:	Visual C++ 6.0
//
//	Version:	1.0
//
//	Created:	2005/03/09
//	Updated:	2003/03/09
//
//	Author:		WYL
///////////////////////////////////////////////////////////////////////

#ifndef SKINGROUPBOX_INCLUDED
#define SKINGROUPBOX_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkinGroupBox : public CButton  
{
public:
	DECLARE_DYNAMIC(CSkinGroupBox)	
	CSkinGroupBox();
	virtual ~CSkinGroupBox();

// Attributes
protected:
	static CBitmap *m_pFrameBitmap;
	static CBitmap *m_pCaptionBitmap;
	static UINT m_nFrameSize;
	static UINT m_nToLeft;
	static UINT m_nToRight;
	static UINT m_nToTop;
	static UINT m_nToBottom;

protected:
	void StretchDraw(CDC* pDCDest, CDC* pDCSrc, CRect rcDest, CRect rcSrc);
	void DrawFrame(CDC* pDC, CRect rect, int nState);
	void DrawCaption(CDC* pDC, CRect rect, CString sTitle, int nState);

// Operations
public:
	static void SetBitmap(CBitmap *pCaptionBmp, CBitmap *pFrameBmp);
	static void SetFrameSize(UINT nFrameSize);
	static void SetCaptionMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom);

	//{{AFX_VIRTUAL(CSkinGroupBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinGroupBox)
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !SKINGROUPBOX_INCLUDED
