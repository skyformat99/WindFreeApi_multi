// DownloadCode.cpp : 实现文件
//

#include "stdafx.h"
#include "DownloadCode.h"
#include "afxdialogex.h"


// CDownloadCode 对话框

IMPLEMENT_DYNAMIC(CDownloadCode, CDialogEx)

CDownloadCode::CDownloadCode(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DOWNLOADCODE, pParent)
{

}

CDownloadCode::~CDownloadCode()
{
}

void CDownloadCode::SetNotifyMsg(wchar_t * wszMsg)
{
	if (wszMsg)
	{
		GetDlgItem(IDC_STATIC_DOWNLOAD_STATUS)->SetWindowTextW(wszMsg);
	}
}

void CDownloadCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDownloadCode, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CONFIRM, &CDownloadCode::OnBnClickedBtnConfirm)
END_MESSAGE_MAP()


// CDownloadCode 消息处理程序


BOOL CDownloadCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDownloadCode::OnBnClickedBtnConfirm()
{
	CDialog::OnOK();
}


BOOL CDownloadCode::PreTranslateMessage(MSG* pMsg)
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
