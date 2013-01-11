#include "StdAfx.h"
#include "REXDRListenerHandler.h"
#include "REXDR_CS_Messages.h"

namespace REXDRServer
{
	REXDRListenerHandler::REXDRListenerHandler(int handlerId, REXDR::Listener::TransportType type, uint16_t port) : 
		id_(handlerId), transportType_(type), port_(port), listener_(NULL), isListenerStopped(true), logger_(NULL)
	{
		InitializeCriticalSectionAndSpinCount(&userlistlock_, 4000);
	}


	REXDRListenerHandler::~REXDRListenerHandler(void)
	{
		if ( !isListenerStopped )
		{
			REXDR::Listener::Stop(listener_);
		}
	
		if ( logger_->IsLoggerEnabled() && logger_->IsDebugEnabled() )
		{
			logger_->Log(Log4X::LogLevel::Debug, "~REXDRListenerHandler(): Listener is being destroyed.");
		}

		REXDR::Dispatcher::DestroyHandle(dispatcher_);
		REXDR::Listener::DestroyHandle(listener_);

		DeleteCriticalSection(&userlistlock_);

	}

	REXDR::Listener::Handle REXDRListenerHandler::GetListenerHandle() const
	{
		return listener_;
	}

	int REXDRListenerHandler::GetListenerHandlerId() const
	{
		return id_;
	}

	bool REXDRListenerHandler::CreateListenerHandle()
	{
		if ( isListenerStopped )
		{
			listener_ = REXDR::Listener::CreateHandle(transportType_, NULL, port_);

			if ( NULL != listener_ )
			{
				if ( logger_->IsDebugEnabled() )
				{
					logger_->LogFormat(Log4X::LogLevel::Debug, "REXDRListener is created and listening at %d", port_);
				}

				// REXDR Dispatcher 생성. Dispatcher는 REXDRListener의 lifecycle과 같은게 좋을듯.
				dispatcher_ = REXDR::Dispatcher::CreateHandle();
				REXDR::Dispatcher::AttachListener(dispatcher_, listener_);

				REXDR::Dispatcher::SetResourceHandler(dispatcher_, "/stallon/usersession/${userid}", REXDR::CMD_GET, REXDRListenerHandler::OnQueryUserSession, this);
				REXDR::Dispatcher::SetResourceHandler(dispatcher_, "/stallon/usersession", REXDR::CMD_POST, REXDRListenerHandler::OnUpdateUserSession, this);
			
				return true;
			}
		}

		if ( logger_->IsFatalEnabled() )
		{
			logger_->Log(Log4X::LogLevel::Fatal, "New listener COULD NOT be created.");
		}
		return false;
	}

	bool REXDRListenerHandler::StartListener()
	{
		if ( NULL != listener_ && isListenerStopped )
		{
			// Set Main Event Handler ... OnAccept/Close/Request
			REXDR::Listener::SetOnAcceptHandler(listener_, REXDRListenerHandler::OnAcceptHandler, this);
			REXDR::Listener::SetOnCloseHandler(listener_, REXDRListenerHandler::OnCloseHandler, this);

			// Start a new REXDRListener
			if ( REXDR::Listener::Start(listener_) )
			{
				isListenerStopped = false;
				if ( logger_->IsDebugEnabled() )
				{
					logger_->LogFormat(Log4X::LogLevel::Debug, "REXDRListener (Id:%d) is starting", id_);
				}
				return true;
			}
		}

		if ( logger_->IsFatalEnabled() )
		{
			logger_->Log(Log4X::LogLevel::Fatal, "Failed to start REXDRListener");
		}
		return false;
	}


	bool REXDRListenerHandler::StopListener()
	{
		if ( NULL != listener_ && !isListenerStopped &&
			 REXDR::Listener::Stop(listener_) )
		{
			if ( logger_->IsDebugEnabled() )
			{
				logger_->Log(Log4X::LogLevel::Debug, "Listener successfully stopped.");
			}

			return true;
		}

		if ( logger_->IsErrorEnabled() )
		{
			logger_->Log(Log4X::LogLevel::Error, "Failed to stop REXDRListener");
		}
		return false;

	}

	size_t REXDRListenerHandler::GetKeepAliveTimeout() const
	{
		return REXDR::Listener::GetKeepAliveTimeout(listener_);
	}


	void REXDRListenerHandler::SetKeepAliveTimeout(size_t timeInMillisecond)
	{
		REXDR::Listener::SetKeepAliveTimeout(listener_, timeInMillisecond);
	}


	void REXDRListenerHandler::SetLogger(Common::Log4XWrapper* logger)
	{
		logger_ = logger;
		REXDR::Listener::SetLogger(logger_->GetLoggerHandle());
	}

	void REXDRListenerHandler::ProcessAccept(REXDR::Listener::Link::Handle link)
	{
		const char* remoteAddr = REXDR::Listener::Link::GetRemoteAddress(link);

		if ( logger_->IsDebugEnabled() )
		{
			logger_->LogFormat(Log4X::LogLevel::Debug, "A new client is accepted from %s", remoteAddr);
		}

	
		REXDR::Listener::Link::SetContext(link, (void*)remoteAddr, REXDRListenerHandler::OnLinkDestroyHandler, this);
	}


	void REXDRListenerHandler::ProcessClose(REXDR::Listener::Link::Handle link)
	{
		const char* remoteAddr = (const char*)REXDR::Listener::Link::GetContext(link);

		if ( logger_->IsDebugEnabled() )
		{
			logger_->LogFormat(Log4X::LogLevel::Debug, "A client@%s is closing.", remoteAddr);
		}
	}


	void REXDRListenerHandler::ProcessRequest(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req)
	{


	}

