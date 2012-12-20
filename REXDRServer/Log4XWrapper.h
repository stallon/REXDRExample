#pragma once

#include <Log4X/Log4X.h>

class Log4XWrapper
{
public:
	enum LogLevel { LL_EMERG,
					LL_ALERT,
				    LL_FATAL,
					LL_ERROR,
					LL_WARNING,
					LL_NOTICE,
					LL_INFO,
					LL_DEBUG,
					LL_TRACE };

	Log4XWrapper(const TCHAR* logNamespace, const TCHAR* configFilename, const TCHAR* loggerName);
	virtual ~Log4XWrapper(void);

	bool IsLoggerEnabled() const;
	LogLevel GetLoggerLevel() const;




private:
	const TCHAR* logNamespace_;

	Log4X::Handle hLogger_;
	bool isAvailable;
};

