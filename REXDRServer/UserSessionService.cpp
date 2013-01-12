#include "StdAfx.h"
#include "UserSessionService.h"
#include "UserSessionContainer.h"
#include "REXDR_CS_Messages.h"
#include "REXDRListenerHandler.h"

namespace Service
{
	UserSessionService::UserSessionService(void)
	{
	}


	UserSessionService::~UserSessionService(void)
	{
	}


	void UserSessionService::OnGet(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req, void* context)
	{
		REXDRServer::REXDRListenerHandler* listener = (REXDRServer::REXDRListenerHandler*)context;
		Common::Log4XWrapper* logger = listener->GetLogger();

		QueryUserSessionStatus query;
		if ( !query.Load(req) )
		{
			if ( logger->IsErrorEnabled() )
			{
				logger->LogFormat(LL_ERROR, "ProcessQuery Load() FAILED. Error(%d). Request = %s", XPlatform::GetLastError(), req.toString());
			}
			return;
		}

		// extract userid from uri option string
		std::string userid = query.option["userid"];

		QueryUserSessionStatusResponse response;
		response.userid = userid;

		// Obtain UserSessionInfo from Storage and prepare for query response
		UserSessionInfoPtr userinfo = UserSessionStorage::Instance().GetSessionInfo(userid);
		
		response.sessionid = userinfo->sessionid;
		response.gameid = userinfo->gameid;
		response.channelid = userinfo->channelid;
		response.sessionstatus = userinfo->status;

		if ( userinfo->status != SESSION_NOTEXIST )
		{
			response.status.code = REXDR::STATUS_OK;

			if ( logger->IsDebugEnabled() )
			{
				logger->LogFormat(LL_DEBUG, "User(%s) Session(%d) query response sent.\n", userid.c_str(), response.sessionid);
			}
		}
		else
		{
			response.status.code = REXDR::STATUS_NOT_FOUND;
			response.status.message = std::string("user not exists -- ") + userid;
			response.errorcode = REXDR::STATUS_NOT_FOUND;

			if ( logger->IsInfoEnabled() )
			{
				logger->LogFormat(LL_INFO, "No such user(%s) found.", userid.c_str());
			}
		}

		REXDR::Listener::Link::Send(link, info, &response);
	}


	void UserSessionService::OnPost(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req, void* context)
	{
		// Obtain Logger reference from context
		REXDRServer::REXDRListenerHandler* listener = (REXDRServer::REXDRListenerHandler*)context;
		Common::Log4XWrapper* logger = listener->GetLogger();

		UpdateUserSessionStatus update;
		if ( !update.Load(req) )
		{
			if ( logger->IsErrorEnabled() )
			{
				logger->LogFormat(LL_ERROR, "ProcessUpdate Load() FAILED. Error(%d). Request = %s", XPlatform::GetLastError(), req.toString());
			}
			return;
		}

		// Update Session Storage
		UserSessionInfoPtr userInfo(new UserSessionInfoType(update.userid, update.sessionid, (UserSessionStatus)update.sessionstatus, update.gameid, update.channelid));
		UserSessionStorage::Instance().SetSessionInfo(userInfo);

		if ( logger->IsDebugEnabled() )
		{
			logger->LogFormat(LL_DEBUG, "User(%s) Session(%d) UPDATED\n", update.userid.c_str(), update.sessionid);
		}

		// Prepare for Update Response
		UpdateUserSessionStatusResponse response;
		response.userid = update.userid;
		response.sessionid = update.sessionid;
		response.errorcode = 0;
		response.status.code = REXDR::STATUS_OK;
		response.status.message = "Update success";

		REXDR::Listener::Link::Send(link, info, &response);

		if ( logger->IsDebugEnabled() )
		{
			logger->LogFormat(LL_DEBUG, "User(%s) Session(%d) response sent.\n", response.userid.c_str(), response.sessionid);
		}
	}


}	// end-of namespace 'UserSessionService'

