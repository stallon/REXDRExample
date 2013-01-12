// REXDRClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>

#include <REXDR/REXDRConnectorAPI.h>
#include "ServiceHandler.h"
#include "REXDR_CS_Messages.h"
#include "Log4XWrapper.h"


#define SERVER_IP		"127.0.0.1"
#define SERVER_PORT		13403

// Definitions for Log4XWrapper
#define LOG4X_NAMESPACE		_T("REXDRClientLog4X")
#define LOG4X_CONFIGFILE	_T("REXDRClientServerLog4X.xml")
#ifdef _DEBUG
#define LOGGER_NAME			_T("DebugLogger")
#else
#define	LOGGER_NAME			_T("ReleaseLogger")
#endif



int _tmain(int argc, _TCHAR* argv[])
{
	// define global logger
	Common::Log4XWrapper gLogger(LOG4X_NAMESPACE, LOG4X_CONFIGFILE, LOGGER_NAME);

	if ( !gLogger.IsLoggerEnabled() )
	{
		printf(">>> [%s] Log4X Initialization Failed. \n", __FUNCTION__);
		return -1;
	}

	// Init connector and establish a connection to the server
	REXDR::Connector::Handle hConnector = REXDR::Connector::CreateHandle(REXDR::Connector::TRANSPORT_TCP, SERVER_IP, SERVER_PORT, 10*1000);
	if ( NULL == hConnector )
	{
		printf(">>> [%s] REXDRConnector COULD NOT be created.\n", __FUNCTION__);
		return -1;
	}

	REXDR::Connector::SetLogger(gLogger.GetLoggerHandle());
	REXDR::Connector::SetCodePage(hConnector, REXDR::CP_UTF_8);
	
	// message-loop
	while ( true )
	{
		int choice;

		printf(">>>>>>>>>> User Session Info Service <<<<<<<<<<\n\n");
		printf("[ 1  ] User Session Info Query\n");
		printf("[ 2  ] User Session Info Update\n");
		printf("[else] Exit Service\n\n");
		printf(">>>> SELECT (1/2) : ");
		std::cin >> choice;

		if ( 1 == choice )	// Session Status Query
		{	
			std::string userid;
			printf("\n> [QUERY] Enter userid : ");
			std::cin >> userid;

			if ( userid.size() == 0 )
			{
				printf("--- Invalid UserId : Zero length \n\n");
				break;
			}

			REXDRClient::ServiceHandler *handler = new REXDRClient::ServiceHandler(hConnector, gLogger);
			handler->SetContext(new std::string(userid));

			if ( !handler->QueryUserSession() )
			{
				if ( gLogger.IsInfoEnabled() )
				{
					gLogger.LogFormat(LL_INFO, "QueryUserSession FAILED for User(%s). REXDRClientHandler will be deleted.", userid.c_str());
				}
				delete handler;
				break;
			}

			continue;	// handler will be deleted in REXDRClient::ProcessQuery() after successful response.
		} 
		else if ( 2 == choice )
		{
			std::string userid;
			unsigned int sessionid;
			std::string gameid;
			std::string channelid;
			std::string reserve;

			printf("\n> [UPDATE] Enter userid (string) : ");
			std::cin >> userid;

			printf("> [UPDATE] Enter sessionid (integer) : ");
			std::cin >> sessionid;

			printf("> [UPDATE] Enter gameid (string) : ");
			std::cin >> gameid;

			printf("> [UPDATE] Enter channelid (string) : ");
			std::cin >> channelid;

			printf("> [UPDATE] Enter reserve (string) : ");
			std::cin >> reserve;

			if ( userid.size() == 0 || sessionid < 0 || sessionid >= UINT_MAX )
			{
				printf("--- Invalid userid or sessionid \n\n");
				break;
			}
			
			REXDRClient::ServiceHandler *handler = new REXDRClient::ServiceHandler(hConnector, gLogger);
			handler->SetContext(new std::string(userid));

			if ( !handler->UpdateUserSession(sessionid, gameid, channelid, reserve) )
			{
				if ( gLogger.IsInfoEnabled() )
				{
					gLogger.LogFormat(LL_INFO, "UpdateUserSession FAILED for User(%s). REXDRClientHandler will be deleted.", userid.c_str());
				}
				delete handler;
				break;
			}

			continue;	// handler will be deleted in REXDRClient::ProcessQuery() after successful response.
		}
		else
		{
			printf("Exit...!!\n");
			break;
		}

	}

	
	REXDR::Connector::DestroyHandle(hConnector);

	return 0;
}
