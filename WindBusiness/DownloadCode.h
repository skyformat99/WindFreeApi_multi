#pragma once
#include "resource.h"

// CDownloadCode 对话框

class CDownloadCode : public CDialogEx
{
	DECLARE_DYNAMIC(CDownloadCode)

public:
	CDownloadCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadCode();

	void SetNotifyMsg(wchar_t *wszMsg);

// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWNLOADCODE };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnConfirm();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
