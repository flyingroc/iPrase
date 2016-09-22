// GMDataParse.h : main header file for the GMDATAPARSE application
//

#if !defined(AFX_GMDATAPARSE_H__1017AD26_020D_4AF5_A99D_79D49A7D1946__INCLUDED_)
#define AFX_GMDATAPARSE_H__1017AD26_020D_4AF5_A99D_79D49A7D1946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGMDataParseApp:
// See GMDataParse.cpp for the implementation of this class
//

class CGMDataParseApp : public CWinApp
{
public:
	CGMDataParseApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGMDataParseApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGMDataParseApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GMDATAPARSE_H__1017AD26_020D_4AF5_A99D_79D49A7D1946__INCLUDED_)
