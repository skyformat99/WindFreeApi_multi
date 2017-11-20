#include "WindData.h"


CWindData::CWindData(void)
{
	clear();
}


CWindData::~CWindData(void)
{
}

void CWindData::clear()
{
	memset(szDatetime, 0, 21);
	memset(m_szCode, 0, 32);
	memset(m_szName, 0, 32);
	memset(m_szDate, 0, 32);
	memset(m_szTime, 0, 32);
	VariantInit(&m_data);
	VariantInit(&m_codes);
	VariantInit(&m_fields);
	VariantInit(&m_times);
	m_errorCode = 0;
}

void CWindData::copy(WindData &wd)
{
	VariantCopy(&m_data, &wd.data);
	VariantCopy(&m_codes, &wd.codes);
	VariantCopy(&m_fields, &wd.fields);
	VariantCopy(&m_times, &wd.times);
	m_errorCode = wd.errorCode;
}

void CWindData::copyKline(WindData &wd, int nKLineType)
{
	copy(wd);
	m_nKlineType = nKLineType;
}

bool CWindData::IsArray(const VARIANT& var)
{
	if((var.vt & VT_ARRAY) && (var.parray != NULL))
	{
		return true;
	}
	return false;
}

long CWindData::GetFieldCount()
{
	if(!IsArray(m_fields))
	{
		return 0;
	}

	long totalCount = 1;
	for(int i=0; i< m_fields.parray->cDims; i++)
	{
		totalCount *= m_fields.parray->rgsabound[i].cElements;
	}

	return totalCount;
}

long CWindData::GetRecordCount()
{
	if(!IsArray(m_codes))
	{
		return 0;
	}

	long totalCount = 1;
	for(int i=0; i< m_codes.parray->cDims; i++)
	{
		totalCount *= m_codes.parray->rgsabound[i].cElements;
	}

	return totalCount;
}

long CWindData::GetTimeCount()
{
	if(!IsArray(m_times))
	{
		return 0;
	}

	long totalCount = 1;
	for(int i=0; i< m_times.parray->cDims; i++)
	{
		totalCount *= m_times.parray->rgsabound[i].cElements;
	}

	return totalCount;
}

DOUBLE CWindData::GetTimeItemByIndex(int index)
{
	if(!IsArray(m_times))
	{
		return 0.0;
	}

	if (VT_R8 != (m_times.vt & VT_BSTR_BLOB) && VT_DATE != (m_times.vt & VT_BSTR_BLOB))
	{
		return 0.0;
	}
	long lPos = (long)index;
	DOUBLE dbl = 0;

	SafeArrayGetElement(m_times.parray, &lPos, &dbl);

	dbl = dbl - 693960;

	return dbl;
}

LPCWSTR CWindData::GetFieldNameByIndex(int index)
{
	if(!IsArray(m_fields))
	{
		return L"";
	}

	if (VT_BSTR != (m_fields.vt & VT_BSTR_BLOB))
	{
		return L"";
	}

	HRESULT hr ;
	BSTR *pbItems;

	if (index >= GetFieldCount())
	{
		return L"";
	}

	hr = SafeArrayAccessData(m_fields.parray, (void HUGEP**)&pbItems);
	if (FAILED(hr))
	{
		return L"";
	}

	LPCWSTR itemStr = pbItems[index];

	SafeArrayUnaccessData(m_fields.parray);

	return itemStr;
}

LPCWSTR CWindData::GetCodeByIndex(int index)
{
	if(!IsArray(m_codes))
	{
		return L"";
	}

	if (VT_BSTR != (m_codes.vt & VT_BSTR_BLOB))
	{
		return L"";
	}

	HRESULT hr ;
	BSTR *pbItems;

	if (index >= GetRecordCount())
	{
		return L"";
	}

	hr = SafeArrayAccessData(m_codes.parray, (void HUGEP**)&pbItems);
	if (FAILED(hr))
	{
		return L"";
	}

	LPCWSTR itemStr = pbItems[index];

	SafeArrayUnaccessData(m_codes.parray);

	return itemStr;
}

bool CWindData::GetDataItem(int time, int codesIndex, int fieldsIndex, VARIANT &var)
{	
	int timenum = GetTimeCount();
	int codenum = GetRecordCount();
	int indnum = GetFieldCount();

	bool bResult = false;

	if (timenum == 0 && codenum == 0 && indnum > 0)
	{
		bResult = true;
		var = GetVarFromArray(m_data, fieldsIndex);
	}

	if (time < timenum && codesIndex < codenum && fieldsIndex < indnum)
	{
		bResult = true;

		int pos = time * codenum * indnum + codesIndex * indnum + fieldsIndex;

		var = GetVarFromArray(m_data, pos);
	}

	if (!bResult)
	{
		VARIANT varTemp;
		VariantInit(&varTemp);
		var = varTemp;
	}

	return bResult;
}

VARIANT CWindData::GetVarFromArray(const VARIANT& safeArray, int pos)
{
	VARIANT var;
	VariantInit(&var);

	if ((safeArray.vt & VT_ARRAY) && (safeArray.parray != NULL))
	{
		PVOID pvData = safeArray.parray->pvData;
		switch (safeArray.vt & VT_BSTR_BLOB)
		{
		case VT_VARIANT:
			var = *((LPVARIANT)pvData + pos);
			break;
		case VT_BSTR:
			{
				var.vt = VT_BSTR;
				BSTR* pCurBSTR = (BSTR*)pvData + pos;
				//直接指向safeArray中的bstr，而不是SysAllocString重新分配一个内存来存储字符串
				var.bstrVal = *pCurBSTR;//SysAllocString(*pCurBSTR);
			}
			break;
		case VT_BOOL:
			var.vt = VT_BOOL;
			var.boolVal = *((VARIANT_BOOL*)pvData + pos);
			break;
		case VT_R4:
			var.vt = VT_R4;
			var.fltVal = *((float*)pvData + pos);
			break;
		case VT_R8:
			var.vt = VT_R8;
			var.dblVal = *((DOUBLE*)pvData + pos);
			break;
		case VT_I4:
			var.vt = VT_I4;
			var.lVal = *((INT*)pvData + pos);
			break;
		case VT_I8:
			var.vt = VT_I8;
			var.llVal = *((LONGLONG*)pvData + pos);
			break;
		case VT_DATE:
			var.vt = VT_DATE;
			var.date = *((DATE*)pvData + pos);
			break;
		default:
			break;
		}
	}

	return var;
}