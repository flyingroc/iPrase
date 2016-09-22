// GMDataParseDlg.h : header file
//

#if !defined(AFX_GMDATAPARSEDLG_H__BB4B4B47_6B21_446E_B769_5A5129F3311C__INCLUDED_)
#define AFX_GMDATAPARSEDLG_H__BB4B4B47_6B21_446E_B769_5A5129F3311C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PraseEdit.h"
#include <list>

/////////////////////////////////////////////////////////////////////////////
// CGMDataParseDlg dialog
struct AppData
{
	int AppDataLen;
	unsigned char data[500];
};

class CGMDataParseDlg : public CDialog
{
// Construction
public:
	CGMDataParseDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGMDataParseDlg)
	enum { IDD = IDD_GMDATAPARSE_DIALOG };
	CComboBox	m_CBXProType;
	CString	m_StrInput;
	//}}AFX_DATA

	CPraseEdit    m_StrOutput;
	CString StrOutPut;

	std::list<AppData> appdata;
	//unsigned char str2byte[1000];
	//int ibytelen;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGMDataParseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	bool Str2Byte (CString inputstr);//将数字串转成报文数据

	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGMDataParseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnBtnParse();
	afx_msg void OnBtnClear();
	afx_msg void OnInputData();
	afx_msg void OnOutPut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GMDATAPARSEDLG_H__BB4B4B47_6B21_446E_B769_5A5129F3311C__INCLUDED_)
