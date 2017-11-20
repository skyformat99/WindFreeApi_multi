
// WindBusiness.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "WindBusiness.h"
#include "WindBusinessDlg.h"
#include <SimpleIni.h>
#include <boost/algorithm/string.hpp> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWindBusinessApp

BEGIN_MESSAGE_MAP(CWindBusinessApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWindBusinessApp ����

CWindBusinessApp::CWindBusinessApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

bool CWindBusinessApp::readConfig(const char * szFileName)
{
	if (!szFileName)
		return false;

	CSimpleIni ini;
	SI_Error err = ini.LoadFile(szFileName);
	if (err < 0)
		return false;

	std::wstring wstrKLineType = ini.GetValue(_T("wind"), _T("klineType"), _T("0,1,5,15,30,60"));
	m_tConfigInfo.wstrDateKlineFileName = ini.GetValue(_T("data"), _T("dateKline"), _T("dateKline.txt"));
	m_tConfigInfo.wstrPath = ini.GetValue(_T("data"), _T("path"), _T("d:\\QData\\2017\\"));
	m_tConfigInfo.wstrBeginTime = ini.GetValue(_T("data"), _T("beginTime"), _T("20170101"));
	m_tConfigInfo.wstrEndTime = ini.GetValue(_T("data"), _T("endTime"), _T("20170831"));
	m_tConfigInfo.PriceAdj = ini.GetValue(_T("data"), _T("PriceAdj"), _T("N"));
	
	int pos = wstrKLineType.find(' ');
	wstrKLineType = wstrKLineType.substr(0,pos);
	boost::split(m_tConfigInfo.vecKLine, wstrKLineType, boost::is_any_of(_T(",")));

	pos = m_tConfigInfo.wstrDateKlineFileName.find(' ');
	m_tConfigInfo.wstrDateKlineFileName = m_tConfigInfo.wstrDateKlineFileName.substr(0, pos);

	pos = m_tConfigInfo.wstrPath.find(' ');
	m_tConfigInfo.wstrPath = m_tConfigInfo.wstrPath.substr(0, pos);

	pos = m_tConfigInfo.wstrBeginTime.find(' ');
	m_tConfigInfo.wstrBeginTime = m_tConfigInfo.wstrBeginTime.substr(0, pos);

	pos = m_tConfigInfo.wstrEndTime.find(' ');
	m_tConfigInfo.wstrEndTime = m_tConfigInfo.wstrEndTime.substr(0, pos);

	pos = m_tConfigInfo.PriceAdj.find(' ');
	m_tConfigInfo.PriceAdj = L"PriceAdj=" + m_tConfigInfo.PriceAdj.substr(0, pos);

	return true;
}

void CWindBusinessApp::setTaskStatus(const char * szTask, EN_TASK_STATUS enTaskStatus)
{
	if (strcmp("DownloadCode", szTask) == 0)
	{
		m_tTaskStatus.enDownloadCode = enTaskStatus;
	}
	else if (strcmp("DownloadKLineData", szTask) == 0)
	{
		m_tTaskStatus.enDownloadKLineData = enTaskStatus;
	}
}

T_TaskStatus & CWindBusinessApp::getTaskStatus()
{
	return m_tTaskStatus;
}


// Ψһ��һ�� CWindBusinessApp ����

CWindBusinessApp theApp;


// CWindBusinessApp ��ʼ��

BOOL CWindBusinessApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CWindBusinessDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

