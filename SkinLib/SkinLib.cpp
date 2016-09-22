#include "StdAfx.h"
#include <Shlwapi.h>

#include "SkinLib.h"
#include "SkinDialog.h"
#include "SkinButton.h"
#include "SkinRadio.h"
#include "SkinCheckBox.h"
#include "SkinGroupBox.h"
#include "SkinEdit.h"

#pragma comment(lib, "Shlwapi.lib")

HHOOK		g_hWndProcHook = NULL;

CSkinLib::CSkinLib(void)
{
	m_pButtonBitmap = NULL;
	m_pRadioBitmap = NULL;
	m_pCheckBoxBitmap = NULL;
	m_pBackgroundBitmap = NULL;
	m_pDlgActiveBitmap = NULL;
	m_pDlgInactiveBitmap = NULL;
	m_pMaxBtnBitmap = NULL;
	m_pMinBtnBitmap = NULL;
	m_pRestoreBtnBitmap = NULL;
	m_pCloseBtnBitmap = NULL;
	m_pMenuBitmap = NULL;
	m_pMenuBackBitmap = NULL;
}

CSkinLib::~CSkinLib(void)
{
	Clear();
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam)
{
	char szTemp[MAX_PATH];
	GetClassName(hWnd, szTemp, sizeof(szTemp));
	CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
	if ( pWnd != NULL)
	{
		if (lstrcmpi(szTemp, "#32770") == 0)
			CSkinDialog::UseSkin(pWnd, BOOL(lParam));
		else// if(lstrcmpi(szTemp, "Button") == 0)
			pWnd->Invalidate(FALSE);
	}

	// 枚举子窗体
	EnumChildWindows(hWnd, EnumWndProc, lParam);

	return TRUE;
}

void CSkinLib::Clear()
{
	if(m_pButtonBitmap != NULL)
	{
		m_pButtonBitmap->DeleteObject();
		delete m_pButtonBitmap;
		m_pButtonBitmap = NULL;
	}
	if(m_pRadioBitmap != NULL)
	{
		m_pRadioBitmap->DeleteObject();
		delete m_pRadioBitmap;
		m_pRadioBitmap = NULL;
	}
	if(m_pCheckBoxBitmap != NULL)
	{
		m_pCheckBoxBitmap->DeleteObject();
		delete m_pCheckBoxBitmap;
		m_pCheckBoxBitmap = NULL;
	}
	if(m_pBackgroundBitmap != NULL)
	{
		m_pBackgroundBitmap->DeleteObject();
		delete m_pBackgroundBitmap;
		m_pBackgroundBitmap = NULL;
	}
	if(m_pDlgActiveBitmap != NULL)
	{
		m_pDlgActiveBitmap->DeleteObject();
		delete m_pDlgActiveBitmap;
		m_pDlgActiveBitmap = NULL;
	}
	if(m_pDlgInactiveBitmap != NULL)
	{
		m_pDlgInactiveBitmap->DeleteObject();
		delete m_pDlgInactiveBitmap;
		m_pDlgInactiveBitmap = NULL;
	}
	if(m_pMaxBtnBitmap != NULL)
	{
		m_pMaxBtnBitmap->DeleteObject();
		delete m_pMaxBtnBitmap;
		m_pMaxBtnBitmap = NULL;
	}
	if(m_pMinBtnBitmap != NULL)
	{
		m_pMinBtnBitmap->DeleteObject();
		delete m_pMinBtnBitmap;
		m_pMinBtnBitmap = NULL;
	}
	if(m_pRestoreBtnBitmap != NULL)
	{
		m_pRestoreBtnBitmap->DeleteObject();
		delete m_pRestoreBtnBitmap;
		m_pRestoreBtnBitmap = NULL;
	}
	if(m_pCloseBtnBitmap != NULL)
	{
		m_pCloseBtnBitmap->DeleteObject();
		delete m_pCloseBtnBitmap;
		m_pCloseBtnBitmap = NULL;
	}
	if(m_pMenuBitmap != NULL)
	{
		m_pMenuBitmap->DeleteObject();
		delete m_pMenuBitmap;
		m_pMenuBitmap = NULL;
	}
	if(m_pMenuBackBitmap != NULL)
	{
		m_pMenuBackBitmap->DeleteObject();
		delete m_pMenuBackBitmap;
		m_pMenuBackBitmap = NULL;
	}
}

