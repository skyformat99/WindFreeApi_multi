#pragma once
#include "resource.h"

// CDownloadCode �Ի���

class CDownloadCode : public CDialogEx
{
	DECLARE_DYNAMIC(CDownloadCode)

public:
	CDownloadCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadCode();

	void SetNotifyMsg(wchar_t *wszMsg);

// �Ի�������
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWNLOADCODE };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnConfirm();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
