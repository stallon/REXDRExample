// REXDRServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "REXDRListenerHandler.h"
#include "Log4XWrapper.h"

#define HTTP_LISTENER_ID	13400
#define	LISTEN_PORT			13403

#define LOG4X_NAMESPACE		_T("REXDRClientServerLog4X")
#define LOG4X_CONFIGFILE	_T("REXDRClientServerLog4X.xml")
#ifdef _DEBUG
#define LOGGER_NAME			_T("DebugLogger")
#else
#define	LOGGER_NAME			_T("ReleaseLogger")
#endif



unsigned __stdcall stopHandler(void* arg)
{
	printf(">>> Press RETURN to exit....\n");

	char buf[2];
	fgets(buf, 2, stdin);

	HANDLE hWaitEvent = *((HANDLE*)arg);
	if ( NULL != INVALID_HANDLE_VALUE )
	{
		::SetEvent(hWaitEvent);
	}

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	// define global logger
	Log4XWrapper gLogger(LOG4X_NAMESPACE, LOG4X_CONFIGFILE, LOGGER_NAME);

	if ( !gLogger.IsLoggerEnabled() )
	{
		printf(">>> [REXDRServer: main.cpp] Log4X Initialization Failed. \n");
		return -1;
	}

	// REXDRServer Initialization
	REXDRListenerHandler httpHandler(HTTP_LISTENER_ID, REXDR::Listener::TRANSPORT_TCP, LISTEN_PORT);
	
	// Logger Setting for REXDRServer
	if ( gLogger.IsLoggerEnabled() ) 
	{
		httpHandler.SetLogger(&gLogger);
	}

	if ( false == httpHandler.CreateListenerHandle() )
	{
		if ( gLogger.IsFatalEnabled() )
		{
			gLogger.LogFormat(Log4X::LogLevel::Fatal, _T("%s: REXDR Listener NOT Created. Exit...\n"), __FUNCTION__);
		}
		return -1;
	}

	httpHandler.SetKeepAliveTimeout( 60 * 1000 );	// keepalive timeout is 1 minutes.

	// Start a REXDRServer
	if ( false == httpHandler.StartListener() )
	{
		
		return -1;
	}

		// Initialize Server-Loop Waiting Event
	HANDLE hWaitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	// Thread for graceful stop
	_beginthreadex(NULL, 0, stopHandler, &hWaitEvent, 0, NULL);

	// Wait until REXDRListenerHandler::StopListener is called by any thread
	::WaitForSingleObject(hWaitEvent, INFINITE);
	httpHandler.StopListener();

	return 0;
}

