/*
*  PraseEdit.h
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

#ifndef _PraseEdit_h_
#define _PraseEdit_h_

/////////////////////////////////////////////////////////////////////////////
// CPraseEdit window

class CPraseEdit : public CEdit
{
	// Construction
public:
	CPraseEdit();
	
	// Attributes
public:
	
	// Operations
public:
	void  AppendString (CString str);
	CString GetEditStr();
	bool EditClear();
	BOOL  IsSelectable() { return m_bSelectable; }
	void  AllowSelection (BOOL bAllowSelect) { m_bSelectable = bAllowSelect; }
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPraseEdit)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CPraseEdit();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CPraseEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	BOOL  m_bSelectable;                          // flag: user can select text in control
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

// End HistoryEdit.h