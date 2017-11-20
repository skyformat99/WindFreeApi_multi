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
	VARIANT	m_data;		///< ����
	VARIANT m_codes;	///< Code�б�
	VARIANT m_fields;	///< ָ���б�
	VARIANT m_times;	///< ʱ���б�
	LONG m_errorCode;	///< ������
	char szDatetime[25];// ����ʱ��
	int m_nDataType;// ���͵��������ͣ�1-ʵʱ����  2-�������ݣ�
	std::string m_strExtraData;
	int m_nKlineType;// K������
	wchar_t m_szCode[32];// ��Ʊ����
	wchar_t m_szName[32];// ��Ʊ����
	wchar_t m_szDate[32];// ����
	wchar_t m_szTime[32];// ʱ��

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

