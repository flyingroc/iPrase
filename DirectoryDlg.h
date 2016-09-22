// DirectoryDlg.h: interface for the CDirectoryDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WCDDIRDLG_H__DE813DEA_1E88_49D6_8431_008B0418E56E__INCLUDED_)
#define AFX_WCDDIRDLG_H__DE813DEA_1E88_49D6_8431_008B0418E56E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirectoryDlg  
{
public:
	CDirectoryDlg();
	virtual ~CDirectoryDlg();
	CString GetDirectory( CWnd *pParent ,LPCSTR lpszRoot ,LPCSTR lpszTitle);
	static CString m_sRootDir;
};
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData );

#endif // !defined(AFX_WCDDIRDLG_H__DE813DEA_1E88_49D6_8431_008B0418E56E__INCLUDED_)
