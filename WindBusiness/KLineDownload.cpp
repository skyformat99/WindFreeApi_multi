// KLineDownload.cpp : 实现文件
//

#include "stdafx.h"
#include "WindBusiness.h"
#include "KLineDownload.h"
#include "afxdialogex.h"
#include <string>
#include <fstream>


// CKLineDownload 对话框

IMPLEMENT_DYNAMIC(CKLineDownload, CDialogEx)

CKLineDownload::CKLineDownload(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KLINEDOWNLOAD, pParent)
{
	m_nLastPos = 0;
	m_nLastErrorPos = 0;
}

CKLineDownload::~CKLineDownload()
{
}

void CKLineDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ERROR_INFO, m_listErrorInfo);
	DDX_Control(pDX, IDC_COMBO_STOCK_CODE, m_comboStockCode);
	DDX_Control(pDX, IDC_COMBO_KLINE_TYPE, m_comboKLineType);
	DDX_Control(pDX, IDC_EDIT_BEGIN_DATE, m_editBeginDate);
	DDX_Control(pDX, IDC_EDIT_END_DATE, m_editEndDate);
	DDX_Control(pDX, IDC_LIST_DOWNLOAD_TIME, m_listDownloadTime);
	DDX_Control(pDX, IDC_PROGRESS, m_progressCtrl);
}


BEGIN_MESSAGE_MAP(CKLineDownload, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_KLINE, &CKLineDownload::OnBnClickedBtnDownloadKline)
	ON_BN_CLICKED(IDC_BTN_SAVE_ERROR_INFO_AS_FILE, &CKLineDownload::OnBnClickedBtnSaveErrorInfoAsFile)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_FILE, &CKLineDownload::OnBnClickedBtnSaveAsFile)
	ON_BN_CLICKED(IDC_BTN_CLEAN_ERROR_INFO, &CKLineDownload::OnBnClickedBtnCleanErrorInfo)
END_MESSAGE_MAP()


// CKLineDownload 消息处理程序


void CKLineDownload::clear()
{
	T_TaskStatus &tTaskStatus = theApp.getTaskStatus();
	m_nLastPos = 0;
	m_listErrorInfo.ResetContent();
	m_listDownloadTime.ResetContent();
	m_comboKLineType.SetCurSel(0);
	m_comboStockCode.SetCurSel(0);
	GetDlgItem(IDC_STATIC_LOADING)->SetWindowText(_T(""));

	CTime time = CTime::GetCurrentTime();
	CString date = time.Format(_T("%Y%m%d"));

	m_editBeginDate.SetWindowText(date);
	m_editEndDate.SetWindowText(date);
}

BOOL CKLineDownload::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	initControl();

	GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_HIDE);

	CenterWindow();

	m_bListErrorShowHBar = false;

	GetDlgItem(IDC_STATIC_LOADING)->SetWindowText(_T(""));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CKLineDownload::initControl()
{

	m_comboStockCode.AddString(_T("所有股票"));
	m_comboStockCode.SetCurSel(0);

	m_comboKLineType.AddString(_T("所有K线"));
	m_comboKLineType.AddString(_T("1分钟K线"));
	m_comboKLineType.AddString(_T("5分钟K线"));
	m_comboKLineType.AddString(_T("15分钟K线"));
	m_comboKLineType.AddString(_T("30分钟K线"));
	m_comboKLineType.AddString(_T("60分钟K线"));
	m_comboKLineType.AddString(_T("日K线"));
	m_comboKLineType.SetCurSel(0);

	CTime time = CTime::GetCurrentTime();
	CString date = time.Format(_T("%Y%m%d"));

	m_editBeginDate.SetWindowText(date);
	m_editEndDate.SetWindowText(date);
}

void CKLineDownload::OnBnClickedBtnDownloadKline()
{
	CString strStart, strEnd;
	m_editBeginDate.GetWindowText(strStart);
	m_editEndDate.GetWindowText(strEnd);

	std::wstring strStartTime = strStart;
	std::wstring strEndTime = strEnd;

	int pos = m_comboKLineType.GetCurSel();
	if (0 == pos)// 下载所有股票的K线数据
	{
		theApp.m_dataHandle.setMinuteIndexs(_T("high,low,open,close,volume,amt"));
		theApp.m_dataHandle.setDateIndexs(_T("high,low,volume,amt,open,close"));
		theApp.m_dataHandle.setDownloadTime(strStartTime, strEndTime);
		theApp.m_dataHandle.startDownloadKlineData();

		EnableControl(FALSE);

		initProgress();
	}
}

