@request(GET, "/stallon/request1/${userId}")
class GetUserStatus
{

};


@response ( request = GetUserStatus )
class UserStatusResponse
{
	string userId;
	string gameId;
	string channelId;
	int32_t userStatus;
};