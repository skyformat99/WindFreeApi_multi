#ifndef DATA_SERVER_H_
#define DATA_SERVER_H_

#include <windows.h>
#include <comutil.h>
#include <string>
#include "WAPIWrapperCpp.h"

#define INDEX_NAME 64
#define INDEX_CODE 32

typedef struct _FieldData
{
	wchar_t szFieldCode[INDEX_CODE];
	wchar_t szFieldValue[INDEX_NAME];
	_FieldData()
	{
		memset(this, 0, sizeof(_FieldData));
	}
}T_FieldData;

class CWindData
{
public:
	VARIANT	m_data;		///< 数据
	VARIANT m_codes;	///< Code列表
	VARIANT m_fields;	///< 指标列表
	VARIANT m_times;	///< 时间列表
	LONG m_errorCode;	///< 错误码
	char szDatetime[25];// 推送时间
	int m_nDataType;// 推送的数据类型（1-实时行情  2-日线数据）
	std::string m_strExtraData;
	int m_nKlineType;// K线类型
	wchar_t m_szCode[32];// 股票代码
	wchar_t m_szName[32];// 股票名称
	wchar_t m_szDate[32];// 日期
	wchar_t m_szTime[32];// 时间

public:
	CWindData(void);
	~CWindData(void);

	void clear();

	void copy(WindData&);
	void copyKline(WindData&, int nKLineType);

	bool IsArray(const VARIANT& var);

	long GetFieldCount();
	LPCWSTR GetFieldNameByIndex(int index);

	long GetRecordCount();

	long GetTimeCount();
	DOUBLE GetTimeItemByIndex(int index);

	LPCWSTR GetCodeByIndex(int index);

	bool GetDataItem(int time, int codesIndex, int fieldsIndex, VARIANT &var);
	VARIANT GetVarFromArray(const VARIANT& safeArray, int pos);

};

#endif

