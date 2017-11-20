#include "XLog4z.h"
#include <assert.h>
#include <string>

namespace XMyTools
{
	XLog4z::XLog4z()
	{
		m_bStartLog4z = false;
	}

	XLog4z::~XLog4z()
	{
	}

	XLog4z & XLog4z::getInstance()
	{
		static XLog4z log4zIns;
		return log4zIns;
	}

	bool XLog4z::initLog4zConfig(const char * szFileName)
	{
		if (!zsummer::log4z::ILog4zManager::getRef().config(szFileName))
			return false;

		if (!zsummer::log4z::ILog4zManager::getRef().start())
			return false;

		m_bStartLog4z = true;

		return true;
	}

	bool XLog4z::uninit()
	{
		m_bStartLog4z = false;

		return zsummer::log4z::ILog4zManager::getRef().stop();
	}

	bool XLog4z::setUpdateConfigTime(int nSecond)
	{
		return zsummer::log4z::ILog4zManager::getRef().setAutoUpdate(nSecond);
	}

	LoggerId XLog4z::createLoggerId(const char *szLoggerName)
	{
		return zsummer::log4z::ILog4zManager::getRef().createLogger(szLoggerName);
	}

	bool XLog4z::write(LoggerId loggerID, const char *szLog, EN_LOG_LEVEL enLogLevel/* = en_level_debug*/)
	{
		if (!m_bStartLog4z)
			return false;

		if (!szLog || strlen(szLog) == 0)
			return false;

		if (en_level_debug == enLogLevel)
			LOGFMT_TRACE(loggerID, szLog);
		else if (en_level_debug == enLogLevel)
			LOGFMT_DEBUG(loggerID, szLog);
		else if (en_level_info == enLogLevel)
			LOGFMT_INFO(loggerID, szLog);
		else if (en_level_warn == enLogLevel)
			LOGFMT_WARN(loggerID, szLog);
		else if (en_level_error == enLogLevel)
			LOGFMT_ERROR(loggerID, szLog);
		else if (en_level_alarm == enLogLevel)
			LOGFMT_ALARM(loggerID, szLog);
		else if (en_level_fatal == enLogLevel)
			LOGFMT_FATAL(loggerID, szLog);

		return true;
	}
}// XMyTools end