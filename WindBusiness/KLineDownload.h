#pragma once
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"


// CKLineDownload 对话框

class CKLineDownload : public CDialogEx
{
	DECLARE_DYNAMIC(CKLineDownload)

public:
	CKLineDownload(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKLineDownload();

	int m_nLastPos;// 最后插入时间的下一个位置
	int m_nLastErrorPos;// 最后错误的下一个位置
	bool m_bListErrorShowHBar;

	CListBox m_listErrorInfo;
	CComboBox m_comboStockCode;
	CComboBox m_comboKLineType;
	CEdit m_editBeginDate;
	CEdit m_editEndDate;
	CListBox m_listDownloadTime;
	CProgressCtrl m_progressCtrl;

// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KLINEDOWNLOAD };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void clear();
	
	virtual BOOL OnInitDialog();
	void initControl();
	void SetNotifyMsg(wchar_t *strNofityMsg);
	void AddDate(const wchar_t *wszDate);
	void setLoadingStock(wchar_t *szStock);
	void EnableControl(BOOL bEnable);
	void initProgress();// 初始化进度条
	void setCurPos(int nPos);// 设置进度条的当前位置
	
	afx_msg void OnBnClickedBtnDownloadKline();
	afx_msg void OnBnClickedBtnSaveErrorInfoAsFile();
	afx_msg void OnBnClickedBtnSaveAsFile();

	
	
	afx_msg void OnBnClickedBtnCleanErrorInfo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
