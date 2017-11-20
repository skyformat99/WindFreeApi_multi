
// WindBusiness.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "DataHandle.h"
#include "dataStruct.h"


// CWindBusinessApp: 
// 有关此类的实现，请参阅 WindBusiness.cpp
//

class CWindBusinessApp : public CWinApp
{
public:
	CWindBusinessApp();

	T_TaskStatus m_tTaskStatus;
	CDataHandle m_dataHandle;
	T_ConfigInfo m_tConfigInfo;
	bool readConfig(const char *szFileName);
	void setTaskStatus(const char *szTask, EN_TASK_STATUS enTaskStatus);
	T_TaskStatus & getTaskStatus();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CWindBusinessApp theApp;