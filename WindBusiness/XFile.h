#ifndef XFILE_H_
#define XFILE_H_
#include <string>
#include <fstream>
#include <io.h>// _finddata_t定义头文件
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
		F_OK = 0,	// 判断是否存在
		R_OK = 2,	// 判断写入权限
		W_OK = 4,	// 判断读取权限
		X_OK = 6,	// 判断执行权限
	};

	class XFile
	{
		xfstring m_fstream;

	public:
		// 写入/读取的文件是否包含中文
		XFile(bool bChinese = false);
		~XFile(void);

		// 打开文件
		bool openFile(const xchar *pszFileName, std::ios_base::openmode nMode);

		bool isOpen();

		// 关闭文件
		void closeFile();

		// 获取文件大小
		int getFileSize();

		/*
		 * readAll
		 *
		 * 读文件
		 *
		 * @param strContent    存储读取的内容[out]
		 *
		 */
		bool readAll(xstring &strData);

		// 读取行
		xstring readLine();

		/*
		 * write
		 *
		 * 保存内容到文件
		 *
		 * @param strContent    要保存的内容[in]
		 *
		 */
		bool write(xstring &strData);

		void flush();

		void save();

		/*
		 * getFileInfo
		 *
		 * 获取文件的信息
		 *
		 * @param strFile 要获取信息的完整文件路径[in]
		 *
		 * @return true--成功  false--失败
		 *
		 * @note 要获取的文件信息包括如下内容：
		 *       文件属性
		 *       文件创建时间
		 *       文件上一次访问时间
		 *       文件上一次修改时间
		 *       文件字节数和文件名
		 */
		bool getFileInfo(xstring strFile, _finddata_t &file_info);

		/*
		* traversalCurFiles
		*
		* 遍历当前文件夹（不遍历子文件夹）
		*
		* @param path 要遍历的路径[in]
		*
		* @return NA
		*/
		void traversalCurFiles(xstring path);

		/*
		* traversalAllFiles
		*
		* 遍历当前目录下文件夹和文件及子文件夹和文件
		*
		* @param path 要遍历的路径[in]
		*
		* @return NA
		*/
		void traversalAllFiles(xstring path);

		// 判断文件是否存在
		bool isExist(xstring strFileName);

		// 判断文件是否有读的权限
		bool isRead(xstring strFileName);

		// 判断文件是否有写的权限
		bool isWrite(xstring strFileName);

		// 判断文件是否有执行的权限
		bool isExcute(xstring strFileName);
	};

}

#endif