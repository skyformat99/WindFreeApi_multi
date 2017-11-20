#ifndef DATA_HANDLE_H_
#define DATA_HANDLE_H_
#include <deque>
#include "dataStruct.h"
#include "constData.h"
#include "WindData.h"
#include <map>
#include "XLog4z.h"

// first:��Ʊ����  second:��Ʊ����
typedef std::map<std::string, std::string> MAP_STOCK;

#define THREAD_COUNT 3

class CDataHandle;

enum EN_SAVE_MODE
{
	EN_SAVE_FILE,  // ���浽txt�ļ�
	EN_SAVE_SQLITE // ���浽sqlite���ݿ���
};

typedef struct _StockCN
{
	std::string strCode;
	std::string strName;
}T_StockCN;

typedef struct _ExtraParam
{
	int pos;
	CDataHandle *pThis;
}T_ExtraParam;

/*
 @brief ������ݴ������������������ݻ��桢������д���ļ�
*/
class CDataHandle
{
private:

	std::deque<CWindData> m_dequeKLineData[10];// �洢���ص�k������
	CRITICAL_SECTION m_cs;
	HANDLE m_hSemaKLineData[THREAD_COUNT];
	HWND m_hMainWnd;// �����ھ��

	T_ExtraParam m_tExtraParam[THREAD_COUNT];

	
	std::wstring m_wstrMinuteIndexs;// �����������ʱ���õ���ָ���ֶ�
	std::wstring m_wstrDateIndexs;// ������������ʱ���õ���ָ���ֶ�

	std::vector<T_StockCN> m_vecStocks[THREAD_COUNT];// ����Ҫ����Ĺ�Ʊ

	wchar_t m_wszBeginTime[32];
	wchar_t m_wszEndTime[32];
	
	bool m_bConnectedWnd;// �Ƿ�����������÷�����
	EN_KLINE m_enKLineStatus;// 

	EN_SAVE_MODE m_enSaveMode;

public:
	CDataHandle();
	~CDataHandle();

	void setSaveMode(EN_SAVE_MODE enSaveMode);
	EN_SAVE_MODE getSaveMode();
	
	void setHWND(HWND hWnd);
	void setDownloadTime(std::wstring wstrBeginTime, std::wstring wstrEndTime);
	std::wstring convertKLineType(int nKLineType);
	bool readStock();// ��ȡҪ����Ĺ�Ʊ
	void setMinuteIndexs(std::wstring strIndexNames);// ����Ҫ�����ָ�꣨��ʽ��high,low,open,close,volume,amt��
	void setDateIndexs(std::wstring strIndexs);
	CString getCurrentTime();

	bool connectWind();// �������
	bool disconnectWind();// �Ͽ����
	void startDownloadKlineData();// ����K������
	void startDownloadStockCode();// ���ع�Ʊ����

	static DWORD WINAPI threadRequestStockCode(LPVOID p);

	// K�����������߳�
	static DWORD WINAPI threadKLineReq(LPVOID p);

	// ����д���߳�
	static DWORD WINAPI threadWriteKLineData(LPVOID p);

	void push_data(int pos, CWindData wd);
	bool pop_data(int pos, CWindData &wd);

	// ���������ǣ������ź�
	void signalTickData();

	std::wstring string_to_wstring(const std::string& str);
	std::string wstring_to_string(const std::wstring& wstr);
	std::wstring toString(const LPVARIANT data);

	// �жϹ�Ʊ�Ƿ�ͣ��
	int isStockSuspension(std::wstring wstrStockCode, std::wstring wstrDate, wchar_t *wszErrorMsg);
	int isStockSuspensionV2(std::wstring wstrStockCode, std::wstring wstrDate, wchar_t *wszErrorMsg);

	// ������������
	bool requestDateData(int nType, std::wstring wstrTime, std::wstring wstrCode, std::wstring wstrName);

	// �����������
	bool requestMinutesData(int nType, std::wstring wstrTime, std::wstring wstrCode, std::wstring wstrName);

	// ���ݸ����Զ����������ͽ��н���
	void parseWndData(int nType, CWindData &wd, std::wstring &strMinutesData, std::wstring &strDateData);

	// ��ȡָ��ʱ���ʱ���
	long getTimeStamp(const char *str_time);
};





#endif

