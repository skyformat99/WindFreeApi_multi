#ifndef DATA_STRUCT_H_
#define DATA_STRUCT_H_
#include <Windows.h>
#include <vector>

typedef DWORD (*FN_THREAD_PROC)(LPVOID);

class CAutoLock
{
	PCRITICAL_SECTION m_pcs;
public:
	CAutoLock(PCRITICAL_SECTION pcs)
	{
		m_pcs = pcs;
		EnterCriticalSection(m_pcs);
	}
	~CAutoLock()
	{
		LeaveCriticalSection(m_pcs);
	}
};

typedef struct _Data
{
	char szCode[32];// ��ʽ��000001
	char szName[32];
	char szYear[9];// ��ʽ��20170901
	char szTime[5];// ��ʽ��1500
	char szKLineType[3];// K�����ͣ�00-��K�� 01-1����K�� 05-5����K�� 15-15����K�� 30-30����K�� 60-60����K�ߣ�
	char  szMarket[5];
	double dbOpenPrice;// ���̼�
	double dbClosePrice;// ���̼�
	double dbHighPrice;// ��߼�
	double dbLowPrice;// ��ͼ�
	long nTradeVolume;// ������
	double dbTurnover;// ���׶�
	long unix;
	_Data()
	{
		memset(this, 0, sizeof(_Data));
	}

}T_Data;

typedef struct _StockInfo
{
	std::wstring strStockCode;
	std::wstring strStockName;
	std::wstring strTime;
}T_StockInfo;

typedef struct _ThreadInfo
{
	int nThreadID;
	HANDLE hThread;
	FN_THREAD_PROC fnThreadProc;
}T_ThreadInfo;

typedef struct _ConfigInfo
{
	std::vector<std::wstring> vecKLine;
	std::wstring wstrDateKlineFileName;
	std::wstring wstrPath;
	std::wstring wstrBeginTime;
	std::wstring wstrEndTime;
	std::wstring PriceAdj;// ��Ȩ��ʽ��N-����Ȩ F-ǰ��Ȩ B-��Ȩ
}T_ConfigInfo;

enum EN_TASK_STATUS
{
	EN_UNSTART,			// δ��ʼ
	EN_Processing,		// ���ڴ���
	EN_COMPLETED		// ���
};

typedef struct _Task
{
	EN_TASK_STATUS enDownloadCode;
	EN_TASK_STATUS enDownloadKLineData;
	_Task()
	{
		enDownloadCode = EN_UNSTART;
		enDownloadKLineData = EN_UNSTART;
	}
}T_TaskStatus;


#endif