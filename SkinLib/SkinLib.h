#pragma once

class CSkinLib
{
public:
	CSkinLib(void);
	~CSkinLib(void);
	CBitmap* LoadBitmap(LPCTSTR lpRelativeFileName);
	void Clear();
public:
	UINT        StrToUINT(LPCTSTR lpValue);
	// Button
	CBitmap*    m_pButtonBitmap;
	CBitmap*    m_pRadioBitmap;
	CBitmap*    m_pCheckBoxBitmap;
	CBitmap*	m_pBackgroundBitmap;
	// Dialog
	CBitmap*    m_pDlgActiveBitmap;
	CBitmap*    m_pDlgInactiveBitmap;
	CBitmap*    m_pMaxBtnBitmap;
	CBitmap*    m_pMinBtnBitmap;
	CBitmap*    m_pRestoreBtnBitmap;
	CBitmap*    m_pCloseBtnBitmap;
	// Menu
	CBitmap*    m_pMenuBitmap;
	CBitmap*    m_pMenuBackBitmap;

protected:
	char m_AppPath[MAX_PATH];
//	char m_SkinPath[MAX_PATH];
	LPTSTR m_SkinPath;
public:
	BOOL LoadSkin(LPCTSTR lpStyleFileName);
	void GetFullPathName(LPCTSTR lpRelativeFile, char* Buf);
	BOOL ExitSkin();
};
