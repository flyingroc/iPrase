// GMDataParseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GMDataParse.h"
#include "GMDataParseDlg.h"

#include "GMDataParseDlg.h"
#include "Protocol101.h"


//#include "directorydlg.h"

#include<LIST>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGMDataParseDlg dialog

CGMDataParseDlg::CGMDataParseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGMDataParseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGMDataParseDlg)
	m_StrInput = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGMDataParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGMDataParseDlg)	
	DDX_Control(pDX, IDC_COMBO_ProType, m_CBXProType);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_StrInput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGMDataParseDlg, CDialog)
	//{{AFX_MSG_MAP(CGMDataParseDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PARSE, OnBtnParse)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDCANCEL2, OnInputData)
	ON_BN_CLICKED(IDCANCEL3, OnOutPut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGMDataParseDlg message handlers

BOOL CGMDataParseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetWindowText("GMiParse");
	
	// TODO: Add extra initialization here
	m_StrOutput.SubclassDlgItem (IDC_EDIT_OUTPUT, this);

	m_CBXProType.AddString("02版101规约");
	m_CBXProType.AddString("97版101规约");	
	m_CBXProType.AddString("电力104规约");
	//m_CBXProType.AddString("光芒_网络规约");
	m_CBXProType.SetCurSel(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGMDataParseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGMDataParseDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGMDataParseDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGMDataParseDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CGMDataParseDlg::OnBtnParse() 
{
	// TODO: Add your control notification handler code here
	
	CString strText,strT;
	int AppNum = 1;
	AppData AppdataTemp;
	CProtocol02_101 Protocol02_101(1,1,2);
	CProtocol02_101 Protocol02_104(2,2,3);
	
	//CProtocolGMWLGY ProtocolGMWLGY;

	GetDlgItemText(IDC_EDIT_INPUT,strText);
	int protoclType = m_CBXProType.GetCurSel();
	appdata.clear();
	if (strText.GetLength()>0)
	{
		Str2Byte(strText);
	}
	while (appdata.size()>0)
	{
		AppdataTemp = appdata.front();
		appdata.pop_front();
		strT.Format("Input% 2d:\r\n",AppNum);
		for (int k = 0;k<AppdataTemp.AppDataLen;k++)
		{
			StrOutPut.Format("%s%02X ",strT,AppdataTemp.data[k]);
			strT = StrOutPut;
		}
		StrOutPut.Format("%s\r\nOutput% 2d:",strT,AppNum++);
		m_StrOutput.AppendString(StrOutPut);
		switch(protoclType)
		{
		case 0: 
			StrOutPut = Protocol02_101.Pmain02_101(AppdataTemp.data,AppdataTemp.AppDataLen);
			break;
		case 1:
			StrOutPut = Protocol02_101.Pmain02_101(AppdataTemp.data,AppdataTemp.AppDataLen);
			break;
		case 2:
			StrOutPut = Protocol02_104.Pmain104(AppdataTemp.data,AppdataTemp.AppDataLen);
			break;		
		case 3:			
			break;
		default:
			break;
		}
		m_StrOutput.AppendString (StrOutPut);
		strT = "\r\n";
		m_StrOutput.AppendString (strT);
	}
}

void CGMDataParseDlg::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_StrOutput.EditClear();
	SetDlgItemText (IDC_EDIT_INPUT, "");
}

bool CGMDataParseDlg::Str2Byte (CString inputstr)
{
	AppData app1data;
	char ChTemp;
	int charlen=0, datalen=0;
	int itemp;
	BYTE tempbytedata[3]={0};

	//inputstr = "25   adf  ea 32";
	int ilength = inputstr.GetLength();	
	char *p =(LPTSTR)(LPCTSTR)inputstr;
	/*
	字符间要加空格，如果没有空格认为2个字母代表一个数据，如果是汉字，抛掉
	*/
	for (int m=0;m<ilength;m++)
	{
		ChTemp = *p;
		if((ChTemp<123)&&(ChTemp>96))//将小写字母转成大写
		{
			ChTemp  = ChTemp-32;
		}
		if(((ChTemp<71)&&(ChTemp>64))||((ChTemp<58)&&(ChTemp>47))||(ChTemp==32))//提取数字
		{
			if(((ChTemp<71)&&(ChTemp>64))||((ChTemp<58)&&(ChTemp>47)))
			{
				switch(ChTemp)
				{
				case '0':itemp =0;
					break;
				case '1':itemp =1;
					break;
				case '2':itemp =2;
					break;
				case '3':itemp =3;
					break;
				case '4':itemp =4;
					break;
				case '5':itemp =5;
					break;
				case '6':itemp =6;
					break;
				case '7':itemp =7;
					break;
				case '8':itemp =8;
					break;
				case '9':itemp =9;
					break;
				case 'A':itemp =10;
					break;
				case 'B':itemp =11;
					break;
				case 'C':itemp =12;
					break;
				case 'D':itemp =13;
					break;
				case 'E':itemp =14;
					break;
				case 'F':itemp =15;
					break;
				default:
					break;
				}
				for (int i=charlen+1;i>0;i--)
				{
					tempbytedata[i] = tempbytedata[i-1];//拼成字节
				}			
				tempbytedata[0] = itemp;
				charlen++;
			}
			if (((ChTemp==32)&&charlen!=0)||(charlen==2))//去掉空格
			{
				charlen = 0;
				app1data.data[datalen++]= BYTE(tempbytedata[1]*16+tempbytedata[0]);//按16进制处理
				tempbytedata[0] = 0;
			}
		}
		else//非报文数据字符
		{
			if (datalen>3)//每条报文最短4个字节数据
			{
				app1data.AppDataLen=datalen;
				appdata.push_back(app1data);
				ZeroMemory(&app1data,sizeof(app1data));
				//memcpy(app1data.data,0,500);
			}
			charlen = 0;
			datalen =0;
		}		
		*p++;
	}
	if (datalen>3)//最后一条报文数据
	{
		app1data.AppDataLen=datalen;
		appdata.push_back(app1data);
		ZeroMemory(&app1data,sizeof(app1data));
		//memcpy(app1data.data,0,500);
	}
	datalen =0;
	return 1;
}

void CGMDataParseDlg::OnInputData() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg( RGB(255,0,0), CC_FULLOPEN, this );
//	dlg.DoModal();	

	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, NULL, this);
	FileDlg.DoModal();

    DWORD MAXFILE = 1024 * 1024;
    int nLength = 0;
    POSITION pos;
    CString  strFile;
    char szTmp[256], *p = NULL;
    char szFilter[] = "JPEG (*.JPG;*.JPEG;*.JPE;*.JFIF)|*.JPG;*.JPEG;*.JPE;*.JFIF|位图 (*.bmp)|*.bmp||";

	OPENFILENAME ofn;
	CHAR szFile[_MAX_PATH];
