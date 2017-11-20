#include "stdafx.h"
#include "WindBusiness.h"
#include "DataHandle.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include "constData.h"
#include "XTransCode.h"
#include "XLog4z.h"
#include "XFile.h"


CDataHandle::CDataHandle()
{
	//m_hSemaKLineData = CreateSemaphore(NULL, 0, 0x0fffffff, NULL);
	InitializeCriticalSection(&m_cs);
	m_hMainWnd = NULL;
	m_bConnectedWnd = false;
	m_enKLineStatus = en_no_request;
	m_enSaveMode = EN_SAVE_FILE;

}

CDataHandle::~CDataHandle()
{
	DeleteCriticalSection(&m_cs);
}

void CDataHandle::setSaveMode(EN_SAVE_MODE enSaveMode)
{
	m_enSaveMode = enSaveMode;
}

EN_SAVE_MODE CDataHandle::getSaveMode()
{
	return m_enSaveMode;
}

void CDataHandle::setHWND(HWND hWnd)
{
	m_hMainWnd = hWnd;
}

void CDataHandle::setDownloadTime(std::wstring wstrBeginTime, std::wstring wstrEndTime)
{
	wcsncpy(m_wszBeginTime, wstrBeginTime.c_str(), wstrBeginTime.length());
	wcsncpy(m_wszEndTime, wstrEndTime.c_str(), wstrEndTime.length());
}

std::wstring CDataHandle::convertKLineType(int nKLineType)
{
	if (0 == nKLineType)
		return _T("00");

	if (1 == nKLineType)
		return _T("01");

	if (5 == nKLineType)
		return _T("05");

	if (15 == nKLineType)
		return _T("15");

	if (30 == nKLineType)
		return _T("30");

	if (60 == nKLineType)
		return _T("60");

	return _T("");
}

bool CDataHandle::readStock()
{
	std::ifstream infile;
	infile.open("stockCode.txt");
	if (!infile.is_open())
	{
		return false;
	}

	MAP_STOCK mapStock;

	std::string strLine;
	while (getline(infile, strLine))
	{
		if (!strLine.empty())
		{
			int pos = strLine.find(',');
			if (std::string::npos != pos)
			{
				std::string strCode = strLine.substr(0, pos);
				std::string strName = strLine.substr(pos+1, -1);
				mapStock.insert(std::make_pair(strCode, strName));
			}
		}
	}

	int nSize = mapStock.size() / 10;
	int nRemain = mapStock.size() % 10;

	MAP_STOCK::iterator it = mapStock.begin();

	int pos;
	for (int i = 0; i < THREAD_COUNT ; ++i)
	{
		int nCount = 1;
		for ( ; it != mapStock.end() ; ++it)
		{
			if (nCount > nSize)
				break;

			++nCount;

			T_StockCN tStockCN;
			tStockCN.strCode = it->first;
			tStockCN.strName = it->second;

			m_vecStocks[i].push_back(tStockCN);
		}
	}

	for (; it != mapStock.end(); ++it)
	{
		T_StockCN tStockCN;
		tStockCN.strCode = it->first;
		tStockCN.strName = it->second;

		m_vecStocks[0].push_back(tStockCN);
	}

	return true;
}

void CDataHandle::setMinuteIndexs(std::wstring wstrIndexs)
{
	m_wstrMinuteIndexs = wstrIndexs;
}

void CDataHandle::setDateIndexs(std::wstring strIndexs)
{
	m_wstrDateIndexs = strIndexs;
}

CString CDataHandle::getCurrentTime()
{
	CTime time = CTime::GetCurrentTime();
	CString date = time.Format(_T("%Y%m%d %H:%M:%S"));

	return date;
}

bool CDataHandle::connectWind()
{ 
	LONG lErrorCode = CWAPIWrapperCpp::start();
	if (0 != lErrorCode)
	{
		wchar_t buffer[128] = {0};
		int bufsize = 128;
		CWAPIWrapperCpp::getErrorMsg(lErrorCode, eCHN, buffer, bufsize);
		if (m_hMainWnd)
		{
			wchar_t strMsg[256];
			wsprintf(strMsg, _T("错误码：%ld, 错误信息：%s"), lErrorCode, buffer);
			MessageBox(m_hMainWnd, strMsg, _T("提示"), MB_OK);
		}

		m_bConnectedWnd = false;
		return false;
	}

	m_bConnectedWnd = true;

	return true;
}

