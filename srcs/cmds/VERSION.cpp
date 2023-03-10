#include "main.hpp"

void	version(Server *srv, int &userfd, Command &cmd)
{
	(void)cmd;
	User * user = srv->getUser(userfd);

	srv->sendReply(userfd, RPL_YOURHOST(user->getNickname(), srv->getName()));
}