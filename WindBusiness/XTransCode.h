#ifndef CTRANS_CODE_H_
#define CTRANS_CODE_H_
#include <string>

namespace XMyTools
{

	class XTransCode
	{
		XTransCode();
	public:
		~XTransCode();

		/*
		* MBToUTF8
		*
		* 多字节编码转为UTF8编码
		* @param pmb 要转换的字符串[in]
		* @param mLen pmb的长度[in]
		* @param strUTF8 转换后的字符串[out]
		*
		* @return true--转换成功  false--转换失败
		*/
		static bool MBToUTF8(std::string &strUTF8, const char* pmb, int mLen);

		/*
		* UTF8TOMB
		*
		* UTF8编码转为多字节编码
		*
		* @param strUTF8 要转换的字符串[in]
		*
		* @return 转换后字符串 [out]
		*/
		static std::string UTF8TOMB(const char *strUTF8);

		/*
		* GBKToUTF8
		*
		* 将gbk编码转换为UTF8编码
		*
		* @param buffer 要转换的字符串[in]
		* @param len buffer的长度[in]
		*
		* @return 转换后字符串，如果为空，则转换失败
		*/
		static std::wstring GBKToUTF8(const wchar_t *buffer, int len);

		/*
		* UTF8TOGBK
		*
		* 将UTF8编码转换为GBK编码
		*
		* @param strUTF8 要转换的字符串[in]
		*
		* @return 转换后字符串
		*/
		static std::string UTF8TOGBK(const char *strUTF8);

		/*
		* wstring_to_string
		* 将 wstring 转换为 string
		*
		* @param wstr	要转换的字符串[in]
		*
		* @return 返回转换后的字符串
		*/
		static std::string wstring_to_string(const std::wstring& wstr);

		/*
		* string_to_wstring
		*
		* 将 string 转换为 wstring
		*
		* @param str	要转换的字符串[in]
		*
		* @return 返回转换后的字符串
		*/
		static std::wstring string_to_wstring(const std::string& str);

		/*
		* string_to_hex
		* 将字符串转换为十六进制
		*
		* @param strSrc 要转换的字符串[in]
		* @param strDest 转换后的十六进制字符串
		*
		* @return NA
		*/
		static void string_to_hex(std::string &strSrc, std::string &strDest);

		/*
		* hex_to_string
		*
		* 将十六进制转换为字符串
		*
		* @param strSrc 要转换的字符串[in]
		* @param strDest 转换后的字符串[out]
		*
		* @return NA
		*/
		static void hex_to_string(std::string &strSrc, std::string &strDest);
	};

}// XMyTools end

#endif