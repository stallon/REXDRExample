#include "StdAfx.h"
#include "REXDRListenerHandler.h"
#include "REXDR_CS_Messages.h"

namespace REXDRServer
{
	REXDRListenerHandler::REXDRListenerHandler(int handlerId, REXDR::Listener::TransportType type, uint16_t port) : 
		id_(handlerId), transportType_(type), port_(port), listener_(NULL), isListenerStopped(true), logger_(NULL)
	{
		
	}

	REXDRListenerHandler::~REXDRListenerHandler(void)
	{
		if ( !isListenerStopped )
		{
			REXDR::Listener::Stop(listener_);
		}
	
		if ( logger_->IsLoggerEnabled() && logger_->IsDebugEnabled() )
		{
			logger_->Log(LL_DEBUG, "~REXDRListenerHandler(): Listener is being destroyed.");
		}

		REXDR::Dispatcher::DestroyHandle(dispatcher_);
		REXDR::Listener::DestroyHandle(listener_);
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
					logger_->LogFormat(LL_DEBUG, "REXDRListener is created and listening at %d", port_);
				}

				// REXDR Dispatcher 생성. Dispatcher는 REXDRListener의 lifecycle과 같은게 좋을듯.
				dispatcher_ = REXDR::Dispatcher::CreateHandle();
				REXDR::Dispatcher::AttachListener(dispatcher_, listener_);
			
				return true;
			}
		}

		if ( logger_->IsFatalEnabled() )
		{
			logger_->Log(LL_FATAL, "New listener COULD NOT be created.");
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
					logger_->LogFormat(LL_DEBUG, "REXDRListener (Id:%d) is starting", id_);
				}
				return true;
			}
		}

		if ( logger_->IsFatalEnabled() )
		{
			logger_->Log(LL_FATAL, "Failed to start REXDRListener");
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
				logger_->Log(LL_DEBUG, "Listener successfully stopped.");
			}

			return true;
		}

		if ( logger_->IsErrorEnabled() )
		{
			logger_->Log(LL_ERROR, "Failed to stop REXDRListener");
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

	Common::Log4XWrapper* REXDRListenerHandler::GetLogger() const
	{
		return logger_;
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
			logger_->LogFormat(LL_DEBUG, "A new client is accepted from %s", remoteAddr);
		}

	
		REXDR::Listener::Link::SetContext(link, (void*)remoteAddr, REXDRListenerHandler::OnLinkDestroyHandler, this);
	}


	void REXDRListenerHandler::ProcessClose(REXDR::Listener::Link::Handle link)
	{
		const char* remoteAddr = (const char*)REXDR::Listener::Link::GetContext(link);

		if ( logger_->IsDebugEnabled() )
		{
			logger_->LogFormat(LL_DEBUG, "A client@%s is closing.", remoteAddr);
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
			logger_->LogFormat(LL_DEBUG, "A client@%s is destroyed.", remoteAddr);
		}
	}


	
	void REXDRListenerHandler::RegisterServiceHandler(REXDR::Dispatcher::IResourceHandler* handler, const std::string& uri)
	{
		REXDR::Dispatcher::SetResourceHandler(dispatcher_, uri, handler, this);

		if ( logger_->IsDebugEnabled() )
		{
			logger_->LogFormat(LL_DEBUG, "ResourceHandler Added ON %s", uri.c_str());
		}
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

}	// end-of namespace 'REXDRServer'