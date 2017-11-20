
// WindBusiness.cpp : 定义应用程序的类行为。
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


// CWindBusinessApp 构造

CWindBusinessApp::CWindBusinessApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
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


// 唯一的一个 CWindBusinessApp 对象

CWindBusinessApp theApp;


// CWindBusinessApp 初始化

BOOL CWindBusinessApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CWindBusinessDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

