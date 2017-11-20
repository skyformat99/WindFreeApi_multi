
// WindBusinessDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CWindBusinessDlg �Ի���



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


// CWindBusinessDlg ��Ϣ�������

BOOL CWindBusinessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_strTitle = _T("���ҵ��");

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetWindowText(m_strTitle);

	theApp.m_dataHandle.setHWND(m_hWnd);

	if (!theApp.m_dataHandle.readStock())
	{
		MessageBox(_T("stockCode.txt�ļ���ȡʧ��"), _T("��ʾ"));
	}

	if (!theApp.readConfig("WindFreeAPI.ini"))
	{
		MessageBox(_T("WindFreeAPI.ini �����ļ���ȡʧ�ܣ�"), _T("��ʾ"));
	}

	m_dlgKLineDownload.Create(CKLineDownload::IDD, this);
	m_dlgDownloadCode.Create(CDownloadCode::IDD, this);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
				strPrompt = _T("����������ִ�У�ȷ���˳�ϵͳ��");
			}
			else
			{
				strPrompt = _T("ȷ���˳�ϵͳ��");
			}

			if (MessageBox(strPrompt, _T("��ʾ"), MB_OKCANCEL) == IDCANCEL)
			{
				return;
			}
		}

		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWindBusinessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	SetWindowText(m_strTitle + _T("--����������÷����������Ժ�..."));

	if (theApp.m_dataHandle.connectWind())
	{
		SetWindowText(m_strTitle +_T("--��¼��÷������ɹ���"));
	}
	else
	{
		SetWindowText(m_strTitle + _T("--��¼��÷�����ʧ�ܣ�"));
	}
}

void CWindBusinessDlg::OnMenuDisconnectWind()
{
	SetWindowText(m_strTitle + _T("--���ڶϿ���÷����������Ժ�..."));

	if (theApp.m_dataHandle.disconnectWind())
	{
		SetWindowText(m_strTitle + _T("--�ѶϿ�����÷����������ӣ�"));
	}
	else
	{
		SetWindowText(m_strTitle + _T("--����÷��������ӶϿ�ʧ�ܣ�"));
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
	if (MessageBox(_T("ȷ���˳�ϵͳ��"), _T("��ʾ"), MB_OKCANCEL) == IDCANCEL)
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
	// TODO: �ڴ���������������
}


void CWindBusinessDlg::OnMenuFileTxt()
{
	theApp.m_dataHandle.setSaveMode(EN_SAVE_FILE);
}


void CWindBusinessDlg::OnMenuSqlite()
{
	theApp.m_dataHandle.setSaveMode(EN_SAVE_SQLITE);
}