bool CDataHandle::disconnectWind()
{
	if (0 == CWAPIWrapperCpp::stop())
	{
		m_bConnectedWnd = false;
		return true;
	}

	return false;
}

void CDataHandle::startDownloadKlineData()
{
	if (!m_bConnectedWnd)
	{
		wchar_t wszMsg[256];
		wsprintf(wszMsg, _T("%s: 请先连接万得！"), getCurrentTime());
		SendMessage(m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszMsg, 0);

		return;
	}

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		m_tExtraParam[i].pos = i;
		m_tExtraParam[i].pThis = this;
		HANDLE h1 = CreateThread(NULL, 0, threadKLineReq, &m_tExtraParam[i], 0, NULL);
		CloseHandle(h1);
	}

	int j = 0;
}

void CDataHandle::startDownloadStockCode()
{
	if (!m_bConnectedWnd)
	{
		wchar_t wszMsg[256];
		wsprintf(wszMsg, _T("%s: 请先连接万得！"), getCurrentTime());
		SendMessage(m_hMainWnd, UM_NOTIFY_REQ_DOWNLOAD_STOCK_CODE, (WPARAM)wszMsg, 0);

		return;
	}

	HANDLE h1 = CreateThread(NULL, 0, threadRequestStockCode, this, 0, NULL);
}

