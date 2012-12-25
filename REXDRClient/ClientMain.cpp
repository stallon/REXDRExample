// REXDRClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <REXDR/REXDRConnectorAPI.h>
#include "Log4XWrapper.h"

#define SERVER_IP		"10.64.50.209"
#define SERVER_PORT		13403

// Definitions for Log4XWrapper
#define LOG4X_NAMESPACE		_T("REXDRClientServerLog4X")
#define LOG4X_CONFIGFILE	_T("REXDRClientServerLog4X.xml")
#ifdef _DEBUG
#define LOGGER_NAME			_T("DebugLogger")
#else
#define	LOGGER_NAME			_T("ReleaseLogger")
#endif



// Callback Function - Default Response Handler
void __stdcall OnResponse( REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp, void* context)
{


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

	REXDR::Connector::Handle hConnector = REXDR::Connector::CreateHandle(REXDR::Connector::TRANSPORT_TCP, SERVER_IP, SERVER_PORT, 




	return 0;
}

