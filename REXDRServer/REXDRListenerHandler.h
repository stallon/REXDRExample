#pragma once

#include "stdafx.h"
#include <REXDR/REXDRListenerAPI.h>
#include <REXDR/REXDRDispatcherAPI.h>


class REXDRListenerHandler
{
public:
	REXDRListenerHandler(int handlerId, REXDR::Listener::TransportType type, uint16_t port);
	virtual ~REXDRListenerHandler(void);

	// functions for Listener Identity
	REXDR::Listener::Handle GetListenerHandle() const;
	int GetListenerHandlerId() const;

	// functions for Listener behavior
	bool CreateListenerHandle();
	bool StartListener();
	bool StopListener();

	// functions for Listener's internal properties
	size_t GetKeepAliveTimeout() const;
	void SetKeepAliveTimeout(size_t timeInMillisecond);
	void SetLogger(Log4X::Handle logger);


	// Event Handlers
	virtual void ProcessAccept(REXDR::Listener::Link::Handle link);
	virtual void ProcessClose(REXDR::Listener::Link::Handle link);
	virtual void ProcessRequest(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req);
	virtual void ProcessLinkDestroy(REXDR::Listener::Link::Handle link);

private:
	int id_;
	REXDR::Listener::TransportType transportType_;
	uint16_t port_;
	REXDR::Listener::Handle	listener_;
	REXDR::Dispatcher::Handle dispatcher_;

	bool isListenerStopped;

private:
	void RegisterMessagesToDispatcher();

	static void __stdcall OnAcceptHandler(REXDR::Listener::Link::Handle link, void* context);
	static void __stdcall OnCloseHandler(REXDR::Listener::Link::Handle link, void* context);
	static void __stdcall OnRequestHandler(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req, void* context);
	static void __stdcall OnLinkDestroyHandler(REXDR::Listener::Link::Handle link, void* context);
};