DWORD CDataHandle::threadRequestStockCode(LPVOID p)
{
	CDataHandle *pThis = (CDataHandle*)p;

	SYSTEMTIME sys;
	GetLocalTime(&sys);

	wchar_t szTime[128];
	wsprintf(szTime, L"%4d%02d%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	std::wstring wstrOption(L"date=");
	wstrOption.append(szTime);
	wstrOption.append(L";sectorid=a001010100000000");

	wchar_t wszMsg[256];
	wsprintf(wszMsg, _T("正在下载股票代码，请稍后..."));
	SendMessage(pThis->m_hMainWnd, UM_NOTIFY_REQ_DOWNLOAD_STOCK_CODE, (WPARAM)wszMsg, 0);

	theApp.setTaskStatus("DownloadCode", EN_Processing);

	WindData wd;
	LONG lErrorCode = CWAPIWrapperCpp::wset(wd, L"sectorconstituent", wstrOption.c_str());
	if (0 != lErrorCode)
	{
		wsprintf(wszMsg, _T("股票代码，股票代码下载失败！"));
		SendMessage(pThis->m_hMainWnd, UM_NOTIFY_REQ_DOWNLOAD_STOCK_CODE, (WPARAM)wszMsg, 0);
		return -1;
	}

	int codeLength = wd.GetCodesLength();
	int fieldsLength = wd.GetFieldsLength();

	// 解析收到的股票代码
	std::vector<T_StockInfo> vecStockInfo;
	for (int i = 0; i < codeLength; i++)
	{
		T_StockInfo stockInfo;
		for (int j = 0; j <fieldsLength; j++)
		{
			VARIANT var;
			wd.GetDataItem(0, i, j, var);
			std::wstring wstrValue = pThis->toString(&var);
			if (j == 0)
				stockInfo.strTime = wstrValue;
			else if (j == 1)
				stockInfo.strStockCode = wstrValue;
			else if (j == 2)
				stockInfo.strStockName = wstrValue;
		}
		vecStockInfo.push_back(stockInfo);
	}

	std::ofstream outfile;
	outfile.open("stockCode.txt", std::ios::out | std::ios::trunc);
	if (!outfile.is_open())
		return -1;

	char szbuf[128];
	std::vector<T_StockInfo>::iterator it;
	for (it = vecStockInfo.begin(); it != vecStockInfo.end(); ++it)
	{
		std::string strCode = pThis->wstring_to_string(it->strStockCode);
		std::string strName = pThis->wstring_to_string(it->strStockName);
		sprintf(szbuf, "%s,%s\n", strCode.c_str(), strName.c_str());
		outfile.write(szbuf, strlen(szbuf));
	}

	outfile.close();

	wsprintf(wszMsg, _T("股票代码下载完成！"));
	SendMessage(pThis->m_hMainWnd, UM_NOTIFY_REQ_DOWNLOAD_STOCK_CODE, (WPARAM)wszMsg, 0);

	theApp.setTaskStatus("DownloadCode", EN_COMPLETED);
	
	return 0;
}

DWORD CDataHandle::threadKLineReq(LPVOID p)
{
	T_ExtraParam tExtraParam = *(T_ExtraParam*)p;


	wchar_t wszMsg[256];

	if (!tExtraParam.pThis->m_bConnectedWnd)
	{
		wsprintf(wszMsg, _T("请先连接万得！"));
		SendMessage(tExtraParam.pThis->m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszMsg, 0);

		return 0;
	}

	std::string strBeginTime = tExtraParam.pThis->wstring_to_string(tExtraParam.pThis->m_wszBeginTime);
	std::string strEndTime = tExtraParam.pThis->wstring_to_string(tExtraParam.pThis->m_wszEndTime);

	std::vector<std::wstring> vecDate;// 保存已下载的日期

	std::vector<std::wstring> &vecKLine = theApp.m_tConfigInfo.vecKLine;

	boost::gregorian::date beginDay, endDay;
	try {
		beginDay = boost::gregorian::from_undelimited_string(strBeginTime);
		endDay = boost::gregorian::from_undelimited_string(strEndTime);
	}
	catch (std::exception &e)
	{
		wsprintf(wszMsg, _T("日期不合法，请重新输入！"));
		SendMessage(tExtraParam.pThis->m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)&wszMsg, 0);

		return 0;
	}

	// 启动写入文件线程
	HANDLE h = CreateThread(NULL, 0, threadWriteKLineData, &tExtraParam, 0, NULL);
	CloseHandle(h);

	theApp.setTaskStatus("DownloadKLineData", EN_Processing);

	tExtraParam.pThis->m_enKLineStatus = en_requesting;

	wsprintf(wszMsg, _T("%s: 正在下载K线数据，请稍后..."), tExtraParam.pThis->getCurrentTime());
	SendMessage(tExtraParam.pThis->m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszMsg, 0);


	for (boost::gregorian::date day(beginDay); day <= endDay; day = day + boost::gregorian::days(1))
	{
		std::wstring wstrDate = tExtraParam.pThis->string_to_wstring(to_iso_string(day));
		vecDate.push_back(wstrDate);

		std::vector<T_StockCN>::iterator itStock;
		for (itStock = tExtraParam.pThis->m_vecStocks[tExtraParam.pos].begin() ; itStock != tExtraParam.pThis->m_vecStocks[tExtraParam.pos].end() ; ++itStock)
		{
			std::wstring wstrCode = tExtraParam.pThis->string_to_wstring(itStock->strCode);
			std::wstring wstrName = tExtraParam.pThis->string_to_wstring(itStock->strName);

			wsprintf(wszMsg, _T("正在下载的代码: %s"), wstrCode.c_str());
			SendMessage(tExtraParam.pThis->m_hMainWnd, UM_NOTIFY_REQ_LOADING, (WPARAM)wszMsg, 0);

			// 判断当前股票在当天是否停牌
			if (0 != tExtraParam.pThis->isStockSuspension(wstrCode, wstrDate, wszMsg))
			{
				SendMessage(tExtraParam.pThis->m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszMsg, 0);

				continue;
			}

			std::vector<std::wstring>::iterator itKLine;
			for (itKLine = vecKLine.begin(); itKLine != vecKLine.end(); ++itKLine)
			{
				int nKLine = std::stoi(*itKLine);
				if (0 == nKLine)// 下载日线数据
				{
					if (!tExtraParam.pThis->requestDateData(nKLine, wstrDate, wstrCode, wstrName))
						continue;
				}
				else// 下载分钟数据
				{
					if (!tExtraParam.pThis->requestMinutesData(nKLine, wstrDate, wstrCode, wstrName))
						continue;
				}
			}

			// 触发信号，将当天当前的股票数据写入文件
			tExtraParam.pThis->signalTickData();
		}
	}

	SendMessage(tExtraParam.pThis->m_hMainWnd, UM_NOTIFY_REQ_COMPLETE, (WPARAM)&vecDate, 0);

	tExtraParam.pThis->m_enKLineStatus = en_request_complete;

	theApp.setTaskStatus("DownloadKLineData", EN_COMPLETED);

	return 0;
}

