#pragma once

#include <string>
#include "REXDR/REXDRConnectorAPI.h"
#include "REXDR_CS_Messages.h"
#include "Log4XWrapper.h"

namespace REXDRClient
{
	class ServiceHandler
	{
	public:
		ServiceHandler(REXDR::Connector::Handle hConnector, Common::Log4XWrapper& logger);
		virtual ~ServiceHandler(void);

	public:
		REXDR::Connector::Handle GetConnectorHandle() const;
		void* GetContext() const;
		void SetContext(void* context);

		bool QueryUserSession();
		bool UpdateUserSession(unsigned int sessionid, const std::string& gameid, const std::string& channelid, const std::string& reserve);

	private:
		static void __stdcall OnQueryUserSessionStatusResponse(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp, void* context);
		static void __stdcall OnUpdateUserSessionStatusResponse(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp, void* context);
//		static void __stdcall OnEventOccurred(REXDR::Connector::Handle hConnector, REXDR::Connector::EventCode eventCode, void* eventData, void* context);

		virtual void ProcessQuery(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp);
		virtual void ProcessUpdate(REXDR::Connector::ErrorCode errorCode, const REXDR::Response& resp);
//		virtual void ProcessEvent(REXDR::Connector::EventCode eventCode, void* eventData);

	private:
		REXDR::Connector::Handle hConnector_;
		Common::Log4XWrapper& logger_;

		void* context_;
	};

}
