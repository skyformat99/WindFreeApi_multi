#ifndef XFILE_H_
#define XFILE_H_
#include <string>
#include <fstream>
#include <io.h>// _finddata_t����ͷ�ļ�
#include <vector>
#include "XType.h"


#ifdef _WINDOWS
#include <Windows.h>
#else
//...
#endif

namespace XMyTools
{

	enum EN_FILE_MODE
	{
		F_OK = 0,	// �ж��Ƿ����
		R_OK = 2,	// �ж�д��Ȩ��
		W_OK = 4,	// �ж϶�ȡȨ��
		X_OK = 6,	// �ж�ִ��Ȩ��
	};

	class XFile
	{
		xfstring m_fstream;

	public:
		// д��/��ȡ���ļ��Ƿ��������
		XFile(bool bChinese = false);
		~XFile(void);

		// ���ļ�
		bool openFile(const xchar *pszFileName, std::ios_base::openmode nMode);

		bool isOpen();

		// �ر��ļ�
		void closeFile();

		// ��ȡ�ļ���С
		int getFileSize();

		/*
		 * readAll
		 *
		 * ���ļ�
		 *
		 * @param strContent    �洢��ȡ������[out]
		 *
		 */
		bool readAll(xstring &strData);

		// ��ȡ��
		xstring readLine();

		/*
		 * write
		 *
		 * �������ݵ��ļ�
		 *
		 * @param strContent    Ҫ���������[in]
		 *
		 */
		bool write(xstring &strData);

		void flush();

		void save();

		/*
		 * getFileInfo
		 *
		 * ��ȡ�ļ�����Ϣ
		 *
		 * @param strFile Ҫ��ȡ��Ϣ�������ļ�·��[in]
		 *
		 * @return true--�ɹ�  false--ʧ��
		 *
		 * @note Ҫ��ȡ���ļ���Ϣ�����������ݣ�
		 *       �ļ�����
		 *       �ļ�����ʱ��
		 *       �ļ���һ�η���ʱ��
		 *       �ļ���һ���޸�ʱ��
		 *       �ļ��ֽ������ļ���
		 */
		bool getFileInfo(xstring strFile, _finddata_t &file_info);

		/*
		* traversalCurFiles
		*
		* ������ǰ�ļ��У����������ļ��У�
		*
		* @param path Ҫ������·��[in]
		*
		* @return NA
		*/
		void traversalCurFiles(xstring path);

		/*
		* traversalAllFiles
		*
		* ������ǰĿ¼���ļ��к��ļ������ļ��к��ļ�
		*
		* @param path Ҫ������·��[in]
		*
		* @return NA
		*/
		void traversalAllFiles(xstring path);

		// �ж��ļ��Ƿ����
		bool isExist(xstring strFileName);

		// �ж��ļ��Ƿ��ж���Ȩ��
		bool isRead(xstring strFileName);

		// �ж��ļ��Ƿ���д��Ȩ��
		bool isWrite(xstring strFileName);

		// �ж��ļ��Ƿ���ִ�е�Ȩ��
		bool isExcute(xstring strFileName);
	};

}

#endif