DWORD CDataHandle::threadWriteKLineData(LPVOID p)
{
	// CDataHandle *pThis = (CDataHandle*)p;
	T_ExtraParam tExtraParam = *(T_ExtraParam*)p;

	wchar_t wszMinutesFileName[256];

	XMyTools::XFile fileDay(true), fileMinutes(true);
	std::wstring wstrDateKline = theApp.m_tConfigInfo.wstrDateKlineFileName;
	std::wstring wstrPath = theApp.m_tConfigInfo.wstrPath;
	if (-1 == _waccess(wstrPath.c_str(), 0))// 目录不存在
	{
		// 创建该目录
		std::string strTemp = XMyTools::XTransCode::wstring_to_string(wstrPath);
		char szCreateDir[256];
		sprintf(szCreateDir, "md %s", strTemp.c_str());
		system(szCreateDir);
	}

	std::wstring strFileName(wstrPath + wstrDateKline);
	fileDay.openFile(strFileName.c_str(), std::ios::out|std::ios::trunc);
	if (!fileDay.isOpen())
	{
		wchar_t wszError[256];
		wsprintf(wszError, _T("[%s]文件打开失败！"), theApp.m_tConfigInfo.wstrDateKlineFileName.c_str());
		SendMessage(tExtraParam.pThis->m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszError, 0);

		return 0;
	}

	while (en_request_complete != tExtraParam.pThis->m_enKLineStatus)
	{
		DWORD dwResult = WaitForSingleObject(tExtraParam.pThis->m_hSemaKLineData, 1000);
		if (WAIT_OBJECT_0 == dwResult)
		{
			for (int i = 0 ; i < 6 ; ++i)
			{
				CWindData wd;

				if (tExtraParam.pThis->pop_data(tExtraParam.pos, wd))
				{
					std::wstring wstrMinutesData, wstrDateData;
					tExtraParam.pThis->parseWndData(wd.m_nKlineType, wd, wstrMinutesData, wstrDateData);

					// 写入分钟数据
					if (!wstrMinutesData.empty())
					{
						if (!fileMinutes.isOpen())
						{
							wsprintf(wszMinutesFileName, _T("%s%s_%s_%s.txt"),
								wstrPath.c_str(), wd.m_szCode,
								theApp.m_tConfigInfo.wstrBeginTime.c_str(), theApp.m_tConfigInfo.wstrEndTime.c_str());

							fileMinutes.openFile(wszMinutesFileName, std::ios::app);
							if (!fileMinutes.isOpen())
							{
								break;
							}
						}

						//std::wstring wstrUtf8;
						//wstrUtf8 = XMyTools::XTransCode::GBKToUTF8(wstrMinutesData.c_str(), wstrMinutesData.length());
						fileMinutes.write(wstrMinutesData);
					}

					// 写入日线数据
					if (!wstrDateData.empty())
					{
						//std::wstring wstrUtf8;
						//wstrUtf8 = XMyTools::XTransCode::GBKToUTF8(wstrDateData.c_str(), wstrDateData.length());
						fileDay.write(wstrDateData);
					}
				}
			}

			// 当前股票数据写入文件完毕，那么就关闭文件
			if (fileMinutes.isOpen())
				fileMinutes.closeFile();

			fileDay.flush();
		}
	}

	fileDay.closeFile();

	wchar_t wszMsg[256];
	wsprintf(wszMsg, _T("%s: K线数据下载完成！"), tExtraParam.pThis->getCurrentTime());
	SendMessage(tExtraParam.pThis->m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszMsg, 0);

	//XMyTools::XLog4z::getInstance().write(pThis->m_loggerId, szMsg);


	wsprintf(wszMsg, _T("下载完成！"));
	SendMessage(tExtraParam.pThis->m_hMainWnd, UM_NOTIFY_REQ_LOADING, (WPARAM)wszMsg, 0);

	return 0;
}

