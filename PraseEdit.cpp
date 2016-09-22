/*
*  PraseEdit.cpp
*
*  Description:
*    可以通过函数调用在编辑框中添加数据
*
*  Author:
*    haipper
*
*  Revision History:
*      Original version
*/


#include "stdafx.h"
#include "PraseEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPraseEdit

CPraseEdit::CPraseEdit()
{
	m_bSelectable = FALSE;
}

CPraseEdit::~CPraseEdit()
{
}

BEGIN_MESSAGE_MAP(CPraseEdit, CEdit)
//{{AFX_MSG_MAP(CPraseEdit)
ON_WM_SETFOCUS()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPraseEdit operations

void CPraseEdit::AppendString(CString str)
{
	CString   strBuffer;    // current contents of edit control
	// Append string
	GetWindowText (strBuffer);
	if (!strBuffer.IsEmpty())
		strBuffer += "\r\n";
	strBuffer += str;
	SetWindowText (strBuffer);
	
	// Scroll the edit control
	LineScroll (GetLineCount(), 0);
}

/////////////////////////////////////////////////////////////////////////////
// CPraseEdit message handlers

void CPraseEdit::OnSetFocus(CWnd* pOldWnd) 
{
	// Don't allow user to select text
	if (m_bSelectable)
		CEdit::OnSetFocus (pOldWnd);
	else
		pOldWnd->SetFocus();
}

CString CPraseEdit::GetEditStr()
{
	CString   strBuffer;    // current contents of edit control
	GetWindowText (strBuffer);
	return strBuffer;
}

bool CPraseEdit::EditClear()
{
	CString strBuffer = "";
	SetWindowText (strBuffer);
	// Scroll the edit control
	LineScroll (0, 0);
	return TRUE;
}