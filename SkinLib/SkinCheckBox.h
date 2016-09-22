///////////////////////////////////////////////////////////////////////
//  Filename	SkinCheckBox.h
//	Class:		CSkinCheckBox
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

#ifndef SKINCHECKBOX_INCLUDED
#define SKINCHECKBOX_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkinCheckBox : public CButton  
{
public:
	BOOL	m_bIsChecked;

public:
	DECLARE_DYNAMIC(CSkinCheckBox)	
	CSkinCheckBox();
	virtual ~CSkinCheckBox();

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
	void DrawCheckBox(CDC* pDC, CRect rect, int nState);

// Operations
public:
	static void SetBitmap(CBitmap *pBmp, CBitmap *pBkBmp);
	static void SetMargins(UINT nLeft, UINT nRight, UINT nTop, UINT nBottom);

	//{{AFX_VIRTUAL(CSkinCheckBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinCheckBox)
	afx_msg void OnNcDestroy();
	afx_msg void OnClicked();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnBMSetState(WPARAM, LPARAM);
	afx_msg LRESULT OnBMSetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnBMGetCheck(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

#endif // !SKINCHECKBOX_INCLUDED
