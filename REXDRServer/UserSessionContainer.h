#pragma once

#include <string>
#include <map>
#include <XBase/XBase.h>

namespace Service
{
	enum UserSessionStatus { SESSION_ACTIVE, SESSION_INACTIVE, SESSION_NOTEXIST };

	typedef struct UserSessionInfo
	{
		UserSessionInfo() {  }

		UserSessionInfo(const std::string& uid, int sid, UserSessionStatus ustatus, const std::string& gid, const std::string& cid) 
			: userid(uid), sessionid(sid), status(ustatus), gameid(gid), channelid(cid) {  }

		UserSessionInfo(const UserSessionInfo& u)
		{
			this->userid = u.userid;
			this->sessionid = u.sessionid;
			this->status = u.status;
			this->gameid = u.gameid;
			this->channelid = u.channelid;
		}

		UserSessionInfo& operator=(const UserSessionInfo& u) 
		{
			if ( this == &u ) 
			{
				return *this;
			}

			this->userid = u.userid;
			this->sessionid = u.sessionid;
			this->status = u.status;
			this->gameid = u.gameid;
			this->channelid = u.channelid;

			return *this;
		}

		std::string userid;
		unsigned int sessionid;
		UserSessionStatus status;
		std::string gameid;
		std::string channelid;
	} UserSessionInfoType;

	// SmartPointer for automatic reference counting for UserSessionInfo structure
	typedef XSystem::XTL::SmartPtr<UserSessionInfoType> UserSessionInfoPtr;


	class UserSessionContainer
	{
	public:
		UserSessionContainer(void);
		virtual ~UserSessionContainer(void);

		const UserSessionInfoPtr GetSessionInfo(const std::string& userid);
		void SetSessionInfo(UserSessionInfoPtr sessionInfo);

	private:
		// Map, Iterator typedef
		typedef std::map<std::string, UserSessionInfoPtr> UserSessionMap;
		typedef std::map<std::string, UserSessionInfoPtr>::iterator UserSessionIter;

		UserSessionMap sessionMap_;

		// Lock Object to protect session map from being accessed by 2 more threads at the same time
		typedef XSystem::Threading::ScopedLock<XSystem::Threading::CriticalSection> ScopedLocker;
		XSystem::Threading::CriticalSection lock_;
	};


	typedef XSystem::XTL::Singleton<UserSessionContainer> UserSessionStorage;

}	// end-of namespace

