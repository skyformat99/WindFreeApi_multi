#include "XString.h"
#include <assert.h>
#include <cctype>

namespace XMyTools
{
	XString::XString()
	{
	}


	XString::~XString()
	{
	}

	int XString::strlen(const char * szText)
	{
		assert(szText != NULL);
		const char *pstr = szText;

		int length = 0;
		while (*pstr++ && ++length);

		return length;
	}

	double XString::atof(char *src)
	{
		double power, value;
		int i, sign;
		assert(src != NULL);//判断字符串是否为空  
		for (i = 0; src[i] == ' '; i++);//除去字符串前的空格  
		sign = (src[i] == '-') ? -1 : 1;
		if (src[i] == '-' || src[i] == '+')//要是有符号位就前进一位  
			i++;
		for (value = 0.0; isdigit(src[i]); i++)//计算小数点前的数字  
			value = value*10.0 + (src[i] - '0');
		if (src[i] == '.')
			i++;
		for (power = 1.0; isdigit(src[i]); i++)//计算小数点后的数字  
		{
			value = value*10.0 + (src[i] - '0');
			power *= 10.0;
		}
		return sign*value / power;
	}


}// XMyTools end
