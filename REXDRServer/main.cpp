// REXDRServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "REXDRListenerHandler.h"
#include <Log4X/Log4X.h>

#define HTTP_LISTENER_ID	13400
#define	LISTEN_PORT			13403
#define LOG4X_NAMESPACE		_T("REXDRServerLog4X")


unsigned __stdcall stopHandler(void* arg)
{
	printf(">>> Press RETURN to exit....\n");

	char buf[2];
	fgets(buf, 2, stdin);

	REXDRListenerHandler* handler = (REXDRListenerHandler*)arg;
	if ( NULL != handler )
	{
		handler->StopListener();
	}

	return 0;
}

void Log(
int _tmain(int argc, _TCHAR* argv[])
{
	// Initialize Log4X
	Log4X::Handle hLog4X = NULL;

	if ( !Log4X::Initialize(LOG4X_NAMESPACE, _T("REXDRServerLog4X.xml")) )
	{
		printf(">>> [REXDRServer: main.cpp] Log4X Initialization Failed. \n");
	}
	else
	{
#ifdef _DEBUG
		hLog4X = Log4X::GetLogger(LOG4X_NAMESPACE, _T("DebugLogger"));
#else
		hLog4X = Log4X::GetLogger(LOG4X_NAMESPACE, _T("ReleaseLogger));
#endif
	}


	// REXDRServer Initialization
	REXDRListenerHandler httpHandler(HTTP_LISTENER_ID, REXDR::Listener::TRANSPORT_TCP, LISTEN_PORT);

	if ( false == httpHandler.CreateListenerHandle() )
	{
		Log4X::GetLoggerLevel(
		Log4X::Error(hLog4X, 
		printf("%s: REXDR Listener NOT Created. Exit...\n", __FUNCTION__);
		return -1;
	}

	// REXDRServer property settings in advance to starting it.
	if ( NULL != hLog4X ) 
	{
		httpHandler.SetLogger(hLog4X);
	}

	httpHandler.SetKeepAliveTimeout( 120 * 1000 );	// keepalive timeout is 2 minutes.


	// Start a REXDRServer
	if ( false == httpHandler.StartListener() )
	{
		
		return -1;
	}

	// Thread for graceful stop
	_beginthreadex(NULL, 0, stopHandler, &httpHandler, 0, NULL);

	// Wait until REXDRListenerHandler::StopListener is called by any thread
	httpHandler.WaitListenerForStop();
	Log4X::Uninitialize(LOG4X_NAMESPACE);

	return 0;
}

