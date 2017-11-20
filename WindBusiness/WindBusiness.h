
// WindBusiness.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DataHandle.h"
#include "dataStruct.h"


// CWindBusinessApp: 
// �йش����ʵ�֣������ WindBusiness.cpp
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

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWindBusinessApp theApp;