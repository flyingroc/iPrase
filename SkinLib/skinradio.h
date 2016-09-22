///////////////////////////////////////////////////////////////////////
//  Filename	SkinRadio.h
//	Class:		CSkinRadio
//
//	Compiler:	Visual C++ 6.0
//
//	Version:	1.0
//
//	Created:	2005/03/07
//	Updated:	2003/03/07
//
//	Author:		WYL
///////////////////////////////////////////////////////////////////////

#ifndef SKINRADIO_INCLUDED
#define SKINRADIO_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkinRadio : public CButton  
{
public:
	BOOL	m_bIsChecked;

public:
	DECLARE_DYNAMIC(CSkinRadio)	
	CSkinRadio();
	virtual ~CSkinRadio();

// Attributes
protected:
	static CBitmap *m_pBitmap;
	static CBitmap *m_pBkBitmap;
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
	void DrawRadioButton(CDC* pDC, CRect rect, int nState);

// Operations
public:
	static void SetBitmap(CBitmap *pBmp, CBitmap *pBkBmp);
	static void SetMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom);

	//{{AFX_VIRTUAL(CSkinRadio)
	public:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinRadio)
	afx_msg void OnNcDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	afx_msg LRESULT OnBMSetState(WPARAM, LPARAM);
	afx_msg LRESULT OnBMSetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnBMGetCheck(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

#endif // !SKINRADIO_INCLUDED