	void REXDRListenerHandler::ProcessLinkDestroy(REXDR::Listener::Link::Handle link)
	{
		const char* remoteAddr = (const char*)REXDR::Listener::Link::GetContext(link);

		if ( logger_->IsDebugEnabled() )
		{
			logger_->LogFormat(Log4X::LogLevel::Debug, "A client@%s is destroyed.", remoteAddr);
		}
	}


	void REXDRListenerHandler::ProcessQuery(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req)
	{
		QueryUserSessionStatus update;
		if ( !update.Load(req) )
		{
			if ( logger_->IsErrorEnabled() )
			{
				logger_->LogFormat(Log4X::LogLevel::Error, "ProcessQuery Load() FAILED. Error(%d). Request = %s", XPlatform::GetLastError(), req.toString());
			}
			return;
		}

		const REXDR::Resource::ElementList& els = req.resource.tokenize();
		std::string userid = els[2];

		QueryUserSessionStatusResponse response;
		response.userid = userid;

		UserSessionIterator itor = userlist_.find(userid);
		if ( itor == userlist_.end() )
		{
			response.status.code = REXDR::STATUS_NOT_FOUND;
			response.status.message = "user not exists";
			response.errorcode = REXDR::STATUS_NOT_FOUND;

			if ( logger_->IsInfoEnabled() )
			{
				logger_->LogFormat(Log4X::LogLevel::Info, "No such user(%s) found.", userid.c_str());
			}
		}
		else
		{
			response.status.code = REXDR::STATUS_OK;

			UserSessionInfo *userinfo = (UserSessionInfo*)itor->second;
			response.sessionid = userinfo->sessionid;
			response.gameid = userinfo->gameid;
			response.channelid = userinfo->channelid;
			response.sessionstatus = userinfo->status;

			if ( logger_->IsDebugEnabled() )
			{
				logger_->LogFormat(Log4X::LogLevel::Debug, "User(%s) Session(%d) query response sent.\n", userid.c_str(), response.sessionid);
			}
		}

		REXDR::Listener::Link::Send(link, info, &response);
	}


	void REXDRListenerHandler::ProcessUpdate(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req)
	{
		UpdateUserSessionStatus update;
		if ( !update.Load(req) )
		{
			if ( logger_->IsErrorEnabled() )
			{
				logger_->LogFormat(Log4X::LogLevel::Error, "ProcessUpdate Load() FAILED. Error(%d). Request = %s", XPlatform::GetLastError(), req.toString());
			}
			return;
		}

		EnterCriticalSection(&userlistlock_);
		UserSessionIterator itor = userlist_.find(update.userid);
		if ( itor == userlist_.end() )
		{
			// add
			UserSessionInfo *userinfo = new UserSessionInfo;
			userinfo->userid = update.userid;
			userinfo->sessionid = update.sessionid;
			userinfo->gameid = update.gameid;
			userinfo->channelid = update.channelid;
			userinfo->status = SESSION_ACTIVE;

			userlist_[userinfo->userid] = userinfo;
		}
		else
		{
			// update
			UserSessionInfo *userinfo = (UserSessionInfo*)itor->second;
			userinfo->sessionid = update.sessionid;
			userinfo->gameid = update.gameid;
			userinfo->channelid = update.channelid;
			userinfo->status = SESSION_ACTIVE;
		}
		LeaveCriticalSection(&userlistlock_);

		if ( logger_->IsDebugEnabled() )
		{
			logger_->LogFormat(Log4X::LogLevel::Debug, "User(%s) Session(%d) UPDATED\n", update.userid.c_str(), update.sessionid);
		}

		UpdateUserSessionStatusResponse response;
		response.userid = update.userid;
		response.sessionid = update.sessionid;
		response.errorcode = 0;
		response.status.code = REXDR::STATUS_OK;
		response.status.message = "Update success";

		REXDR::Listener::Link::Send(link, info, &response);

		if ( logger_->IsDebugEnabled() )
		{
			logger_->LogFormat(Log4X::LogLevel::Debug, "User(%s) Session(%d) response sent.\n", response.userid.c_str(), response.sessionid);
		}
	}

	void REXDRListenerHandler::RegisterMessagesToDispatcher()
	{
		// 
		//REXDR::Dispatcher::SetResourceHandler(dispatcher_, "/request/type1", 
	}

	void __stdcall REXDRListenerHandler::OnAcceptHandler(REXDR::Listener::Link::Handle link, void* context)
	{
		REXDRListenerHandler* handle = (REXDRListenerHandler*)context;
		handle->ProcessAccept(link);
	}

	void __stdcall REXDRListenerHandler::OnCloseHandler(REXDR::Listener::Link::Handle link, void* context)
	{
		REXDRListenerHandler* handle = (REXDRListenerHandler*)context;
		handle->ProcessClose(link);
	}

	void __stdcall REXDRListenerHandler::OnRequestHandler(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req, void* context)
	{
		REXDRListenerHandler* handle = (REXDRListenerHandler*)context;
		handle->ProcessRequest(link, info, req);
	}

	void __stdcall REXDRListenerHandler::OnLinkDestroyHandler(REXDR::Listener::Link::Handle link, void* context)
	{
		REXDRListenerHandler* handle = (REXDRListenerHandler*)context;
		handle->ProcessLinkDestroy(link);
	}

	void __stdcall REXDRListenerHandler::OnQueryUserSession(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req, void* context)
	{
		REXDRListenerHandler* handler = (REXDRListenerHandler*)context;
		handler->ProcessQuery(link, info, req);
	}

	void __stdcall REXDRListenerHandler::OnUpdateUserSession(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req, void* context)
	{
		REXDRListenerHandler* handler = (REXDRListenerHandler*)context;
		handler->ProcessUpdate(link, info, req);
	}

}	// end-of namespace 'REXDRServer'