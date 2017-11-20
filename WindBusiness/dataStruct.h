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
	char szCode[32];// 格式：000001
	char szName[32];
	char szYear[9];// 格式：20170901
	char szTime[5];// 格式：1500
	char szKLineType[3];// K线类型（00-日K线 01-1分钟K线 05-5分钟K线 15-15分钟K线 30-30分钟K线 60-60分钟K线）
	char  szMarket[5];
	double dbOpenPrice;// 开盘价
	double dbClosePrice;// 收盘价
	double dbHighPrice;// 最高价
	double dbLowPrice;// 最低价
	long nTradeVolume;// 交易量
	double dbTurnover;// 交易额
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
	std::wstring PriceAdj;// 复权方式：N-不复权 F-前复权 B-后复权
}T_ConfigInfo;

enum EN_TASK_STATUS
{
	EN_UNSTART,			// 未开始
	EN_Processing,		// 正在处理
	EN_COMPLETED		// 完成
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