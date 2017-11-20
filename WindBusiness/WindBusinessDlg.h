
// WindBusinessDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "KLineDownload.h"
#include "DownloadCode.h"


// CWindBusinessDlg �Ի���
class CWindBusinessDlg : public CDialogEx
{
// ����
public:
	CWindBusinessDlg(CWnd* pParent = NULL);	// ��׼���캯��

	CString m_strTitle;
	CKLineDownload m_dlgKLineDownload;
	CDownloadCode m_dlgDownloadCode;
	

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDBUSINESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuStockCode();
	afx_msg void OnMenuKline();
	afx_msg void OnMenuConnectWind();
	afx_msg void OnMenuDisconnectWind();
	afx_msg LRESULT OnNotifyReqKLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyReqComplete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyReqLoading(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyReqStockCode(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnMenuExit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuGetTradeDay();
	afx_msg void OnMenuFileTxt();
	afx_msg void OnMenuSqlite();
};
