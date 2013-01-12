#include "StdAfx.h"
#include "UserSessionContainer.h"


namespace Service
{

	UserSessionContainer::UserSessionContainer(void)
	{
	}


	UserSessionContainer::~UserSessionContainer(void)
	{
		this->sessionMap_.clear();
	}


	const UserSessionInfoPtr UserSessionContainer::GetSessionInfo(const std::string& userid)
	{
		ScopedLocker Lock(lock_);

		UserSessionIter it = sessionMap_.find(userid);
		if ( it != sessionMap_.end() )
		{
			return it->second;
		}

		return UserSessionInfoPtr(new UserSessionInfoType(userid, 0, SESSION_NOTEXIST, "", ""));
	}

	void UserSessionContainer::SetSessionInfo(UserSessionInfoPtr info)
	{
		ScopedLocker Lock(lock_);

		if ( NULL == info ) return;

		UserSessionIter it = sessionMap_.find(info->userid);
		if ( it != sessionMap_.end() )
		{
			sessionMap_.erase(it);
		}
		
		sessionMap_[info->userid] = info;
	}

}	// end-of namespace 'UserSessionService'