//	CHAR szFilter[] = "图片文件(*.gif; *.bmp; *.jpg)\0*.gif; *.bmp; *.jpg\0GIF文件(*.gif)\0*.gif\0BMP文件(*.bmp)\0*.bmp\0JPG文件(*.jpg)\0*.jpg\0\0";
	
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.Flags = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST;
	ofn.nMaxFile = sizeof(szFile);
	*szFile = '\0';
	ofn.lpstrFile = szFile;
	ofn.lpstrTitle = "选择图片";
	ofn.lpstrDefExt = "gif";
	ofn.lpstrFilter = szFilter;
//	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "所有文件 (*.*)|*.*||", this);
	

// 	if(!GetOpenFileName(&ofn))
// 		return;
	
    p = new char[MAXFILE];
    if(NULL == p)
    {
        this->MessageBox("分配内存失败！");
        return;
    }
    memset(p, 0x00, MAXFILE);

	
// 	CFileDialog FileDlg(TRUE, NULL, NULL, 
//         /*OFN_ALLOWMULTISELECT | OFN_EXPLORER |*/ OFN_READONLY , szFilter, NULL);

//	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "所有文件 (*.*)|*.*||", this);

	FileDlg.m_ofn.lpstrTitle = _T("open");	
    FileDlg.m_ofn.nMaxFile  = MAXFILE;
	FileDlg.m_ofn.hwndOwner = m_hWnd;
    FileDlg.m_ofn.lpstrFile = p;
	FileDlg.DoModal();
    if(IDOK != FileDlg.DoModal())
    {
        if(CommDlgExtendedError())
        {
            this->MessageBox("添加文件失败！");
        }
        delete []p;
        return;
    }
    pos = FileDlg.GetStartPosition();
    if(!pos)
    {
        delete []p;
        return;
    }
	
    while(true)
    {
        strFile = FileDlg.GetNextPathName(pos);
        if(!pos) break;
    }
    memset(szTmp, 0x00, sizeof(szTmp));
	
    delete []p;
}

void CGMDataParseDlg::OnOutPut() //将分析结果导出为文本文件
{
	// TODO: Add your control notification handler code here
	CString StrFilePath,StrFileName,StrFile;
	char Pathbuf[MAX_PATH];
	FILE * fp;
	_getcwd(Pathbuf,MAX_PATH);
	StrFilePath=Pathbuf;

	GetDlgItemText(IDC_EDIT_OUTPUT,StrFile);

	CTime m_t=CTime::GetCurrentTime();
	CString StrTime=m_t.Format("%Y年%m月%d日%H_%M_%S");

	StrFileName.Format(StrFilePath+"\\iParse"+StrTime+".txt");
	if (StrFile.GetLength()>0)
	{
		if((fp=fopen(StrFileName,"w"))==NULL)
		{
			return;
		}
		fprintf(fp,StrFile);
		fclose(fp);
		StrFilePath.Format("%s导出成功",StrFileName);
		m_StrOutput.AppendString (StrFilePath);
	}
}
