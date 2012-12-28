#include "Log4XWrapper.h"
#include <stdio.h>
#include <Windows.h>
#include <stdarg.h>


namespace Common
{

Log4XWrapper::Log4XWrapper(const TCHAR* logNamespace, const TCHAR* configFilename, const TCHAR* loggerName)
	: logNamespace_(logNamespace)
{
//	_tcscpy_s(logNamespace_, sizeof(logNamespace), logNamespace);

	if ( !Log4X::Initialize(logNamespace_, configFilename) )
	{
		printf(">>> Log4XWrapper: Log4X FAILED to initialize.\n");
		hLogger_ = NULL;
		isAvailable = false;
	}
	else
	{
		hLogger_ = Log4X::GetLogger(logNamespace, loggerName);

		if ( NULL != hLogger_ )
		{
			printf(">>> Log4XWrapper: Log4X successfully initialized.\n");
			isAvailable = true;
		}
		else
		{ 
			printf(">>> Log4XWrapper: Log4X FAILED to initialize. (No Logger Found) \n");
			isAvailable = false;
		}
	}
}


Log4XWrapper::~Log4XWrapper(void)
{
	Log4X::Uninitialize(logNamespace_);
	printf(">>> Log4XWrapper: Log4X is uninitialized. \n");
}


bool Log4XWrapper::IsLoggerEnabled() const
{
	return isAvailable;
}


Log4X::Handle& Log4XWrapper::GetLoggerHandle() 
{
	return hLogger_;
}

Log4X::LogLevel::Type Log4XWrapper::GetLoggerLevel() const
{
	return Log4X::GetLoggerLevel(hLogger_);
}


bool Log4XWrapper::IsEmergEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Emerg;
}

bool Log4XWrapper::IsAlertEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Alert;
}

bool Log4XWrapper::IsFatalEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Fatal;
}

bool Log4XWrapper::IsErrorEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Error;
}

bool Log4XWrapper::IsWarningEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Warning;
}

bool Log4XWrapper::IsInfoEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Info;
}

bool Log4XWrapper::IsNoticeEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Notice;
}

bool Log4XWrapper::IsDebugEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Debug;
}

bool Log4XWrapper::IsTraceEnabled() const
{
	return Log4X::GetLoggerLevel(hLogger_) <= Log4X::LogLevel::Trace;
}


void Log4XWrapper::Log(Log4X::LogLevel::Type loglevel, const char* message)
{
	if ( !isAvailable )
	{
		printf(">>> [%s]: Logger is NOT Available.\n", __FUNCTION__);
		return;
	}

	switch ( loglevel )
	{
	case Log4X::LogLevel::Emerg:
		Log4X::Emerg(hLogger_, message);
		break;
	case Log4X::LogLevel::Alert:
		Log4X::Alert(hLogger_, message);
		break;
	case Log4X::LogLevel::Fatal:
		Log4X::Fatal(hLogger_, message);
		break;
	case Log4X::LogLevel::Error:
		Log4X::Error(hLogger_, message);
		break;
	case Log4X::LogLevel::Warning:
		Log4X::Warning(hLogger_, message);
		break;
	case Log4X::LogLevel::Info:
		Log4X::Info(hLogger_, message);
		break;
	case Log4X::LogLevel::Notice:
		Log4X::Notice(hLogger_, message);
		break;
	case Log4X::LogLevel::Debug:
		Log4X::Debug(hLogger_, message);
		break;
	case Log4X::LogLevel::Trace:
		Log4X::Trace(hLogger_, message);
		break;
	default:
		break;
	}
}


void Log4XWrapper::LogFormat(Log4X::LogLevel::Type loglevel, const char* format, ...)
{
	char message[1024];

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(message, sizeof(message), format, argptr);
	va_end(argptr);
	
	this->Log(loglevel, message);
}


}