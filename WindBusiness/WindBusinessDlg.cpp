
// WindBusinessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindBusiness.h"
#include "WindBusinessDlg.h"
#include "afxdialogex.h"
#include "constData.h"
#include <fstream>
#include "XTransCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindBusinessDlg 对话框



CWindBusinessDlg::CWindBusinessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINDBUSINESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindBusinessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);;
}

BEGIN_MESSAGE_MAP(CWindBusinessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_STOCK_CODE__DOWNLOAD, &CWindBusinessDlg::OnMenuStockCode)
	ON_COMMAND(ID_MENU_KLINE, &CWindBusinessDlg::OnMenuKline)
	
	ON_COMMAND(ID_MENU_CONNECT_WIND, &CWindBusinessDlg::OnMenuConnectWind)
	ON_COMMAND(ID_MENU_DISCONNECT_WIND, &CWindBusinessDlg::OnMenuDisconnectWind)
	ON_MESSAGE(UM__NOTIFY_REQ_KLINE, &CWindBusinessDlg::OnNotifyReqKLine)
	ON_MESSAGE(UM_NOTIFY_REQ_COMPLETE, &CWindBusinessDlg::OnNotifyReqComplete)
	ON_MESSAGE(UM_NOTIFY_REQ_LOADING, &CWindBusinessDlg::OnNotifyReqLoading)
	ON_MESSAGE(UM_NOTIFY_REQ_DOWNLOAD_STOCK_CODE, &CWindBusinessDlg::OnNotifyReqStockCode)
	
	ON_COMMAND(ID_MENU_EXIT, &CWindBusinessDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_GET_TRADE_DAY, &CWindBusinessDlg::OnMenuGetTradeDay)
	ON_COMMAND(ID_MENU_FILE_TXT, &CWindBusinessDlg::OnMenuFileTxt)
	ON_COMMAND(ID_MENU_SQLITE, &CWindBusinessDlg::OnMenuSqlite)
END_MESSAGE_MAP()


// CWindBusinessDlg 消息处理程序

BOOL CWindBusinessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_strTitle = _T("万得业务");

	// TODO: 在此添加额外的初始化代码
	SetWindowText(m_strTitle);

	theApp.m_dataHandle.setHWND(m_hWnd);

	if (!theApp.m_dataHandle.readStock())
	{
		MessageBox(_T("stockCode.txt文件读取失败"), _T("提示"));
	}

	if (!theApp.readConfig("WindFreeAPI.ini"))
	{
		MessageBox(_T("WindFreeAPI.ini 配置文件读取失败！"), _T("提示"));
	}

	m_dlgKLineDownload.Create(CKLineDownload::IDD, this);
	m_dlgDownloadCode.Create(CDownloadCode::IDD, this);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWindBusinessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if ((nID & 0xFFF0) == SC_CLOSE)
		{
			CString strPrompt;
			T_TaskStatus &tTaskStatus = theApp.getTaskStatus();
			if (tTaskStatus.enDownloadCode == EN_Processing ||
				tTaskStatus.enDownloadKLineData == EN_Processing)
			{
				strPrompt = _T("有任务正在执行，确定退出系统？");
			}
			else
			{
				strPrompt = _T("确定退出系统？");
			}

			if (MessageBox(strPrompt, _T("提示"), MB_OKCANCEL) == IDCANCEL)
			{
				return;
			}
		}

		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWindBusinessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWindBusinessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWindBusinessDlg::OnMenuStockCode()
{
	theApp.m_dataHandle.startDownloadStockCode();
}


void CWindBusinessDlg::OnMenuKline()
{
	T_TaskStatus &tTaskStatus = theApp.getTaskStatus();
	if (tTaskStatus.enDownloadKLineData != EN_Processing)
	{
		m_dlgKLineDownload.clear();
	}
	m_dlgKLineDownload.ShowWindow(SW_SHOW);
}


void CWindBusinessDlg::OnMenuConnectWind()
{
	SetWindowText(m_strTitle + _T("--正在连接万得服务器，请稍后..."));

	if (theApp.m_dataHandle.connectWind())
	{
		SetWindowText(m_strTitle +_T("--登录万得服务器成功！"));
	}
	else
	{
		SetWindowText(m_strTitle + _T("--登录万得服务器失败！"));
	}
}

void CWindBusinessDlg::OnMenuDisconnectWind()
{
	SetWindowText(m_strTitle + _T("--正在断开万得服务器，请稍后..."));

	if (theApp.m_dataHandle.disconnectWind())
	{
		SetWindowText(m_strTitle + _T("--已断开与万得服务器的连接！"));
	}
	else
	{
		SetWindowText(m_strTitle + _T("--与万得服务器连接断开失败！"));
	}
}

LRESULT CWindBusinessDlg::OnNotifyReqKLine(WPARAM wParam, LPARAM lParam)
{
	wchar_t* wszMsg = (wchar_t*)wParam;

	m_dlgKLineDownload.SetNotifyMsg(wszMsg);

	return 0;
}

LRESULT CWindBusinessDlg::OnNotifyReqComplete(WPARAM wParam, LPARAM lParam)
{
	std::vector<std::wstring> &vecDate = *(std::vector<std::wstring>*)wParam;

	std::vector<std::wstring>::iterator itDate;
	for (itDate = vecDate.begin(); itDate != vecDate.end(); ++itDate)
	{
		m_dlgKLineDownload.AddDate(itDate->c_str());
	}
	
	m_dlgKLineDownload.EnableControl(TRUE);


	return LRESULT();
}

LRESULT CWindBusinessDlg::OnNotifyReqLoading(WPARAM wParam, LPARAM lParam)
{
	wchar_t *szMsg = (wchar_t*)wParam;
	m_dlgKLineDownload.setLoadingStock(szMsg);

	return 0;
}

LRESULT CWindBusinessDlg::OnNotifyReqStockCode(WPARAM wParam, LPARAM lParam)
{
	wchar_t *wszMsg = (wchar_t*)wParam;
	m_dlgDownloadCode.SetNotifyMsg(wszMsg);
	if (!m_dlgDownloadCode.IsWindowVisible())
		m_dlgDownloadCode.ShowWindow(SW_SHOW);
	
	return 0;
}

void CWindBusinessDlg::OnMenuExit()
{
	if (MessageBox(_T("确定退出系统？"), _T("提示"), MB_OKCANCEL) == IDCANCEL)
	{
		SendMessage(WM_CLOSE, 0, 0);
	}
}


BOOL CWindBusinessDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;

		case VK_ESCAPE:
			return TRUE;

		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CWindBusinessDlg::OnMenuGetTradeDay()
{
	// TODO: 在此添加命令处理程序代码
}


void CWindBusinessDlg::OnMenuFileTxt()
{
	theApp.m_dataHandle.setSaveMode(EN_SAVE_FILE);
}


void CWindBusinessDlg::OnMenuSqlite()
{
	theApp.m_dataHandle.setSaveMode(EN_SAVE_SQLITE);
}
