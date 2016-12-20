// ComTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ComTest.h"
#include "ComTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CComTestDlg dialog




CComTestDlg::CComTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CComTestDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST2, m_List);
	DDX_Control(pDX, IDC_LIST3, m_List10);
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CComTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SEND_DATA, &CComTestDlg::OnBnClickedSendData)
	ON_MESSAGE(UM_TEXT_OUT, &CComTestDlg::OnMsgStatusCtoListBox)
	ON_BN_CLICKED(IDC_OPEN_BUTTON1, &CComTestDlg::OnBnClickedOpenButton1)
	ON_BN_CLICKED(IDABORT, &CComTestDlg::OnBnClickedAbort)
	ON_MESSAGE(UM_GET_DATA, &CComTestDlg::OnMsgSensorDtoShareM)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT1, &CComTestDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CComTestDlg message handlers

BOOL CComTestDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	C3DM_GX3.GetCwndPoint((CWnd*)AfxGetMainWnd());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CComTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CComTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CComTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CComTestDlg::OnBnClickedSendData()
{
	// TODO: Add your control notification handler code here

	//C3DM_GX3.SendData();
}


LRESULT CComTestDlg::OnMsgStatusCtoListBox(WPARAM wParam, LPARAM lParam)
{	

	//리스트 박스에 상황을 기록하는 메세지함수
	CString str = (LPCTSTR)(LPSTR)wParam;
	m_List.AddString(str);

	return 0;
}

void CComTestDlg::OnBnClickedOpenButton1()
{
	// TODO: Add your control notification handler code here
	
	m_port1;
	m_port2;

	CString strPort = "COM4";
	int baudrate = 115200; //19200;
	int data = 8;
	int parity = 0;
	int stop = 0;//1;
	C3DM_GX3.messageID = UM_GET_DATA;
	C3DM_GX3.OpenPort2(strPort,baudrate,data,parity,stop);

	SetTimer(0,10,0);
	SetTimer(1,10,0);
}

void CComTestDlg::OnBnClickedAbort()
{
	// TODO: Add your control notification handler code here
	KillTimer(0);
	KillTimer(1);
	C3DM_GX3.ClosePort();
}

LRESULT CComTestDlg::OnMsgSensorDtoShareM(WPARAM wParam, LPARAM lParam)
{	

	//센서가 입력을 받았으니 공유 메모리에 기록하라는 메세지 함수

	CString str;
	str.Format("%lf %lf %lf", C3DM_GX3.ReceiveData.roll/3.14*180, C3DM_GX3.ReceiveData.pitch/3.14*180, C3DM_GX3.ReceiveData.yaw/3.14*180);
	m_List.AddString(str);


	//if line number of list is over than 5000, clear lines.
	int nCount = m_List.GetCount();
	if(nCount > 5000)
		m_List.ResetContent();
	if ( nCount > 0 )
		m_List.SetCurSel( nCount-1 );

	return 0;
}


void CComTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == 0)
	{
		C3DM_GX3.SendData();

	}


	if(nIDEvent == 1)
	{
		;
	}


	CDialog::OnTimer(nIDEvent);
}


void CComTestDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

#1015

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
