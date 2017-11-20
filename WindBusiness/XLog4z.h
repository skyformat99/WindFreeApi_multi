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
		 * @brief 初始化log4z库的配置并启动logz4
		 *
		 * @param szFileName log4z的配置文件名
		 *
		 * return 返回false，初始化失败，否则初始化成功
		 */ 
		bool initLog4zConfig(const char *szFileName);

		/*
		* @name uninit
		*
		* @brief 停止log4z
		*
		* return 返回false，停止失败，否则停止成功
		*/
		bool uninit();

		/*
		* @name setUpdateConfigTime
		*
		* @brief 设置配置的更新时间
		*
		* @param nSecond 更新的时间间隔。单位：秒，0表示禁止自动更新
		*
		* return 返回false，设置失败，否则设置成功
		*/
		bool setUpdateConfigTime(int nSecond);

		/*
		* @name createLoggerId
		*
		* @brief 创建loggerId
		*
		* @param szLoggerName 自定义的logger名称
		*
		* return 返回-1，创建失败，否则创建成功
		*/
		LoggerId createLoggerId(const char *szLoggerName); 

		/*
		* @name write
		*
		* @brief 写入日志
		*
		* @param loggerID createLoggerId返回的loggerId
		*
		* @param szLog 要写入的日志
		*
		* @param enLogLevel 日志等级。取值见：EN_LOG_LEVEL
		*
		* return 返回false， 写入失败，否则写入成功
		*/
		bool write(LoggerId loggerID, const char *szLog, EN_LOG_LEVEL enLogLevel = en_level_debug);

	};
	

}// XMyTools end

#endif

