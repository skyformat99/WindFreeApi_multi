#ifndef CLOG4Z_MANAGER_H_
#define CLOG4Z_MANAGER_H_
#include <log4z.h>

namespace XMyTools
{
	enum EN_LOG_LEVEL
	{
		en_level_trace,
		en_level_debug,
		en_level_info,
		en_level_warn,
		en_level_error,
		en_level_alarm,
		en_level_fatal,
	};

	class XLog4z
	{
		bool m_bStartLog4z;
	private:
		XLog4z();

	public:
		~XLog4z();
		static XLog4z& getInstance();

		/*
		 * @name initLog4zConfig
		 *
		 * @brief ��ʼ��log4z������ò�����logz4
		 *
		 * @param szFileName log4z�������ļ���
		 *
		 * return ����false����ʼ��ʧ�ܣ������ʼ���ɹ�
		 */ 
		bool initLog4zConfig(const char *szFileName);

		/*
		* @name uninit
		*
		* @brief ֹͣlog4z
		*
		* return ����false��ֹͣʧ�ܣ�����ֹͣ�ɹ�
		*/
		bool uninit();

		/*
		* @name setUpdateConfigTime
		*
		* @brief �������õĸ���ʱ��
		*
		* @param nSecond ���µ�ʱ��������λ���룬0��ʾ��ֹ�Զ�����
		*
		* return ����false������ʧ�ܣ��������óɹ�
		*/
		bool setUpdateConfigTime(int nSecond);

		/*
		* @name createLoggerId
		*
		* @brief ����loggerId
		*
		* @param szLoggerName �Զ����logger����
		*
		* return ����-1������ʧ�ܣ����򴴽��ɹ�
		*/
		LoggerId createLoggerId(const char *szLoggerName); 

		/*
		* @name write
		*
		* @brief д����־
		*
		* @param loggerID createLoggerId���ص�loggerId
		*
		* @param szLog Ҫд�����־
		*
		* @param enLogLevel ��־�ȼ���ȡֵ����EN_LOG_LEVEL
		*
		* return ����false�� д��ʧ�ܣ�����д��ɹ�
		*/
		bool write(LoggerId loggerID, const char *szLog, EN_LOG_LEVEL enLogLevel = en_level_debug);

	};
	

}// XMyTools end

#endif