VOID BeginSubclassing( HWND hWnd )
{
	char szTemp[MAX_PATH];
	static char szClass[][32] = 
	{
		"Button",					// 按钮类
		"Edit",						// 编辑框类
		"ComboBox",					// 组合框类
		"ListBox",					// 列表框
		"ScrollBar",
		"Static",
		"#32770",
		"msctls_hotkey32",			// 热键
		"SysIPAddress32",			// IP 地址
		"SysListView32",			// 列表查看
		"SysTreeView32",			// 树形查看
		"SysDateTimePick32",		// 日期/时间选择
		"msctls_updown32",			// 旋转
		"SysMonthCal32",			// 月历
		"SysTabControl32",			// TAB 控件
		"msctls_progress32",		// 进度条
		"msctls_trackbar32",		// 追踪条
	};
	// 查找判断匹配的类名称
	GetClassName(hWnd, szTemp, sizeof(szTemp));
	for (DWORD lReturn = 0; lReturn < (sizeof(szClass) / sizeof(szClass[0])); lReturn++)
		if (lstrcmpi(szTemp, szClass[lReturn]) == 0)
			break;

	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	switch (lReturn)
	{
	case 0:
		{
			CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
			if ( pWnd != NULL)
				return;

			dwStyle &= SS_TYPEMASK;

			switch(dwStyle)
			{
			case BS_OWNERDRAW:
				break;
			case BS_DEFPUSHBUTTON:		// 默认按钮
			case BS_PUSHBUTTON:			// 普通按钮
				{
					CSkinButton *pButton = new CSkinButton();
					pButton->SubclassWindow( hWnd );
				}
				break;
			case BS_CHECKBOX:			// 复选框
			case BS_AUTOCHECKBOX:		// 自动复选框
			case BS_3STATE:				// 三状态复选框
			case BS_AUTO3STATE:			// 自动三状态复选框
				{
					CSkinCheckBox *pButton = new CSkinCheckBox();
					pButton->SubclassWindow( hWnd );
				}
				break;
			case BS_RADIOBUTTON:		// 单选框
			case BS_AUTORADIOBUTTON:	// 自动单选框
				{
					CSkinRadio *pButton = new CSkinRadio();
					pButton->SubclassWindow( hWnd );
				}
				break;
			case BS_GROUPBOX:
				{
					CSkinGroupBox *pButton = new CSkinGroupBox();
					pButton->SubclassWindow( hWnd );
				}
				break;
			default:
				break;
			}
		}
		break;
	case 1:
		{
// 			CSkinEdit *pButton = new CSkinEdit();
// 			pButton->SubclassWindow( hWnd );
		}
		break;
	case 6:
		{
			CSkinDialog::m_cWndProc = (WNDPROC) SetWindowLong(hWnd, GWL_WNDPROC, (LONG) CSkinDialog::WindowProc);
		}
		break;
	}		
}

// 窗口消息 HOOK 回调函数
LRESULT CALLBACK WndProcHook(int iCode, WPARAM wParam, LPARAM lParam)
{
	if (iCode == HC_ACTION)
	{
		switch (((CWPSTRUCT*) lParam)->message)
		{
		case WM_CREATE:
			BeginSubclassing(((CWPSTRUCT*) lParam)->hwnd);
			break;
			
		default:
			break;
		}
	}
	
	return CallNextHookEx(g_hWndProcHook, iCode, wParam, lParam);
}

