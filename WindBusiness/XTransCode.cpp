#include "XTransCode.h"
#include <vector>
#include <iterator>
#include <sstream>// std::istringstream 的定义位置
#include <iomanip>// setfill 和 setw 的定义位置


#ifdef _WINDOWS
#include <Windows.h>
#else
//...
#endif

namespace XMyTools
{


	XTransCode::XTransCode()
	{
	}


	XTransCode::~XTransCode()
	{
	}


	bool XTransCode::MBToUTF8(std::string &strUTF8, const char* pmb, int mLen)
	{
		//#ifdef WIN32
		std::vector<char> pu8;
		// convert an MBCS string to widechar   
		int nLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);

		WCHAR* lpszW = NULL;
		try
		{
			lpszW = new WCHAR[nLen];
		}
		catch (std::bad_alloc &memExp)
		{
			return false;
		}

		int nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, lpszW, nLen);

		if (nRtn != nLen)
		{
			delete[] lpszW;
			return false;
		}
		// convert an widechar string to utf8  
		int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);
		if (utf8Len <= 0)
		{
			return false;
		}
		pu8.resize(utf8Len);
		nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, &*pu8.begin(), utf8Len, NULL, NULL);
		delete[] lpszW;

		if (nRtn != utf8Len)
		{
			pu8.clear();
			return false;
		}

		copy(pu8.begin(), pu8.end(), std::back_inserter(strUTF8));
		std::vector<char>().swap(pu8);

		return true;
	}

	std::string XTransCode::UTF8TOMB(const char *strUTF8)
	{
#ifdef WIN32
		int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
		wchar_t *wszGBK = new wchar_t[nLen + 1];
		memset(wszGBK, 0, nLen * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, nLen);

		nLen = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char *szGBK = new char[nLen + 1];
		memset(szGBK, 0, nLen + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, nLen, NULL, NULL);

		std::string strTemp(szGBK);

		if (szGBK)
			delete[] szGBK;

		if (wszGBK)
			delete[] wszGBK;
#else

#endif

		return strTemp;
	}

	std::wstring XTransCode::GBKToUTF8(const wchar_t *buffer, int len)
	{

		std::string strUTF8;
		

#ifdef WIN32
		int size = WideCharToMultiByte(CP_UTF8, 0, buffer, len, NULL, 0, NULL, NULL);
		if (0 == size)
		{
			return L"";
		}

		strUTF8.resize(size);
		::WideCharToMultiByte(CP_UTF8, 0, buffer, len, const_cast<char*>(strUTF8.c_str()), size, NULL, NULL);
#else

#endif
		std::wstring wstrUTF8 = string_to_wstring(strUTF8);

		return wstrUTF8;
	}

	std::string XTransCode::UTF8TOGBK(const char *strUTF8)
	{
		int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
		wchar_t *wszGBK = new wchar_t[nLen + 1];
		memset(wszGBK, 0, nLen * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, nLen);

		nLen = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char *szGBK = new char[nLen + 1];
		memset(szGBK, 0, nLen + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, nLen, NULL, NULL);

		std::string strTemp(szGBK);

		if (szGBK)
			delete[] szGBK;

		if (wszGBK)
			delete[] wszGBK;

		return strTemp;
	}

	std::string XTransCode::wstring_to_string(const std::wstring& wstr)
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

	std::wstring XTransCode::string_to_wstring(const std::string& str)
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

	void XTransCode::string_to_hex(std::string &strSrc, std::string &strDest)
	{
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(2) << std::hex;
		for (size_t i = 0, sz = strSrc.size(); i < sz; ++i)
		{
			oss << (int)(unsigned char)strSrc[i] << ' ';
		}
		strDest = oss.str();
	}

	void XTransCode::hex_to_string(std::string &strSrc, std::string &strDest)
	{
		std::istringstream iss(strSrc);
		iss >> std::hex;
		int ch;
		while (iss >> ch)
		{
			strDest += (char)ch;
		}
	}

}// XMyTools end