#pragma once
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"


// CKLineDownload �Ի���

class CKLineDownload : public CDialogEx
{
	DECLARE_DYNAMIC(CKLineDownload)

public:
	CKLineDownload(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKLineDownload();

	int m_nLastPos;// ������ʱ�����һ��λ��
	int m_nLastErrorPos;// ���������һ��λ��
	bool m_bListErrorShowHBar;

	CListBox m_listErrorInfo;
	CComboBox m_comboStockCode;
	CComboBox m_comboKLineType;
	CEdit m_editBeginDate;
	CEdit m_editEndDate;
	CListBox m_listDownloadTime;
	CProgressCtrl m_progressCtrl;

// �Ի�������
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KLINEDOWNLOAD };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void clear();
	
	virtual BOOL OnInitDialog();
	void initControl();
	void SetNotifyMsg(wchar_t *strNofityMsg);
	void AddDate(const wchar_t *wszDate);
	void setLoadingStock(wchar_t *szStock);
	void EnableControl(BOOL bEnable);
	void initProgress();// ��ʼ��������
	void setCurPos(int nPos);// ���ý������ĵ�ǰλ��
	
	afx_msg void OnBnClickedBtnDownloadKline();
	afx_msg void OnBnClickedBtnSaveErrorInfoAsFile();
	afx_msg void OnBnClickedBtnSaveAsFile();

	
	
	afx_msg void OnBnClickedBtnCleanErrorInfo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
