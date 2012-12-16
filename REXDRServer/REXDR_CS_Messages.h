/* This file, generated by REXDR Compiler for C++ version 1.7.1.2155. Build: Sep 17 2012. 10:31:38. */

#ifndef __REXDR_REXDR_CS_MESSAGES_H_E839EAC5_45D8_11E2_B436_005056C00008__
#define __REXDR_REXDR_CS_MESSAGES_H_E839EAC5_45D8_11E2_B436_005056C00008__

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <REXDR/REXDR.h>


#ifndef __REXDR_MESSAGE_CLASS_GETUSERSTATUS_ID_104080538_DECLARED__
#define __REXDR_MESSAGE_CLASS_GETUSERSTATUS_ID_104080538_DECLARED__

/* class GetUserStatus */
class GetUserStatus : public REXDR::IRequestMessage
{
public:

	/* message id */
	enum { tagMsgID = 104080538 };

protected:

	int32_t _msgID;
	std::string _lastErrorField, _lastErrorDetail;

public:

	/* default data fields for request message */
	const REXDR::Command command;
	REXDR::Resource resource;
	REXDR::Option option;

public:

	/* constructor */
	GetUserStatus( const std::string& userId = "" ) :
		command( REXDR::CMD_GET ), resource( "/stallon/request1/%s", userId.c_str() ), option()
	{
		_msgID = tagMsgID;

		/* user-defined member */
	}

	GetUserStatus( const GetUserStatus& rhs ) :
		command( REXDR::CMD_GET )
	{
		_msgID = tagMsgID;

		_copy( rhs );
	}

	GetUserStatus& operator=( const GetUserStatus& rhs )
	{
		if ( this == &rhs ) {
			return *this;
		}

		_copy( rhs );
		return *this;
	}

private:

	void _copy( const GetUserStatus& from )
	{
		_msgID = from._msgID;

		_lastErrorField.clear();
		_lastErrorDetail.clear();

		this->resource = from.resource;
		this->option = from.option;

		/* user-defined member */
	}

public:

	/* destructor */
	virtual ~GetUserStatus()
	{
	}

public:

	/* REXDR::IRequestMessage::Save() */
	virtual bool Save( REXDR::Request* req ) const
	{
		req->messageID = this->GetID();

		req->command = command;
		req->resource = resource;
		req->option = option;

		return Save( &( req->param ) );
	}

protected:

	/* REXDR::IMessage::Save() */
	virtual bool Save( REXDR::Value* value ) const
	{
		value->set( REXDR::Value::TYPE_MAP );

		value->messageID = this->GetID();

		return true;
	}

public:

	/* REXDR::IRequestMessage::Load() */
	virtual bool Load( const REXDR::Request& req )
	{
		/* command */
		if ( command != req.command ) {
			return false;
		}

		/*
		 * resource
		 */
		resource = req.resource;

		/* resource[2]; string userId */
		if ( true == resource[2].empty() ) {
			return false;
		}

		/*
		 * option
		 */
		option = req.option;

		/*
		 * param
		 */
		return Load( req.param );
	}

protected:

	/* REXDR::IMessage::Load() */
	virtual bool Load( const REXDR::Value& value )
	{
		return true;
	}

public:

	std::string GetLastError( size_t depth = 0 ) const
	{
		if ( true == _lastErrorField.empty() ) {
			return "";
		}

		std::string lastError = _lastErrorField;
		if ( 0 == depth ) {
			lastError = "Cannot load \"GetUserStatus::" + lastError;
		}

		bool detail = false;

		if ( false == detail ) {
			lastError.append( "\"; " ).append( _lastErrorDetail );
		}

		return lastError;
	}

private:

	void SetLastError( const char* field, const char* expected, const REXDR::Value& actual )
	{
		_lastErrorField = field;
		_lastErrorDetail = "expected="; _lastErrorDetail += expected;
		_lastErrorDetail += ", actual="; _lastErrorDetail += REXDR::Value::typeToString( actual.type() );

		if ( REXDR::Value::TYPE_INT == actual.type() ) {
			if ( 0 == ::strncmp( expected, "int", 3 ) || 0 == ::strncmp( expected, "uint", 4 ) ) {
				_lastErrorDetail += " (out of range)";
			}
		}

		if ( REXDR::Value::TYPE_NULL == actual.type() ) {
			_lastErrorDetail += " (null or not exists)";
		}
	}

public:

	/* [RE]XDR::IMessage::Clear() */
	virtual void Clear()
	{
		/* user-defined member */
	}

public:

	/* [RE]XDR::IMessage::GetID() */
	virtual int32_t GetID() const
	{
		return _msgID;
	}

public:

	/* [RE]XDR::IMessage::GetName() */
	virtual const char* GetName() const
	{
		return "GetUserStatus";
	}

};
/* end of class GetUserStatus */

#endif /* __REXDR_MESSAGE_CLASS_GETUSERSTATUS_ID_104080538_DECLARED__ */


#ifndef __REXDR_MESSAGE_CLASS_USERSTATUSRESPONSE_ID_266968474_DECLARED__
#define __REXDR_MESSAGE_CLASS_USERSTATUSRESPONSE_ID_266968474_DECLARED__

/* class UserStatusResponse */
class UserStatusResponse : public REXDR::IResponseMessage
{
public:

