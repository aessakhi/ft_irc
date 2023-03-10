#include "main.hpp"

void	srv_time(Server *srv, int &userfd, Command &cmd)
{
	(void)cmd;
	User *user = srv->getUser(userfd);

	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	std::string time_str = asctime(timeinfo);
	time_str.erase(time_str.end() - 1);

	srv->sendReply(userfd, RPL_TIME(user->getNickname(), srv->getName(), time_str));
}