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
		assert(src != NULL);//�ж��ַ����Ƿ�Ϊ��  
		for (i = 0; src[i] == ' '; i++);//��ȥ�ַ���ǰ�Ŀո�  
		sign = (src[i] == '-') ? -1 : 1;
		if (src[i] == '-' || src[i] == '+')//Ҫ���з���λ��ǰ��һλ  
			i++;
		for (value = 0.0; isdigit(src[i]); i++)//����С����ǰ������  
			value = value*10.0 + (src[i] - '0');
		if (src[i] == '.')
			i++;
		for (power = 1.0; isdigit(src[i]); i++)//����С����������  
		{
			value = value*10.0 + (src[i] - '0');
			power *= 10.0;
		}
		return sign*value / power;
	}


}// XMyTools end
