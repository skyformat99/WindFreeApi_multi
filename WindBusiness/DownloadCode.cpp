// DownloadCode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DownloadCode.h"
#include "afxdialogex.h"


// CDownloadCode �Ի���

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


// CDownloadCode ��Ϣ�������


BOOL CDownloadCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
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
