#include "StdAfx.h"
#include "Log4XWrapper.h"


Log4XWrapper::Log4XWrapper(const TCHAR* logNamespace, const TCHAR* configFilename, const TCHAR* loggerName)
	: logNamespace_(logNamespace)
{
//	_tcscpy_s(logNamespace_, sizeof(logNamespace), logNamespace);

	if ( !Log4X::Initialize(logNamespace_, configFilename) )
	{
		printf(">>> [%s]: Log4X FAILED to initialize.\n");
		hLogger_ = NULL;
		isAvailable = false;
	}
	else
	{
		hLogger_ = Log4X::GetLogger(logNamespace, loggerName);
		isAvailable = true;
	}
}


Log4XWrapper::~Log4XWrapper(void)
{
	Log4X::Uninitialize(logNamespace_);
}


bool Log4XWrapper::IsLoggerEnabled() const
{
	return isAvailable;
}


Log4XWrapper::LogLevel Log4XWrapper::GetLoggerLevel() const
{
	return Log4X::GetLoggerLevel(hLogger_);
}