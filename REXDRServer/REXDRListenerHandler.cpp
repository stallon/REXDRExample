#include "StdAfx.h"
#include "REXDRListenerHandler.h"


REXDRListenerHandler::REXDRListenerHandler(int handlerId, REXDR::Listener::TransportType type, uint16_t port) : 
	id_(handlerId), transportType_(type), port_(port), listener_(NULL), isListenerStopped(true)
{
	// Initialize Event Object
	hEventStop = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}


REXDRListenerHandler::~REXDRListenerHandler(void)
{
	if ( !isListenerStopped )
	{
		REXDR::Listener::Stop(listener_);
	}
	
	printf(">>> ~REXDRListenerHandler(): Listener is being destroyed.\n");
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
			printf(">>> [%s] REXDRListener is created @ %d\n", __FUNCTION__, port_);

			// REXDR Dispatcher 생성. Dispatcher는 REXDRListener의 lifecycle과 같은게 좋을듯.
			dispatcher_ = REXDR::Dispatcher::CreateHandle();
			REXDR::Dispatcher::AttachListener(dispatcher_, listener_);

			return true;
		}
	}

	printf(">>> [%s] New listener COULD NOT be created.\n",  __FUNCTION__);
	return false;
}

bool REXDRListenerHandler::StartListener()
{
	if ( NULL != listener_ && isListenerStopped )
	{
		// Set Main Event Handler ... OnAccept/Close/Request
		REXDR::Listener::SetOnAcceptHandler(listener_, REXDRListenerHandler::OnAcceptHandler, this);
		REXDR::Listener::SetOnCloseHandler(listener_, REXDRListenerHandler::OnCloseHandler, this);
		REXDR::Listener::SetOnReceiveHandler(listener_, REXDRListenerHandler::OnRequestHandler, this);
		

		// Start a new REXDRListener
		if ( REXDR::Listener::Start(listener_) )
		{
			isListenerStopped = false;
			printf(">>> [%s] REXDRListener (Id:%d) is starting...\n", __FUNCTION__, id_);
			return true;
		}
	}

	printf(">>> [%s] FAILED to start a listener.\n", __FUNCTION__);
	return false;
}


bool REXDRListenerHandler::StopListener()
{
	if ( NULL != listener_ && !isListenerStopped &&
		 REXDR::Listener::Stop(listener_) )
	{
		printf(">>> [%s] Listener successfully stopped.\n", __FUNCTION__);
		::SetEvent(hEventStop);
		return true;
	}

	printf(">>> [%s] FAILED to stop the listener.\n", __FUNCTION__);
	return false;

}

void REXDRListenerHandler::WaitListenerForStop()
{
	::WaitForSingleObject(hEventStop, INFINITE);
}


void REXDRListenerHandler::ProcessAccept(REXDR::Listener::Link::Handle link)
{
	const char* remoteAddr = REXDR::Listener::Link::GetRemoteAddress(link);
	printf(">>> [%s] A new client is accepted from %s.\n", __FUNCTION__, remoteAddr);

	REXDR::Listener::Link::SetContext(link, (void*)remoteAddr, REXDRListenerHandler::OnLinkDestroyHandler, this);
}


void REXDRListenerHandler::ProcessClose(REXDR::Listener::Link::Handle link)
{
	const char* remoteAddr = (const char*)REXDR::Listener::Link::GetContext(link);
	printf(">>> [%s] A client@%s is closing.\n", __FUNCTION__, remoteAddr);

}


void REXDRListenerHandler::ProcessRequest(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req)
{


}

void REXDRListenerHandler::ProcessLinkDestroy(REXDR::Listener::Link::Handle link)
{
	const char* remoteAddr = (const char*)REXDR::Listener::Link::GetContext(link);
	printf(">>> [%s] A client@%s is destroyed. \n", __FUNCTION__, remoteAddr);
}


void REXDRListenerHandler::RegisterMessagesToDispatcher()
{
	// 
	REXDR::Dispatcher::SetResourceHandler(dispatcher_, "/request/type1", 
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

