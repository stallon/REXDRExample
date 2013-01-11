#pragma once

#include <REXDR/REXDRListenerAPI.h>
#include <string>

namespace REXDRServer
{
	class REXDRServiceHandler
	{
	public:
		REXDRServiceHandler(const std::string& serviceURI) : serviceURI_(serviceURI) {  }
		virtual ~REXDRServiceHandler(void) { };

		const std::string& GetServiceURI() const;
		virtual void DoService(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req) = 0;

	private:
		static void __stdcall DoDispatch(REXDR::Listener::Link::Handle link, const REXDR::MessageInfo& info, const REXDR::Request& req, void* context);
		std::string serviceURI_;
	};

}	// end-of namespace REXDRServer


