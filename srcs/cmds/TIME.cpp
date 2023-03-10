#include "main.hpp"

void	srv_time(Server *srv, int &userfd, Command &cmd)
{
	(void)cmd;
	User *user = srv->getUser(userfd);

	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	srv->sendReply(userfd, RPL_TIME(user->getNickname(), srv->getName(), asctime(timeinfo)));
}