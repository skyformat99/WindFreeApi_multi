
// WindBusinessDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "KLineDownload.h"
#include "DownloadCode.h"


// CWindBusinessDlg 对话框
class CWindBusinessDlg : public CDialogEx
{
// 构造
public:
	CWindBusinessDlg(CWnd* pParent = NULL);	// 标准构造函数

	CString m_strTitle;
	CKLineDownload m_dlgKLineDownload;
	CDownloadCode m_dlgDownloadCode;
	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDBUSINESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
