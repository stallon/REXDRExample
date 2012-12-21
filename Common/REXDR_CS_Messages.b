@request(GET, "/stallon/request1/${userId:string}/${sessionid:uint32_t}")
class GetUserStatus
{

};


@response ( request = GetUserStatus )
class UserStatusResponse
{
	uint32_t sessionid;

	string userId;
	string gameId;
	string channelId;
	int32_t userStatus;
};