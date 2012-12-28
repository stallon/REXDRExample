@request(GET, "/stallon/usersession/${userid:string}")
class QueryUserSessionStatus
{

};


@response
class QueryUserSessionStatusResponse
{
	string userid;
	uint32_t sessionid;
	string gameid;
	string channelid;
	uint32_t sessionstatus;
	uint32_t errorcode;
};


@request(POST, "/stallon/usersession")
class UpdateUserSessionStatus
{
	string userid;
	uint32_t sessionid;
	string gameid;
	string channelid;
	string reserve;
};


@response
class UpdateUserSessionStatusResponse
{
	string userid;
	uint32_t sessionid;
	uint32_t errorcode;
};

