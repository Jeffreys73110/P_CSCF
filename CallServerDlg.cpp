// CallServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CallServer.h"
#include "CallServerDlg.h"


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
	virtual BOOL OnInitDialog();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char szFileName[MAX_PATH];
	HINSTANCE hInstance = GetModuleHandle(NULL);
	GetModuleFileName(hInstance, szFileName, MAX_PATH);
	GetDlgItem(IDC_EDIT1)->SetWindowText(szFileName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CCallServerDlg dialog
//#include "CDD.h"

CCallServerDlg::CCallServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCallServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCallServerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CCallServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallServerDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCallServerDlg, CDialog)
	//{{AFX_MSG_MAP(CCallServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallServerDlg message handlers

BOOL CCallServerDlg::OnInitDialog()
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
	
	lpSIP_Server=NULL;
	m_Edit1="";
	// TODO: Add extra initialization here
	PostMessage(START_EVENT,NULL,NULL);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCallServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCallServerDlg::OnPaint() 
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
HCURSOR CCallServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCallServerDlg::DestroyWindow() 
{
	if (lpSIP_Server)	{ lpSIP_Server->DestroyWindow(); delete lpSIP_Server; lpSIP_Server=NULL;}
	return CDialog::DestroyWindow();
}

LRESULT CCallServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	int		i;
	CTime	Time1;
	char	S1[200];

    if (message==START_EVENT) // Do CDMS & Search_DNS
	{
		m_Edit1+="SIP Proxy Server\r\nSearching SQL .... ";
		// ==== Start CDMS Server ====
		m_Edit1+="Searching SQL .... ";
		Time1=CTime::GetCurrentTime();
		sprintf(S1,"Completely[%d.%d.%d-%d:%d]\r\n",Time1.GetYear(),Time1.GetMonth(),Time1.GetDay(),Time1.GetHour(),Time1.GetMinute());
		m_Edit1+=S1;
		// Start Server
		lpSIP_Server=new CSIP_Server();
		if (i=lpSIP_Server->Start())	{ m_Edit1+=lpSIP_Server->ErrMsg; m_Edit1+="\r\n"; }
		m_Edit1+="SIP Start OK\r\n";
		sprintf(S1,"Local IP = %s\r\n",lpSIP_Server->Local_IP); m_Edit1+=S1;
		GetDlgItem(IDC_BUTTON1)->SetWindowText(m_Edit1);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CCallServerDlg::OnButton1() 
{
	if (!lpSIP_Server) return;
	if (lpSIP_Server->IsWindowVisible())	lpSIP_Server->ShowWindow(SW_HIDE);
	else	lpSIP_Server->ShowWindow(SW_SHOW);
}
