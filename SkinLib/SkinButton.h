///////////////////////////////////////////////////////////////////////
//  Filename	SkinButton.h
//	Class:		CSkinButton
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

#ifndef SKINBUTTON_INCLUDED
#define SKINBUTTON_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSkinButton : public CButton
{
public:
	DECLARE_DYNAMIC(CSkinButton)	
	CSkinButton();
	virtual ~CSkinButton();

// Attributes
protected:
	static CBitmap *m_pBitmap;
	static UINT m_nToLeft;
	static UINT m_nToRight;
	static UINT m_nToTop;
	static UINT m_nToBottom;

	BOOL	m_bMouseOver;
	BOOL	m_bIsPressed;
	BOOL	m_bIsFocused;
	BOOL	m_bIsDisabled;

protected:
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	void DrawButton(CDC* pDC, CRect rcDest, CRect rcSrc);
	void DrawText(CDC* pDC, CRect rc);

// Operations
public:
	static void SetBitmap(CBitmap *pBmp);
	static void SetMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom);

	//{{AFX_VIRTUAL(CSkinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !SKINBUTTON_INCLUDED