void CKLineDownload::SetNotifyMsg(wchar_t * strNofityMsg)
{
	if (strNofityMsg)
	{
		CString str(strNofityMsg);

		// 根据文本宽度，是否显示滚动条
		if (!m_bListErrorShowHBar)
		{
			CRect rt;
			m_listErrorInfo.GetWindowRect(rt);

			CDC *pDC = GetDC();
			CSize &cs = pDC->GetTextExtent(str);

			if (cs.cx > rt.Width())
			{
				m_listErrorInfo.SetHorizontalExtent(cs.cx);
				m_bListErrorShowHBar = true;
			}
		}
		
		m_listErrorInfo.AddString(strNofityMsg);
	}
}

void CKLineDownload::AddDate(const wchar_t * wszDate)
{
	if (wszDate)
	{
		m_listDownloadTime.AddString(wszDate);
	}
}


void CKLineDownload::OnBnClickedBtnSaveErrorInfoAsFile()
{

	std::ofstream outfile;
	outfile.open("errorInfo.txt", std::ios::app);
	if (!outfile.is_open())
	{
		return;
	}

	int nCount = m_listErrorInfo.GetCount();

	CString strSelAll = _T("");
	CString strText;

	int i;
	for (i = m_nLastErrorPos; i<nCount; i++)
	{
		m_listErrorInfo.GetText(i, strText);
		strSelAll += strText;
		strSelAll += _T("\r\n");
	}

	m_nLastErrorPos = i;

	if (!strSelAll.IsEmpty())
	{
		std::string strText = theApp.m_dataHandle.wstring_to_string(strSelAll.GetBuffer());
		outfile.write(strText.c_str(), strText.length());
	}

	outfile.close();
}

void CKLineDownload::OnBnClickedBtnSaveAsFile()
{
	std::ofstream outfile;
	outfile.open("downloadTime.txt", std::ios::app);
	if (!outfile.is_open())
	{
		return;
	}

	int nCount = m_listDownloadTime.GetCount();
	if (nCount > 0)
	{
		CString strSelAll = _T("");
		CString strText;
		int i;
		for (i = m_nLastPos; i < nCount; i++)
		{
			m_listDownloadTime.GetText(i, strText);   // 根据索引获得选择的字符串
			strSelAll += strText;
			strSelAll += _T("\r\n");
		}
		m_nLastPos = i;

		if (!strSelAll.IsEmpty())
		{
			std::string strText = theApp.m_dataHandle.wstring_to_string(strSelAll.GetBuffer());
			outfile.write(strText.c_str(), strText.length());
		}
	}

	outfile.close();
}

void CKLineDownload::setLoadingStock(wchar_t * szStock)
{
	if (szStock)
	{
		GetDlgItem(IDC_STATIC_LOADING)->SetWindowText(szStock);
	}
}

void CKLineDownload::EnableControl(BOOL bEnable)
{
	GetDlgItem(IDC_BTN_DOWNLOAD_KLINE)->EnableWindow(bEnable);
	m_editBeginDate.SetReadOnly(!bEnable);
	m_editEndDate.SetReadOnly(!bEnable);

	//if (!bEnable)
	//	GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_SHOW);
	//else
	//	GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_HIDE);
}

void CKLineDownload::initProgress()
{
	m_progressCtrl.SetRange(0, 100);
	m_progressCtrl.SetStep(1);
	m_progressCtrl.SetPos(0);
}

void CKLineDownload::setCurPos(int nPos)
{
	m_progressCtrl.SetPos(nPos);
}

void CKLineDownload::OnBnClickedBtnCleanErrorInfo()
{
	m_bListErrorShowHBar = false;
	m_listErrorInfo.ResetContent();
	m_listErrorInfo.SetHorizontalExtent(0);
}


BOOL CKLineDownload::PreTranslateMessage(MSG* pMsg)
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
