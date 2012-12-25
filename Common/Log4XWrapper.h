#pragma once

#include <tchar.h>
#include <Log4X/Log4X.h>


class Log4XWrapper
{
public:
	Log4XWrapper(const TCHAR* logNamespace, const TCHAR* configFilename, const TCHAR* loggerName);
	virtual ~Log4XWrapper(void);

	bool IsLoggerEnabled() const;
	Log4X::Handle& GetLoggerHandle();
	Log4X::LogLevel::Type GetLoggerLevel() const;

	bool IsEmergEnabled() const;
	bool IsAlertEnabled() const;
	bool IsFatalEnabled() const;
	bool IsErrorEnabled() const;
	bool IsWarningEnabled() const;
	bool IsInfoEnabled() const;
	bool IsNoticeEnabled() const;
	bool IsDebugEnabled() const;
	bool IsTraceEnabled() const;

	void Log(Log4X::LogLevel::Type loglevel, const TCHAR* message);
	void LogFormat(Log4X::LogLevel::Type loglevel, const TCHAR* format, ...);


private:
	const TCHAR* logNamespace_;

	Log4X::Handle hLogger_;
	bool isAvailable;
};

