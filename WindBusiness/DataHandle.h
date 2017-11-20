#ifndef DATA_HANDLE_H_
#define DATA_HANDLE_H_
#include <deque>
#include "dataStruct.h"
#include "constData.h"
#include "WindData.h"
#include <map>
#include "XLog4z.h"

// first:股票代码  second:股票名称
typedef std::map<std::string, std::string> MAP_STOCK;

#define THREAD_COUNT 3

class CDataHandle;

enum EN_SAVE_MODE
{
	EN_SAVE_FILE,  // 保存到txt文件
	EN_SAVE_SQLITE // 保存到sqlite数据库中
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
 @brief 万得数据处理，包括数据请求、数据缓存、将数据写入文件
*/
class CDataHandle
{
private:

	std::deque<CWindData> m_dequeKLineData[10];// 存储返回的k线数据
	CRITICAL_SECTION m_cs;
	HANDLE m_hSemaKLineData[THREAD_COUNT];
	HWND m_hMainWnd;// 主窗口句柄

	T_ExtraParam m_tExtraParam[THREAD_COUNT];

	
	std::wstring m_wstrMinuteIndexs;// 请求分钟数据时，用到的指标字段
	std::wstring m_wstrDateIndexs;// 请求日线数据时，用到的指标字段

	std::vector<T_StockCN> m_vecStocks[THREAD_COUNT];// 保存要请求的股票

	wchar_t m_wszBeginTime[32];
	wchar_t m_wszEndTime[32];
	
	bool m_bConnectedWnd;// 是否连接上了万得服务器
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
	bool readStock();// 读取要请求的股票
	void setMinuteIndexs(std::wstring strIndexNames);// 设置要请求的指标（格式：high,low,open,close,volume,amt）
	void setDateIndexs(std::wstring strIndexs);
	CString getCurrentTime();

	bool connectWind();// 连接万得
	bool disconnectWind();// 断开万得
	void startDownloadKlineData();// 下载K线数据
	void startDownloadStockCode();// 下载股票代码

	static DWORD WINAPI threadRequestStockCode(LPVOID p);

	// K线数据请求线程
	static DWORD WINAPI threadKLineReq(LPVOID p);

	// 数据写入线程
	static DWORD WINAPI threadWriteKLineData(LPVOID p);

	void push_data(int pos, CWindData wd);
	bool pop_data(int pos, CWindData &wd);

	// 当有数据是，触发信号
	void signalTickData();

	std::wstring string_to_wstring(const std::string& str);
	std::string wstring_to_string(const std::wstring& wstr);
	std::wstring toString(const LPVARIANT data);

	// 判断股票是否停牌
	int isStockSuspension(std::wstring wstrStockCode, std::wstring wstrDate, wchar_t *wszErrorMsg);
	int isStockSuspensionV2(std::wstring wstrStockCode, std::wstring wstrDate, wchar_t *wszErrorMsg);

	// 请求日线数据
	bool requestDateData(int nType, std::wstring wstrTime, std::wstring wstrCode, std::wstring wstrName);

	// 请求分钟数据
	bool requestMinutesData(int nType, std::wstring wstrTime, std::wstring wstrCode, std::wstring wstrName);

	// 根据根据自定义数据类型进行解析
	void parseWndData(int nType, CWindData &wd, std::wstring &strMinutesData, std::wstring &strDateData);

	// 获取指定时间的时间戳
	long getTimeStamp(const char *str_time);
};





#endif

