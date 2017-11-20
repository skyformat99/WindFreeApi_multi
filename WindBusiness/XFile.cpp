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
		// ����һ
		if (!m_fstream.is_open())
			return false;

		if (0 == getFileSize())
			return false;

		strData = xstring((std::istreambuf_iterator<xchar>(m_fstream)), std::istreambuf_iterator<xchar>());

		// ������
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
		////���ļ����Ҿ��  
		//int handle = _findfirst(strFile.c_str(), &file_info);

		////����ֵΪ-1�����ʧ��  
		//if (-1 == handle)
		//	return false;

		return true;
	}

	void XFile::traversalCurFiles(xstring path)
	{
		//_finddata_t file_info;

		////���Զ������ĺ�׺Ϊ*.exe��*.txt���������ض���׺���ļ���*.*��ͨ�����ƥ����������,·�����ӷ��������б��/���ɿ�ƽ̨
		//xstring current_path = path + "/*.*";

		////���ļ����Ҿ��  
		//int handle = _findfirst(current_path.c_str(), &file_info);

		//if (-1 == handle)
		//	return;

		//do
		//{
		//	//�ж��Ƿ���Ŀ¼
		//	xstring attribute;
		//	if (file_info.attrib == _A_SUBDIR) //��Ŀ¼
		//		attribute = "dir";
		//	else
		//		attribute = "file";

		//	if (strcmp(file_info.name, ".") != 0 && strcmp(file_info.name, "..") != 0)
		//	{
		//		//��ӡ���ļ���(����׺)���ļ�����޸�ʱ�䡢�ļ��ֽ���(�ļ�����ʾ0)���ļ�/Ŀ¼
		//		//��õ�����޸�ʱ����time_t��ʽ�ĳ����ͣ���Ҫ����������ת������ʱ����ʾ
		//		//std::cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << std::endl; 
		//	}

		//} while (-1 != _findnext(handle, &file_info));

		////�ر��ļ����  
		//_findclose(handle);
	}

	void XFile::traversalAllFiles(xstring path)
	{
		//_finddata_t file_info;

		//// Ҳ������/*��ƥ������
		//xstring current_path = path + "/*.*";

		//int nHanleFile = _findfirst(current_path.c_str(), &file_info);

		////����ֵΪ-1�����ʧ��  
		//if (-1 == nHanleFile)
		//	return;

		//do
		//{
		//	//�ж��Ƿ���Ŀ¼  
		//	if (_A_SUBDIR == file_info.attrib)
		//	{
		//		// ��ӡ��ǰ���ļ�������
		//		// std::cout << file_info.name << std::endl;  

		//		//�ݹ������Ŀ¼
		//		if ((strcmp(file_info.name, "..") != 0) && (strcmp(file_info.name, ".") != 0))  //.�ǵ�ǰĿ¼��..���ϲ�Ŀ¼�������ų������������  
		//			traversalAllFiles(path + '/' + file_info.name); // ��windows�¿�����\\ת��ָ��������Ƽ�  
		//	}
		//	else
		//	{
		//		// ��ӡ��ǰ���ļ�����
		//		// std::cout << file_info.name << std::endl;  
		//	}
		//} while (-1 != _findnext(nHanleFile, &file_info));  //����0�������

		////�ر��ļ����  
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
