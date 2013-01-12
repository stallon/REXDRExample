#pragma once

#include <REXDR\REXDRDispatcherAPI.h>

namespace Service
{
	class UserSessionService : public REXDR::Dispatcher::ResourceHandlerImpl
	{
	public:
		UserSessionService(void);
		virtual ~UserSessionService(void);

		virtual void OnGet(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& /* req */, void* /* context */ );
		virtual void OnPost(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& /* req */, void* /* context */ );

	private:

	};

}	// end-of namespace

