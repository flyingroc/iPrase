// DirectoryDlg.cpp: implementation of the CDirectoryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectoryDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirectoryDlg::CDirectoryDlg(){}
CDirectoryDlg::~CDirectoryDlg(){}

//返回路径名
CString CDirectoryDlg::GetDirectory( CWnd *pParent ,LPCSTR lpszRoot, LPCSTR lpszTitle)
{
	CString str;
	
	BROWSEINFO bi;
	bi.hwndOwner = pParent->m_hWnd; 
	bi.pidlRoot = 0;                  
	bi.pszDisplayName = 0;           
    bi.lpszTitle= lpszTitle; 
	bi.lpfn = BrowseCallbackProc; 
	bi.lParam = 0; 
	bi.ulFlags =BIF_RETURNONLYFSDIRS| BIF_STATUSTEXT;
	
	m_sRootDir = lpszRoot;
	
	LPITEMIDLIST lpItemId=::SHBrowseForFolder( &bi );//打开对话框,提示输入目录名
	if (lpItemId){
		LPTSTR szBuf = str.GetBuffer(MAX_PATH);
		::SHGetPathFromIDList(lpItemId, szBuf );//获取选择的目录
		::GlobalFree( lpItemId );
		str.ReleaseBuffer();
	}
	return str;
}
CString CDirectoryDlg::m_sRootDir="";
int CALLBACK BrowseCallbackProc( HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData)
{
	TCHAR buf[MAX_PATH] ;
	switch(uMsg)
	{
		// when dialog is first initialized, change directory to one chosen above
	case BFFM_INITIALIZED:
		strcpy(buf,CDirectoryDlg::m_sRootDir);
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)buf);
		break ;
		// if you picked BIF_STATUSTEXT above, you can fill status here
	case BFFM_SELCHANGED:
		if (::SHGetPathFromIDList((LPITEMIDLIST)lp,buf))
			SendMessage( hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)buf );
		break ;
	}
	return 0;
}