void CDataHandle::push_data(int pos, CWindData wd)
{
	CAutoLock lock(&m_cs);
	m_dequeKLineData[pos].push_back(wd);
}

bool CDataHandle::pop_data(int pos, CWindData &wd)
{
	CAutoLock lock(&m_cs);
	if (!m_dequeKLineData[pos].empty())
	{
		wd = m_dequeKLineData[pos].front();
		m_dequeKLineData[pos].pop_front();
		return true;
	}

	return false;
}

void CDataHandle::signalTickData()
{
	ReleaseSemaphore(m_hSemaKLineData, 1, NULL);
}

std::wstring CDataHandle::string_to_wstring(const std::string& str)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = str.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

std::string CDataHandle::wstring_to_string(const std::wstring& wstr)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = wstr.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char *chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}

std::wstring CDataHandle::toString(const LPVARIANT data)
{
	std::wstring msg;
	char szValue[32] = {0};
	switch (data->vt & VT_BSTR_BLOB)
	{
	case VT_I4:
		sprintf(szValue, "%d", data->intVal);
		break;
	case VT_I8:
		printf(szValue, "%I64d", data->llVal);
		break;
	case VT_R4:
		sprintf(szValue, "%f", data->fltVal);
		break;
	case VT_R8:
		sprintf(szValue, "%f", data->dblVal);
		break;
	case VT_EMPTY:
		sprintf(szValue, "NaN");
		break;
	case VT_BSTR:
		msg = data->bstrVal;
		break;
	case VT_DATE:
		msg = WindData::DateToString(data->date);
		break;
	default:
		break;
	}

	if (msg.empty())
	{
		msg = string_to_wstring(szValue);
	}

	return msg;
}

