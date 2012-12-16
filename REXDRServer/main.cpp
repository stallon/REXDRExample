// REXDRServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "REXDRListenerHandler.h"

#define HTTP_LISTENER_ID	13400
#define	LISTEN_PORT			13403

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

int _tmain(int argc, _TCHAR* argv[])
{
	// REXDRServer Initialization
	REXDRListenerHandler httpHandler(HTTP_LISTENER_ID, REXDR::Listener::TRANSPORT_TCP, LISTEN_PORT);

	if ( false == httpHandler.CreateListenerHandle() )
	{
		printf(">>> %s: REXDR Listener NOT Created. Exit...\n", __FUNCTION__);
		return -1;
	}

	if ( false == httpHandler.StartListener() )
	{
		printf(">>> %s: REXDR Listener NOT Started. Exit...\n", __FUNCTION__);
		return -1;
	}

	// Thread for graceful stop
	_beginthreadex(NULL, 0, stopHandler, &httpHandler, 0, NULL);

	// Wait until REXDRListenerHandler::StopListener is called by any thread
	httpHandler.WaitListenerForStop();

	return 0;
}