BOOL CSkinLib::LoadSkin(LPCTSTR lpSkinFile)
{
	Clear();

	CString sFile = lpSkinFile;
	m_SkinPath = sFile.GetBuffer(MAX_PATH);
	PathRemoveFileSpec(m_SkinPath);
	PathAddBackslash(m_SkinPath);
	sFile.ReleaseBuffer();

	char szBitmap[MAX_PATH];

	// PUSHBUTTON
	GetPrivateProfileString( "Buttons", "bitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pButtonBitmap = LoadBitmap(szBitmap);
	if (m_pButtonBitmap == NULL)	return FALSE;

	// RADIOBUTTON
	GetPrivateProfileString( "RadioButton", "bitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pRadioBitmap = LoadBitmap(szBitmap);
	if (m_pRadioBitmap == NULL)	return FALSE;

	// CHECKBOX
	GetPrivateProfileString( "CheckBox", "bitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pCheckBoxBitmap = LoadBitmap(szBitmap);
	if (m_pCheckBoxBitmap == NULL)	return FALSE;

	// BACKGROUND
	GetPrivateProfileString( "RadioButton", "background", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pBackgroundBitmap = LoadBitmap(szBitmap);
	if (m_pBackgroundBitmap == NULL)	return FALSE;

	// EDIT
	GetPrivateProfileString( "Edit", "backcolor", "", szBitmap, MAX_PATH, lpSkinFile );
	int nColor = (UINT)atol(szBitmap);
	CSkinEdit::SetBkColor(nColor);

	// MENU
	GetPrivateProfileString( "Menu", "bitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pMenuBitmap = LoadBitmap(szBitmap);
	if (m_pMenuBitmap == NULL)	return FALSE;

	GetPrivateProfileString( "Menu", "background", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pMenuBackBitmap = LoadBitmap(szBitmap);
	if (m_pMenuBackBitmap == NULL)	return FALSE;

	// DIALOG
	GetPrivateProfileString( "Dialog", "border_active", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pDlgActiveBitmap = LoadBitmap(szBitmap);
	if (m_pDlgActiveBitmap == NULL)		return FALSE;

	GetPrivateProfileString( "Dialog", "border_inactive", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pDlgInactiveBitmap = LoadBitmap(szBitmap);
	if (m_pDlgInactiveBitmap == NULL)	return FALSE;

	GetPrivateProfileString( "MaxBtn", "maxbitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pMaxBtnBitmap = LoadBitmap(szBitmap);
	if (m_pMaxBtnBitmap == NULL)	return FALSE;

	GetPrivateProfileString( "MaxBtn", "restorebitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pRestoreBtnBitmap = LoadBitmap(szBitmap);
	if (m_pRestoreBtnBitmap == NULL)	return FALSE;

	GetPrivateProfileString( "MinBtn", "bitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pMinBtnBitmap = LoadBitmap(szBitmap);
	if (m_pMinBtnBitmap == NULL)	return FALSE;

	GetPrivateProfileString( "CloseBtn", "bitmap", "", szBitmap, MAX_PATH, lpSkinFile );
	m_pCloseBtnBitmap = LoadBitmap(szBitmap);
	if (m_pCloseBtnBitmap == NULL)	return FALSE;

	UINT nTopLeft = GetPrivateProfileInt( "Dialog", "topLeft", 0, lpSkinFile );
	UINT nTopRight = GetPrivateProfileInt( "Dialog", "topRight", 0, lpSkinFile );
	UINT nBottomLeft = GetPrivateProfileInt( "Dialog", "bottomLeft", 0, lpSkinFile );
	UINT nBottomRight = GetPrivateProfileInt( "Dialog", "bottomRight", 0, lpSkinFile );

	UINT nLeft = GetPrivateProfileInt( "Dialog", "toLeft", 0, lpSkinFile );
	UINT nRight = GetPrivateProfileInt( "Dialog", "toRight", 0, lpSkinFile );
	UINT nTop = GetPrivateProfileInt( "Dialog", "toTop", 0, lpSkinFile );
	UINT nBottom = GetPrivateProfileInt( "Dialog", "toBottom", 0, lpSkinFile );

	CSkinDialog::SetBorderBitmap(m_pDlgActiveBitmap, m_pDlgInactiveBitmap);
	CSkinDialog::SetCaptionBtnBitmap(m_pMaxBtnBitmap, m_pMinBtnBitmap, m_pRestoreBtnBitmap, m_pCloseBtnBitmap);
	CSkinDialog::SetMargins(nTopLeft, nTopRight, nBottomLeft, nBottomRight, 
							nLeft, nRight, nTop, nBottom);

	POINT ptOffset;
	UINT nAlign = GetPrivateProfileInt( "MaxBtn", "align", 1, lpSkinFile );
	ptOffset.x = GetPrivateProfileInt( "MaxBtn", "XOffset", 0, lpSkinFile );
	ptOffset.y = GetPrivateProfileInt( "MaxBtn", "YOffset", 0, lpSkinFile );
	CSkinDialog::SetMaxButton(nAlign, ptOffset, m_pMaxBtnBitmap, m_pRestoreBtnBitmap);

	nAlign = GetPrivateProfileInt( "MinBtn", "align", 1, lpSkinFile );
	ptOffset.x = GetPrivateProfileInt( "MinBtn", "XOffset", 0, lpSkinFile );
	ptOffset.y = GetPrivateProfileInt( "MinBtn", "YOffset", 0, lpSkinFile );
	CSkinDialog::SetMinButton(nAlign, ptOffset, m_pMinBtnBitmap);

	nAlign = GetPrivateProfileInt( "CloseBtn", "align", 1, lpSkinFile );
	ptOffset.x = GetPrivateProfileInt( "CloseBtn", "XOffset", 0, lpSkinFile );
	ptOffset.y = GetPrivateProfileInt( "CloseBtn", "YOffset", 0, lpSkinFile );
	CSkinDialog::SetCloseButton(nAlign, ptOffset, m_pCloseBtnBitmap);

	nLeft = GetPrivateProfileInt( "Buttons", "toLeft", 0, lpSkinFile );
	nRight = GetPrivateProfileInt( "Buttons", "toRight", 0, lpSkinFile );
	nTop = GetPrivateProfileInt( "Buttons", "toTop", 0, lpSkinFile );
	nBottom = GetPrivateProfileInt( "Buttons", "toBottom", 0, lpSkinFile );

	CSkinButton::SetBitmap(m_pButtonBitmap);
	CSkinButton::SetMargins(nLeft, nRight, nTop, nBottom);

	CSkinRadio::SetBitmap(m_pRadioBitmap, m_pBackgroundBitmap);
	CSkinCheckBox::SetBitmap(m_pCheckBoxBitmap, m_pBackgroundBitmap);
	CSkinGroupBox::SetBitmap(m_pMenuBitmap, m_pMenuBackBitmap);
	CSkinGroupBox::SetFrameSize(3);
	CSkinGroupBox::SetCaptionMargins(3,3,3,3);

	CSkinEdit::SetBitmap(m_pMenuBackBitmap);
	CSkinEdit::SetFrameSize(2);

	EnumThreadWindows(GetCurrentThreadId(), EnumWndProc, TRUE);

	if ( NULL == g_hWndProcHook )
		g_hWndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, WndProcHook, 0, ::GetCurrentThreadId());

	return TRUE;
}

BOOL CSkinLib::ExitSkin()
{
	BOOL bReturn = FALSE;
	EnumThreadWindows(GetCurrentThreadId(), EnumWndProc, FALSE);

	bReturn = UnhookWindowsHookEx(g_hWndProcHook);
	g_hWndProcHook = NULL;

	CSkinDialog::m_cWndProc = NULL;

	return bReturn;
}

CBitmap* CSkinLib::LoadBitmap(LPCTSTR lpRelativeFileName)
{
	if(lpRelativeFileName == NULL)
		return NULL;

	char StyleFileName[MAX_PATH];
	ZeroMemory(StyleFileName,MAX_PATH);
	strcpy(StyleFileName,m_SkinPath);
	strcat(StyleFileName,lpRelativeFileName);
	HANDLE hHandle = LoadImage(NULL,StyleFileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hHandle == NULL)
		return NULL;

	CBitmap* pBitmap = new CBitmap();
	if(!pBitmap->Attach(hHandle))
	{
		DeleteObject(hHandle);
		return NULL;
	}
	return pBitmap;
}

UINT CSkinLib::StrToUINT(LPCTSTR lpValue)
{
	if(lpValue == NULL)return 0;
	return (UINT)atol(lpValue);
}

void CSkinLib::GetFullPathName(LPCTSTR lpRelativeFile, char* Buf)
{
	if(lpRelativeFile == NULL)return;

	ZeroMemory(Buf,MAX_PATH);
	strcpy(Buf,m_SkinPath);
	strcat(Buf,lpRelativeFile);
}
