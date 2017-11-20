#include "XFile.h"


namespace XMyTools
{

	XFile::XFile(bool bChinese/* = false*/)
	{
		if (bChinese)
		{
			m_fstream.imbue(std::locale("chs"));
		}
	}


	XFile::~XFile(void)
	{
	}

	bool XFile::openFile(const xchar *pszFileName, std::ios_base::openmode nMode)
	{
		m_fstream.open(pszFileName, nMode);
		if (!m_fstream.is_open())
			return false;

		return true;
	}

	bool XFile::isOpen()
	{
		return m_fstream.is_open();
	}

	void XFile::closeFile()
	{
		if (m_fstream.is_open())
			m_fstream.close();
	}

	int XFile::getFileSize()
	{
		if (!m_fstream.is_open())
			return false;

		m_fstream.seekg(0, std::ios_base::end);
		int nSize = m_fstream.tellg();

		m_fstream.seekg(0);

		return nSize;
	}

	bool XFile::readAll(xstring &strData)
	{
		// 方法一
		if (!m_fstream.is_open())
			return false;

		if (0 == getFileSize())
			return false;

		strData = xstring((std::istreambuf_iterator<xchar>(m_fstream)), std::istreambuf_iterator<xchar>());

		// 方法二
		/*xstringstream buffer;
		buffer << ifs.rdbuf();
		strContent = xstring(buffer.str());*/

		return true;
	}

	xstring XFile::readLine()
	{
		if (!m_fstream.is_open())
			return false;

		if (0 == getFileSize())
			return false;

		xstring strLine;
		getline(m_fstream, strLine);

		return strLine;
	}

	bool XFile::write(xstring &strData)
	{
		if (!m_fstream.is_open())
			return false;

		m_fstream.write(strData.c_str(), strData.size());
		m_fstream.write(X("\n"), wcslen(X("\n")));

		return true;
	}

	void XFile::flush()
	{
		m_fstream.flush();
	}

	void XFile::save()
	{
		if (m_fstream.is_open())
			m_fstream.close();
	}

	bool XFile::getFileInfo(xstring strFile, _finddata_t &file_info)
	{
		////打开文件查找句柄  
		//int handle = _findfirst(strFile.c_str(), &file_info);

		////返回值为-1则查找失败  
		//if (-1 == handle)
		//	return false;

		return true;
	}

	void XFile::traversalCurFiles(xstring path)
	{
		//_finddata_t file_info;

		////可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台
		//xstring current_path = path + "/*.*";

		////打开文件查找句柄  
		//int handle = _findfirst(current_path.c_str(), &file_info);

		//if (-1 == handle)
		//	return;

		//do
		//{
		//	//判断是否子目录
		//	xstring attribute;
		//	if (file_info.attrib == _A_SUBDIR) //是目录
		//		attribute = "dir";
		//	else
		//		attribute = "file";

		//	if (strcmp(file_info.name, ".") != 0 && strcmp(file_info.name, "..") != 0)
		//	{
		//		//打印：文件名(带后缀)、文件最后修改时间、文件字节数(文件夹显示0)、文件/目录
		//		//获得的最后修改时间是time_t格式的长整型，需要用其他方法转成正常时间显示
		//		//std::cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << std::endl; 
		//	}

		//} while (-1 != _findnext(handle, &file_info));

		////关闭文件句柄  
		//_findclose(handle);
	}

	void XFile::traversalAllFiles(xstring path)
	{
		//_finddata_t file_info;

		//// 也可以用/*来匹配所有
		//xstring current_path = path + "/*.*";

		//int nHanleFile = _findfirst(current_path.c_str(), &file_info);

		////返回值为-1则查找失败  
		//if (-1 == nHanleFile)
		//	return;

		//do
		//{
		//	//判断是否子目录  
		//	if (_A_SUBDIR == file_info.attrib)
		//	{
		//		// 打印当前的文件夹名称
		//		// std::cout << file_info.name << std::endl;  

		//		//递归遍历子目录
		//		if ((strcmp(file_info.name, "..") != 0) && (strcmp(file_info.name, ".") != 0))  //.是当前目录，..是上层目录，必须排除掉这两种情况  
		//			traversalAllFiles(path + '/' + file_info.name); // 在windows下可以用\\转义分隔符，不推荐  
		//	}
		//	else
		//	{
		//		// 打印当前的文件名称
		//		// std::cout << file_info.name << std::endl;  
		//	}
		//} while (-1 != _findnext(nHanleFile, &file_info));  //返回0则遍历完

		////关闭文件句柄  
		//_findclose(nHanleFile);
	}

	bool XFile::isExist(xstring strFileName)
	{
		//int nRet =_access(strFileName.c_str(), F_OK);
		//if (-1 == nRet)
		//	return false;

		return true;
	}

	bool XFile::isRead(xstring strFileName)
	{
		//int nRet = _access(strFileName.c_str(), R_OK);
		//if (-1 == nRet)
		//	return false;

		return true;
	}

	bool XFile::isWrite(xstring strFileName)
	{
		//int nRet = _access(strFileName.c_str(), W_OK);
		//if (-1 == nRet)
		//	return false;

		return true;
	}

	bool XFile::isExcute(xstring strFileName)
	{
		//int nRet = _access(strFileName.c_str(), X_OK);
		//if (-1 == nRet)
		//	return false;

		return true;
	}

}