int CDataHandle::isStockSuspension(std::wstring wstrStockCode, std::wstring wstrDate, wchar_t *wszErrorMsg)
{
	wchar_t wszBuf[128];
	wsprintf(wszBuf, _T("tradeDate=%s"), wstrDate.c_str());
	WindData wd;
	LONG lErrorCode = CWAPIWrapperCpp::wss(wd, wstrStockCode.c_str(), _T("trade_status"), wszBuf);
	if (0 != lErrorCode)
	{
		int bufsize = 128;
		CWAPIWrapperCpp::getErrorMsg(lErrorCode, eCHN, wszBuf, bufsize);
		wsprintf(wszErrorMsg, _T("%s: wss执行失败, 原因：[%s]在[%s]%s[行号:%d]"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str(), wszBuf, __LINE__);
		return -1;
	}

	VARIANT var;
	wd.GetDataItem(0, 0, 0, var);

	std::wstring wstrTradeStatus = toString(&var);

	if (wstrTradeStatus.compare(_T("停牌一天")) == 0)
	{
		wsprintf(wszErrorMsg, _T("%s: [%s]在[%s]%s"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str(), wstrTradeStatus.c_str());
		return 1;
	}
	else if (wstrTradeStatus.compare(_T("NaN")) == 0)
	{
		wsprintf(wszErrorMsg, _T("%s: [%s]在[%s]天还没有上市"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str());
		return 1;
	}
	else if (wstrTradeStatus.compare(_T("盘中停牌")) == 0)
	{
		wsprintf(wszErrorMsg, _T("%s: [%s]在[%s]盘中停牌"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str());
		return 1;
	}

	return 0;
}

int CDataHandle::isStockSuspensionV2(std::wstring wstrStockCode, std::wstring wstrDate, wchar_t * wszErrorMsg)
{
	wchar_t wszBuf[128];
	WindData wd;
	LONG lErrorCode = CWAPIWrapperCpp::wsd(wd, wstrStockCode.c_str(), _T("trade_status"), wstrDate.c_str(), wstrDate.c_str());
	if (0 != lErrorCode)
	{
		int bufsize = 128;
		CWAPIWrapperCpp::getErrorMsg(lErrorCode, eCHN, wszBuf, bufsize);
		wsprintf(wszErrorMsg, _T("%s: wsd执行失败, 原因：[%s]在[%s]%s[行号:%d]"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str(), wszBuf, __LINE__);
		return -1;
	}

	int timeLength = wd.GetTimesLength();
	int codesLen = wd.GetCodesLength();
	int fieldsLen = wd.GetFieldsLength();

	for (int i = 0; i < codesLen; i++)
	{
		for (int j = 0; j <fieldsLen; j++)
		{
			VARIANT var;
			wd.GetDataItem(0, i, j, var);

			std::wstring wstrTradeStatus = toString(&var);

			if (wstrTradeStatus.compare(_T("停牌一天")) == 0)
			{
				wsprintf(wszErrorMsg, _T("%s: [%s]在[%s]%s"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str(), wstrTradeStatus.c_str());
				return 1;
			}
			else if (wstrTradeStatus.compare(_T("NaN")) == 0)
			{
				wsprintf(wszErrorMsg, _T("%s: [%s]在[%s]天还没有上市"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str());
				return 1;
			}
			else if (wstrTradeStatus.compare(_T("盘中停牌")) == 0)
			{
				wsprintf(wszErrorMsg, _T("%s: [%s]在[%s]盘中停牌"), getCurrentTime(), wstrStockCode.c_str(), wstrDate.c_str());
				return 1;
			}
		}
	}

	return 0;
}

bool CDataHandle::requestDateData(int nType, std::wstring wstrTime, std::wstring wstrCode, std::wstring wstrName)
{
	WindData wd;
	LONG lErrorCode = CWAPIWrapperCpp::wsd(wd, wstrCode.c_str(), m_wstrDateIndexs.c_str(), wstrTime.c_str(), wstrTime.c_str(), theApp.m_tConfigInfo.PriceAdj.c_str());
	if (0 != lErrorCode)
	{
		int bufsize = 128;
		WCHAR buffer[128] = { 0 };

		CWAPIWrapperCpp::getErrorMsg(lErrorCode, eCHN, buffer, bufsize);

		if (m_hMainWnd)
		{
			wchar_t wszMsg[256];
			wsprintf(wszMsg, _T("%s: wsd执行失败, 原因：[%s]在[%s]天，日线%s[行号:%d]"), getCurrentTime(), wstrCode.c_str(), wstrTime.c_str(), buffer, __LINE__);
			::SendMessage(m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszMsg, 0);
		}
		return false;
	}

	CWindData newWD;
	wcsncpy(newWD.m_szCode, wstrCode.c_str(), wstrCode.length());
	wcsncpy(newWD.m_szName, wstrName.c_str(), wstrName.length());
	wcsncpy(newWD.m_szDate, wstrTime.c_str(), wstrTime.length());
	wcsncpy(newWD.m_szTime, L"1500", wcslen(L"1500"));
	newWD.copyKline(wd, nType);
	//push_data(newWD);

	return true;
}

bool CDataHandle::requestMinutesData(int nType, std::wstring wstrTime, std::wstring wstrCode, std::wstring wstrName)
{
	char szbuf[128];
	sprintf(szbuf, "BOLL_N=26;BOLL_Width=2;BOLL_IO=1;BarSize=%d", nType);
	std::wstring wstrOption = string_to_wstring(szbuf);

	std::wstring strYear = wstrTime.substr(0,4);
	std::wstring strMonth = wstrTime.substr(4, 2);
	std::wstring strDay = wstrTime.substr(6, 2);

	wchar_t wszBeginTime[64];
	wchar_t wszEndTime[64];
	wsprintf(wszBeginTime, _T("%s-%s-%s 09:30:00"), strYear.c_str(), strMonth.c_str(), strDay.c_str());
	wsprintf(wszEndTime, _T("%s-%s-%s 15:01:00"), strYear.c_str(), strMonth.c_str(), strDay.c_str());// 因为请求的时间不包括结束时间点，为了得到15:00的数据，所以向后加1分钟

	WindData wd;
	LONG lErrorCode = CWAPIWrapperCpp::wsi(wd, wstrCode.c_str(), m_wstrMinuteIndexs.c_str(), wszBeginTime, wszEndTime, wstrOption.c_str());
	if (0 != lErrorCode)
	{
		int bufsize = 128;
		WCHAR buffer[128] = { 0 };

		CWAPIWrapperCpp::getErrorMsg(lErrorCode, eCHN, buffer, bufsize);
		
		if (m_hMainWnd)
		{
			wchar_t wszMsg[256];
			wsprintf(wszMsg, _T("%s: wsi执行失败, 原因：[%s]在[%s]天，%d分钟%s[行号:%d]"), getCurrentTime(), wstrCode.c_str(), wstrTime.c_str(), nType, buffer, __LINE__);
			::SendMessage(m_hMainWnd, UM__NOTIFY_REQ_KLINE, (WPARAM)wszMsg, 0);
		}
		return false;
	}


	CWindData newWD;
	wcsncpy(newWD.m_szCode, wstrCode.c_str(), wstrCode.length());
	wcsncpy(newWD.m_szName, wstrName.c_str(), wstrName.length());
	wcsncpy(newWD.m_szDate, wstrTime.c_str(), wstrTime.length());
	newWD.copyKline(wd, nType);
	//push_data(newWD);

	return true;
}

void CDataHandle::parseWndData(int nType, CWindData &wd, std::wstring &strMinutesData, std::wstring &strDateData)
{
	strMinutesData.clear();
	strDateData.clear();

	int fieldsLen = wd.GetFieldCount();// 字段个数
	int codesLen = wd.GetRecordCount();// 记录数
	int timeLen = wd.GetTimeCount();// 日期天数

	std::wstring wstrCode;
	std::wstring wstrName;
	std::wstring wstrMarket;

	wchar_t wszValue[512];

	wstrCode = wd.m_szCode;
	wstrName = wd.m_szName;
	int pos = wstrCode.find('.');
	if (std::string::npos != pos)
	{
		wstrMarket = wstrCode.substr(pos + 1, -1);
		if (wstrMarket.compare(_T("SZ")) == 0)
		{
			wstrMarket = _T("SZSE");
		}
		else if (wstrMarket.compare(_T("SH")) == 0)
		{
			wstrMarket = _T("SSE");
		}
		wstrCode = wstrCode.substr(0, pos);
	}

	std::vector<T_FieldData> vecRecord;

	// 解析字段名
	std::string strTitle;
	for (int i = 0; i < fieldsLen; ++i)
	{
		T_FieldData tFiledData;
		std::wstring wstrFieldName = wd.GetFieldNameByIndex(i);
		transform(wstrFieldName.begin(), wstrFieldName.end(), wstrFieldName.begin(), ::tolower);
		wcsncpy(tFiledData.szFieldCode, wstrFieldName.c_str(), wstrFieldName.length());
		vecRecord.push_back(tFiledData);
	}

	std::wstring formatDate = L"%Y-%m-%d 15:00";
	std::wstring formatTime = L"%Y-%m-%d %H:%M:%S";

	// 解析字段值
	if (wd.m_nKlineType != 0)// 分钟数据
	{
		for (int i = 0; i < timeLen; i++)
		{
			std::wstring wstrRecord;

			// 解析时间字段
			DATE date = wd.GetTimeItemByIndex(i);
			std::wstring wstrDateTime;
			WCHAR* time = NULL;
			if (0 == nType)// 日K线
			{
				time = WindData::DateToString(date, formatDate.c_str());
			}
			else// 分钟K线
			{
				time = WindData::DateToString(date, formatTime.c_str());
			}

			if (NULL != time)
			{
				wstrDateTime = time;
				WindData::FreeString(time);
			}

			// 解析数据字段
			for (int j = 0; j < fieldsLen; j++)
			{
				T_FieldData &tField = vecRecord.at(j);
				VARIANT var;
				wd.GetDataItem(i, 0, j, var);
				std::wstring wstrValue = toString(&var);
				wcsncpy(tField.szFieldValue, wstrValue.c_str(), wstrValue.length());
				
			}

			// 拼接“代码和名称”
			wstrRecord += wstrCode + _T(",");
			wstrRecord += wstrName + _T(",");

			std::wstring wstrYear, wstrDate, wstrTime;

			// 日期和时间
			if (0 == nType)
			{
				wstrDate = wd.m_szDate;
				wstrTime = wd.m_szTime;
				wstrYear = wstrDate.substr(0,4);
			}
			else
			{
				std::wstring strTempTime = wstrDateTime;
				boost::algorithm::erase_all(strTempTime, "-");
				boost::algorithm::erase_all(strTempTime, ":");
				int pos = strTempTime.find(' ');
				if (std::string::npos != pos)
				{
					wstrDate = strTempTime.substr(0, pos);
					wstrTime = strTempTime.substr(pos + 1, 4);
					wstrYear = wstrDate.substr(0, 4);
				}
			}
			wstrRecord += wstrYear + _T(",");
			wstrRecord += wstrDate + _T(",");
			wstrRecord += wstrTime + _T(",");

			// k线类型
			std::wstring strKLineType = convertKLineType(wd.m_nKlineType);
			wstrRecord += strKLineType + _T(",");

			// 拼接其它字段值
			std::vector<T_FieldData>::iterator it;
			for (it = vecRecord.begin(); it != vecRecord.end(); ++it)
			{
				wsprintf(wszValue, _T("%s,"), it->szFieldValue);
				wstrRecord += wszValue;
			}

			// 时间戳
			std::string strTimeStamep = wstring_to_string(wstrDateTime);
			long nTimeStamep = getTimeStamp(strTimeStamep.c_str());
			wsprintf(wszValue, _T("%ld"), nTimeStamep);
			wstrRecord += wszValue;

			// 市场
			wstrRecord += _T(",");
			wstrRecord += wstrMarket;
			wstrRecord += _T("\n");

			strMinutesData += wstrRecord;
		}

		strMinutesData = strMinutesData.substr(0, strMinutesData.length() - 1);
	}
	else// 日线数据
	{
		for (int i = 0; i < codesLen; i++)
		{
			std::wstring wstrRecord;

			for (int j = 0; j <fieldsLen; j++)
			{
				T_FieldData &pushData = vecRecord.at(j);
				VARIANT var;
				wd.GetDataItem(0, i, j, var);
				std::wstring wstrValue = toString(&var);
				wcsncpy(pushData.szFieldValue, wstrValue.c_str(), wstrValue.length());
			}

			// 日期和时间
			std::wstring wstrDate = wd.m_szDate;
			std::wstring wstrYear = wstrDate.substr(0, 4);

			wstrRecord += wstrYear + _T(",");
			wstrRecord += wstrDate + _T(",");

			// 拼接“代码和名称”
			wstrRecord += wstrCode + _T(",");
			wstrRecord += wstrName + _T(",");

			// 拼接其它字段值
			std::vector<T_FieldData>::iterator it;
			for (it = vecRecord.begin(); it != vecRecord.end(); ++it)
			{
				wsprintf(wszValue, _T("%s,"), it->szFieldValue);
				wstrRecord += wszValue;
			}

			// 市场
			wstrRecord += wstrMarket;
			wstrRecord += _T("\n");

			strDateData += wstrRecord;
		}
		strDateData = strDateData.substr(0, strDateData.length() - 1);
	}
}

long CDataHandle::getTimeStamp(const char *str_time)
{
	struct tm stm;
	int iY, iM, iD, iH, iMin, iS;

	memset(&stm, 0, sizeof(stm));

	iY = atoi(str_time);
	iM = atoi(str_time + 5);
	iD = atoi(str_time + 8);
	iH = atoi(str_time + 11);
	iMin = atoi(str_time + 14);
	iS = atoi(str_time + 17);

	stm.tm_year = iY - 1900;
	stm.tm_mon = iM - 1;
	stm.tm_mday = iD;
	stm.tm_hour = iH;
	stm.tm_min = iMin;
	stm.tm_sec = iS;

	return mktime(&stm);
}