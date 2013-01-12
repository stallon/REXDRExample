#pragma once

#include <tchar.h>
#include <Log4X/Log4X.h>

#define LL_OFF		Log4X::LogLevel::Off
#define LL_EMERG	Log4X::LogLevel::Emerg
#define LL_ALERT	Log4X::LogLevel::Alert
#define LL_FATAL	Log4X::LogLevel::Fatal
#define LL_ERROR	Log4X::LogLevel::Error
#define LL_WARN		Log4X::LogLevel::Warning
#define LL_INFO		Log4X::LogLevel::Info
#define LL_DEBUG	Log4X::LogLevel::Debug
#define LL_TRACE	Log4X::LogLevel::Trace


namespace Common
{
	class Log4XWrapper
	{
	public:

		typedef Log4X::LogLevel::Type	LogLevel;

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

		void Log(LogLevel loglevel, const char* message);
		void LogFormat(LogLevel loglevel, const char* format, ...);


	private:
		const TCHAR* logNamespace_;

		Log4X::Handle hLogger_;
		bool isAvailable;
	};

}
