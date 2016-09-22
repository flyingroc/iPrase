///////////////////////////////////////////////////////////////////////
//  Filename	SkinEdit.h
//	Class:		CSkinEdit
//
//	Compiler:	Visual C++ 6.0
//
//	Version:	1.0
//
//	Created:	2005/03/14
//	Updated:	2003/03/14
//
//	Author:		WYL
///////////////////////////////////////////////////////////////////////

#ifndef SKINEDIT_INCLUDED
#define SKINEDIT_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSkinEdit : public CEdit
{
// Construction
public:
	DECLARE_DYNAMIC(CSkinEdit)
	CSkinEdit();
	virtual ~CSkinEdit();

// Attributes
protected:
	static COLORREF m_crBack;
	static CBitmap *m_pFrameBitmap;
	static UINT		m_nFrameSize;

// Operations
public:
	static void SetBkColor(COLORREF crBack);
	static void SetBitmap(CBitmap* pBmp);
	static void SetFrameSize(UINT nFrameSize);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinEdit)
	afx_msg void OnHscroll();
	afx_msg void OnVscroll();
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !SKINEDIT_INCLUDED
