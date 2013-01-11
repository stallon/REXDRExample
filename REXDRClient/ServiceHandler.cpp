
#include "StdAfx.h"
#include "ServiceHandler.h"
#include "Helper.h"

namespace REXDRClient 
{

ServiceHandler::ServiceHandler(REXDR::Connector::Handle hConnector, Common::Log4XWrapper& logger)
	: hConnector_(hConnector), logger_(logger), context_(NULL)
{

}


ServiceHandler::~ServiceHandler(void)
{
	if ( NULL != context_)
	{
		delete context_;
	}
}


REXDR::Connector::Handle ServiceHandler::GetConnectorHandle() const
{
	return hConnector_;
}

void* ServiceHandler::GetContext() const
{
	return context_;
}

void ServiceHandler::SetContext(void* context)
{
	if ( NULL != context_ )
	{
		delete context_;
	}

	this->context_ = context;
}


bool ServiceHandler::QueryUserSession()
{
	if ( NULL == context_ )
	{
		if ( logger_.IsErrorEnabled() )
		{
			logger_.Log(Log4X::LogLevel::Error, "QueryUserSession() FAILED. userid context is NULL.");
		}
		return false;
	}

	std::string* userid = (std::string*)context_;
	QueryUserSessionStatus query(*userid);

	if ( !REXDR::Connector::Request(hConnector_, &query, (void*)this, &ServiceHandler::OnQueryUserSessionStatusResponse ) )
	{
		if ( logger_.IsErrorEnabled() )
		{
			logger_.LogFormat(Log4X::LogLevel::Error, "Request:QueryUserSession FAILED for User(%s), ErrorCode = %d", userid->c_str(), XPlatform::GetLastError());
			return false;
		}
	}

	if ( logger_.IsDebugEnabled() )
	{
		logger_.LogFormat(Log4X::LogLevel::Debug, "Request:QueryUserSession SUCCEEDED for User(%s)", userid->c_str());
	}

	return true;
}


bool ServiceHandler::UpdateUserSession(unsigned int sessionid, const std::string& gameid, const std::string& channelid, const std::string& reserve )
{
	if ( NULL == context_ )
	{
		if ( logger_.IsErrorEnabled() )
		{
			logger_.Log(Log4X::LogLevel::Error, "UpdateUserSession() FAILED. userid context is NULL.");
		}
		return false;
	}

	UpdateUserSessionStatus update;
	update.userid = *(std::string*)context_;
	update.sessionid = sessionid;
	update.gameid = gameid;
	update.channelid = channelid;
	update.reserve = reserve;

	if ( !REXDR::Connector::Request(hConnector_, &update, (void*)this, &ServiceHandler::OnUpdateUserSessionStatusResponse ) )
	{
		if ( logger_.IsErrorEnabled() )
		{
			logger_.LogFormat(Log4X::LogLevel::Error, "Request:UpdateUserSession FAILED for User(%s), ErrorCode = %d", update.userid.c_str(), XPlatform::GetLastError());
			return false;
		}
	}

	if ( logger_.IsDebugEnabled() )
	{
		logger_.LogFormat(Log4X::LogLevel::Debug, "Request:QueryUserSession SUCCEEDED for User(%s)", update.userid.c_str());
	}

	return true;
}


void __stdcall ServiceHandler::OnQueryUserSessionStatusResponse(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp, void* context)
{
	ServiceHandler *handler = static_cast<ServiceHandler*>(context);
	handler->ProcessQuery(errorCode, resp);
}


void __stdcall ServiceHandler::OnUpdateUserSessionStatusResponse(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp, void* context)
{
	ServiceHandler *handler = static_cast<ServiceHandler*>(context);
	handler->ProcessUpdate(errorCode, resp);
}

//void __stdcall ServiceHandler::OnEventOccurred(REXDR::Connector::Handle hConnector, REXDR::Connector::EventCode eventCode, void* eventData, void* context)
//{
//	ServiceHandler* handler = static_cast<ServiceHandler*>(context);
//	handler->ProcessEvent(eventCode, eventData);
//}

void ServiceHandler::ProcessQuery(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp)
{
	QueryUserSessionStatusResponse response;

	switch ( errorCode )
	{
	case REXDR::Connector::ERR_SUCCESS:
		if ( !response.Load(resp) )
		{
			if ( logger_.IsErrorEnabled() )
			{
				logger_.LogFormat(Log4X::LogLevel::Error, "ProcessQuery:Load() FAILED for User(%s), Error: %s", response.userid.c_str(), response.GetLastError());
			}

			return;
		}

		// print the result onto Console
		printf("===> [RESULT] User(%s), session(%d), game(%s), channel(%s), status(%d), error(%d)\n\n", 
			response.userid.c_str(), response.sessionid, response.gameid.c_str(), response.channelid.c_str(), response.sessionstatus, response.errorcode);
		break;

	default:
		if ( logger_.IsErrorEnabled() )
		{
			logger_.LogFormat(Log4X::LogLevel::Error, "ProcessQuery FAILED, ErrorCode(%d) Response = %s", errorCode, resp.toString());
		}
		break;
	}

	// delete this handler
	delete this;
}


void ServiceHandler::ProcessUpdate(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp)
{
	UpdateUserSessionStatusResponse response;

	switch ( errorCode )
	{
	case REXDR::Connector::ERR_SUCCESS:
		if ( !response.Load(resp) )
		{
			if ( logger_.IsErrorEnabled() )
			{
				logger_.LogFormat(Log4X::LogLevel::Error, "ProcessUpdate:Load() FAILED for User(%s), Error: %s", response.userid.c_str(), response.GetLastError());
			}

			return;
		}

		// print the result onto Console
		printf("===> [RESULT] User(%s), Session(%d), status(%d)\n\n", 
			response.userid.c_str(), response.sessionid, response.errorcode);
		break;

	default:
		if ( logger_.IsErrorEnabled() )
		{
			logger_.LogFormat(Log4X::LogLevel::Error, "ProcessUpdate FAILED, ErrorCode(%d). Response = %s", errorCode, resp.toString());
		}
		break;
	}

	// delete this handler
	if ( logger_.IsInfoEnabled() )
	{
		logger_.Log(Log4X::LogLevel::Info, "REXDRClient ServiceHandler will be destroyed. ");
	}
	delete this;
}


//void ServiceHandler::ProcessEvent(REXDR::Connector::EventCode eventCode, void* eventData)
//{
//	switch (eventCode)
//	{
//	case REXDR::Connector::EVT_CONNECTION_ESTABLISHED:
//		printf(">>> [EVENT] EVT_CONNECTION_ESTABLISHED \n");
//		break;
//	case REXDR::Connector::EVT_CONNECTION_CLOSED:
//		printf(">>> [EVENT] EVT_CONNECTION_CLOSED \n");
//		break;
//	case REXDR::Connector::EVT_CONNECTION_FAILED:
//		printf(">>> [EVENT] EVT_CONNECTION_FAILED \n");
//		break;
//	default:
//		printf(">>> [EVENT] UNKNOWN Event(%d) \n", eventCode);
//		break;
//	}
//}

}	// end of 'namespace REXDRClient'
