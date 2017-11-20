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
		* ���ֽڱ���תΪUTF8����
		* @param pmb Ҫת�����ַ���[in]
		* @param mLen pmb�ĳ���[in]
		* @param strUTF8 ת������ַ���[out]
		*
		* @return true--ת���ɹ�  false--ת��ʧ��
		*/
		static bool MBToUTF8(std::string &strUTF8, const char* pmb, int mLen);

		/*
		* UTF8TOMB
		*
		* UTF8����תΪ���ֽڱ���
		*
		* @param strUTF8 Ҫת�����ַ���[in]
		*
		* @return ת�����ַ��� [out]
		*/
		static std::string UTF8TOMB(const char *strUTF8);

		/*
		* GBKToUTF8
		*
		* ��gbk����ת��ΪUTF8����
		*
		* @param buffer Ҫת�����ַ���[in]
		* @param len buffer�ĳ���[in]
		*
		* @return ת�����ַ��������Ϊ�գ���ת��ʧ��
		*/
		static std::wstring GBKToUTF8(const wchar_t *buffer, int len);

		/*
		* UTF8TOGBK
		*
		* ��UTF8����ת��ΪGBK����
		*
		* @param strUTF8 Ҫת�����ַ���[in]
		*
		* @return ת�����ַ���
		*/
		static std::string UTF8TOGBK(const char *strUTF8);

		/*
		* wstring_to_string
		* �� wstring ת��Ϊ string
		*
		* @param wstr	Ҫת�����ַ���[in]
		*
		* @return ����ת������ַ���
		*/
		static std::string wstring_to_string(const std::wstring& wstr);

		/*
		* string_to_wstring
		*
		* �� string ת��Ϊ wstring
		*
		* @param str	Ҫת�����ַ���[in]
		*
		* @return ����ת������ַ���
		*/
		static std::wstring string_to_wstring(const std::string& str);

		/*
		* string_to_hex
		* ���ַ���ת��Ϊʮ������
		*
		* @param strSrc Ҫת�����ַ���[in]
		* @param strDest ת�����ʮ�������ַ���
		*
		* @return NA
		*/
		static void string_to_hex(std::string &strSrc, std::string &strDest);

		/*
		* hex_to_string
		*
		* ��ʮ������ת��Ϊ�ַ���
		*
		* @param strSrc Ҫת�����ַ���[in]
		* @param strDest ת������ַ���[out]
		*
		* @return NA
		*/
		static void hex_to_string(std::string &strSrc, std::string &strDest);
	};

}// XMyTools end

#endif