	/* message id */
	enum { tagMsgID = 266968474 };

protected:

	int32_t _msgID;
	std::string _lastErrorField, _lastErrorDetail;

public:

	/* default data fields for response message */
	REXDR::Status status;

	/* user defined data fields */
	std::string userId;
	std::string gameId;
	std::string channelId;
	int32_t userStatus;

public:

	/* constructor */
	UserStatusResponse()
	{
		_msgID = tagMsgID;

		/* user-defined member */
		userStatus = 0;
	}

	UserStatusResponse( const UserStatusResponse& rhs )
	{
		_msgID = tagMsgID;

		_copy( rhs );
	}

	UserStatusResponse& operator=( const UserStatusResponse& rhs )
	{
		if ( this == &rhs ) {
			return *this;
		}

		_copy( rhs );
		return *this;
	}

private:

	void _copy( const UserStatusResponse& from )
	{
		_msgID = from._msgID;

		_lastErrorField.clear();
		_lastErrorDetail.clear();

		this->status = from.status;

		/* user-defined member */
		this->userId = from.userId;
		this->gameId = from.gameId;
		this->channelId = from.channelId;
		this->userStatus = from.userStatus;
	}

public:

	/* destructor */
	virtual ~UserStatusResponse()
	{
	}

public:

	/* REXDR::IResponseMessage::Save() */
	virtual bool Save( REXDR::Response* resp ) const
	{
		resp->messageID = this->GetID();

		resp->status = status;

		return Save( &( resp->result ) );
	}

protected:

	/* REXDR::IMessage::Save() */
	virtual bool Save( REXDR::Value* value ) const
	{
		value->set( REXDR::Value::TYPE_MAP );

		/* string userId */
		(*value)["userId"] = this->userId;

		/* string gameId */
		(*value)["gameId"] = this->gameId;

		/* string channelId */
		(*value)["channelId"] = this->channelId;

		/* int32_t userStatus */
		(*value)["userStatus"] = this->userStatus;

		value->messageID = this->GetID();

		return true;
	}

public:

	/* REXDR::IResponseMessage::Load() */
	virtual bool Load( const REXDR::Response& resp )
	{
		/* status */
		status = resp.status;

		/*
		 * result
		 */
		return Load( resp.result );
	}

protected:

	/* REXDR::IMessage::Load() */
	virtual bool Load( const REXDR::Value& value )
	{
		/* string userId */
		if ( false == REXDR::ValueHandler::Load<std::string>( &( this->userId ), value["userId"] ) ) {
			SetLastError( "userId", "string", value["userId"] );
			return false;
		}

		/* string gameId */
		if ( false == REXDR::ValueHandler::Load<std::string>( &( this->gameId ), value["gameId"] ) ) {
			SetLastError( "gameId", "string", value["gameId"] );
			return false;
		}

		/* string channelId */
		if ( false == REXDR::ValueHandler::Load<std::string>( &( this->channelId ), value["channelId"] ) ) {
			SetLastError( "channelId", "string", value["channelId"] );
			return false;
		}

		/* int32_t userStatus */
		if ( false == REXDR::ValueHandler::Load<int32_t>( &( this->userStatus ), value["userStatus"] ) ) {
			SetLastError( "userStatus", "int32_t", value["userStatus"] );
			return false;
		}

		return true;
	}

public:

	std::string GetLastError( size_t depth = 0 ) const
	{
		if ( true == _lastErrorField.empty() ) {
			return "";
		}

		std::string lastError = _lastErrorField;
		if ( 0 == depth ) {
			lastError = "Cannot load \"UserStatusResponse::" + lastError;
		}

		bool detail = false;

		if ( false == detail ) {
			lastError.append( "\"; " ).append( _lastErrorDetail );
		}

		return lastError;
	}

private:

	void SetLastError( const char* field, const char* expected, const REXDR::Value& actual )
	{
		_lastErrorField = field;
		_lastErrorDetail = "expected="; _lastErrorDetail += expected;
		_lastErrorDetail += ", actual="; _lastErrorDetail += REXDR::Value::typeToString( actual.type() );

		if ( REXDR::Value::TYPE_INT == actual.type() ) {
			if ( 0 == ::strncmp( expected, "int", 3 ) || 0 == ::strncmp( expected, "uint", 4 ) ) {
				_lastErrorDetail += " (out of range)";
			}
		}

		if ( REXDR::Value::TYPE_NULL == actual.type() ) {
			_lastErrorDetail += " (null or not exists)";
		}
	}

public:

	/* [RE]XDR::IMessage::Clear() */
	virtual void Clear()
	{
		/* user-defined member */
		userId.clear();
		gameId.clear();
		channelId.clear();
		userStatus = 0;
	}

public:

	/* [RE]XDR::IMessage::GetID() */
	virtual int32_t GetID() const
	{
		return _msgID;
	}

public:

	/* [RE]XDR::IMessage::GetName() */
	virtual const char* GetName() const
	{
		return "UserStatusResponse";
	}

};
/* end of class UserStatusResponse */

#endif /* __REXDR_MESSAGE_CLASS_USERSTATUSRESPONSE_ID_266968474_DECLARED__ */


#endif /* __REXDR_REXDR_CS_MESSAGES_H_E839EAC5_45D8_11E2_B436_005056C00008__ */
